#include "details.h"

#include "../core/check.h"
#include "../core/text.h"

enum { RESOURCE_NAME_WIDTH = 8, STAT_NAME_WIDTH = 16, HUNDRED = 100, TENTH = 10 };

static const char RESOURCE_NAMES[RESOURCE_KIND_COUNT][RESOURCE_NAME_WIDTH] = { " food", " wood", " stone", " gold" };

static const char STAT_NAMES[UNIT_STAT_COUNT][STAT_NAME_WIDTH] = {
    "", "", "", "", "Hit points ", ", attack ", ", melee armor ", ", pierce armor ", ", range ", ", speed ",
    ", reload time "
};

static void add(DetailText *detail, const char *text)
{
    if (!CHECK(detail != NULL) || !CHECK(text != NULL)) {
        return;
    }
    (void)text_append(detail->text, sizeof detail->text, text);
}

static void add_number(DetailText *detail, long value)
{
    if (!CHECK(detail != NULL) || !CHECK(value > -HUNDRED * HUNDRED * HUNDRED)) {
        return;
    }
    (void)text_append_number(detail->text, sizeof detail->text, value);
}

static void add_hundredths(DetailText *detail, int value)
{
    if (!CHECK(detail != NULL) || !CHECK(value >= 0)) {
        return;
    }
    add_number(detail, value / HUNDRED);
    add(detail, ".");
    add_number(detail, (value % HUNDRED) / TENTH);
}

static void add_cost(DetailText *detail, const int cost[RESOURCE_KIND_COUNT])
{
    int resource;
    bool first = true;

    if (!CHECK(detail != NULL) || !CHECK(cost != NULL)) {
        return;
    }
    add(detail, "Cost:");
    for (resource = 0; resource < RESOURCE_KIND_COUNT; ++resource) {
        if (cost[resource] > 0) {
            add(detail, first ? " " : ", ");
            add_number(detail, cost[resource]);
            add(detail, RESOURCE_NAMES[resource]);
            first = false;
        }
    }
    add(detail, first ? " nothing\r\n" : "\r\n");
}

static void add_unit(DetailText *detail, const CivDesign *design, const Catalog *catalog)
{
    int found;
    int stat;

    if (!CHECK(design != NULL) || !CHECK(catalog != NULL) || (design->unique_unit.origin == UNIT_ORIGIN_NONE)) {
        add(detail, "No unit is selected yet.");
        return;
    }
    found = catalog_find_unit(catalog, (design->unique_unit.origin == UNIT_ORIGIN_CUSTOM) ? UNIT_ROLE_CUSTOM
                                                                                          : UNIT_ROLE_VANILLA,
                              design->unique_unit.id);
    if (found == CATALOG_MISSING) {
        add(detail, "The selected unit is not in the catalog.");
        return;
    }
    add(detail, catalog->units[found].name);
    add(detail, "\r\n");
    add_cost(detail, catalog->units[found].stats);
    for (stat = (int)UNIT_STAT_HP; stat < UNIT_STAT_COUNT; ++stat) {
        add(detail, STAT_NAMES[stat]);
        if (stat >= (int)UNIT_STAT_SPEED) {
            add_hundredths(detail, catalog->units[found].stats[stat]);
        } else {
            add_number(detail, catalog->units[found].stats[stat]);
        }
    }
}

static void add_tech(DetailText *detail, const Catalog *catalog, int found)
{
    if (!CHECK(detail != NULL) || !CHECK(catalog != NULL) || (found == CATALOG_MISSING)) {
        add(detail, "No tech is selected yet.");
        return;
    }
    add(detail, catalog->techs[found].name);
    add(detail, "\r\n");
    add(detail, catalog->techs[found].description);
    add(detail, "\r\n");
    add_cost(detail, catalog->techs[found].cost);
    add(detail, "Research time: ");
    add_number(detail, catalog->techs[found].time);
    add(detail, " seconds");
}

