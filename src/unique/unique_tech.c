#include "../forge/recipe_parts.h"

#include "../bonus/bonus_build.h"
#include "../mod/mod_strings.h"

enum { CASTLE_BUILDING = 82, DEFAULT_TECH_TIME = 60, PAID = 1, KEPT_CAPACITY = 512, UNIQUE_PASSES = 2, SWAP_PASS = 1 };

typedef struct {
    const RecipeUniqueCopy *copy;
    DatEffect effect;
    int number;
    int kept_count;
} UniquePass;

static const int UNIQUE_TECH_AGES[TECH_TIER_COUNT] = { RECIPE_CASTLE_AGE, RECIPE_IMPERIAL_AGE };
static const int UNIQUE_TECH_BUTTONS[TECH_TIER_COUNT] = { 7, 8 };
static DatChange kept[KEPT_CAPACITY];

static bool already_kept(const DatChange *change, int kept_count)
{
    int index;

    for (index = 0; index < kept_count; ++index) {
        if (unique_same_target(&kept[index], change)) {
            return true;
        }
    }
    return false;
}

static bool add_unique_change(DatEdit *edit, const DatChange *change, int *kept_count)
{
    if (*kept_count < KEPT_CAPACITY) {
        kept[*kept_count] = *change;
        ++*kept_count;
    }
    return dat_edit_add_change(edit, change);
}

static bool wanted_in_pass(const UniquePass *pass, UniqueAim aim, const DatChange *change)
{
    if (pass->number != SWAP_PASS) {
        return aim == UNIQUE_KEEP;
    }
    return (aim == UNIQUE_SWAP) && !already_kept(change, pass->kept_count);
}

static bool copy_unique_pass(const RecipeInput *input, DatEdit *edit, UniquePass *pass)
{
    const RecipeUniqueCopy *copy;
    RecipeChange found;
    UniqueAim aim;
    int index;
    bool fine = true;

    copy = pass->copy;
    for (index = 0; fine && (index < pass->effect.change_count) && (index < RECIPE_CHANGE_LIMIT); ++index) {
        found = recipe_change_at(input, &pass->effect, index);
        recipe_remap_change(&found.change);
        aim = unique_aim(&copy->swap, &found.change);
        fine = found.read;
        if (fine && wanted_in_pass(pass, aim, &found.change)) {
            fine = add_unique_change(edit, &found.change, &pass->kept_count);
        }
    }
    return fine;
}

static int copy_unique_effect(const RecipeInput *input, DatEdit *edit, const RecipeUniqueCopy *copy)
{
    static UniquePass pass;
    int added;
    bool fine = true;

    if (!recipe_read_effect(input, copy->effect_id, &pass.effect)) {
        return DAT_NO_ID;
    }
    pass.copy = copy;
    pass.kept_count = 0;
    added = dat_edit_add_effect(edit, pass.effect.name);
    for (pass.number = 0; fine && (pass.number < UNIQUE_PASSES); ++pass.number) {
        fine = copy_unique_pass(input, edit, &pass);
    }
    return fine ? added : DAT_NO_ID;
}

static void clear_costs(DatTech *tech)
{
    int slot;

    for (slot = 0; slot < DAT_TECH_COST_SLOTS; ++slot) {
        tech->costs[slot].type = DAT_NO_ID;
        tech->costs[slot].amount = 0;
        tech->costs[slot].flag = 0;
    }
}

static void set_costs(DatTech *tech, const TechPick *pick, const TechEntry *entry)
{
    int resource;
    int slot = 0;
    int cost;

    clear_costs(tech);
    for (resource = 0; (resource < RESOURCE_KIND_COUNT) && (slot < DAT_TECH_COST_SLOTS); ++resource) {
        cost = civ_override_or(pick->values[resource], entry->cost[resource]);
        if (cost > 0) {
            tech->costs[slot].type = resource;
            tech->costs[slot].amount = cost;
            tech->costs[slot].flag = PAID;
            ++slot;
        }
    }
}

