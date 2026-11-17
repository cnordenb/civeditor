#include "hero.h"

#include "../core/check.h"
#include "../core/text.h"
#include "../dat/dat_parts.h"
#include "../mod/mod_strings.h"

enum {
    CASTLE = 82,
    CASTLE_BUTTON = 4,
    CASTLE_HOT_KEY = 16730,
    TRAIN_TIME = 60,
    IMPERIAL_AGE = 103,
    FOOD = 0,
    GOLD = 3,
    PRICE = 500,
    PAID = 1,
    HIT_POINTS_FLOOR = 300,
    HERO_STATUS = 1,
    SET_ATTRIBUTE = 0,
    ENABLE_UNIT = 2,
    ADD_ATTRIBUTE = 4,
    TRAINABLE_AT_ONCE = 126,
    LIMIT_FLAGS = 127,
    RETRAINED_AFTER_DEATH = 4,
    RESEARCHED_BY_ITSELF = 1,
    ENABLING_CHANGE_COUNT = 3
};

static const char ENABLING_NAME[] = "__hero_imp_enable_";

static const DatTechCost HERO_COSTS[DAT_UNIT_COST_SLOTS] = {
    { FOOD, PRICE, PAID }, { GOLD, PRICE, PAID }, { DAT_NO_ID, 0, 0 }
};

static const DatTech ENABLING_TECH = {
    .required_techs = { IMPERIAL_AGE, DAT_NO_ID, DAT_NO_ID, DAT_NO_ID, DAT_NO_ID, DAT_NO_ID },
    .costs = { { DAT_NO_ID, 0, 0 }, { DAT_NO_ID, 0, 0 }, { DAT_NO_ID, 0, 0 } },
    .required_tech_count = 1,
    .civ = DAT_NO_ID,
    .full_tech_mode = RESEARCHED_BY_ITSELF,
    .language_dll_name = DAT_NO_ID,
    .language_dll_description = DAT_NO_ID,
    .effect_id = DAT_NO_ID,
    .icon_id = DAT_NO_ID,
    .language_dll_help = DAT_NO_ID,
    .language_dll_tech_tree = DAT_NO_ID,
    .repeatable = 1,
    .research_location_count = 1,
    .research_locations = { { DAT_NO_ID, 0, 0, DAT_NO_ID } }
};

static DatUnitList units;

static bool unit_is_listed(int unit_id)
{
    if (!CHECK(unit_id >= 0) || !CHECK(units.count <= DAT_UNIT_CAPACITY)) {
        return false;
    }
    return (unit_id < units.count) && (units.at[unit_id] != 0U);
}

bool hero_find(const DatTarget *target, int unit_id, Hero *hero)
{
    const DatMap *map;

    if (unit_id < 0) {
        return false;
    }
    map = target->map;
    if (!dat_list_units(target->dat, map->civ_at[target->civ], &units) || !unit_is_listed(unit_id)) {
        return false;
    }
    hero->id = unit_id;
    hero->name_sid = HERO_UNNAMED;
    return dat_unit_read(target->dat, units.at[unit_id], &hero->unit);
}

DatChange hero_hidden(const Hero *hero)
{
    DatChange hidden = { ENABLE_UNIT, hero->id, 0, DAT_NO_ID, 0.0F };

    return hidden;
}

static bool add_enabling_tech(const DatTarget *target, const Hero *hero)
{
    DatChange changes[ENABLING_CHANGE_COUNT] = {
        { ENABLE_UNIT, DAT_NO_ID, 1, DAT_NO_ID, 0.0F },
        { SET_ATTRIBUTE, DAT_NO_ID, DAT_NO_ID, TRAINABLE_AT_ONCE, 1.0F },
        { ADD_ATTRIBUTE, DAT_NO_ID, DAT_NO_ID, LIMIT_FLAGS, (float)RETRAINED_AFTER_DEATH }
    };
    DatTech tech = ENABLING_TECH;
    int change;
    bool fine;

    (void)(text_copy(tech.name, sizeof tech.name, ENABLING_NAME) &&
           text_append_number(tech.name, sizeof tech.name, target->civ));
    tech.civ = target->civ;
    tech.effect_id = dat_edit_add_effect(target->edit, tech.name);
    fine = (tech.effect_id != DAT_NO_ID);
    for (change = 0; fine && (change < ENABLING_CHANGE_COUNT); ++change) {
        changes[change].a = hero->id;
        fine = dat_edit_add_change(target->edit, &changes[change]);
    }
    return fine && (dat_edit_add_tech(target->edit, &tech) != DAT_NO_ID);
}

