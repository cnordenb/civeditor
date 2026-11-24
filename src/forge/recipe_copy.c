#include "recipe_parts.h"

#include "../core/check.h"
#include "../bonus/bonus_scale.h"
#include "../mod/mod_strings.h"

typedef struct {
    int from[RECIPE_COPY_CAPACITY];
    int to[RECIPE_COPY_CAPACITY];
    double times[RECIPE_COPY_CAPACITY];
    int count;
} TechPlan;

enum { UNIT_FORMS = 2, REQUIRED_TECH_WIDTH = 2, REQUIREMENT_CAPACITY = 16, NO_REQUIREMENT = -1 };

typedef struct {
    BonusRequirement lines[REQUIREMENT_CAPACITY];
    int count;
} RequirementPlan;

static TechPlan plan;
static RequirementPlan requirements;

static size_t place_in(const size_t *places, int count, int id)
{
    return (id < count) ? places[id] : 0U;
}

static size_t place_of(const RecipeInput *input, bool tech, int id)
{
    const DatMap *map;

    if (id < 0) {
        return 0U;
    }
    map = input->map;
    return tech ? place_in(map->tech_at, map->tech_count, id) : place_in(map->effect_at, map->effect_count, id);
}

bool recipe_read_tech(const RecipeInput *input, int id, DatTech *tech)
{
    size_t at = place_of(input, true, id);

    return (at != 0U) && dat_tech_read(input->dat, at, tech);
}

bool recipe_read_effect(const RecipeInput *input, int id, DatEffect *effect)
{
    size_t at = place_of(input, false, id);

    return (at != 0U) && dat_effect_read(input->dat, at, effect);
}

RecipeChange recipe_change_at(const RecipeInput *input, const DatEffect *effect, int index)
{
    RecipeChange found = { { 0, 0, 0, 0, 0.0F }, false };

    found.read = dat_change_read(input->dat, effect->changes_at + ((size_t)index * DAT_CHANGE_SIZE), &found.change);
    return found;
}

static bool neutralize_tech(const RecipeInput *input, DatEdit *edit, size_t at)
{
    DatTech old;
    size_t location_at;
    int location;
    bool fine;

    if (!dat_tech_read(input->dat, at, &old)) {
        return false;
    }
    if (old.civ != input->slot) {
        return true;
    }
    fine = dat_edit_patch_i16(edit, old.civ_at, RECIPE_LOST_CIV);
    for (location = 0; location < old.research_location_count;
         ++location) {
        location_at = old.research_locations_at + ((size_t)location * DAT_RESEARCH_LOCATION_SIZE);
        fine = fine && dat_edit_patch_i16(edit, location_at, DAT_NO_ID);
    }
    return fine;
}

bool recipe_neutralize(const RecipeInput *input, DatEdit *edit)
{
    const DatMap *map;
    bool fine = true;
    int index;

    map = input->map;
    for (index = 0; (index < map->tech_count) && (index < DAT_TECH_CAPACITY); ++index) {
        fine = fine && neutralize_tech(input, edit, map->tech_at[index]);
    }
    return fine;
}

void recipe_plan_reset(void)
{
    plan.count = 0;
    requirements.count = 0;
    (void)CHECK(plan.count == 0);
}

static int plan_index(int tech_id)
{
    int index;

    if (!CHECK(plan.count >= 0) || !CHECK(plan.count <= RECIPE_COPY_CAPACITY)) {
        return -1;
    }
    for (index = 0; (index < plan.count) && (index < RECIPE_COPY_CAPACITY); ++index) {
        if (plan.from[index] == tech_id) {
            return index;
        }
    }
    return -1;
}

static bool plan_add(int tech_id, double times)
{
    if (!CHECK(tech_id >= 0) || (plan_index(tech_id) >= 0)) {
        return true;
    }
    if (!CHECK(plan.count < RECIPE_COPY_CAPACITY)) {
        return false;
    }
    plan.from[plan.count] = tech_id;
    plan.times[plan.count] = times;
    ++plan.count;
    return true;
}

static bool plan_requirements(const RecipeInput *input, int position)
{
    DatTech planned;
    DatTech required;
    int slot;
    bool fine = true;

    if (!recipe_read_tech(input, plan.from[position], &planned)) {
        return false;
    }
    for (slot = 0; slot < DAT_REQUIRED_TECH_SLOTS; ++slot) {
        if (recipe_read_tech(input, planned.required_techs[slot], &required) && (required.civ >= 0)) {
            fine = fine && plan_add(planned.required_techs[slot], 1);
        }
    }
    return fine;
}

