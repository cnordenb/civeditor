#include "small_dat.h"

#include <string.h>

#include "../../src/core/check.h"

enum {
    TERRAIN_BLOCK_HEAD_SIZE = 140,
    TERRAIN_COUNT = 200,
    TERRAIN_HEAD_SIZE = 8,
    TERRAIN_MIDDLE_SIZE = 28,
    TERRAIN_TAIL_SIZE = 364,
    TERRAIN_BLOCK_TAIL_SIZE = 63,
    RANDOM_MAPS_SIZE = 8,
    CIV_COUNT_SIZE = 2,
    TECH_COUNT_SIZE = 2,
    EMPTY_TEXT_SIZE = 4,
    COUNT_SIZE = 2,
    HEAD_TO_SOUNDS_SIZE = 6,
    SOUND_HEAD_SIZE = 4,
    SOUND_CACHE_SIZE = 6,
    SOUND_ITEM_TAIL_SIZE = 10,
    GRAPHIC_TO_DELTA_COUNT = 18,
    GRAPHIC_TO_ANGLE_FLAG = 6,
    GRAPHIC_TO_ANGLE_COUNT = 2,
    GRAPHIC_REST_SIZE = 17,
    GRAPHIC_DELTA_SIZE = 16,
    GRAPHIC_ANGLE_SOUND_SIZE = 24,
    HEADER_COUNT_SIZE = 4,
    EFFECT_COUNT_SIZE = 4,
    TAIL_SIZE = 28,
    RICH_ROOM = 1024
};

enum {
    TREE_COUNTS_SIZE = 9,
    TREE_NODE_HEAD_SIZE = 5,
    TREE_LINKED_HEAD_SIZE = 9,
    TREE_ID_LISTS = 3,
    TREE_ID_SIZE = 4,
    TREE_AGE_SIZE = 110,
    TREE_BUILDING_SIZE = 103,
    TREE_UNIT_SIZE = 88,
    TREE_UNIT_REST_SIZE = 16,
    TREE_RESEARCH_SIZE = 96
};

enum {
    SMALL_UNIT_LIMIT = 255,
    PLAYER_TYPE_SIZE = 1,
    CIV_IDS_AND_ICON_SIZE = 7,
    UNIT_COUNT_SIZE = 2,
    UNIT_POINTER_SIZE = 4,
    UNIT_NAME_AT = 180,
    BUILDING_UNIT_SIZE = 562,
    CREATABLE_UNIT_SIZE = 473,
    TRAIN_LOCATION_COUNT_AT = 365,
    CIV_RECORD_SIZE = 576,
    TASK_COUNT_AT = 259,
    ARMS_AT = 263,
    ARMS_SIZE = 20,
    COSTS_SIZE = 18,
    RESOURCE_SIZE = 4,
    BYTE_VALUES = 256
};

static const char VERSION[] = "VER 8.9";
static const unsigned char EMPTY_TEXT[EMPTY_TEXT_SIZE] = { 0x60, 0x0A, 0x00, 0x00 };

SmallDat small = { .unit = { 0, SMALL_BUILDING_CLASS, SMALL_BUILDING_TYPE, 0, false } };

static size_t put_empty_text(size_t at)
{
    size_t index;

    if (!CHECK(at > 0U) || !CHECK(at < SMALL_DAT_CAPACITY)) {
        return SMALL_DAT_CAPACITY;
    }
    for (index = 0U; (index < EMPTY_TEXT_SIZE) && (at + index < SMALL_DAT_CAPACITY); ++index) {
        small.data[at + index] = EMPTY_TEXT[index];
    }
    return at + EMPTY_TEXT_SIZE;
}

static size_t put_terrain(size_t at)
{
    if (!CHECK(at > 0U) || !CHECK(at < SMALL_DAT_CAPACITY)) {
        return SMALL_DAT_CAPACITY;
    }
    at = put_empty_text(at + TERRAIN_HEAD_SIZE);
    at = put_empty_text(at);
    at = put_empty_text(at + TERRAIN_MIDDLE_SIZE);
    return at + TERRAIN_TAIL_SIZE;
}

static size_t put_tasks(size_t unit_at, int graphic)
{
    size_t task_at = unit_at + TASK_COUNT_AT + COUNT_SIZE;

    if (!CHECK(unit_at > 0U) || !CHECK(task_at + DAT_TASK_SIZE < SMALL_DAT_CAPACITY) || (small.unit.tasks == 0)) {
        return 0U;
    }
    small.data[unit_at + TASK_COUNT_AT] = 1U;
    small.data[task_at + DAT_TASK_PROCEEDING_GRAPHIC_AT] = (unsigned char)graphic;
    small.data[task_at + DAT_TASK_CARRYING_GRAPHIC_AT] = (unsigned char)graphic;
    return DAT_TASK_SIZE;
}