static bool keep_own_ids(const DatTarget *target, const Hero *hero)
{
    DatEdit *edit;
    size_t at;

    edit = target->edit;
    at = hero->unit.copy_ids_at;
    return dat_edit_patch_i16(edit, at, hero->id) && dat_edit_patch_i16(edit, at + DAT_SHORT_WIDTH, hero->id) &&
           dat_edit_patch_u8(edit, hero->unit.at + DAT_UNIT_DISABLED_AT, 0);
}

static bool name_hero(const DatTarget *target, const Hero *hero)
{
    DatEdit *edit;
    size_t at;

    if (hero->name_sid < 0L) {
        return true;
    }
    edit = target->edit;
    at = hero->unit.at;
    return dat_edit_patch_i32(edit, at + DAT_UNIT_NAME_AT, hero->name_sid) &&
           dat_edit_patch_i32(edit, at + DAT_UNIT_CREATION_AT, hero->name_sid + MOD_LABEL_OFFSET) &&
           dat_edit_patch_i32(edit, at + DAT_UNIT_HELP_AT, hero->name_sid + MOD_HELP_OFFSET);
}

static bool put_cost(DatEdit *edit, size_t at, const DatTechCost *cost)
{
    DatPatch patch = { 0U, 0U, { 0U } };
    DatSink sink = dat_sink_over(patch.bytes, sizeof patch.bytes, 0U);

    dat_sink_i16(&sink, cost->type);
    dat_sink_i16(&sink, cost->amount);
    dat_sink_i16(&sink, cost->flag);
    return dat_edit_patch_sunk(edit, at, &patch, &sink);
}

static bool train_in_castle(DatEdit *edit, size_t at)
{
    return dat_edit_patch_i16(edit, at, TRAIN_TIME) && dat_edit_patch_i16(edit, at + DAT_TRAIN_BUILDING_AT, CASTLE) &&
           dat_edit_patch_u8(edit, at + DAT_TRAIN_BUTTON_AT, CASTLE_BUTTON) &&
           dat_edit_patch_i32(edit, at + DAT_TRAIN_HOT_KEY_AT, CASTLE_HOT_KEY);
}

static bool train_at_castle(const DatTarget *target, const Hero *hero)
{
    DatEdit *edit;
    int slot;
    bool fine;

    if (hero->unit.train_location_count <= 0) {
        return true;
    }
    edit = target->edit;
    fine = train_in_castle(edit, hero->unit.train_locations_at) &&
           dat_edit_patch_u8(edit, hero->unit.hero_mode_at, HERO_STATUS);
    for (slot = 0; fine && (slot < DAT_UNIT_COST_SLOTS); ++slot) {
        fine = put_cost(edit, hero->unit.costs_at + ((size_t)slot * DAT_UNIT_COST_SIZE), &HERO_COSTS[slot]);
    }
    return fine;
}

static bool raise_hit_points(const DatTarget *target, const Hero *hero)
{
    DatCursor cursor;
    size_t at;

    at = hero->unit.at + DAT_UNIT_HIT_POINTS_AT;
    if (!dat_cursor_start(&cursor, target->dat, at)) {
        return false;
    }
    return (dat_cursor_i16(&cursor) >= HIT_POINTS_FLOOR) || dat_edit_patch_i16(target->edit, at, HIT_POINTS_FLOOR);
}

bool hero_build(const DatTarget *target, const Hero *hero)
{
    return add_enabling_tech(target, hero) && keep_own_ids(target, hero) && name_hero(target, hero) &&
           train_at_castle(target, hero) && raise_hit_points(target, hero);
}
