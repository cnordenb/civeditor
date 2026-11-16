#include "dat_map.h"

#include <string.h>

#include "../core/check.h"
#include "dat_parts.h"

enum { TECH_HEAD_SIZE = 55, TECH_REPEATABLE_SIZE = 1, TAIL_SIZE = 28 };

static const char VERSION[] = "VER 8.9";

enum {
    TREE_GROUPS_SIZE = 4,
    TREE_ID_SIZE = 4,
    TREE_LIST_KINDS = 3,
    TREE_NODE_HEAD_SIZE = 5,
    TREE_LINKED_HEAD_SIZE = 9,
    TREE_COMMON_SIZE = 84,
    TREE_AGE_REST_SIZE = 26,
    TREE_BUILDING_REST_SIZE = 19,
    TREE_UNIT_LINE_SIZE = 4,
    TREE_UNIT_REST_SIZE = 16,
    TREE_RESEARCH_REST_SIZE = 12
};

typedef enum {
    TREE_AGE,
    TREE_BUILDING,
    TREE_UNIT,
    TREE_RESEARCH,
    TREE_KIND_COUNT
} TreeKind;

typedef struct {
    size_t head;
    int lists_before;
    size_t middle;
    int lists_after;
    size_t rest;
} TreeShape;

static const TreeShape TREE_SHAPES[TREE_KIND_COUNT] = {
    { TREE_NODE_HEAD_SIZE, TREE_LIST_KINDS, TREE_COMMON_SIZE + TREE_AGE_REST_SIZE, 0, 0U },
    { TREE_NODE_HEAD_SIZE, TREE_LIST_KINDS, TREE_COMMON_SIZE + TREE_BUILDING_REST_SIZE, 0, 0U },
    { TREE_LINKED_HEAD_SIZE, 0, TREE_COMMON_SIZE + TREE_UNIT_LINE_SIZE, 1, TREE_UNIT_REST_SIZE },
    { TREE_LINKED_HEAD_SIZE, TREE_LIST_KINDS, TREE_COMMON_SIZE + TREE_RESEARCH_REST_SIZE, 0, 0U }
};


static bool map_version(DatCursor *cursor)
{
    const unsigned char *data;

    if (!CHECK(cursor != NULL) || !CHECK(cursor->at == 0U) || !dat_cursor_skip(cursor, sizeof VERSION)) {
        return false;
    }
    data = cursor->data;
    return CHECK(memcmp(data, VERSION, sizeof VERSION) == 0);
}

static bool map_effects(DatCursor *cursor, DatMap *map)
{
    long effect_count;
    int effect;

    if (!CHECK(cursor != NULL) || !CHECK(map != NULL)) {
        return false;
    }
    map->effect_count_at = cursor->at;
    effect_count = dat_cursor_i32(cursor);
    if (!CHECK(effect_count >= 0L) || !CHECK(effect_count <= DAT_EFFECT_CAPACITY)) {
        return false;
    }
    map->effect_count = (int)effect_count;
    for (effect = 0; (effect < map->effect_count) && (effect < DAT_EFFECT_CAPACITY) && cursor->fine; ++effect) {
        map->effect_at[effect] = cursor->at;
        (void)dat_cursor_skip_text(cursor);
        (void)dat_cursor_skip_items(cursor, dat_cursor_i16(cursor), DAT_CHANGE_SIZE);
    }
    map->unit_headers_at = cursor->at;
    return cursor->fine;
}

static bool map_civs(DatCursor *cursor, DatMap *map)
{
    int civ;

    if (!CHECK(cursor != NULL) || !CHECK(map != NULL)) {
        return false;
    }
    map->civ_count_at = cursor->at;
    map->civ_count = dat_cursor_i16(cursor);
    if (!CHECK(map->civ_count >= 0) || !CHECK(map->civ_count <= DAT_CIV_CAPACITY)) {
        return false;
    }
    for (civ = 0; (civ < map->civ_count) && (civ < DAT_CIV_CAPACITY) && cursor->fine; ++civ) {
        map->civ_at[civ] = cursor->at;
        (void)dat_skip_civ(cursor);
    }
    return cursor->fine;
}

static bool read_tech_count(DatCursor *cursor, DatMap *map)
{
    map->tech_count_at = cursor->at;
    map->tech_count = dat_cursor_i16(cursor);
    return CHECK(map->tech_count >= 0) && CHECK(map->tech_count <= DAT_TECH_CAPACITY);
}

