#include "dat_parts.h"

#include "../core/check.h"

enum {
    TERRAIN_POINTER_ARRAYS = 2,
    TERRAIN_PASS_SIZE = 20,
    PLAYER_COLOUR_SIZE = 36,
    SOUND_HEAD_SIZE = 4,
    SOUND_CACHE_SIZE = 6,
    SOUND_ITEM_TAIL_SIZE = 10,
    GRAPHIC_TEXT_COUNT = 3,
    GRAPHIC_TO_DELTA_COUNT = 18,
    GRAPHIC_TO_ANGLE_FLAG = 6,
    GRAPHIC_TO_ANGLE_COUNT = 2,
    GRAPHIC_REST_SIZE = 17,
    GRAPHIC_DELTA_SIZE = 16,
    GRAPHIC_ANGLE_SOUND_SIZE = 24,
    TERRAIN_BLOCK_HEAD_SIZE = 140,
    TERRAIN_COUNT = 200,
    TERRAIN_HEAD_SIZE = 8,
    TERRAIN_MIDDLE_SIZE = 28,
    TERRAIN_TAIL_SIZE = 364,
    TERRAIN_BLOCK_TAIL_SIZE = 63
};

enum {
    UNIT_BASE_AFTER_TYPE = 135,
    RESOURCE_STORAGES_SIZE = 21,
    UNIT_SOUNDS_SIZE = 22,
    UNIT_COPY_IDS_SIZE = 4,
    SPEED_SIZE = 4,
    DEAD_FISH_SIZE = 41,
    BIRD_HEAD_SIZE = 10,
    DROP_SITE_SIZE = 2,
    BIRD_MIDDLE_SIZE = 14,
    COMBATANT_HEAD_SIZE = 2,
    ATTACK_SIZE = 4,
    COMBATANT_TAIL_SIZE = 80,
    COMBATANT_TO_ATTACK_GRAPHIC = 46,
    PROJECTILE_SIZE = 9,
    CREATABLE_TAIL_SIZE = 97,
    CREATABLE_TO_HERO_MODE = 9,
    CREATABLE_TO_GARRISON_GRAPHIC = 10,
    CIV_IDS_SIZE = 4,
    RESOURCE_SIZE = 4,
    ICON_SET_SIZE = 1,
    TYPE_FLAG = 20,
    TYPE_DEAD_FISH = 30,
    TYPE_BIRD = 40,
    TYPE_COMBATANT = 50,
    TYPE_PROJECTILE = 60,
    TYPE_CREATABLE = 70,
    TYPE_BUILDING = 80,
    TYPE_TREE = 90
};

static bool skip_terrain_restrictions(DatCursor *cursor)
{
    int restriction_count;
    int terrain_count;

    if (!CHECK(cursor != NULL) || !cursor->fine) {
        return false;
    }
    restriction_count = dat_cursor_i16(cursor);
    terrain_count = dat_cursor_i16(cursor);
    return CHECK(restriction_count >= 0) && CHECK(terrain_count >= 0) &&
           dat_cursor_skip_items(cursor, restriction_count, TERRAIN_POINTER_ARRAYS * DAT_POINTER_SIZE) &&
           dat_cursor_skip_items(cursor, restriction_count, (size_t)terrain_count * TERRAIN_PASS_SIZE);
}

static bool skip_sound(DatCursor *cursor)
{
    int item_count;
    int item;

    if (!CHECK(cursor != NULL) || !dat_cursor_skip(cursor, SOUND_HEAD_SIZE)) {
        return false;
    }
    item_count = dat_cursor_i16(cursor);
    (void)dat_cursor_skip(cursor, SOUND_CACHE_SIZE);
    for (item = 0; (item < item_count) && (item < DAT_COUNT_LIMIT) && cursor->fine; ++item) {
        (void)dat_cursor_skip_text(cursor);
        (void)dat_cursor_skip(cursor, SOUND_ITEM_TAIL_SIZE);
    }
    return CHECK(item_count >= 0) && cursor->fine;
}