static size_t put_arms(size_t at)
{
    static const unsigned char ARMS[ARMS_SIZE] = {
        2U, 0U, SMALL_MELEE_CLASS, 0U, SMALL_ATTACK, 0U, SMALL_PIERCE_CLASS, 0U, SMALL_PIERCE_ATTACK, 0U,
        2U, 0U, SMALL_MELEE_CLASS, 0U, SMALL_MELEE_ARMOR, 0U, SMALL_PIERCE_CLASS, 0U, SMALL_PIERCE_ARMOR, 0U
    };
    static const unsigned char RANKED_ARMS[ARMS_SIZE] = {
        2U, 0U, SMALL_BONUS_CLASS, 0U, SMALL_BONUS_ATTACK, 0U, SMALL_MELEE_CLASS, 0U, SMALL_ATTACK, 0U,
        2U, 0U, SMALL_MELEE_CLASS, 0U, SMALL_MELEE_ARMOR, 0U, SMALL_PIERCE_CLASS, 0U, SMALL_PIERCE_ARMOR, 0U
    };

    if (!small.unit.armed || !CHECK(at + ARMS_SIZE < SMALL_DAT_CAPACITY)) {
        return 0U;
    }
    (void)memcpy(&small.data[at], (small.elite > 0) ? RANKED_ARMS : ARMS, sizeof ARMS);
    return ARMS_SIZE - (2U * COUNT_SIZE);
}

static void put_costs(size_t costs_at)
{
    static const unsigned char COSTS[COSTS_SIZE] = { 0U, 0U, SMALL_FOOD,       0U, 1U, 0U, 3U, 0U, SMALL_GOLD,
                                                     0U, 1U, 0U, SMALL_POPULATION, 0U, 1U, 0U, 0U, 0U };

    if (small.elite > 0) {
        (void)memcpy(&small.data[costs_at], COSTS, sizeof COSTS);
    }
}

static size_t put_elite(size_t unit_at, size_t end, size_t combat_at)
{
    const float reload = (float)SMALL_ELITE_RELOAD;
    size_t size = end - unit_at;

    if ((small.elite == 0) || !CHECK(end + size < SMALL_DAT_CAPACITY)) {
        return end;
    }
    (void)memcpy(&small.data[end], &small.data[unit_at], size);
    small.data[end + DAT_UNIT_HIT_POINTS_AT] = (unsigned char)SMALL_ELITE_HP;
    (void)memcpy(&small.data[end + (combat_at - unit_at) + DAT_COMBAT_RELOAD_AT], &reload, sizeof reload);
    return end + size;
}

static int last_unit(void)
{
    return (small.elite > small.unit.index) ? small.elite : small.unit.index;
}

static size_t put_civ(size_t at, int standing_graphic)
{
    size_t pointers = (size_t)(last_unit() + 1) * UNIT_POINTER_SIZE;
    size_t resources = (size_t)small.resources * RESOURCE_SIZE;
    size_t unit_at;
    size_t tasks;
    size_t grown;

    if (!CHECK(at > 0U) || !CHECK(small.unit.index >= 0) || !CHECK(small.unit.index < SMALL_UNIT_LIMIT) ||
        !CHECK(at + CIV_RECORD_SIZE + pointers + resources < SMALL_DAT_CAPACITY)) {
        return SMALL_DAT_CAPACITY;
    }
    at = put_empty_text(at + PLAYER_TYPE_SIZE);
    small.data[at] = (unsigned char)(small.resources % BYTE_VALUES);
    small.data[at + 1U] = (unsigned char)(small.resources / BYTE_VALUES);
    at += CIV_IDS_AND_ICON_SIZE + resources;
    small.data[at] = (unsigned char)(last_unit() + 1);
    unit_at = at + UNIT_COUNT_SIZE + pointers;
    small.data[at + UNIT_COUNT_SIZE + ((size_t)small.unit.index * UNIT_POINTER_SIZE)] = 1U;
    small.data[unit_at - UNIT_POINTER_SIZE] = 1U;
    small.data[unit_at] = (unsigned char)small.unit.type;
    small.data[unit_at + DAT_UNIT_CLASS_AT] = (unsigned char)small.unit.unit_class;
    small.data[unit_at + DAT_UNIT_GRAPHICS_AT] = (unsigned char)standing_graphic;
    (void)put_empty_text(unit_at + UNIT_NAME_AT);
    if (small.unit.type == SMALL_BUILDING_TYPE) {
        return unit_at + BUILDING_UNIT_SIZE;
    }
    tasks = put_tasks(unit_at, standing_graphic);
    grown = tasks + put_arms(unit_at + ARMS_AT + tasks);
    small.data[unit_at + TRAIN_LOCATION_COUNT_AT + grown] = 1U;
    put_costs(unit_at + TRAIN_LOCATION_COUNT_AT + grown - COSTS_SIZE);
    return put_elite(unit_at, unit_at + CREATABLE_UNIT_SIZE + grown, unit_at + ARMS_AT + tasks + ARMS_SIZE);
}

