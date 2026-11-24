#include "bonus_build.h"

#include <string.h>

#include "../core/check.h"
#include "../core/text.h"
#include "../dat/dat_parts.h"
#include "bonus_scale.h"

enum { NOWHERE = -1, NO_STRING = -1, NO_HOT_KEY = -1, NO_RESOURCE = -1, REPEATABLE = 1, ONE_LOCATION = 1 };

static const char BONUS_NAME[] = "C-Bonus EC-list";
static const int FIELD_WIDTHS[UNIT_FIELD_TYPE_COUNT] = { DAT_BYTE_WIDTH, DAT_SHORT_WIDTH, DAT_LONG_WIDTH };
static const int PART_SIZES[UNIT_PART_COUNT] = { DAT_UNIT_FIXED_SIZE, DAT_UNIT_COST_SLOTS * DAT_UNIT_COST_SIZE,
                                                 DAT_TRAIN_LOCATION_SIZE };

typedef struct {
    const BonusChange *first;
    int effect_id;
    int made;
    double times;
} BonusGroups;

static DatChange change_of(const BonusChange *line)
{
    DatChange change = { 0, 0, 0, 0, 0.0F };

    change.type = line->type;
    change.a = line->a;
    change.b = line->b;
    change.c = line->c;
    change.d = line->d;
    return change;
}

static void shape_tech(DatTech *tech, const BonusChange *first, int effect_id, int slot)
{
    int index;

    (void)memset(tech, 0, sizeof *tech);
    for (index = 0; index < DAT_REQUIRED_TECH_SLOTS; ++index) {
        tech->required_techs[index] = (index < first->requires.count) ? first->requires.values[index] : DAT_NO_ID;
    }
    for (index = 0; index < DAT_TECH_COST_SLOTS; ++index) {
        tech->costs[index].type = NO_RESOURCE;
    }
    tech->required_tech_count =
        (first->requires.count < DAT_REQUIRED_TECH_SLOTS) ? first->requires.count : DAT_REQUIRED_TECH_SLOTS;
    tech->civ = slot;
    tech->language_dll_name = NO_STRING;
    tech->language_dll_description = NO_STRING;
    tech->effect_id = effect_id;
    tech->icon_id = DAT_NO_ID;
    tech->language_dll_help = NO_STRING;
    tech->language_dll_tech_tree = NO_STRING;
    (void)text_copy(tech->name, sizeof tech->name, BONUS_NAME);
    tech->repeatable = REPEATABLE;
    tech->research_location_count = ONE_LOCATION;
    tech->research_locations[0].location_id = NOWHERE;
    tech->research_locations[0].hot_key_id = NO_HOT_KEY;
}

static bool add_bonus_tech(const BonusBuild *build, const BonusChange *first, int effect_id)
{
    DatTech tech;

    if (effect_id == DAT_NO_ID) {
        return false;
    }
    shape_tech(&tech, first, effect_id, build->target.civ);
    return dat_edit_add_tech(build->target.edit, &tech) != DAT_NO_ID;
}

static bool patch_field(DatEdit *edit, size_t at, const UnitPatch *patch)
{
    if (patch->type == UNIT_FIELD_U8) {
        return dat_edit_patch_u8(edit, at, (int)patch->value);
    }
    if (patch->type == UNIT_FIELD_I16) {
        return dat_edit_patch_i16(edit, at, (int)patch->value);
    }
    return dat_edit_patch_f32(edit, at, patch->value);
}

static bool patch_fits(const UnitPatch *patch)
{
    return CHECK(patch->offset + FIELD_WIDTHS[patch->type] <= PART_SIZES[patch->part]);
}

static size_t part_at(const DatUnit *unit, UnitPart part)
{
    if (part == UNIT_PART_COSTS) {
        return unit->costs_at;
    }
    return (unit->train_location_count > 0) ? unit->train_locations_at : 0U;
}