static bool skip_sounds(DatCursor *cursor)
{
    int sound_count;
    int sound;

    if (!CHECK(cursor != NULL) || !cursor->fine) {
        return false;
    }
    sound_count = dat_cursor_i16(cursor);
    for (sound = 0; (sound < sound_count) && (sound < DAT_COUNT_LIMIT) && cursor->fine; ++sound) {
        (void)skip_sound(cursor);
    }
    return CHECK(sound_count >= 0) && cursor->fine;
}

static bool skip_graphic(DatCursor *cursor)
{
    int delta_count;
    int angle_sounds_used;
    int angle_count;
    int text;

    if (!CHECK(cursor != NULL) || !cursor->fine) {
        return false;
    }
    for (text = 0; text < GRAPHIC_TEXT_COUNT; ++text) {
        (void)dat_cursor_skip_text(cursor);
    }
    (void)dat_cursor_skip(cursor, GRAPHIC_TO_DELTA_COUNT);
    delta_count = dat_cursor_i16(cursor);
    (void)dat_cursor_skip(cursor, GRAPHIC_TO_ANGLE_FLAG);
    angle_sounds_used = dat_cursor_u8(cursor);
    (void)dat_cursor_skip(cursor, GRAPHIC_TO_ANGLE_COUNT);
    angle_count = dat_cursor_i16(cursor);
    (void)dat_cursor_skip(cursor, GRAPHIC_REST_SIZE);
    return CHECK(delta_count >= 0) && dat_cursor_skip_items(cursor, delta_count, GRAPHIC_DELTA_SIZE) &&
           dat_cursor_skip_items(cursor, (angle_sounds_used != 0) ? angle_count : 0, GRAPHIC_ANGLE_SOUND_SIZE);
}

static bool skip_graphics(DatCursor *cursor)
{
    DatCursor slots;
    int graphic_count;
    int graphic;

    if (!CHECK(cursor != NULL) || !cursor->fine) {
        return false;
    }
    graphic_count = dat_cursor_i16(cursor);
    slots = *cursor;
    (void)dat_cursor_skip_items(cursor, graphic_count, DAT_POINTER_SIZE);
    for (graphic = 0; (graphic < graphic_count) && (graphic < DAT_COUNT_LIMIT) && cursor->fine; ++graphic) {
        if (dat_cursor_i32(&slots) != 0L) {
            (void)skip_graphic(cursor);
        }
    }
    return CHECK(graphic_count >= 0) && cursor->fine;
}

static bool skip_two_texts(DatCursor *cursor)
{
    return CHECK(cursor != NULL) && dat_cursor_skip_text(cursor) && dat_cursor_skip_text(cursor);
}

static bool skip_terrain(DatCursor *cursor)
{
    if (!CHECK(cursor != NULL) || !CHECK(cursor->fine)) {
        return false;
    }
    return dat_cursor_skip(cursor, TERRAIN_HEAD_SIZE) && skip_two_texts(cursor) &&
           dat_cursor_skip(cursor, TERRAIN_MIDDLE_SIZE) && dat_cursor_skip_text(cursor) &&
           dat_cursor_skip(cursor, TERRAIN_TAIL_SIZE);
}

static bool skip_terrain_block(DatCursor *cursor)
{
    int terrain;

    if (!CHECK(cursor != NULL) || !dat_cursor_skip(cursor, TERRAIN_BLOCK_HEAD_SIZE)) {
        return false;
    }
    for (terrain = 0; (terrain < TERRAIN_COUNT) && cursor->fine; ++terrain) {
        (void)skip_terrain(cursor);
    }
    return CHECK(cursor->fine) && dat_cursor_skip(cursor, TERRAIN_BLOCK_TAIL_SIZE);
}

static bool skip_random_maps(DatCursor *cursor)
{
    long map_count;

    if (!CHECK(cursor != NULL) || !cursor->fine) {
        return false;
    }
    map_count = dat_cursor_i32(cursor);
    return CHECK(map_count == 0L) && dat_cursor_skip(cursor, DAT_POINTER_SIZE);
}

static bool skip_sounds_and_graphics(DatCursor *cursor)
{
    return CHECK(cursor != NULL) && skip_sounds(cursor) && skip_graphics(cursor);
}

