#include "details.h"

#include "../core/check.h"
#include "../core/text.h"

enum { RESOURCE_NAME_WIDTH = 8, STAT_NAME_WIDTH = 16, HUNDRED = 100, TENTH = 10 };

static const char RESOURCE_NAMES[RESOURCE_KIND_COUNT][RESOURCE_NAME_WIDTH] = { " food", " wood", " stone", " gold" };

static const char STAT_NAMES[UNIT_STAT_COUNT][STAT_NAME_WIDTH] = {
    "", "", "", "", "Hit points ", ", attack ", ", melee armor ", ", pierce armor ", ", range ", ", speed ",
    ", reload time "
};

static void add_number(DetailText *detail, long value)
{
    (void)text_append_number(detail->text, sizeof detail->text, value);
}

static void add_hundredths(DetailText *detail, int value)
{
    add_number(detail, value / HUNDRED);
    summary_add(detail, ".");
    add_number(detail, (value % HUNDRED) / TENTH);
}

static void add_cost(DetailText *detail, const int cost[RESOURCE_KIND_COUNT])
{
    int resource;
    bool first = true;

    summary_add(detail, "Cost:");
    for (resource = 0; resource < RESOURCE_KIND_COUNT; ++resource) {
        if (cost[resource] > 0) {
            summary_add(detail, first ? " " : ", ");
            add_number(detail, cost[resource]);
            summary_add(detail, RESOURCE_NAMES[resource]);
            first = false;
        }
    }
    summary_add(detail, first ? " nothing\r\n" : "\r\n");
}

static void add_unit(DetailText *detail, const CivDesign *design, const Catalog *catalog)
{
    int found;
    int stat;

    if (design->unique_unit.origin == UNIT_ORIGIN_NONE) {
        summary_add(detail, "No unit is selected yet.");
        return;
    }
    found = catalog_find(catalog, CATALOG_UNITS,
                         (design->unique_unit.origin == UNIT_ORIGIN_CUSTOM) ? UNIT_ROLE_CUSTOM : UNIT_ROLE_VANILLA,
                         design->unique_unit.id);
    if (found == CATALOG_MISSING) {
        summary_add(detail, "The selected unit is not in the catalog.");
        return;
    }
    summary_add(detail, catalog->units[found].name);
    summary_add(detail, "\r\n");
    add_cost(detail, catalog->units[found].stats);
    for (stat = (int)UNIT_STAT_HP; stat < UNIT_STAT_COUNT; ++stat) {
        summary_add(detail, STAT_NAMES[stat]);
        if (stat >= (int)UNIT_STAT_SPEED) {
            add_hundredths(detail, catalog->units[found].stats[stat]);
        } else {
            add_number(detail, catalog->units[found].stats[stat]);
        }
    }
}

static void add_tech(DetailText *detail, const Catalog *catalog, int found)
{
    if (found == CATALOG_MISSING) {
        summary_add(detail, "No tech is selected yet.");
        return;
    }
    summary_add(detail, catalog->techs[found].name);
    summary_add(detail, "\r\n");
    summary_add(detail, catalog->techs[found].description);
    summary_add(detail, "\r\n");
    add_cost(detail, catalog->techs[found].cost);
    summary_add(detail, "Research time: ");
    add_number(detail, catalog->techs[found].time);
    summary_add(detail, " seconds");
}

static void add_bonuses(DetailText *detail, const CivDesign *design, const Catalog *catalog)
{
    int pick;
    int found;

    add_number(detail, design->bonus_count);
    summary_add(detail, " of 24 chosen.\r\n");
    for (pick = 0; (pick < design->bonus_count) && (pick < CIV_BONUS_CAPACITY); ++pick) {
        found = catalog_find_bonus(catalog, false, design->bonuses[pick].id);
        summary_add(detail, "\r\n- ");
        summary_add(detail, (found == CATALOG_MISSING) ? "a bonus not in the catalog"
                                                       : catalog->bonuses[found].description);
    }
}

static void add_team_bonus(DetailText *detail, const CivDesign *design, const Catalog *catalog)
{
    int found = CATALOG_MISSING;

    if (design->team_bonus_count > 0) {
        found = catalog_find_bonus(catalog, true, design->team_bonuses[0].id);
    }
    summary_add(detail, (found == CATALOG_MISSING) ? "No team bonus is selected yet."
                                           : catalog->team_bonuses[found].description);
}

static void add_picked_tech(DetailText *detail, const CivDesign *design, const Catalog *catalog, ControlId control)
{
    TechTier tier = (control == CONTROL_CASTLE_TECH_INFO) ? TECH_TIER_CASTLE : TECH_TIER_IMPERIAL;

    if ((control == CONTROL_CASTLE_TECH_INFO) || (control == CONTROL_IMPERIAL_TECH_INFO)) {
        add_tech(detail, catalog, catalog_find(catalog, CATALOG_TECHS, (int)tier, design->techs[tier].km_index));
    }
}

DetailText details_of(const CivDesign *design, const Catalog *catalog, ControlId control)
{
    DetailText detail;

    detail.text[0] = '\0';
    switch ((int)control) {
    case CONTROL_TREE_INFO:
        summary_add_tree(&detail, design, " buildings and ", " techs are in the tree.");
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
    case CONTROL_HERO_NAME:
        summary_add(&detail, design->texts[TEXT_FIELD_HERO_NAME]);
        break;
    default:
        add_picked_tech(&detail, design, catalog, control);
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
    summary_add(&detail, "Step ");
    add_number(&detail, (long)step + 1L);
    summary_add(&detail, " of ");
    add_number(&detail, (long)STEP_COUNT);
    summary_add(&detail, ": ");
    summary_add(&detail, wizard_page(step)->title);
    return detail;
}

DetailText details_window_title(const CivDesign *design, WizardStep step)
{
    DetailText title = details_title(step);
    DetailText detail;
    const char *name;

    detail.text[0] = '\0';
    name = civ_design_text(design, TEXT_FIELD_NAME);
    summary_add(&detail, (name[0] != '\0') ? name : "A new civ");
    summary_add(&detail, " - ");
    summary_add(&detail, title.text);
    summary_add(&detail, " - civeditor");
    return detail;
}

DetailText details_changed(int count)
{
    DetailText detail = { .text = "" };

    add_number(&detail, (long)count);
    summary_add(&detail, (count == 1) ? " civ changed" : " civs changed");
    return detail;
}

DetailText details_civ_row(const char *name, bool changed)
{
    DetailText detail = { .text = "" };

    summary_add(&detail, name);
    summary_add(&detail, changed ? "*" : "");
    return detail;
}
