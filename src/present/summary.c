#include "summary.h"

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

void summary_add(SummaryText *summary, const char *text)
{
    (void)text_append(summary->text, sizeof summary->text, text);
}

static void add_line(SummaryText *summary, const char *label, const char *value)
{
    summary_add(summary, label);
    summary_add(summary, (value[0] != '\0') ? value : NOTHING);
    summary_add(summary, NEW_LINE);
}

static void add_number(SummaryText *summary, const char *before, int value)
{
    char digits[TEXT_NUMBER_CAPACITY] = "";

    (void)text_append_number(digits, sizeof digits, value);
    summary_add(summary, before);
    summary_add(summary, digits);
}

static void add_choice(SummaryText *summary, const Catalog *catalog, AppearanceKind kind, int value)
{
    const char *name;

    name = catalog_appearance_name(catalog, kind, value);
    summary_add(summary, name);
    if (name[0] == '\0') {
        add_number(summary, "#", value);
    }
}

static void add_looks(SummaryText *summary, const CivDesign *design, const Catalog *catalog)
{
    int choice;

    summary_add(summary, "Looks: ");
    for (choice = 0; choice < LOOK_COUNT; ++choice) {
        summary_add(summary, LOOK_LABELS[choice]);
        add_choice(summary, catalog, LOOK_KINDS[choice], design->choices[choice]);
    }
    summary_add(summary, "\r\nReplaces in the game: ");
    add_choice(summary, catalog, APPEARANCE_CIV, design->choices[CHOICE_REPLACE]);
    summary_add(summary, NEW_LINE);
}

static void add_bonuses(SummaryText *summary, const CivDesign *design, const Catalog *catalog)
{
    int pick;
    int found;

    add_number(summary, "\r\nCiv bonuses: ", design->bonus_count);
    add_number(summary, " of ", CIV_BONUS_CAPACITY);
    summary_add(summary, NEW_LINE);
    for (pick = 0; (pick < design->bonus_count) && (pick < CIV_BONUS_CAPACITY); ++pick) {
        found = catalog_find_bonus(catalog, false, design->bonuses[pick].id);
        summary_add(summary, "- ");
        if (found == CATALOG_MISSING) {
            add_number(summary, "bonus #", design->bonuses[pick].id);
        } else {
            summary_add(summary, catalog->bonuses[found].description);
        }
        summary_add(summary, NEW_LINE);
    }
    found = (design->team_bonus_count > 0) ? catalog_find_bonus(catalog, true, design->team_bonuses[0].id)
                                           : CATALOG_MISSING;
    add_line(summary, "Team bonus: ", (found == CATALOG_MISSING) ? "" : catalog->team_bonuses[found].description);
}

static const char *unit_name(const CivDesign *design, const Catalog *catalog)
{
    UnitRole role;
    int found;

    if (design->unique_unit.origin == UNIT_ORIGIN_NONE) {
        return "";
    }
    role = (design->unique_unit.origin == UNIT_ORIGIN_CUSTOM) ? UNIT_ROLE_CUSTOM : UNIT_ROLE_VANILLA;
    found = catalog_find(catalog, CATALOG_UNITS, (int)role, design->unique_unit.id);
    return (found == CATALOG_MISSING) ? "an unknown unit" : catalog->units[found].name;
}

static const char *tech_name(const CivDesign *design, const Catalog *catalog, TechTier tier)
{
    int found;

    if (design->techs[tier].km_index == CIV_NO_CHOICE) {
        return "";
    }
    found = catalog_find(catalog, CATALOG_TECHS, (int)tier, design->techs[tier].km_index);
    return (found == CATALOG_MISSING) ? "an unknown tech" : catalog->techs[found].name;
}

static void add_named(SummaryText *summary, const char *given, const char *base)
{
    summary_add(summary, (given[0] != '\0') ? given : ((base[0] != '\0') ? base : NOTHING));
    if ((given[0] != '\0') && (base[0] != '\0')) {
        summary_add(summary, ", from ");
        summary_add(summary, base);
    }
    summary_add(summary, NEW_LINE);
}

void summary_add_tree(SummaryText *summary, const CivDesign *design, const char *before_techs,
                      const char *after_techs)
{
    add_number(summary, "", design->tree[TREE_UNITS].count);
    add_number(summary, " units, ", design->tree[TREE_BUILDINGS].count);
    add_number(summary, before_techs, design->tree[TREE_TECHS].count);
    summary_add(summary, after_techs);
}

static void add_when(SummaryText *summary, bool wanted, const char *text)
{
    if (!wanted) {
        return;
    }
    summary_add(summary, text);
}