static bool map_techs(DatCursor *cursor, DatMap *map)
{
    int tech;

    if (!CHECK(cursor != NULL) || !CHECK(map != NULL) || !read_tech_count(cursor, map)) {
        return false;
    }
    for (tech = 0; (tech < map->tech_count) && (tech < DAT_TECH_CAPACITY) && cursor->fine; ++tech) {
        map->tech_at[tech] = cursor->at;
        (void)dat_cursor_skip(cursor, TECH_HEAD_SIZE);
        (void)dat_cursor_skip_text(cursor);
        (void)dat_cursor_skip(cursor, TECH_REPEATABLE_SIZE);
        (void)dat_cursor_skip_items(cursor, dat_cursor_i16(cursor), DAT_RESEARCH_LOCATION_SIZE);
    }
    map->tail_at = cursor->at;
    return cursor->fine && dat_cursor_skip(cursor, TAIL_SIZE);
}

static bool skip_id_lists(DatCursor *cursor, int list_count)
{
    int list;

    if (!CHECK(cursor != NULL) || !CHECK(list_count <= TREE_LIST_KINDS)) {
        return false;
    }
    for (list = 0; (list < list_count) && (list < TREE_LIST_KINDS); ++list) {
        (void)dat_cursor_skip_items(cursor, dat_cursor_u8(cursor), TREE_ID_SIZE);
    }
    return cursor->fine;
}

static bool skip_tree_node(DatCursor *cursor, const TreeShape *shape)
{
    if (!CHECK(cursor != NULL) || !CHECK(shape != NULL)) {
        return false;
    }
    return dat_cursor_skip(cursor, shape->head) && skip_id_lists(cursor, shape->lists_before) &&
           dat_cursor_skip(cursor, shape->middle) && skip_id_lists(cursor, shape->lists_after) &&
           dat_cursor_skip(cursor, shape->rest);
}

static bool skip_tree_nodes(DatCursor *cursor, TreeKind kind, int count)
{
    int node;

    if (!CHECK(cursor != NULL) || !CHECK(kind < TREE_KIND_COUNT)) {
        return false;
    }
    for (node = 0; (node < count) && (node < DAT_COUNT_LIMIT) && cursor->fine; ++node) {
        (void)skip_tree_node(cursor, &TREE_SHAPES[kind]);
    }
    return CHECK(count >= 0) && cursor->fine;
}

static bool skip_tech_tree(DatCursor *cursor)
{
    int age_count;
    int building_count;
    int unit_count;
    int research_count;

    if (!CHECK(cursor != NULL) || !cursor->fine) {
        return false;
    }
    age_count = dat_cursor_u8(cursor);
    building_count = dat_cursor_u8(cursor);
    unit_count = dat_cursor_i16(cursor);
    research_count = dat_cursor_u8(cursor);
    return dat_cursor_skip(cursor, TREE_GROUPS_SIZE) && skip_tree_nodes(cursor, TREE_AGE, age_count) &&
           skip_tree_nodes(cursor, TREE_BUILDING, building_count) && skip_tree_nodes(cursor, TREE_UNIT, unit_count) &&
           CHECK(skip_tree_nodes(cursor, TREE_RESEARCH, research_count));
}

static bool map_head(DatCursor *cursor)
{
    return CHECK(map_version(cursor)) && CHECK(dat_skip_head(cursor));
}

static bool map_tech_tables(DatCursor *cursor, DatMap *map)
{
    return CHECK(map_techs(cursor, map)) && CHECK(skip_tech_tree(cursor));
}

static bool map_tables(DatCursor *cursor, DatMap *map)
{
    return CHECK(map_effects(cursor, map)) && CHECK(dat_skip_unit_headers(cursor)) && CHECK(map_civs(cursor, map)) &&
           map_tech_tables(cursor, map);
}

bool dat_map_read(const unsigned char *data, size_t size, DatMap *map)
{
    DatBytes dat;
    DatCursor cursor;

    if (!CHECK(data != NULL) || !CHECK(size > 0U) || !CHECK(map != NULL)) {
        return false;
    }
    dat.data = data;
    dat.size = size;
    map->effect_count = 0;
    map->civ_count = 0;
    map->tech_count = 0;
    return dat_cursor_start(&cursor, &dat, 0U) && map_head(&cursor) && map_tables(&cursor, map) &&
           CHECK(cursor.at == size);
}

