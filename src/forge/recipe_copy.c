#include "recipe_parts.h"

#include "../core/check.h"
#include "../mod/mod_strings.h"

typedef struct {
    int from[RECIPE_COPY_CAPACITY];
    int to[RECIPE_COPY_CAPACITY];
    int count;
} TechPlan;

enum { KEPT_CAPACITY = 512, UNIQUE_PASSES = 2, SWAP_PASS = 1, UNIT_FORMS = 2 };

static TechPlan plan;
static DatChange kept[KEPT_CAPACITY];

bool recipe_read_tech(const RecipeInput *input, int id, DatTech *tech)
{
    const DatMap *map;

    if (!CHECK(input != NULL) || !CHECK(tech != NULL)) {
        return false;
    }
    map = input->map;
    if ((id < 0) || (id >= map->tech_count) || (id >= DAT_TECH_CAPACITY)) {
        return false;
    }
    return dat_tech_read(input->dat, map->tech_at[id], tech);
}

bool recipe_read_effect(const RecipeInput *input, int id, DatEffect *effect)
{
    const DatMap *map;

    if (!CHECK(input != NULL) || !CHECK(effect != NULL)) {
        return false;
    }
    map = input->map;
    if ((id < 0) || (id >= map->effect_count) || (id >= DAT_EFFECT_CAPACITY)) {
        return false;
    }
    return dat_effect_read(input->dat, map->effect_at[id], effect);
}

RecipeChange recipe_change_at(const RecipeInput *input, const DatEffect *effect, int index)
{
    RecipeChange found = { { 0, 0, 0, 0, 0.0F }, false };

    if (!CHECK(input != NULL) || !CHECK(effect != NULL) || (index < 0) || (index >= effect->change_count)) {
        return found;
    }
    found.read = dat_change_read(input->dat, effect->changes_at + ((size_t)index * DAT_CHANGE_SIZE), &found.change);
    return found;
}