bool dat_skip_head(DatCursor *cursor)
{
    int colour_count;

    if (!CHECK(cursor != NULL) || !skip_terrain_restrictions(cursor)) {
        return false;
    }
    colour_count = dat_cursor_i16(cursor);
    return CHECK(colour_count >= 0) && dat_cursor_skip_items(cursor, colour_count, PLAYER_COLOUR_SIZE) &&
           skip_sounds_and_graphics(cursor) && skip_terrain_block(cursor) && skip_random_maps(cursor);
}

bool dat_skip_unit_headers(DatCursor *cursor)
{
    long header_count;
    long header;

    if (!CHECK(cursor != NULL) || !cursor->fine) {
        return false;
    }
    header_count = dat_cursor_i32(cursor);
    for (header = 0L; (header < header_count) && (header < DAT_COUNT_LIMIT) && cursor->fine; ++header) {
        if (dat_cursor_u8(cursor) != 0) {
            (void)dat_cursor_skip_items(cursor, dat_cursor_i16(cursor), DAT_TASK_SIZE);
        }
    }
    return CHECK(header_count >= 0L) && CHECK(header_count <= DAT_COUNT_LIMIT) && cursor->fine;
}

static bool skip_unit_base(DatCursor *cursor, DatUnit *unit)
{
    if (!CHECK(cursor != NULL) || !CHECK(unit != NULL) || !CHECK(cursor->fine)) {
        return false;
    }
    (void)dat_cursor_skip(cursor, UNIT_BASE_AFTER_TYPE);
    (void)dat_cursor_skip(cursor, RESOURCE_STORAGES_SIZE);
    unit->damage_graphic_count = dat_cursor_u8(cursor);
    unit->damage_graphics_at = cursor->at;
    if (!dat_cursor_skip_items(cursor, unit->damage_graphic_count, DAT_DAMAGE_GRAPHIC_SIZE) ||
        !dat_cursor_skip(cursor, UNIT_SOUNDS_SIZE) || !dat_cursor_skip_text(cursor)) {
        return false;
    }
    unit->copy_ids_at = cursor->at;
    return dat_cursor_skip(cursor, UNIT_COPY_IDS_SIZE);
}

static bool skip_bird(DatCursor *cursor, DatUnit *unit)
{
    int drop_site_count;

    if (!CHECK(cursor != NULL) || !CHECK(unit != NULL) || !dat_cursor_skip(cursor, BIRD_HEAD_SIZE)) {
        return false;
    }
    drop_site_count = dat_cursor_i16(cursor);
    (void)dat_cursor_skip_items(cursor, drop_site_count, DROP_SITE_SIZE);
    (void)dat_cursor_skip(cursor, BIRD_MIDDLE_SIZE);
    unit->task_count = dat_cursor_i16(cursor);
    unit->tasks_at = cursor->at;
    return CHECK(drop_site_count >= 0) && CHECK(unit->task_count >= 0) &&
           dat_cursor_skip_items(cursor, unit->task_count, DAT_TASK_SIZE);
}

static bool skip_combatant(DatCursor *cursor, DatUnit *unit)
{
    int attack_count;
    int armour_count;

    if (!CHECK(cursor != NULL) || !CHECK(unit != NULL) || !dat_cursor_skip(cursor, COMBATANT_HEAD_SIZE)) {
        return false;
    }
    attack_count = dat_cursor_i16(cursor);
    (void)dat_cursor_skip_items(cursor, attack_count, ATTACK_SIZE);
    armour_count = dat_cursor_i16(cursor);
    (void)dat_cursor_skip_items(cursor, armour_count, ATTACK_SIZE);
    unit->attack_graphic_at = cursor->at + COMBATANT_TO_ATTACK_GRAPHIC;
    return CHECK(attack_count >= 0) && CHECK(armour_count >= 0) && dat_cursor_skip(cursor, COMBATANT_TAIL_SIZE);
}

