#include "recipe_parts.h"

#include "../core/check.h"
#include "../core/text.h"
#include "../bonus/bonus_build.h"
#include "../dat/dat_parts.h"
#include "../hero/hero.h"
#include "../look/look.h"
#include "../mod/mod_strings.h"

enum { CASTLE_BUILDING = 82, SCOUT_RESOURCE = 263, RESOURCE_SIZE = 4, DEFAULT_TECH_TIME = 60, PAID = 1 };

typedef struct {
    const RecipeInput *input;
    DatEdit *edit;
    DatCiv civ;
    RecipeReport *report;
    DatTarget target;
    Hero hero;
    bool has_hero;
} RecipeRun;

static const int UNIQUE_TECH_AGES[TECH_TIER_COUNT] = { RECIPE_CASTLE_AGE, RECIPE_IMPERIAL_AGE };
static const int UNIQUE_TECH_BUTTONS[TECH_TIER_COUNT] = { 7, 8 };

static RecipeChanges tree;

static bool plan_bonus_tech(const RecipeInput *input, int tech_id)
{
    DatTech tech;
    DatChange unlock;

    if (!CHECK(input != NULL) || !recipe_read_tech(input, tech_id, &tech)) {
        return false;
    }
    if (tech.civ >= 0) {
        return recipe_plan_tech(input, tech_id);
    }
    if (recipe_find_unlock(input, tech_id, &unlock)) {
        recipe_add_change(&tree, &unlock);
    }
    return true;
}

static BonusBuild bonus_build_of(const RecipeRun *run)
{
    BonusBuild build = { NULL, { NULL, NULL, NULL, 0 } };
    const RecipeInput *input;

    if (!CHECK(run != NULL) || !CHECK(run->input != NULL)) {
        return build;
    }
    input = run->input;
    build.catalog = input->catalog;
    build.target = run->target;
    return build;
}

static void plan_bonuses(const RecipeRun *run)
{
    const RecipeInput *input;
    const CivDesign *design;
    const Catalog *catalog;
    const BonusEntry *entry;
    RecipeReport *report;
    BonusBuild build = bonus_build_of(run);
    int pick;
    int effect;
    int found;
    bool fine;

    if (!CHECK(run != NULL) || !CHECK(run->report != NULL)) {
        return;
    }
    input = run->input;
    report = run->report;
    design = input->design;
    catalog = input->catalog;
    for (pick = 0; (pick < design->bonus_count) && (pick < CIV_BONUS_CAPACITY); ++pick) {
        found = catalog_find_bonus(catalog, design->bonuses[pick].id);
        if ((found == CATALOG_MISSING) || (catalog->bonuses[found].effects.count == 0)) {
            fine = (found != CATALOG_MISSING) && (bonus_build_civ(&build, design->bonuses[pick].id) > 0);
            report->bonuses_built += fine ? 1 : 0;
            report->bonuses_skipped += fine ? 0 : 1;
            continue;
        }
        entry = &catalog->bonuses[found];
        fine = true;
        for (effect = 0; fine && (effect < entry->effects.count) && (effect < TSV_LIST_LIMIT); ++effect) {
            fine = plan_bonus_tech(input, entry->effects.values[effect]);
        }
        report->bonuses_built += fine ? 1 : 0;
        report->bonuses_skipped += fine ? 0 : 1;
    }
}

static void plan_unit(const RecipeInput *input, RecipeReport *report)
{
    const CivDesign *design;
    const Catalog *catalog;
    const UnitEntry *entry;
    int found;

    if (!CHECK(input != NULL) || !CHECK(report != NULL)) {
        return;
    }
    design = input->design;
    catalog = input->catalog;
    found = (design->unique_unit.origin == UNIT_ORIGIN_VANILLA)
                ? catalog_find_unit(catalog, UNIT_ROLE_VANILLA, design->unique_unit.id)
                : CATALOG_MISSING;
    if ((found == CATALOG_MISSING) || (catalog->units[found].make_tech < 0)) {
        report->unit_skipped = true;
        return;
    }
    entry = &catalog->units[found];
    report->unit_skipped = !recipe_plan_tech(input, entry->make_tech) ||
                           ((entry->elite_tech >= 0) && !recipe_plan_tech(input, entry->elite_tech));
    report->unit_techs_built = report->unit_skipped ? 0 : ((entry->elite_tech >= 0) ? 2 : 1);
}