bool dat_map_has_civ(const DatMap *map, int civ)
{
    return CHECK(map != NULL) && CHECK(civ >= 0) && CHECK(civ < map->civ_count) && CHECK(civ < DAT_CIV_CAPACITY);
}

bool dat_civ_read(const DatBytes *dat, size_t at, DatCiv *civ)
{
    DatCursor cursor;
    int resource_count;

    if (!CHECK(dat != NULL) || !CHECK(civ != NULL) || !dat_cursor_start(&cursor, dat, at)) {
        return false;
    }
    (void)dat_cursor_skip(&cursor, DAT_PLAYER_TYPE_SIZE);
    (void)dat_cursor_text(&cursor, civ->name, sizeof civ->name);
    resource_count = dat_cursor_i16(&cursor);
    civ->tech_tree_id_at = cursor.at;
    civ->tech_tree_id = dat_cursor_i16(&cursor);
    civ->team_bonus_id_at = cursor.at;
    civ->team_bonus_id = dat_cursor_i16(&cursor);
    (void)dat_cursor_skip_items(&cursor, resource_count, DAT_LONG_WIDTH);
    civ->icon_set_at = cursor.at;
    return cursor.fine;
}

static void skip_units_before(DatCursor *cursor, DatCursor *slots, int unit_id)
{
    int unit;

    if (!CHECK(cursor != NULL) || !CHECK(slots != NULL)) {
        return;
    }
    for (unit = 0; (unit < unit_id) && (unit < DAT_COUNT_LIMIT) && cursor->fine; ++unit) {
        if (dat_cursor_i32(slots) != 0L) {
            (void)dat_skip_unit(cursor);
        }
    }
}

bool dat_find_unit(const DatBytes *dat, size_t civ_at, int unit_id, size_t *unit_at)
{
    DatCursor cursor;
    DatCursor slots;
    int unit_count;

    if (!CHECK(dat != NULL) || !CHECK(unit_at != NULL) || !dat_cursor_start(&cursor, dat, civ_at)) {
        return false;
    }
    unit_count = dat_skip_civ_head(&cursor);
    if ((unit_id < 0) || (unit_id >= unit_count)) {
        return false;
    }
    slots = cursor;
    (void)dat_cursor_skip_items(&cursor, unit_count, DAT_POINTER_SIZE);
    skip_units_before(&cursor, &slots, unit_id);
    *unit_at = cursor.at;
    return (dat_cursor_i32(&slots) != 0L) && cursor.fine;
}

static void place_units(DatUnitList *list, DatCursor *cursor, DatCursor slots)
{
    int unit;

    if (!CHECK(list != NULL) || !CHECK(cursor != NULL)) {
        return;
    }
    for (unit = 0; (unit < list->count) && (unit < DAT_UNIT_CAPACITY) && cursor->fine; ++unit) {
        list->at[unit] = 0U;
        if (dat_cursor_i32(&slots) != 0L) {
            list->at[unit] = cursor->at;
            (void)dat_skip_unit(cursor);
        }
    }
}

bool dat_list_units(const DatBytes *dat, size_t civ_at, DatUnitList *list)
{
    DatCursor cursor;
    DatCursor slots;

    if (!CHECK(dat != NULL) || !CHECK(list != NULL) || !dat_cursor_start(&cursor, dat, civ_at)) {
        return false;
    }
    list->count = dat_skip_civ_head(&cursor);
    if (!CHECK(list->count >= 0) || !CHECK(list->count <= DAT_UNIT_CAPACITY)) {
        list->count = 0;
        return false;
    }
    slots = cursor;
    (void)dat_cursor_skip_items(&cursor, list->count, DAT_POINTER_SIZE);
    place_units(list, &cursor, slots);
    return cursor.fine;
}

bool dat_unit_read(const DatBytes *dat, size_t at, DatUnit *unit)
{
    DatCursor cursor;
    DatCursor fields;

    if (!CHECK(dat != NULL) || !CHECK(unit != NULL) || !dat_cursor_start(&cursor, dat, at) ||
        !dat_cursor_start(&fields, dat, at + DAT_UNIT_CLASS_AT) || !dat_read_unit(&cursor, unit)) {
        return false;
    }
    unit->unit_class = dat_cursor_i16(&fields);
    (void)dat_cursor_skip(&fields, DAT_UNIT_DEAD_UNIT_AT - DAT_UNIT_CLASS_AT - DAT_SHORT_WIDTH);
    unit->dead_unit = dat_cursor_i16(&fields);
    return fields.fine;
}