bool recipe_plan_tech(const RecipeInput *input, int tech_id, double times)
{
    int first = plan.count;
    int position;
    bool fine;

    fine = plan_add(tech_id, times);
    for (position = first; fine && (position < plan.count) && (position < RECIPE_COPY_CAPACITY); ++position) {
        fine = plan_requirements(input, position);
    }
    if (!fine) {
        plan.count = first;
    }
    return fine;
}

static int free_slot(const DatTech *tech)
{
    int slot;

    for (slot = 0; slot < DAT_REQUIRED_TECH_SLOTS; ++slot) {
        if (tech->required_techs[slot] < 0) {
            return slot;
        }
    }
    return -1;
}

bool recipe_plan_requirement(const RecipeInput *input, const BonusRequirement *line)
{
    DatTech tech;

    if (!recipe_read_tech(input, line->tech, &tech) || (free_slot(&tech) < 0) ||
        !CHECK(requirements.count < REQUIREMENT_CAPACITY) || !recipe_plan_tech(input, line->requirement, 1)) {
        return false;
    }
    requirements.lines[requirements.count] = *line;
    ++requirements.count;
    return true;
}

static int remapped(int tech_id)
{
    int found = plan_index(tech_id);

    if (!CHECK(found < RECIPE_COPY_CAPACITY) || (found < 0)) {
        return tech_id;
    }
    return plan.to[found];
}

void recipe_remap_change(DatChange *change)
{
    int tech_id;

    if (change->type == RECIPE_DISABLE_TECH) {
        tech_id = remapped((int)change->d);
        change->d = (float)tech_id;
    } else if ((change->type == DAT_CHANGE_TECH_COST) || (change->type == DAT_CHANGE_TECH_TIME)) {
        change->a = remapped(change->a);
    }
}

bool recipe_copy_changes(const RecipeInput *input, DatEdit *edit, const DatEffect *effect, bool remap, double times)
{
    RecipeChange found;
    int index;
    bool fine = true;

    for (index = 0; fine && (index < effect->change_count) && (index < RECIPE_CHANGE_LIMIT); ++index) {
        found = recipe_change_at(input, effect, index);
        if (remap) {
            recipe_remap_change(&found.change);
        }
        fine = found.read && bonus_add_scaled(edit, &found.change, times);
    }
    return fine;
}

int recipe_copy_effect(const RecipeInput *input, DatEdit *edit, int effect_id, double times)
{
    DatEffect effect;
    int added;

    if (!recipe_read_effect(input, effect_id, &effect)) {
        return DAT_NO_ID;
    }
    added = dat_edit_add_effect(edit, effect.name);
    return recipe_copy_changes(input, edit, &effect, true, times) ? added : DAT_NO_ID;
}

const UnitEntry *recipe_design_unit(const RecipeInput *input)
{
    const CivDesign *design;
    const Catalog *catalog;
    int found;

    design = input->design;
    catalog = input->catalog;
    found = (design->unique_unit.origin == UNIT_ORIGIN_VANILLA)
                ? catalog_find(catalog, CATALOG_UNITS, UNIT_ROLE_VANILLA, design->unique_unit.id)
                : CATALOG_MISSING;
    return (found == CATALOG_MISSING) ? NULL : &catalog->units[found];
}

UniqueSwap recipe_unique_swap(const RecipeInput *input, int source_civ)
{
    UniqueSwap swap = { { DAT_NO_ID, DAT_NO_ID }, { DAT_NO_ID, DAT_NO_ID } };
    const UnitEntry *own = recipe_design_unit(input);

    swap.from = unique_unit_of_civ(input, source_civ);
    if (own != NULL) {
        swap.to = unique_unit_of_techs(input, own->make_tech, own->elite_tech);
    }
    return swap;
}

static bool name_unit_at(DatEdit *edit, size_t at, int sid)
{
    return dat_edit_patch_i32(edit, at + DAT_UNIT_NAME_AT, sid) &&
           dat_edit_patch_i32(edit, at + DAT_UNIT_CREATION_AT, sid + MOD_LABEL_OFFSET) &&
           dat_edit_patch_i32(edit, at + DAT_UNIT_HELP_AT, sid + MOD_HELP_OFFSET);
}