static int add_named_effect(const RecipeRun *run, const char *suffix)
{
    char name[DAT_NAME_CAPACITY];
    const RecipeInput *input;
    const CivDesign *design;

    if (!CHECK(run != NULL) || !CHECK(suffix != NULL)) {
        return DAT_NO_ID;
    }
    input = run->input;
    design = input->design;
    if (!text_copy(name, sizeof name, design->texts[TEXT_FIELD_NAME]) || !text_append(name, sizeof name, suffix)) {
        return DAT_NO_ID;
    }
    return dat_edit_add_effect(run->edit, name);
}

static int team_source(const RecipeInput *input)
{
    const CivDesign *design;
    const Catalog *catalog;
    const BonusEntry *entry;
    int found;

    if (!CHECK(input != NULL) || !CHECK(input->design != NULL)) {
        return DAT_NO_ID;
    }
    design = input->design;
    catalog = input->catalog;
    found = (design->team_bonus_count > 0) ? catalog_find_team_bonus(catalog, design->team_bonuses[0].id)
                                           : CATALOG_MISSING;
    entry = (found == CATALOG_MISSING) ? NULL : &catalog->team_bonuses[found];
    return ((entry == NULL) || (entry->effects.count == 0)) ? DAT_NO_ID : entry->effects.values[0];
}

static bool add_team_bonus(RecipeRun *run)
{
    const RecipeInput *input;
    const CivDesign *design;
    RecipeReport *report;
    DatEffect effect;
    RecipeChange found;
    BonusBuild build = bonus_build_of(run);
    int added = add_named_effect(run, " Team Bonus");
    int index;
    bool fine = true;

    if (!CHECK(run != NULL) || (added == DAT_NO_ID)) {
        return false;
    }
    input = run->input;
    design = input->design;
    report = run->report;
    if (recipe_read_effect(input, team_source(input), &effect) && (effect.change_count > 0)) {
        for (index = 0; fine && (index < effect.change_count) && (index < RECIPE_CHANGE_LIMIT); ++index) {
            found = recipe_change_at(input, &effect, index);
            fine = found.read && dat_edit_add_change(run->edit, &found.change);
        }
        report->team_bonuses_built = 1;
    } else if ((design->team_bonus_count > 0) && (bonus_build_team(&build, design->team_bonuses[0].id) > 0)) {
        report->team_bonuses_built = 1;
    } else {
        report->team_bonuses_skipped = (design->team_bonus_count > 0) ? 1 : 0;
    }
    return fine && dat_edit_patch_i16(run->edit, run->civ.team_bonus_id_at, added);
}

static bool add_tech_tree(RecipeRun *run)
{
    int added = add_named_effect(run, " Tech Tree");
    int index;
    bool fine = true;

    if (!CHECK(run != NULL) || (added == DAT_NO_ID)) {
        return false;
    }
    recipe_sort_changes(&tree);
    for (index = 0; fine && (index < tree.count) && (index < RECIPE_TREE_CAPACITY); ++index) {
        fine = dat_edit_add_change(run->edit, &tree.changes[index]);
    }
    return fine && dat_edit_patch_i16(run->edit, run->civ.tech_tree_id_at, added);
}

static void set_costs(DatTech *tech, const TechPick *pick, const TechEntry *entry)
{
    bool chosen = false;
    int resource;
    int slot = 0;
    int amount;

    if (!CHECK(tech != NULL) || !CHECK(pick != NULL) || !CHECK(entry != NULL)) {
        return;
    }
    for (resource = 0; resource < RESOURCE_KIND_COUNT; ++resource) {
        chosen = chosen || (pick->cost[resource] > 0);
    }
    for (resource = 0; resource < DAT_TECH_COST_SLOTS; ++resource) {
        tech->costs[resource].type = DAT_NO_ID;
        tech->costs[resource].amount = 0;
        tech->costs[resource].flag = 0;
    }
    for (resource = 0; (resource < RESOURCE_KIND_COUNT) && (slot < DAT_TECH_COST_SLOTS); ++resource) {
        amount = chosen ? pick->cost[resource] : entry->cost[resource];
        if (amount > 0) {
            tech->costs[slot].type = resource;
            tech->costs[slot].amount = amount;
            tech->costs[slot].flag = PAID;
            ++slot;
        }
    }
}

