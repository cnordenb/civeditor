#include "../forge/recipe_parts.h"

#include "../core/check.h"
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

    if (!CHECK(change != NULL) || !CHECK(kept_count <= KEPT_CAPACITY)) {
        return false;
    }
    for (index = 0; (index < kept_count) && (index < KEPT_CAPACITY); ++index) {
        if (unique_same_target(&kept[index], change)) {
            return true;
        }
    }
    return false;
}

static bool add_unique_change(DatEdit *edit, const DatChange *change, int *kept_count)
{
    if (!CHECK(change != NULL) || !CHECK(kept_count != NULL)) {
        return false;
    }
    if (*kept_count < KEPT_CAPACITY) {
        kept[*kept_count] = *change;
        ++*kept_count;
    }
    return dat_edit_add_change(edit, change);
}

static bool wanted_in_pass(const UniquePass *pass, UniqueAim aim, const DatChange *change)
{
    if (!CHECK(pass != NULL) || !CHECK(change != NULL)) {
        return false;
    }
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

    if (!CHECK(input != NULL) || !CHECK(pass != NULL) ||
        !CHECK(pass->copy != NULL)) {
        return false;
    }
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

    if (!CHECK(input != NULL) || !CHECK(copy != NULL) || !recipe_read_effect(input, copy->effect_id, &pass.effect)) {
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

static bool any_cost(const TechPick *pick)
{
    bool chosen = false;
    int resource;

    if (!CHECK(pick != NULL) || !CHECK(RESOURCE_KIND_COUNT > 0)) {
        return false;
    }
    for (resource = 0; resource < RESOURCE_KIND_COUNT; ++resource) {
        chosen = chosen || (pick->cost[resource] > 0);
    }
    return chosen;
}

static void clear_costs(DatTech *tech)
{
    int slot;

    if (!CHECK(tech != NULL) || !CHECK(DAT_TECH_COST_SLOTS > 0)) {
        return;
    }
    for (slot = 0; slot < DAT_TECH_COST_SLOTS; ++slot) {
        tech->costs[slot].type = DAT_NO_ID;
        tech->costs[slot].amount = 0;
        tech->costs[slot].flag = 0;
    }
}

static void set_costs(DatTech *tech, const TechPick *pick, const TechEntry *entry)
{
    const int *costs;
    int resource;
    int slot = 0;

    if (!CHECK(tech != NULL) || !CHECK(pick != NULL) || !CHECK(entry != NULL)) {
        return;
    }
    costs = any_cost(pick) ? pick->cost : entry->cost;
    clear_costs(tech);
    for (resource = 0; (resource < RESOURCE_KIND_COUNT) && (slot < DAT_TECH_COST_SLOTS); ++resource) {
        if (costs[resource] > 0) {
            tech->costs[slot].type = resource;
            tech->costs[slot].amount = costs[resource];
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

static int research_time(const TechPick *pick, const TechEntry *entry)
{
    if (!CHECK(pick != NULL) || !CHECK(entry != NULL)) {
        return DEFAULT_TECH_TIME;
    }
    if (pick->time > 0) {
        return pick->time;
    }
    return (entry->time > 0) ? entry->time : DEFAULT_TECH_TIME;
}

static bool shape_unique_tech(const RecipeInput *input, TechTier tier, DatTech *tech)
{
    const CivDesign *design;
    const Catalog *catalog;
    const TechEntry *entry;
    int found;
    int slot;

    if (!CHECK(input != NULL) || !CHECK(tech != NULL)) {
        return false;
    }
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

bool recipe_add_unique_tech(const RecipeInput *input, DatEdit *edit, RecipeReport *report, TechTier tier)
{
    RecipeUniqueCopy copy;
    DatTech tech;

    if (!CHECK(input != NULL) || !CHECK(edit != NULL) || !CHECK(report != NULL)) {
        return false;
    }
    if (!shape_unique_tech(input, tier, &tech)) {
        ++report->unique_techs_skipped;
        return true;
    }
    copy.effect_id = tech.effect_id;
    copy.swap = recipe_unique_swap(input, tech.civ);
    tech.civ = input->slot;
    if (tech.effect_id >= 0) {
        tech.effect_id = copy_unique_effect(input, edit, &copy);
    }
    report->unique_tech_ids[tier] = dat_edit_add_tech(edit, &tech);
    if (report->unique_tech_ids[tier] == DAT_NO_ID) {
        return false;
    }
    ++report->unique_techs_built;
    return true;
}
