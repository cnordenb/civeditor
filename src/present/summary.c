#include "summary.h"

#include "../core/check.h"
#include "../core/text.h"

enum { LOOK_LABEL_WIDTH = 16, LOOK_COUNT = (int)CHOICE_SCOUT + 1 };

static const char NEW_LINE[] = "\r\n";
static const char NOTHING[] = "none yet";

static const char LOOK_LABELS[LOOK_COUNT][LOOK_LABEL_WIDTH] = {
    "architecture ", ", language ", ", wonder ", ", castle ", ", monk ", ", scout "
};

static const AppearanceKind LOOK_KINDS[LOOK_COUNT] = {
    APPEARANCE_ARCHITECTURE, APPEARANCE_CIV, APPEARANCE_CIV, APPEARANCE_CIV, APPEARANCE_MONK, APPEARANCE_SCOUT
};

static void add(SummaryText *summary, const char *text)
{
    if (!CHECK(summary != NULL) || !CHECK(text != NULL)) {
        return;
    }
    (void)text_append(summary->text, sizeof summary->text, text);
}

static void add_line(SummaryText *summary, const char *label, const char *value)
{
    if (!CHECK(label != NULL) || !CHECK(value != NULL)) {
        return;
    }
    add(summary, label);
    add(summary, (value[0] != '\0') ? value : NOTHING);
    add(summary, NEW_LINE);
}

static void add_number(SummaryText *summary, const char *before, int value)
{
    char digits[TEXT_NUMBER_CAPACITY] = "";

    if (!CHECK(summary != NULL) || !CHECK(before != NULL)) {
        return;
    }
    (void)text_append_number(digits, sizeof digits, value);
    add(summary, before);
    add(summary, digits);
}

static void add_choice(SummaryText *summary, const Catalog *catalog, AppearanceKind kind, int value)
{
    const char *name;

    if (!CHECK(summary != NULL) || !CHECK(catalog != NULL)) {
        return;
    }
    name = catalog_appearance_name(catalog, kind, value);
    add(summary, name);
    if (name[0] == '\0') {
        add_number(summary, "#", value);
    }
}

static void add_looks(SummaryText *summary, const CivDesign *design, const Catalog *catalog)
{
    int choice;

    if (!CHECK(design != NULL) || !CHECK(catalog != NULL)) {
        return;
    }
    add(summary, "Looks: ");
    for (choice = 0; choice < LOOK_COUNT; ++choice) {
        add(summary, LOOK_LABELS[choice]);
        add_choice(summary, catalog, LOOK_KINDS[choice], design->choices[choice]);
    }
    add(summary, "\r\nReplaces in the game: ");
    add_choice(summary, catalog, APPEARANCE_CIV, design->choices[CHOICE_REPLACE]);
    add(summary, NEW_LINE);
}

static void add_bonuses(SummaryText *summary, const CivDesign *design, const Catalog *catalog)
{
    int pick;
    int found;

    if (!CHECK(design != NULL) || !CHECK(catalog != NULL)) {
        return;
    }
    add_number(summary, "\r\nCiv bonuses: ", design->bonus_count);
    add_number(summary, " of ", CIV_BONUS_CAPACITY);
    add(summary, NEW_LINE);
    for (pick = 0; (pick < design->bonus_count) && (pick < CIV_BONUS_CAPACITY); ++pick) {
        found = catalog_find_bonus(catalog, false, design->bonuses[pick].id);
        add(summary, "- ");
        if (found == CATALOG_MISSING) {
            add_number(summary, "bonus #", design->bonuses[pick].id);
        } else {
            add(summary, catalog->bonuses[found].description);
        }
        add(summary, NEW_LINE);
    }
    found = (design->team_bonus_count > 0) ? catalog_find_bonus(catalog, true, design->team_bonuses[0].id)
                                           : CATALOG_MISSING;
    add_line(summary, "Team bonus: ", (found == CATALOG_MISSING) ? "" : catalog->team_bonuses[found].description);
}

static const char *unit_name(const CivDesign *design, const Catalog *catalog)
{
    UnitRole role;
    int found;

    if (!CHECK(design != NULL) || !CHECK(catalog != NULL) || (design->unique_unit.origin == UNIT_ORIGIN_NONE)) {
        return "";
    }
    role = (design->unique_unit.origin == UNIT_ORIGIN_CUSTOM) ? UNIT_ROLE_CUSTOM : UNIT_ROLE_VANILLA;
    found = catalog_find_unit(catalog, role, design->unique_unit.id);
    return (found == CATALOG_MISSING) ? "an unknown unit" : catalog->units[found].name;
}