static void place_in_castle(DatTech *tech, TechTier tier, int time)
{
    long hot_key;

    if (!CHECK(tech != NULL) || !CHECK((tier == TECH_TIER_CASTLE) || (tier == TECH_TIER_IMPERIAL))) {
        return;
    }
    hot_key = (tech->research_location_count > 0) ? tech->research_locations[0].hot_key_id : 0L;
    tech->research_location_count = 1;
    tech->research_locations[0].location_id = CASTLE_BUILDING;
    tech->research_locations[0].research_time = time;
    tech->research_locations[0].button_id = UNIQUE_TECH_BUTTONS[tier];
    tech->research_locations[0].hot_key_id = hot_key;
}

static void name_unique_tech(const RecipeInput *input, TechTier tier, DatTech *tech)
{
    int sid;

    if (!CHECK(input != NULL) || !CHECK(tech != NULL)) {
        return;
    }
    sid = catalog_string_id(input->catalog, input->slot, (CivString)tier);
    if (sid == CATALOG_MISSING) {
        return;
    }
    tech->language_dll_name = sid;
    tech->language_dll_description = sid + MOD_LABEL_OFFSET;
    tech->language_dll_help = sid + MOD_TOOLTIP_OFFSET;
    tech->language_dll_tech_tree = sid + MOD_TECH_TREE_OFFSET;
}

static bool shape_unique_tech(const RecipeRun *run, TechTier tier, DatTech *tech)
{
    const RecipeInput *input;
    const CivDesign *design;
    const Catalog *catalog;
    const TechEntry *entry;
    int found;
    int slot;

    if (!CHECK(run != NULL) || !CHECK(tech != NULL)) {
        return false;
    }
    input = run->input;
    design = input->design;
    catalog = input->catalog;
    found = catalog_find_tech(catalog, tier, design->techs[tier].km_index);
    entry = (found == CATALOG_MISSING) ? NULL : &catalog->techs[found];
    if ((entry == NULL) || (entry->effects.count == 0) || !recipe_read_tech(input, entry->effects.values[0], tech)) {
        return false;
    }
    for (slot = 0; slot < DAT_REQUIRED_TECH_SLOTS; ++slot) {
        tech->required_techs[slot] = (slot == 0) ? UNIQUE_TECH_AGES[tier] : DAT_NO_ID;
    }
    tech->required_tech_count = 1;
    tech->repeatable = 1;
    set_costs(tech, &design->techs[tier], entry);
    place_in_castle(tech, tier, (design->techs[tier].time > 0) ? design->techs[tier].time
                                : ((entry->time > 0) ? entry->time : DEFAULT_TECH_TIME));
    name_unique_tech(input, tier, tech);
    return true;
}

static bool add_unique_tech(RecipeRun *run, TechTier tier)
{
    const RecipeInput *input;
    RecipeUniqueCopy copy;
    RecipeReport *report;
    DatTech tech;

    if (!CHECK(run != NULL) || !CHECK(run->report != NULL)) {
        return false;
    }
    input = run->input;
    report = run->report;
    if (!shape_unique_tech(run, tier, &tech)) {
        ++report->unique_techs_skipped;
        return true;
    }
    copy.effect_id = tech.effect_id;
    copy.swap = recipe_unique_swap(input, tech.civ);
    tech.civ = input->slot;
    if (tech.effect_id >= 0) {
        tech.effect_id = recipe_copy_unique_effect(input, run->edit, &copy);
    }
    report->unique_tech_ids[tier] = dat_edit_add_tech(run->edit, &tech);
    if (report->unique_tech_ids[tier] == DAT_NO_ID) {
        return false;
    }
    ++report->unique_techs_built;
    return true;
}