static void place_in_castle(DatTech *tech, TechTier tier, int time)
{
    long hot_key;

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

    sid = catalog_string_id(input->catalog, input->slot, (CivString)tier);
    if (sid == CATALOG_MISSING) {
        return;
    }
    tech->language_dll_name = sid;
    tech->language_dll_description = sid + MOD_LABEL_OFFSET;
    tech->language_dll_help = sid + MOD_TOOLTIP_OFFSET;
    tech->language_dll_tech_tree = sid + MOD_TECH_TREE_OFFSET;
}

static int research_time(const TechPick *pick, const TechEntry *entry)
{
    return civ_override_or(pick->values[TECH_VALUE_TIME], (entry->time > 0) ? entry->time : DEFAULT_TECH_TIME);
}

static bool shape_unique_tech(const RecipeInput *input, TechTier tier, DatTech *tech)
{
    const CivDesign *design;
    const Catalog *catalog;
    const TechEntry *entry;
    int found;
    int slot;

    design = input->design;
    catalog = input->catalog;
    found = catalog_find(catalog, CATALOG_TECHS, (int)tier, design->techs[tier].km_index);
    if (found == CATALOG_MISSING) {
        return false;
    }
    entry = &catalog->techs[found];
    if ((entry->effects.count == 0) || !recipe_read_tech(input, entry->effects.values[0], tech)) {
        return false;
    }
    for (slot = 1; slot < DAT_REQUIRED_TECH_SLOTS; ++slot) {
        tech->required_techs[slot] = DAT_NO_ID;
    }
    tech->required_techs[0] = UNIQUE_TECH_AGES[tier];
    tech->required_tech_count = 1;
    tech->repeatable = 1;
    set_costs(tech, &design->techs[tier], entry);
    place_in_castle(tech, tier, research_time(&design->techs[tier], entry));
    name_unique_tech(input, tier, tech);
    return true;
}

static bool copy_tech_effect(const RecipeInput *input, DatEdit *edit, int tech_id, double times)
{
    DatTech tech;
    DatEffect effect;

    return !recipe_read_tech(input, tech_id, &tech) || !recipe_read_effect(input, tech.effect_id, &effect) ||
           recipe_copy_changes(input, edit, &effect, true, times);
}

static bool add_effect(const RecipeInput *input, DatEdit *edit, const BonusPick *pick)
{
    const Catalog *catalog = input->catalog;
    int found = catalog_find_bonus(catalog, false, pick->id);
    double times = civ_multiplier_times(pick->multiplier);
    BonusBuild build = { catalog, { input->dat, input->map, edit, input->slot } };
    int tech;
    bool fine = true;

    if (found == CATALOG_MISSING) {
        return true;
    }
    for (tech = 0; fine && (tech < catalog->bonuses[found].effects.count); ++tech) {
        fine = copy_tech_effect(input, edit, catalog->bonuses[found].effects.values[tech], times);
    }
    return fine && (bonus_build_lines(&build, pick->id, false, times) != BONUS_BUILD_FAILED);
}

static int build_effects(const RecipeInput *input, DatEdit *edit, const TechPick *pick)
{
    int added = dat_edit_add_effect(edit, "Unique tech effects");
    int index;
    bool fine = added != DAT_NO_ID;

    for (index = 0; fine && (index < pick->effect_count); ++index) {
        fine = add_effect(input, edit, &pick->effects[index]);
    }
    return fine ? added : DAT_NO_ID;
}

bool recipe_add_unique_tech(const RecipeInput *input, DatEdit *edit, RecipeReport *report, TechTier tier)
{
    const CivDesign *design = input->design;
    RecipeUniqueCopy copy;
    DatTech tech;

    if (!shape_unique_tech(input, tier, &tech)) {
        ++report->unique_techs_skipped;
        return true;
    }
    copy.effect_id = tech.effect_id;
    copy.swap = recipe_unique_swap(input, tech.civ);
    tech.civ = input->slot;
    if (design->techs[tier].effect_count > 0) {
        tech.effect_id = build_effects(input, edit, &design->techs[tier]);
    } else if (tech.effect_id >= 0) {
        tech.effect_id = copy_unique_effect(input, edit, &copy);
    }
    report->unique_tech_ids[tier] = dat_edit_add_tech(edit, &tech);
    if (report->unique_tech_ids[tier] == DAT_NO_ID) {
        return false;
    }
    ++report->unique_techs_built;
    return true;
}
