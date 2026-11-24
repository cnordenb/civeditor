#include "recipe_parts.h"

#include "../core/text.h"
#include "../bonus/bonus_build.h"
#include "../dat/dat_parts.h"
#include "../hero/hero.h"
#include "../look/look.h"
#include "../mod/mod_strings.h"

enum { SCOUT_RESOURCE = 263, RESOURCE_SIZE = 4 };

typedef struct {
    const RecipeInput *input;
    DatEdit *edit;
    DatCiv civ;
    RecipeReport *report;
    DatTarget target;
    Hero hero;
    bool has_hero;
} RecipeRun;


static RecipeChanges tree;

static bool plan_bonus_tech(const RecipeInput *input, int tech_id, double times, bool *shared)
{
    DatTech tech;
    DatChange unlock;

    if (!recipe_read_tech(input, tech_id, &tech)) {
        return false;
    }
    if (tech.civ >= 0) {
        return recipe_plan_tech(input, tech_id, times);
    }
    *shared = true;
    if (recipe_find_unlock(input, tech_id, &unlock)) {
        recipe_add_change(&tree, &unlock);
    }
    return true;
}

static BonusBuild bonus_build_of(const RecipeRun *run)
{
    BonusBuild build = { NULL, { NULL, NULL, NULL, 0 } };
    const RecipeInput *input;

    input = run->input;
    build.catalog = input->catalog;
    build.target = run->target;
    return build;
}

static bool plan_bonus_techs(const RecipeInput *input, const BonusEntry *entry, double times, bool *shared)
{
    int effect;
    bool fine = true;

    for (effect = 0; fine && (effect < entry->effects.count) && (effect < TSV_LIST_LIMIT); ++effect) {
        fine = plan_bonus_tech(input, entry->effects.values[effect], times, shared);
    }
    return fine;
}

static int plan_bonus_requirements(const RecipeInput *input, int bonus_id)
{
    const Catalog *catalog;
    int line;
    int planned = 0;

    catalog = input->catalog;
    for (line = 0; (line < catalog->requirement_count) && (line < CATALOG_REQUIREMENT_CAPACITY); ++line) {
        if (catalog->requirements[line].bonus_id != bonus_id) {
            continue;
        }
        if (!recipe_plan_requirement(input, &catalog->requirements[line])) {
            return BONUS_BUILD_FAILED;
        }
        ++planned;
    }
    return planned;
}

static int build_bonus_lines(const RecipeRun *run, const BonusBuild *build, int bonus_id, double times)
{
    int built = bonus_build_civ(build, bonus_id, times);
    int required;

    if (built == BONUS_BUILD_FAILED) {
        return BONUS_BUILD_FAILED;
    }
    required = plan_bonus_requirements(run->input, bonus_id);
    if (required == BONUS_BUILD_FAILED) {
        return BONUS_BUILD_FAILED;
    }
    return built + required;
}

static bool plan_bonus(const RecipeRun *run, const BonusBuild *build, const BonusPick *pick)
{
    const RecipeInput *input;
    const Catalog *catalog;
    RecipeReport *report;
    double times = civ_multiplier_times(pick->multiplier);
    bool shared = false;
    bool planned;
    int found;

    input = run->input;
    report = run->report;
    catalog = input->catalog;
    found = catalog_find_bonus(catalog, false, pick->id);
    if (found == CATALOG_MISSING) {
        return false;
    }
    if (catalog->bonuses[found].effects.count == 0) {
        return build_bonus_lines(run, build, pick->id, times) > 0;
    }
    planned = plan_bonus_techs(input, &catalog->bonuses[found], times, &shared);
    if (shared && (pick->multiplier.millionths > CIV_MILLIONTHS)) {
        ++report->multipliers_skipped;
    }
    return planned && (build_bonus_lines(run, build, pick->id, times) >= 0);
}

static void plan_bonuses(const RecipeRun *run)
{
    const RecipeInput *input;
    const CivDesign *design;
    RecipeReport *report;
    BonusBuild build = bonus_build_of(run);
    int pick;

    input = run->input;
    report = run->report;
    design = input->design;
    for (pick = 0; (pick < design->bonus_count) && (pick < CIV_BONUS_CAPACITY); ++pick) {
        if (plan_bonus(run, &build, &design->bonuses[pick])) {
            ++report->bonuses_built;
        } else {
            ++report->bonuses_skipped;
        }
    }
}

static bool plan_unit_techs(const RecipeInput *input, const UnitEntry *entry)
{
    return recipe_plan_tech(input, entry->make_tech, 1) &&
           ((entry->elite_tech < 0) || recipe_plan_tech(input, entry->elite_tech, 1));
}

static void plan_unit(const RecipeInput *input, RecipeReport *report)
{
    const UnitEntry *entry = recipe_design_unit(input);

    report->unit_skipped = (entry == NULL) || (entry->make_tech < 0) || !plan_unit_techs(input, entry);
    if (!report->unit_skipped) {
        report->unit_techs_built = (entry->elite_tech >= 0) ? 2 : 1;
    }
}