static bool set_scout(const RecipeRun *run)
{
    const RecipeInput *input;
    const CivDesign *design;
    DatCursor cursor;
    size_t at;

    if (!CHECK(run != NULL) || !CHECK(run->input != NULL)) {
        return false;
    }
    input = run->input;
    design = input->design;
    if (design->choices[CHOICE_SCOUT] < 0) {
        return true;
    }
    if (!dat_cursor_start(&cursor, input->dat, run->civ.tech_tree_id_at - DAT_SHORT_WIDTH) ||
        (dat_cursor_i16(&cursor) <= SCOUT_RESOURCE)) {
        return false;
    }
    at = run->civ.team_bonus_id_at + DAT_SHORT_WIDTH + ((size_t)SCOUT_RESOURCE * RESOURCE_SIZE);
    return dat_edit_patch_f32(run->edit, at, (float)design->choices[CHOICE_SCOUT]);
}

static bool build_looks(const RecipeRun *run)
{
    const RecipeInput *input;
    LookSources sources;

    if (!CHECK(run != NULL) || !CHECK(run->input != NULL)) {
        return false;
    }
    input = run->input;
    return look_sources_of(input->catalog, input->design, &sources) && look_build(&run->target, &sources);
}

static void plan_hero(RecipeRun *run)
{
    const RecipeInput *input;
    const CivDesign *design;
    RecipeReport *report;
    DatChange hidden;

    if (!CHECK(run != NULL) || !CHECK(run->input != NULL)) {
        return;
    }
    input = run->input;
    design = input->design;
    report = run->report;
    run->has_hero = (design->hero_unit >= 0) && hero_find(&run->target, design->hero_unit, &run->hero);
    report->hero_skipped = (design->hero_unit >= 0) && !run->has_hero;
    if (run->has_hero) {
        run->hero.name_sid = (design->texts[TEXT_FIELD_HERO_NAME][0] == '\0')
                                 ? HERO_UNNAMED
                                 : catalog_string_id(input->catalog, input->slot, CIV_STRING_HERO);
        hidden = hero_hidden(&run->hero);
        recipe_add_change(&tree, &hidden);
    }
}

static bool start_run(const RecipeInput *input, DatEdit *edit, RecipeRun *run)
{
    const DatMap *map;

    if (!CHECK(input != NULL) || !CHECK(run != NULL)) {
        return false;
    }
    map = input->map;
    if ((input->slot <= 0) || (input->slot >= map->civ_count) || (input->slot >= DAT_CIV_CAPACITY)) {
        return false;
    }
    tree.count = 0;
    tree.overflowed = false;
    recipe_plan_reset();
    run->input = input;
    run->edit = edit;
    run->target = (DatTarget){ input->dat, map, edit, input->slot };
    run->has_hero = false;
    return dat_civ_read(input->dat, map->civ_at[input->slot], &run->civ);
}

bool recipe_apply(const RecipeInput *input, DatEdit *edit, RecipeReport *report)
{
    RecipeReport empty = { 0, 0, 0, 0, 0, 0, 0, 0, false, false, false, { DAT_NO_ID, DAT_NO_ID } };
    RecipeRun run;

    if (!CHECK(edit != NULL) || !CHECK(report != NULL) || !start_run(input, edit, &run)) {
        return false;
    }
    *report = empty;
    run.report = report;
    plan_bonuses(&run);
    plan_unit(input, report);
    plan_hero(&run);
    if (!recipe_wire_tree(input, &tree, report)) {
        return false;
    }
    report->tree_overflowed = tree.overflowed;
    return recipe_neutralize(input, edit) && add_team_bonus(&run) && add_tech_tree(&run) &&
           recipe_commit_plan(input, edit) && add_unique_tech(&run, TECH_TIER_CASTLE) &&
           add_unique_tech(&run, TECH_TIER_IMPERIAL) && set_scout(&run) && recipe_name_unique_unit(input, edit) &&
           build_looks(&run) && (!run.has_hero || hero_build(&run.target, &run.hero));
}
