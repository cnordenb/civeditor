#include "bonus_build.h"

#include <string.h>

#include "../core/check.h"
#include "../core/text.h"
#include "../dat/dat_parts.h"

enum { NOWHERE = -1, NO_STRING = -1, NO_HOT_KEY = -1, NO_RESOURCE = -1, REPEATABLE = 1, ONE_LOCATION = 1 };

static const char BONUS_NAME[] = "C-Bonus EC-list";
static const int FIELD_WIDTHS[UNIT_FIELD_TYPE_COUNT] = { DAT_BYTE_WIDTH, DAT_SHORT_WIDTH, DAT_LONG_WIDTH };

static DatChange change_of(const BonusChange *line)
{
    DatChange change = { 0, 0, 0, 0, 0.0F };

    if (!CHECK(line != NULL) || !CHECK(line->type >= 0)) {
        return change;
    }
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

    if (!CHECK(tech != NULL) || !CHECK(first != NULL)) {
        return;
    }
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

    if (!CHECK(build != NULL) || !CHECK(first != NULL) || (effect_id == DAT_NO_ID)) {
        return false;
    }
    shape_tech(&tech, first, effect_id, build->target.civ);
    return dat_edit_add_tech(build->target.edit, &tech) != DAT_NO_ID;
}

static bool patch_unit(const DatTarget *target, const UnitPatch *patch)
{
    const DatMap *map;
    size_t at = 0U;

    if (!CHECK(target != NULL) || !CHECK(patch != NULL) || !CHECK(target->map != NULL) ||
        !CHECK(((int)patch->type >= 0) && ((int)patch->type < (int)UNIT_FIELD_TYPE_COUNT)) ||
        !CHECK((patch->offset >= 0) && (patch->offset + FIELD_WIDTHS[patch->type] <= DAT_UNIT_FIXED_SIZE))) {
        return false;
    }
    map = target->map;
    if (!CHECK((target->civ >= 0) && (target->civ < map->civ_count) && (target->civ < DAT_CIV_CAPACITY))) {
        return false;
    }
    if (!dat_find_unit(target->dat, map->civ_at[target->civ], patch->unit, &at)) {
        return true;
    }
    at += (size_t)patch->offset;
    if (patch->type == UNIT_FIELD_U8) {
        return dat_edit_patch_u8(target->edit, at, (int)patch->value);
    }
    if (patch->type == UNIT_FIELD_I16) {
        return dat_edit_patch_i16(target->edit, at, (int)patch->value);
    }
    return dat_edit_patch_f32(target->edit, at, patch->value);
}

static int patch_units(const BonusBuild *build, int bonus_id)
{
    const UnitPatch *patch;
    const Catalog *catalog;
    int patched = 0;
    int index;
    bool fine = true;

    if (!CHECK(build != NULL) || !CHECK(build->catalog != NULL)) {
        return BONUS_BUILD_FAILED;
    }
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

int bonus_build_civ(const BonusBuild *build, int bonus_id)
{
    const BonusChange *first = NULL;
    const BonusChange *line;
    const Catalog *catalog;
    DatChange change;
    int effect_id = DAT_NO_ID;
    int made = 0;
    int patched;
    int index;
    bool fine = true;

    if (!CHECK(build != NULL) || !CHECK(build->catalog != NULL)) {
        return BONUS_BUILD_FAILED;
    }
    catalog = build->catalog;
    for (index = 0; fine && (index < catalog->bonus_change_count) && (index < CATALOG_BONUS_CHANGE_CAPACITY); ++index) {
        line = &catalog->bonus_changes[index];
        if (line->team || (line->bonus_id != bonus_id)) {
            continue;
        }
        if ((first == NULL) || (line->entry != first->entry)) {
            fine = (first == NULL) || add_bonus_tech(build, first, effect_id);
            made += (first == NULL) ? 0 : 1;
            first = line;
            effect_id = fine ? dat_edit_add_effect(build->target.edit, BONUS_NAME) : DAT_NO_ID;
        }
        change = change_of(line);
        fine = (effect_id != DAT_NO_ID) && dat_edit_add_change(build->target.edit, &change);
    }
    if (fine && (first != NULL)) {
        fine = add_bonus_tech(build, first, effect_id);
        ++made;
    }
    patched = fine ? patch_units(build, bonus_id) : BONUS_BUILD_FAILED;
    return (patched == BONUS_BUILD_FAILED) ? BONUS_BUILD_FAILED : made + patched;
}

int bonus_build_team(const BonusBuild *build, int team_bonus_id)
{
    const BonusChange *line;
    const Catalog *catalog;
    DatChange change;
    int added = 0;
    int index;
    bool fine = true;

    if (!CHECK(build != NULL) || !CHECK(build->catalog != NULL)) {
        return BONUS_BUILD_FAILED;
    }
    catalog = build->catalog;
    for (index = 0; fine && (index < catalog->bonus_change_count) && (index < CATALOG_BONUS_CHANGE_CAPACITY); ++index) {
        line = &catalog->bonus_changes[index];
        if (!line->team || (line->bonus_id != team_bonus_id)) {
            continue;
        }
        change = change_of(line);
        fine = dat_edit_add_change(build->target.edit, &change);
        ++added;
    }
    return fine ? added : BONUS_BUILD_FAILED;
}