static void add_missing(SummaryText *summary, const CivDesign *design)
{
    if (civ_design_is_complete(design)) {
        return;
    }
    summary_add(summary, "\r\nStill needed before a mod can be built:");
    add_when(summary, design->texts[TEXT_FIELD_NAME][0] == '\0', " a name;");
    add_when(summary, design->unique_unit.origin == UNIT_ORIGIN_NONE, " a unique unit;");
    add_when(summary, design->techs[TECH_TIER_CASTLE].km_index == CIV_NO_CHOICE, " a castle tech;");
    add_when(summary, design->techs[TECH_TIER_IMPERIAL].km_index == CIV_NO_CHOICE, " an imperial tech;");
    add_when(summary, civ_design_hero_lacks_name(design), " a name for the hero;");
    summary_add(summary, NEW_LINE);
}

bool summary_write(SummaryText *summary, const CivDesign *design, const Catalog *catalog)
{
    int hero;

    summary->text[0] = '\0';
    add_line(summary, "Name: ", design->texts[TEXT_FIELD_NAME]);
    add_line(summary, "Tagline: ", design->texts[TEXT_FIELD_TAGLINE]);
    add_line(summary, "Description: ", design->texts[TEXT_FIELD_DESCRIPTION]);
    add_looks(summary, design, catalog);
    summary_add(summary, "Tech tree: ");
    summary_add_tree(summary, design, " buildings, ", " techs\r\n");
    add_bonuses(summary, design, catalog);
    summary_add(summary, NEW_LINE);
    summary_add(summary, "Unique unit: ");
    add_named(summary, design->texts[TEXT_FIELD_UNIT_NAME], unit_name(design, catalog));
    summary_add(summary, "Castle tech: ");
    add_named(summary, design->texts[TEXT_FIELD_CASTLE_TECH_NAME],
              tech_name(design, catalog, TECH_TIER_CASTLE));
    summary_add(summary, "Imperial tech: ");
    add_named(summary, design->texts[TEXT_FIELD_IMPERIAL_TECH_NAME],
              tech_name(design, catalog, TECH_TIER_IMPERIAL));
    hero = catalog_find(catalog, CATALOG_UNITS, UNIT_ROLE_HERO, design->hero_unit);
    summary_add(summary, "Hero: ");
    add_named(summary, design->texts[TEXT_FIELD_HERO_NAME],
              (hero == CATALOG_MISSING) ? "" : catalog->units[hero].name);
    add_missing(summary, design);
    return text_length(summary->text, sizeof summary->text) + 1U < sizeof summary->text;
}

static void add_report(SummaryText *summary, const char *name, const RecipeReport *report)
{
    summary_add(summary, "\n\n");
    summary_add(summary, name);
    add_number(summary, "\nCiv bonuses built: ", report->bonuses_built);
    add_number(summary, " of ", report->bonuses_built + report->bonuses_skipped);
    summary_add(summary, (report->bonuses_skipped > 0) ? "; the others are not built yet." : ".");
    add_when(summary, report->team_bonuses_skipped > 0, "\nThe team bonus is not built yet.");
    add_when(summary, report->multipliers_skipped > 0, "\nA bonus made of the game's techs counts only once.");
    add_when(summary, report->unit_skipped, "\nOnly the game's own unique units are built.");
    add_when(summary, report->unique_techs_skipped > 0, "\nA unique tech is not built yet.");
    add_when(summary, report->tree_overflowed, "\nThe tech tree needed too many changes.");
    add_when(summary, report->hero_skipped, "\nThe hero is not built: the civ lacks its unit.");
}

bool summary_write_build(SummaryText *summary, const char *prefix, const BuiltCivs *built)
{
    int civ;

    (void)text_copy(summary->text, sizeof summary->text, "The mod was built as ");
    summary_add(summary, prefix);
    summary_add(summary, "-data and ");
    summary_add(summary, prefix);
    summary_add(summary, "-ui in the game's mods folder.");
    for (civ = 0; civ < built->count; ++civ) {
        add_report(summary, civ_design_text(built->designs[civ], TEXT_FIELD_NAME), &built->reports[civ]);
    }
    summary_add(summary, "\n\nTurn on the UI mod in the game's mod manager, then pick the data mod in the lobby.");
    return text_length(summary->text, sizeof summary->text) + 1U < sizeof summary->text;
}

static bool is_whole(const RecipeReport *report)
{
    return (report->bonuses_skipped == 0) && (report->team_bonuses_skipped == 0) &&
           (report->multipliers_skipped == 0) && (report->unique_techs_skipped == 0) && !report->unit_skipped &&
           !report->tree_overflowed && !report->hero_skipped;
}

bool summary_build_is_whole(const BuiltCivs *built)
{
    int civ;

    for (civ = 0; civ < built->count; ++civ) {
        if (!is_whole(&built->reports[civ])) {
            return false;
        }
    }
    return true;
}