static bool patch_unit_part(const DatTarget *target, size_t unit_at, const UnitPatch *patch)
{
    DatUnit unit;
    size_t at;

    if (!dat_unit_read(target->dat, unit_at, &unit)) {
        return false;
    }
    at = part_at(&unit, patch->part);
    return (at != 0U) && patch_field(target->edit, at + (size_t)patch->offset, patch);
}

static bool patch_unit(const DatTarget *target, const UnitPatch *patch)
{
    const DatMap *map;
    size_t at = 0U;

    if (!patch_fits(patch)) {
        return false;
    }
    map = target->map;
    if (!dat_find_unit(target->dat, map->civ_at[target->civ], patch->unit, &at)) {
        return true;
    }
    if (patch->part != UNIT_PART_START) {
        return patch_unit_part(target, at, patch);
    }
    return patch_field(target->edit, at + (size_t)patch->offset, patch);
}

static int patch_units(const BonusBuild *build, int bonus_id)
{
    const UnitPatch *patch;
    const Catalog *catalog;
    int patched = 0;
    int index;
    bool fine = true;

    catalog = build->catalog;
    for (index = 0; fine && (index < catalog->unit_patch_count) && (index < CATALOG_UNIT_PATCH_CAPACITY); ++index) {
        patch = &catalog->unit_patches[index];
        if (patch->bonus_id == bonus_id) {
            fine = patch_unit(&build->target, patch);
            ++patched;
        }
    }
    return fine ? patched : BONUS_BUILD_FAILED;
}

static bool close_group(const BonusBuild *build, BonusGroups *groups)
{
    if (groups->first == NULL) {
        return true;
    }
    ++groups->made;
    return add_bonus_tech(build, groups->first, groups->effect_id);
}

static bool take_line(const BonusBuild *build, BonusGroups *groups, const BonusChange *line)
{
    const BonusChange *first;
    DatChange change = change_of(line);

    first = groups->first;
    if ((first == NULL) || (line->entry != first->entry)) {
        if (!close_group(build, groups)) {
            return false;
        }
        groups->first = line;
        groups->effect_id = dat_edit_add_effect(build->target.edit, BONUS_NAME);
    }
    return (groups->effect_id != DAT_NO_ID) && bonus_add_scaled(build->target.edit, &change, groups->times);
}

static bool take_lines(const BonusBuild *build, BonusGroups *groups, int bonus_id)
{
    const BonusChange *line;
    const Catalog *catalog;
    int index;
    bool fine = true;

    catalog = build->catalog;
    for (index = 0; fine && (index < catalog->bonus_change_count) && (index < CATALOG_BONUS_CHANGE_CAPACITY); ++index) {
        line = &catalog->bonus_changes[index];
        if (!line->team && (line->bonus_id == bonus_id)) {
            fine = take_line(build, groups, line);
        }
    }
    return fine;
}

int bonus_build_civ(const BonusBuild *build, int bonus_id, double times)
{
    BonusGroups groups = { NULL, DAT_NO_ID, 0, times };
    int patched;

    if (!take_lines(build, &groups, bonus_id) || !close_group(build, &groups)) {
        return BONUS_BUILD_FAILED;
    }
    patched = patch_units(build, bonus_id);
    return (patched == BONUS_BUILD_FAILED) ? BONUS_BUILD_FAILED : groups.made + patched;
}

int bonus_build_lines(const BonusBuild *build, int bonus_id, bool team, double times)
{
    const BonusChange *line;
    const Catalog *catalog;
    DatChange change;
    int added = 0;
    int index;
    bool fine = true;

    catalog = build->catalog;
    for (index = 0; fine && (index < catalog->bonus_change_count) && (index < CATALOG_BONUS_CHANGE_CAPACITY); ++index) {
        line = &catalog->bonus_changes[index];
        if ((line->team != team) || (line->bonus_id != bonus_id)) {
            continue;
        }
        change = change_of(line);
        fine = bonus_add_scaled(build->target.edit, &change, times);
        ++added;
    }
    return fine ? added : BONUS_BUILD_FAILED;
}