static const char *tech_name(const CivDesign *design, const Catalog *catalog, TechTier tier)
{
    int found;

    if (!CHECK(design != NULL) || !CHECK(catalog != NULL) || (design->techs[tier].km_index == CIV_NO_CHOICE)) {
        return "";
    }
    found = catalog_find_tech(catalog, tier, design->techs[tier].km_index);
    return (found == CATALOG_MISSING) ? "an unknown tech" : catalog->techs[found].name;
}

static void add_named(SummaryText *summary, const char *given, const char *base)
{
    if (!CHECK(given != NULL) || !CHECK(base != NULL)) {
        return;
    }
    add(summary, (given[0] != '\0') ? given : ((base[0] != '\0') ? base : NOTHING));
    if ((given[0] != '\0') && (base[0] != '\0')) {
        add(summary, ", from ");
        add(summary, base);
    }
    add(summary, NEW_LINE);
}

static void add_tree(SummaryText *summary, const CivDesign *design)
{
    if (!CHECK(summary != NULL) || !CHECK(design != NULL)) {
        return;
    }
    add_number(summary, "Tech tree: ", design->tree[TREE_UNITS].count);
    add_number(summary, " units, ", design->tree[TREE_BUILDINGS].count);
    add_number(summary, " buildings, ", design->tree[TREE_TECHS].count);
    add(summary, " techs\r\n");
}

static void add_missing(SummaryText *summary, const CivDesign *design)
{
    if (!CHECK(summary != NULL) || !CHECK(design != NULL) || civ_design_is_complete(design)) {
        return;
    }
    add(summary, "\r\nStill needed before a mod can be built:");
    add(summary, (design->texts[TEXT_FIELD_NAME][0] == '\0') ? " a name;" : "");
    add(summary, (design->unique_unit.origin == UNIT_ORIGIN_NONE) ? " a unique unit;" : "");
    add(summary, (design->techs[TECH_TIER_CASTLE].km_index == CIV_NO_CHOICE) ? " a castle tech;" : "");
    add(summary, (design->techs[TECH_TIER_IMPERIAL].km_index == CIV_NO_CHOICE) ? " an imperial tech;" : "");
    add(summary, civ_design_hero_lacks_name(design) ? " a name for the hero;" : "");
    add(summary, NEW_LINE);
}

bool summary_write(SummaryText *summary, const CivDesign *design, const Catalog *catalog)
{
    int hero;

    if (!CHECK(summary != NULL) || !CHECK(design != NULL) || !CHECK(catalog != NULL)) {
        return false;
    }
    summary->text[0] = '\0';
    add_line(summary, "Name: ", design->texts[TEXT_FIELD_NAME]);
    add_line(summary, "Tagline: ", design->texts[TEXT_FIELD_TAGLINE]);
    add_line(summary, "Description: ", design->texts[TEXT_FIELD_DESCRIPTION]);
    add_looks(summary, design, catalog);
    add_tree(summary, design);
    add_bonuses(summary, design, catalog);
    add(summary, NEW_LINE);
    add(summary, "Unique unit: ");
    add_named(summary, design->texts[TEXT_FIELD_UNIT_NAME], unit_name(design, catalog));
    add(summary, "Castle tech: ");
    add_named(summary, design->texts[TEXT_FIELD_CASTLE_TECH_NAME],
              tech_name(design, catalog, TECH_TIER_CASTLE));
    add(summary, "Imperial tech: ");
    add_named(summary, design->texts[TEXT_FIELD_IMPERIAL_TECH_NAME],
              tech_name(design, catalog, TECH_TIER_IMPERIAL));
    hero = catalog_find_unit(catalog, UNIT_ROLE_HERO, design->hero_unit);
    add(summary, "Hero: ");
    add_named(summary, design->texts[TEXT_FIELD_HERO_NAME],
              (hero == CATALOG_MISSING) ? "" : catalog->units[hero].name);
    add_missing(summary, design);
    return text_length(summary->text, sizeof summary->text) + 1U < sizeof summary->text;
}