static bool skip_creatable(DatCursor *cursor, DatUnit *unit)
{
    if (!CHECK(cursor != NULL) || !CHECK(unit != NULL)) {
        return false;
    }
    unit->costs_at = cursor->at;
    if (!dat_cursor_skip(cursor, (size_t)DAT_UNIT_COST_SLOTS * DAT_UNIT_COST_SIZE)) {
        return false;
    }
    unit->train_location_count = dat_cursor_i16(cursor);
    unit->train_locations_at = cursor->at;
    (void)dat_cursor_skip_items(cursor, unit->train_location_count, DAT_TRAIN_LOCATION_SIZE);
    unit->hero_mode_at = cursor->at + CREATABLE_TO_HERO_MODE;
    unit->garrison_graphic_at = cursor->at + CREATABLE_TO_GARRISON_GRAPHIC;
    return CHECK(unit->train_location_count >= 0) && dat_cursor_skip(cursor, CREATABLE_TAIL_SIZE);
}

static void skip_unit_parts(DatCursor *cursor, DatUnit *unit, int type)
{
    if (!CHECK(cursor != NULL) || !CHECK(unit != NULL)) {
        return;
    }
    if (type >= TYPE_BIRD) {
        (void)skip_bird(cursor, unit);
    }
    if (type >= TYPE_COMBATANT) {
        (void)skip_combatant(cursor, unit);
    }
    if (type == TYPE_PROJECTILE) {
        (void)dat_cursor_skip(cursor, PROJECTILE_SIZE);
    }
    if (type >= TYPE_CREATABLE) {
        (void)skip_creatable(cursor, unit);
    }
    if (type == TYPE_BUILDING) {
        unit->building_at = cursor->at;
        (void)dat_cursor_skip(cursor, DAT_BUILDING_SIZE);
    }
}

bool dat_read_unit(DatCursor *cursor, DatUnit *unit)
{
    const DatUnit absent = { 0U, 0, 0, 0U, 0, 0U, 0U, 0U, 0, 0U, 0U, 0U, 0, 0U, 0U, 0U };
    int type;

    if (!CHECK(cursor != NULL) || !CHECK(unit != NULL) || !cursor->fine) {
        return false;
    }
    *unit = absent;
    unit->at = cursor->at;
    type = dat_cursor_u8(cursor);
    if (!skip_unit_base(cursor, unit) || (type == TYPE_TREE) || (type < TYPE_FLAG)) {
        return CHECK(cursor->fine);
    }
    (void)dat_cursor_skip(cursor, SPEED_SIZE);
    if (type >= TYPE_DEAD_FISH) {
        unit->walking_graphic_at = cursor->at;
        (void)dat_cursor_skip(cursor, DEAD_FISH_SIZE);
    }
    skip_unit_parts(cursor, unit, type);
    return CHECK(cursor->fine);
}

bool dat_skip_unit(DatCursor *cursor)
{
    DatUnit unit;

    return CHECK(cursor != NULL) && dat_read_unit(cursor, &unit);
}

int dat_skip_civ_head(DatCursor *cursor)
{
    int resource_count;
    int unit_count;

    if (!CHECK(cursor != NULL) || !dat_cursor_skip(cursor, DAT_PLAYER_TYPE_SIZE) || !dat_cursor_skip_text(cursor)) {
        return -1;
    }
    resource_count = dat_cursor_i16(cursor);
    (void)dat_cursor_skip(cursor, CIV_IDS_SIZE);
    (void)dat_cursor_skip_items(cursor, resource_count, RESOURCE_SIZE);
    (void)dat_cursor_skip(cursor, ICON_SET_SIZE);
    unit_count = dat_cursor_i16(cursor);
    return (CHECK(resource_count >= 0) && CHECK(unit_count >= 0) && cursor->fine) ? unit_count : -1;
}

bool dat_skip_civ(DatCursor *cursor)
{
    DatCursor slots;
    int unit_count = dat_skip_civ_head(cursor);
    int unit;

    if (!CHECK(cursor != NULL) || (unit_count < 0)) {
        return false;
    }
    slots = *cursor;
    (void)dat_cursor_skip_items(cursor, unit_count, DAT_POINTER_SIZE);
    for (unit = 0; (unit < unit_count) && (unit < DAT_COUNT_LIMIT) && cursor->fine; ++unit) {
        if (dat_cursor_i32(&slots) != 0L) {
            (void)dat_skip_unit(cursor);
        }
    }
    return cursor->fine;
}