static bool neutralize_tech(const RecipeInput *input, DatEdit *edit, size_t at)
{
    DatTech old;
    size_t location_at;
    int location;
    bool fine;

    if (!CHECK(input != NULL) || !CHECK(edit != NULL) || !dat_tech_read(input->dat, at, &old)) {
        return false;
    }
    if (old.civ != input->slot) {
        return true;
    }
    fine = dat_edit_patch_i16(edit, old.civ_at, RECIPE_LOST_CIV);
    for (location = 0; (location < old.research_location_count) && (location < DAT_RESEARCH_LOCATION_CAPACITY);
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

    if (!CHECK(input != NULL) || !CHECK(edit != NULL)) {
        return false;
    }
    map = input->map;
    for (index = 0; (index < map->tech_count) && (index < DAT_TECH_CAPACITY); ++index) {
        fine = fine && neutralize_tech(input, edit, map->tech_at[index]);
    }
    return fine;
}

void recipe_plan_reset(void)
{
    plan.count = 0;
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

static bool plan_add(int tech_id)
{
    if (!CHECK(tech_id >= 0) || (plan_index(tech_id) >= 0)) {
        return true;
    }
    if (!CHECK(plan.count < RECIPE_COPY_CAPACITY)) {
        return false;
    }
    plan.from[plan.count] = tech_id;
    ++plan.count;
    return true;
}

static bool plan_requirements(const RecipeInput *input, int position)
{
    DatTech planned;
    DatTech required;
    int slot;
    bool fine = true;

    if (!CHECK(input != NULL) || !recipe_read_tech(input, plan.from[position], &planned)) {
        return false;
    }
    for (slot = 0; slot < DAT_REQUIRED_TECH_SLOTS; ++slot) {
        if (recipe_read_tech(input, planned.required_techs[slot], &required) && (required.civ >= 0)) {
            fine = fine && plan_add(planned.required_techs[slot]);
        }
    }
    return fine;
}

bool recipe_plan_tech(const RecipeInput *input, int tech_id)
{
    int first = plan.count;
    int position;
    bool fine;

    if (!CHECK(input != NULL) || !CHECK(tech_id >= 0)) {
        return false;
    }
    fine = plan_add(tech_id);
    for (position = first; fine && (position < plan.count) && (position < RECIPE_COPY_CAPACITY); ++position) {
        fine = plan_requirements(input, position);
    }
    return fine;
}

static int remapped(int tech_id)
{
    int found = plan_index(tech_id);

    if (!CHECK(found < RECIPE_COPY_CAPACITY) || (found < 0)) {
        return tech_id;
    }
    return plan.to[found];
}

static void remap_change(DatChange *change)
{
    int tech_id;

    if (!CHECK(change != NULL) || !CHECK(plan.count <= RECIPE_COPY_CAPACITY)) {
        return;
    }
    if (change->type == RECIPE_DISABLE_TECH) {
        tech_id = remapped((int)change->d);
        change->d = (float)tech_id;
    } else if ((change->type == RECIPE_TECH_COST) || (change->type == RECIPE_TECH_TIME)) {
        change->a = remapped(change->a);
    }
}

int recipe_copy_effect(const RecipeInput *input, DatEdit *edit, int effect_id)
{
    DatEffect effect;
    RecipeChange found;
    int added;
    int index;
    bool fine = true;

    if (!CHECK(input != NULL) || !CHECK(edit != NULL) || !recipe_read_effect(input, effect_id, &effect)) {
        return DAT_NO_ID;
    }
    added = dat_edit_add_effect(edit, effect.name);
    for (index = 0; fine && (index < effect.change_count) && (index < RECIPE_CHANGE_LIMIT); ++index) {
        found = recipe_change_at(input, &effect, index);
        remap_change(&found.change);
        fine = found.read && dat_edit_add_change(edit, &found.change);
    }
    return (fine && (added != DAT_NO_ID)) ? added : DAT_NO_ID;
}

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

int recipe_copy_unique_effect(const RecipeInput *input, DatEdit *edit, const RecipeUniqueCopy *copy)
{
    DatEffect effect;
    RecipeChange found;
    UniqueAim aim;
    int kept_count = 0;
    int added;
    int pass;
    int index;
    bool fine = true;

    if (!CHECK(input != NULL) || !CHECK(copy != NULL) || !recipe_read_effect(input, copy->effect_id, &effect)) {
        return DAT_NO_ID;
    }
    added = dat_edit_add_effect(edit, effect.name);
    for (pass = 0; fine && (pass < UNIQUE_PASSES); ++pass) {
        for (index = 0; fine && (index < effect.change_count) && (index < RECIPE_CHANGE_LIMIT); ++index) {
            found = recipe_change_at(input, &effect, index);
            remap_change(&found.change);
            aim = unique_aim(&copy->swap, &found.change);
            fine = found.read;
            if (fine && (pass != SWAP_PASS) && (aim == UNIQUE_KEEP)) {
                fine = add_unique_change(edit, &found.change, &kept_count);
            } else if (fine && (pass == SWAP_PASS) && (aim == UNIQUE_SWAP) &&
                       !already_kept(&found.change, kept_count)) {
                fine = add_unique_change(edit, &found.change, &kept_count);
            }
        }
    }
    return (fine && (added != DAT_NO_ID)) ? added : DAT_NO_ID;
}

static const UnitEntry *design_unit(const RecipeInput *input)
{
    const CivDesign *design;
    const Catalog *catalog;
    int found;

    if (!CHECK(input != NULL) || !CHECK((input->design != NULL) && (input->catalog != NULL))) {
        return NULL;
    }
    design = input->design;
    catalog = input->catalog;
    found = (design->unique_unit.origin == UNIT_ORIGIN_VANILLA)
                ? catalog_find_unit(catalog, UNIT_ROLE_VANILLA, design->unique_unit.id)
                : CATALOG_MISSING;
    return (found == CATALOG_MISSING) ? NULL : &catalog->units[found];
}

static UniqueSource source_of(const RecipeInput *input)
{
    UniqueSource source = { NULL, NULL, NULL };

    if (!CHECK(input != NULL) || !CHECK(input->map != NULL)) {
        return source;
    }
    source.dat = input->dat;
    source.map = input->map;
    source.catalog = input->catalog;
    return source;
}

UniqueSwap recipe_unique_swap(const RecipeInput *input, int source_civ)
{
    UniqueSwap swap = { { DAT_NO_ID, DAT_NO_ID }, { DAT_NO_ID, DAT_NO_ID } };
    const UnitEntry *own = design_unit(input);
    UniqueSource source = source_of(input);

    if (!CHECK(input != NULL) || !CHECK(source.catalog != NULL)) {
        return swap;
    }
    swap.from = unique_unit_of_civ(&source, source_civ);
    if (own != NULL) {
        swap.to = unique_unit_of_techs(&source, own->make_tech, own->elite_tech);
    }
    return swap;
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

    if (!CHECK(input != NULL) || !CHECK(edit != NULL)) {
        return false;
    }
    map = input->map;
    if (!CHECK((input->slot >= 0) && (input->slot < map->civ_count) && (input->slot < DAT_CIV_CAPACITY))) {
        return false;
    }
    for (form = 0; fine && (form < UNIT_FORMS); ++form) {
        sid = catalog_string_id(input->catalog, input->slot, KINDS[form]);
        if ((sid != CATALOG_MISSING) && dat_find_unit(input->dat, map->civ_at[input->slot], units[form], &at)) {
            fine = dat_edit_patch_i32(edit, at + DAT_UNIT_NAME_AT, sid) &&
                   dat_edit_patch_i32(edit, at + DAT_UNIT_CREATION_AT, sid + MOD_LABEL_OFFSET) &&
                   dat_edit_patch_i32(edit, at + DAT_UNIT_HELP_AT, sid + MOD_HELP_OFFSET);
        }
    }
    return fine;
}

bool recipe_name_unique_unit(const RecipeInput *input, DatEdit *edit)
{
    const UnitEntry *own = design_unit(input);
    UniqueSource source = source_of(input);

    if (!CHECK(input != NULL) || !CHECK(edit != NULL)) {
        return false;
    }
    if ((own == NULL) || !mod_unit_has_own_texts(input->design)) {
        return true;
    }
    return name_units(input, edit, unique_unit_of_techs(&source, own->make_tech, own->elite_tech));
}

RecipeUnitIds recipe_unique_unit_ids(const RecipeInput *input)
{
    RecipeUnitIds ids = { DAT_NO_ID, DAT_NO_ID, DAT_NO_ID };
    const UnitEntry *own = design_unit(input);
    UniqueSource source = source_of(input);
    UniqueUnit unit;

    if (!CHECK(input != NULL) || (own == NULL) || (plan_index(own->make_tech) < 0)) {
        return ids;
    }
    unit = unique_unit_of_techs(&source, own->make_tech, own->elite_tech);
    ids.unit = unit.base;
    ids.elite_unit = unit.elite;
    ids.elite_tech = (plan_index(own->elite_tech) < 0) ? DAT_NO_ID : remapped(own->elite_tech);
    return ids;
}

static void name_elite_tech(const RecipeInput *input, int tech_id, DatTech *copy)
{
    const UnitEntry *own = design_unit(input);
    int sid;

    if (!CHECK(input != NULL) || !CHECK(copy != NULL) || (own == NULL) || (own->elite_tech != tech_id) ||
        !mod_unit_has_own_texts(input->design)) {
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

    if (!CHECK(input != NULL) || !CHECK(edit != NULL) || !recipe_read_tech(input, plan.from[position], &copy)) {
        return false;
    }
    copy.civ = input->slot;
    for (slot = 0; slot < DAT_REQUIRED_TECH_SLOTS; ++slot) {
        copy.required_techs[slot] = remapped(copy.required_techs[slot]);
    }
    if (copy.effect_id >= 0) {
        copy.effect_id = recipe_copy_effect(input, edit, copy.effect_id);
        if (copy.effect_id == DAT_NO_ID) {
            return false;
        }
    }
    name_elite_tech(input, plan.from[position], &copy);
    return CHECK(dat_edit_add_tech(edit, &copy) == plan.to[position]);
}

bool recipe_commit_plan(const RecipeInput *input, DatEdit *edit)
{
    int base;
    int position;
    bool fine = true;

    if (!CHECK(input != NULL) || !CHECK(edit != NULL)) {
        return false;
    }
    base = edit->old_tech_count + edit->tech_count;
    for (position = 0; (position < plan.count) && (position < RECIPE_COPY_CAPACITY); ++position) {
        plan.to[position] = base + position;
    }
    for (position = 0; fine && (position < plan.count) && (position < RECIPE_COPY_CAPACITY); ++position) {
        fine = copy_planned(input, edit, position);
    }
    return fine;
}