static size_t put_sounds(size_t at)
{
    if (!CHECK(at > 0U) || !CHECK(at + RICH_ROOM < SMALL_DAT_CAPACITY) || !small.rich) {
        return at + COUNT_SIZE;
    }
    small.data[at] = 1U;
    at += COUNT_SIZE + SOUND_HEAD_SIZE;
    small.data[at] = 1U;
    at = put_empty_text(at + COUNT_SIZE + SOUND_CACHE_SIZE);
    return at + SOUND_ITEM_TAIL_SIZE;
}

static size_t put_graphics(size_t at)
{
    if (!CHECK(at > 0U) || !CHECK(at + RICH_ROOM < SMALL_DAT_CAPACITY) || !small.rich) {
        return at + COUNT_SIZE;
    }
    small.data[at] = 2U;
    small.data[at + COUNT_SIZE + UNIT_POINTER_SIZE] = 1U;
    at = put_empty_text(at + COUNT_SIZE + (2U * UNIT_POINTER_SIZE));
    at = put_empty_text(at);
    at = put_empty_text(at) + GRAPHIC_TO_DELTA_COUNT;
    small.data[at] = 1U;
    at += COUNT_SIZE + GRAPHIC_TO_ANGLE_FLAG;
    small.data[at] = 1U;
    at += 1U + GRAPHIC_TO_ANGLE_COUNT;
    small.data[at] = 1U;
    return at + COUNT_SIZE + GRAPHIC_REST_SIZE + GRAPHIC_DELTA_SIZE + GRAPHIC_ANGLE_SOUND_SIZE;
}

static size_t put_unit_headers(size_t at)
{
    if (!CHECK(at > 0U) || !CHECK(at + RICH_ROOM < SMALL_DAT_CAPACITY) || !small.rich) {
        return at + HEADER_COUNT_SIZE;
    }
    small.data[at] = 2U;
    at += HEADER_COUNT_SIZE + 1U;
    small.data[at] = 1U;
    small.data[at + 1U] = 1U;
    return at + 1U + COUNT_SIZE + DAT_TASK_SIZE;
}

static size_t put_tech_tree(size_t at)
{
    if (!CHECK(at > 0U) || !CHECK(at + RICH_ROOM < SMALL_DAT_CAPACITY) || !small.rich) {
        return at + TREE_COUNTS_SIZE;
    }
    small.data[at] = 1U;
    small.data[at + 1U] = 1U;
    small.data[at + 2U] = 1U;
    small.data[at + 4U] = 1U;
    at += TREE_COUNTS_SIZE + TREE_NODE_HEAD_SIZE;
    small.data[at] = 1U;
    at += TREE_ID_LISTS + TREE_ID_SIZE + TREE_AGE_SIZE;
    at += TREE_NODE_HEAD_SIZE + TREE_ID_LISTS + TREE_BUILDING_SIZE;
    at += TREE_LINKED_HEAD_SIZE + TREE_UNIT_SIZE;
    small.data[at] = 1U;
    at += 1U + TREE_ID_SIZE + TREE_UNIT_REST_SIZE;
    return at + TREE_LINKED_HEAD_SIZE + TREE_ID_LISTS + TREE_RESEARCH_SIZE;
}

bool small_dat_start(int civ_count)
{
    size_t at = sizeof VERSION + HEAD_TO_SOUNDS_SIZE;
    int terrain;
    int civ;

    if (!CHECK(civ_count >= 0) || !CHECK(civ_count <= SMALL_CIV_LIMIT)) {
        return false;
    }
    (void)memset(small.data, 0, sizeof small.data);
    (void)memcpy(small.data, VERSION, sizeof VERSION);
    at = put_graphics(put_sounds(at)) + TERRAIN_BLOCK_HEAD_SIZE;
    for (terrain = 0; terrain < TERRAIN_COUNT; ++terrain) {
        at = put_terrain(at);
    }
    at = put_unit_headers(at + TERRAIN_BLOCK_TAIL_SIZE + RANDOM_MAPS_SIZE + EFFECT_COUNT_SIZE);
    small.data[at] = (unsigned char)civ_count;
    at += CIV_COUNT_SIZE;
    for (civ = 0; (civ < civ_count) && (civ < SMALL_CIV_LIMIT); ++civ) {
        at = put_civ(at, SMALL_FIRST_GRAPHIC + civ);
    }
    small.size = put_tech_tree(at + TECH_COUNT_SIZE + TAIL_SIZE);
    small.bytes.data = small.data;
    small.bytes.size = small.size;
    return CHECK(small.size < SMALL_DAT_CAPACITY) && CHECK(dat_map_read(small.data, small.size, &small.map)) &&
           CHECK(dat_edit_start(&small.edit, &small.map));
}

bool small_dat_write(DatBytes *result)
{
    DatBytes original;
    DatOutput output;

    if (!CHECK(result != NULL) || !CHECK(small.size > 0U)) {
        return false;
    }
    original.data = small.data;
    original.size = small.size;
    output.data = small.written;
    output.capacity = sizeof small.written;
    output.length = 0U;
    if (!CHECK(dat_edit_write(&small.edit, &original, &output))) {
        return false;
    }
    result->data = small.written;
    result->size = output.length;
    return CHECK(dat_map_read(small.written, output.length, &small.written_map));
}