bool dat_effect_read(const DatBytes *dat, size_t at, DatEffect *effect)
{
    DatCursor cursor;

    if (!CHECK(dat != NULL) || !CHECK(effect != NULL) || !dat_cursor_start(&cursor, dat, at)) {
        return false;
    }
    (void)dat_cursor_text(&cursor, effect->name, sizeof effect->name);
    effect->change_count = dat_cursor_i16(&cursor);
    effect->changes_at = cursor.at;
    return CHECK(effect->change_count >= 0) &&
           dat_cursor_skip_items(&cursor, effect->change_count, DAT_CHANGE_SIZE);
}

bool dat_change_read(const DatBytes *dat, size_t at, DatChange *change)
{
    DatCursor cursor;

    if (!CHECK(dat != NULL) || !CHECK(change != NULL) || !dat_cursor_start(&cursor, dat, at)) {
        return false;
    }
    change->type = dat_cursor_u8(&cursor);
    change->a = dat_cursor_i16(&cursor);
    change->b = dat_cursor_i16(&cursor);
    change->c = dat_cursor_i16(&cursor);
    change->d = dat_cursor_f32(&cursor);
    return cursor.fine;
}

static void take_tech_head(DatCursor *cursor, DatTech *tech)
{
    int slot;

    if (!CHECK(cursor != NULL) || !CHECK(tech != NULL)) {
        return;
    }
    for (slot = 0; slot < DAT_REQUIRED_TECH_SLOTS; ++slot) {
        tech->required_techs[slot] = dat_cursor_i16(cursor);
    }
    for (slot = 0; slot < DAT_TECH_COST_SLOTS; ++slot) {
        tech->costs[slot].type = dat_cursor_i16(cursor);
        tech->costs[slot].amount = dat_cursor_i16(cursor);
        tech->costs[slot].flag = dat_cursor_u8(cursor);
    }
    tech->required_tech_count = dat_cursor_i16(cursor);
    tech->civ_at = cursor->at;
    tech->civ = dat_cursor_i16(cursor);
    tech->full_tech_mode = dat_cursor_i16(cursor);
    tech->language_dll_name = dat_cursor_i32(cursor);
    tech->language_dll_description = dat_cursor_i32(cursor);
    tech->effect_id = dat_cursor_i16(cursor);
    tech->type = dat_cursor_i16(cursor);
    tech->icon_id = dat_cursor_i16(cursor);
    tech->language_dll_help = dat_cursor_i32(cursor);
    tech->language_dll_tech_tree = dat_cursor_i32(cursor);
}

static void take_research_location(DatCursor *cursor, DatResearchLocation *location)
{
    if (!CHECK(cursor != NULL) || !CHECK(location != NULL)) {
        return;
    }
    location->location_id = dat_cursor_i16(cursor);
    location->research_time = dat_cursor_i16(cursor);
    location->button_id = dat_cursor_u8(cursor);
    location->hot_key_id = dat_cursor_i32(cursor);
}

bool dat_tech_read(const DatBytes *dat, size_t at, DatTech *tech)
{
    DatCursor cursor;
    int location;

    if (!CHECK(dat != NULL) || !CHECK(tech != NULL) || !dat_cursor_start(&cursor, dat, at)) {
        return false;
    }
    take_tech_head(&cursor, tech);
    (void)dat_cursor_text(&cursor, tech->name, sizeof tech->name);
    tech->repeatable = dat_cursor_u8(&cursor);
    tech->research_location_count = dat_cursor_i16(&cursor);
    tech->research_locations_at = cursor.at;
    if (!CHECK(tech->research_location_count >= 0) ||
        !CHECK(tech->research_location_count <= DAT_RESEARCH_LOCATION_CAPACITY)) {
        return false;
    }
    for (location = 0; (location < tech->research_location_count) && (location < DAT_RESEARCH_LOCATION_CAPACITY);
         ++location) {
        take_research_location(&cursor, &tech->research_locations[location]);
    }
    return cursor.fine;
}