static bool name_units(const RecipeInput *input, DatEdit *edit, UniqueUnit unit)
{
    static const CivString KINDS[UNIT_FORMS] = { CIV_STRING_UNIT, CIV_STRING_ELITE_UNIT };
    const int units[UNIT_FORMS] = { unit.base, unit.elite };
    const DatMap *map;
    size_t at;
    int form;
    int sid;
    bool fine = true;

    map = input->map;
    for (form = 0; fine && (form < UNIT_FORMS); ++form) {
        sid = catalog_string_id(input->catalog, input->slot, KINDS[form]);
        if ((sid != CATALOG_MISSING) && dat_find_unit(input->dat, map->civ_at[input->slot], units[form], &at)) {
            fine = name_unit_at(edit, at, sid);
        }
    }
    return fine;
}

bool recipe_name_unique_unit(const RecipeInput *input, DatEdit *edit)
{
    const UnitEntry *own = recipe_design_unit(input);

    if ((own == NULL) || !mod_unit_has_own_texts(input->design)) {
        return true;
    }
    return name_units(input, edit, unique_unit_of_techs(input, own->make_tech, own->elite_tech));
}

RecipeUnitIds recipe_unique_unit_ids(const RecipeInput *input)
{
    RecipeUnitIds ids = { DAT_NO_ID, DAT_NO_ID, DAT_NO_ID };
    const UnitEntry *own = recipe_design_unit(input);
    UniqueUnit unit;

    if ((own == NULL) || (plan_index(own->make_tech) < 0)) {
        return ids;
    }
    unit = unique_unit_of_techs(input, own->make_tech, own->elite_tech);
    ids.unit = unit.base;
    ids.elite_unit = unit.elite;
    ids.elite_tech = (plan_index(own->elite_tech) < 0) ? DAT_NO_ID : remapped(own->elite_tech);
    return ids;
}

static void name_elite_tech(const RecipeInput *input, int tech_id, DatTech *copy)
{
    const UnitEntry *own = recipe_design_unit(input);
    int sid;

    if ((own == NULL) || (own->elite_tech != tech_id) || !mod_unit_has_own_texts(input->design)) {
        return;
    }
    sid = catalog_string_id(input->catalog, input->slot, CIV_STRING_ELITE_UNIT);
    if (sid == CATALOG_MISSING) {
        return;
    }
    copy->language_dll_name = sid;
    copy->language_dll_description = sid + MOD_LABEL_OFFSET;
    copy->language_dll_help = sid + MOD_HELP_OFFSET;
    copy->language_dll_tech_tree = DAT_NO_ID;
}

static bool copy_planned(const RecipeInput *input, DatEdit *edit, int position)
{
    DatTech copy;
    int slot;

    if (!recipe_read_tech(input, plan.from[position], &copy)) {
        return false;
    }
    copy.civ = input->slot;
    for (slot = 0; slot < DAT_REQUIRED_TECH_SLOTS; ++slot) {
        copy.required_techs[slot] = remapped(copy.required_techs[slot]);
    }
    if (copy.effect_id >= 0) {
        copy.effect_id = recipe_copy_effect(input, edit, copy.effect_id, plan.times[position]);
        if (copy.effect_id == DAT_NO_ID) {
            return false;
        }
    }
    name_elite_tech(input, plan.from[position], &copy);
    return CHECK(dat_edit_add_tech(edit, &copy) == plan.to[position]);
}

static bool add_requirement(const RecipeInput *input, DatEdit *edit, const BonusRequirement *line)
{
    DatTech tech;
    size_t at;
    int slot;

    if (!recipe_read_tech(input, line->tech, &tech)) {
        return false;
    }
    for (slot = 0; slot < DAT_REQUIRED_TECH_SLOTS; ++slot) {
        at = place_of(input, true, line->tech) + ((size_t)slot * REQUIRED_TECH_WIDTH);
        if ((tech.required_techs[slot] == NO_REQUIREMENT) && !dat_edit_is_patched(edit, at)) {
            return dat_edit_patch_i16(edit, at, remapped(line->requirement));
        }
    }
    return false;
}

static bool add_requirements(const RecipeInput *input, DatEdit *edit)
{
    int line;
    bool fine = true;

    for (line = 0; fine && (line < requirements.count) && (line < REQUIREMENT_CAPACITY); ++line) {
        fine = add_requirement(input, edit, &requirements.lines[line]);
    }
    return fine;
}

bool recipe_commit_plan(const RecipeInput *input, DatEdit *edit)
{
    int base;
    int position;
    bool fine = true;

    base = edit->old_tech_count + edit->tech_count;
    for (position = 0; (position < plan.count) && (position < RECIPE_COPY_CAPACITY); ++position) {
        plan.to[position] = base + position;
    }
    for (position = 0; fine && (position < plan.count) && (position < RECIPE_COPY_CAPACITY); ++position) {
        fine = copy_planned(input, edit, position);
    }
    return fine && add_requirements(input, edit);
}