static int add_named_effect(const RecipeRun *run, const char *suffix)
{
    char name[DAT_NAME_CAPACITY];
    const RecipeInput *input;
    const CivDesign *design;

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
    int found;

    design = input->design;
    catalog = input->catalog;
    if (design->team_bonus_count == 0) {
        return DAT_NO_ID;
    }
    found = catalog_find_bonus(catalog, true, design->team_bonuses[0].id);
    if ((found == CATALOG_MISSING) || (catalog->team_bonuses[found].effects.count == 0)) {
        return DAT_NO_ID;
    }
    return catalog->team_bonuses[found].effects.values[0];
}

static void build_team_bonus_from_changes(const RecipeRun *run)
{
    const RecipeInput *input;
    const CivDesign *design;
    RecipeReport *report;
    BonusBuild build = bonus_build_of(run);
    double times;

    input = run->input;
    design = input->design;
    report = run->report;
    if (design->team_bonus_count == 0) {
        return;
    }
    times = civ_multiplier_times(design->team_bonuses[0].multiplier);
    if (bonus_build_lines(&build, design->team_bonuses[0].id, true, times) > 0) {
        report->team_bonuses_built = 1;
    } else {
        report->team_bonuses_skipped = 1;
    }
}

static bool add_team_bonus(RecipeRun *run)
{
    const RecipeInput *input;
    const CivDesign *design;
    RecipeReport *report;
    DatEffect effect;
    int added = add_named_effect(run, " Team Bonus");
    bool fine = true;

    if (added == DAT_NO_ID) {
        return false;
    }
    input = run->input;
    design = input->design;
    report = run->report;
    if (recipe_read_effect(input, team_source(input), &effect) && (effect.change_count > 0)) {
        fine = recipe_copy_changes(input, run->edit, &effect, false,
                                   civ_multiplier_times(design->team_bonuses[0].multiplier));
        report->team_bonuses_built = 1;
    } else {
        build_team_bonus_from_changes(run);
    }
    return fine && dat_edit_patch_i16(run->edit, run->civ.team_bonus_id_at, added);
}

static bool add_tech_tree(RecipeRun *run)
{
    int added = add_named_effect(run, " Tech Tree");
    int index;
    bool fine = true;

    if (added == DAT_NO_ID) {
        return false;
    }
    recipe_sort_changes(&tree);
    for (index = 0; fine && (index < tree.count) && (index < RECIPE_TREE_CAPACITY); ++index) {
        fine = dat_edit_add_change(run->edit, &tree.changes[index]);
    }
    return fine && dat_edit_patch_i16(run->edit, run->civ.tech_tree_id_at, added);
}

static bool set_scout(const RecipeRun *run)
{
    const RecipeInput *input;
    const CivDesign *design;
    DatCursor cursor;
    size_t at;

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

    input = run->input;
    return look_sources_of(input->catalog, input->design, &sources) && look_build(&run->target, &sources) &&
           look_dress_units(&run->target, input->catalog, input->design);
}

static void plan_hero(RecipeRun *run)
{
    const RecipeInput *input;
    const CivDesign *design;
    RecipeReport *report;
    DatChange hidden;

    input = run->input;
    design = input->design;
    report = run->report;
    if (design->hero_unit < 0) {
        return;
    }
    run->has_hero = hero_find(&run->target, design->hero_unit, &run->hero);
    report->hero_skipped = !run->has_hero;
    if (run->has_hero) {
        run->hero.name_sid = civ_design_hero_lacks_name(design)
                                 ? HERO_UNNAMED
                                 : catalog_string_id(input->catalog, input->slot, CIV_STRING_HERO);
        hidden = hero_hidden(&run->hero);
        recipe_add_change(&tree, &hidden);
    }
}

static bool start_run(const RecipeInput *input, DatEdit *edit, RecipeRun *run)
{
    const DatMap *map;

    map = input->map;
    if ((input->slot <= 0) || (input->slot >= map->civ_count)) {
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

static bool plan_all(RecipeRun *run)
{
    RecipeReport *report;
    bool wired;

    report = run->report;
    plan_bonuses(run);
    plan_unit(run->input, report);
    plan_hero(run);
    wired = recipe_wire_tree(run->input, &tree, report);
    report->tree_overflowed = wired && tree.overflowed;
    return wired;
}

static bool add_civ_effects(RecipeRun *run)
{
    return recipe_neutralize(run->input, run->edit) && add_team_bonus(run) && add_tech_tree(run);
}

static bool add_civ_techs(RecipeRun *run)
{
    return recipe_commit_plan(run->input, run->edit) &&
           recipe_add_unique_tech(run->input, run->edit, run->report, TECH_TIER_CASTLE) &&
           recipe_add_unique_tech(run->input, run->edit, run->report, TECH_TIER_IMPERIAL);
}

static bool change_units(const RecipeRun *run)
{
    RecipeReport *report = run->report;

    if (!set_scout(run) || !recipe_name_unique_unit(run->input, run->edit) || !build_looks(run)) {
        return false;
    }
    report->unit_stats_skipped = unique_build_stats(run->input, &run->target);
    return !run->has_hero || hero_build(&run->target, &run->hero);
}

bool recipe_apply(const RecipeInput *input, DatEdit *edit, RecipeReport *report)
{
    RecipeReport empty = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, false, false, false, { DAT_NO_ID, DAT_NO_ID } };
    RecipeRun run;

    if (!start_run(input, edit, &run)) {
        return false;
    }
    *report = empty;
    run.report = report;
    return plan_all(&run) && add_civ_effects(&run) && add_civ_techs(&run) && change_units(&run);
}