static void add_bonuses(DetailText *detail, const CivDesign *design, const Catalog *catalog)
{
    int pick;
    int found;

    if (!CHECK(design != NULL) || !CHECK(catalog != NULL)) {
        return;
    }
    add_number(detail, design->bonus_count);
    add(detail, " of 24 chosen.\r\n");
    for (pick = 0; (pick < design->bonus_count) && (pick < CIV_BONUS_CAPACITY); ++pick) {
        found = catalog_find_bonus(catalog, design->bonuses[pick].id);
        add(detail, "\r\n- ");
        add(detail, (found == CATALOG_MISSING) ? "a bonus not in the catalog" : catalog->bonuses[found].description);
    }
}

static void add_team_bonus(DetailText *detail, const CivDesign *design, const Catalog *catalog)
{
    int found = CATALOG_MISSING;

    if (!CHECK(design != NULL) || !CHECK(catalog != NULL)) {
        return;
    }
    if (design->team_bonus_count > 0) {
        found = catalog_find_team_bonus(catalog, design->team_bonuses[0].id);
    }
    add(detail, (found == CATALOG_MISSING) ? "No team bonus is selected yet."
                                           : catalog->team_bonuses[found].description);
}

static void add_tree(DetailText *detail, const CivDesign *design)
{
    if (!CHECK(detail != NULL) || !CHECK(design != NULL)) {
        return;
    }
    add_number(detail, design->tree[TREE_UNITS].count);
    add(detail, " units, ");
    add_number(detail, design->tree[TREE_BUILDINGS].count);
    add(detail, " buildings and ");
    add_number(detail, design->tree[TREE_TECHS].count);
    add(detail, " techs are in the tree.");
}

DetailText details_of(const CivDesign *design, const Catalog *catalog, ControlId control)
{
    DetailText detail;

    detail.text[0] = '\0';
    if (!CHECK(design != NULL) || !CHECK(catalog != NULL)) {
        return detail;
    }
    switch ((int)control) {
    case CONTROL_TREE_INFO:
        add_tree(&detail, design);
        break;
    case CONTROL_BONUS_INFO:
        add_bonuses(&detail, design, catalog);
        break;
    case CONTROL_TEAM_INFO:
        add_team_bonus(&detail, design, catalog);
        break;
    case CONTROL_UNIT_INFO:
        add_unit(&detail, design, catalog);
        break;
    case CONTROL_CASTLE_TECH_INFO:
        add_tech(&detail, catalog,
                 catalog_find_tech(catalog, TECH_TIER_CASTLE, design->techs[TECH_TIER_CASTLE].km_index));
        break;
    case CONTROL_IMPERIAL_TECH_INFO:
        add_tech(&detail, catalog,
                 catalog_find_tech(catalog, TECH_TIER_IMPERIAL, design->techs[TECH_TIER_IMPERIAL].km_index));
        break;
    default:
        break;
    }
    return detail;
}

DetailText details_title(WizardStep step)
{
    DetailText detail;

    detail.text[0] = '\0';
    if (!CHECK((int)step >= 0) || !CHECK(step < STEP_COUNT)) {
        return detail;
    }
    add(&detail, "Step ");
    add_number(&detail, (long)step + 1L);
    add(&detail, " of 9: ");
    add(&detail, wizard_title(step));
    return detail;
}

DetailText details_window_title(const CivDesign *design, WizardStep step)
{
    DetailText title = details_title(step);
    DetailText detail;
    const char *name;

    detail.text[0] = '\0';
    if (!CHECK(design != NULL) || !CHECK(title.text[0] != '\0')) {
        return detail;
    }
    name = civ_design_text(design, TEXT_FIELD_NAME);
    add(&detail, (name[0] != '\0') ? name : "A new civ");
    add(&detail, " - ");
    add(&detail, title.text);
    add(&detail, " - civbuilder");
    return detail;
}
