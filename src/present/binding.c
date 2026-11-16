#include "binding.h"

#include "../core/check.h"
#include "../core/text.h"

enum { NO = -1, KIND_NAME_WIDTH = 12 };

typedef enum {
    PICK_NONE,
    PICK_UNIT,
    PICK_TECH
} PickKind;

static const Binding BINDINGS[CONTROL_COUNT] = {
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_APPEARANCE, NO, CHOICE_BASE, APPEARANCE_BASE },
    { SOURCE_NONE, TEXT_FIELD_NAME, NO, NO },
    { SOURCE_NONE, TEXT_FIELD_TAGLINE, NO, NO },
    { SOURCE_NONE, TEXT_FIELD_DESCRIPTION, NO, NO },
    { SOURCE_APPEARANCE, NO, CHOICE_ARCHITECTURE, APPEARANCE_ARCHITECTURE },
    { SOURCE_APPEARANCE, NO, CHOICE_LANGUAGE, APPEARANCE_CIV },
    { SOURCE_APPEARANCE, NO, CHOICE_WONDER, APPEARANCE_CIV },
    { SOURCE_APPEARANCE, NO, CHOICE_CASTLE, APPEARANCE_CIV },
    { SOURCE_APPEARANCE, NO, CHOICE_MONK, APPEARANCE_MONK },
    { SOURCE_APPEARANCE, NO, CHOICE_SCOUT, APPEARANCE_SCOUT },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_TREE, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_BONUS, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_TEAM_BONUS, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_UNIT, NO, NO, NO },
    { SOURCE_NONE, TEXT_FIELD_UNIT_NAME, NO, NO },
    { SOURCE_NONE, TEXT_FIELD_UNIT_DESCRIPTION, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_CASTLE_TECH, NO, NO, NO },
    { SOURCE_NONE, TEXT_FIELD_CASTLE_TECH_NAME, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_IMPERIAL_TECH, NO, NO, NO },
    { SOURCE_NONE, TEXT_FIELD_IMPERIAL_TECH_NAME, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_HERO, NO, NO, NO },
    { SOURCE_NONE, TEXT_FIELD_HERO_NAME, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_APPEARANCE, NO, CHOICE_REPLACE, APPEARANCE_CIV },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO }
};

static const char TREE_KIND_NAMES[TREE_KIND_COUNT][KIND_NAME_WIDTH] = { "Unit: ", "Building: ", "Tech: " };

const Binding *binding_of(ControlId control)
{
    static const Binding NONE = { SOURCE_NONE, NO, NO, NO };

    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT)) {
        return &NONE;
    }
    return &BINDINGS[control];
}

static PickKind pick_kind(ListSource source)
{
    switch ((int)source) {
    case SOURCE_UNIT:
    case SOURCE_HERO:
        return PICK_UNIT;
    case SOURCE_CASTLE_TECH:
    case SOURCE_IMPERIAL_TECH:
        return PICK_TECH;
    default:
        return PICK_NONE;
    }
}

static bool is_pick_source(ListSource source)
{
    return pick_kind(source) != PICK_NONE;
}

static TechTier tier_of(ListSource source)
{
    return (source == SOURCE_CASTLE_TECH) ? TECH_TIER_CASTLE : TECH_TIER_IMPERIAL;
}

static int ticked_size(const Catalog *catalog, ListSource source)
{
    if (!CHECK(catalog != NULL) || !CHECK(source < SOURCE_COUNT)) {
        return 0;
    }
    if (source == SOURCE_TREE) {
        return catalog->tree_count;
    }
    if (source == SOURCE_BONUS) {
        return catalog->bonus_count;
    }
    if (source == SOURCE_TEAM_BONUS) {
        return catalog->team_bonus_count;
    }
    return (source == SOURCE_APPEARANCE) ? catalog->appearance_count : 0;
}

static int source_size(const Catalog *catalog, ListSource source)
{
    if (!CHECK(catalog != NULL) || !CHECK(source < SOURCE_COUNT)) {
        return 0;
    }
    if (pick_kind(source) == PICK_UNIT) {
        return catalog->unit_count;
    }
    return (pick_kind(source) == PICK_TECH) ? catalog->tech_count : ticked_size(catalog, source);
}

static bool item_belongs(const Catalog *catalog, ListItem item)
{
    ListSource source;

    if (!CHECK(catalog != NULL) || !CHECK(item.item >= 0) || !CHECK(item.item < LIST_ROW_CAPACITY)) {
        return false;
    }
    source = BINDINGS[item.control].source;
    if (pick_kind(source) == PICK_UNIT) {
        return (catalog->units[item.item].role == UNIT_ROLE_HERO) == (source == SOURCE_HERO);
    }
    if (pick_kind(source) == PICK_TECH) {
        return catalog->techs[item.item].tier == tier_of(source);
    }
    if (source == SOURCE_APPEARANCE) {
        return (int)catalog->appearance[item.item].kind == BINDINGS[item.control].appearance;
    }
    return source != SOURCE_NONE;
}

static bool item_matches(const Catalog *catalog, ListItem item, const ListQuery *query)
{
    RowText row;

    if (!CHECK(catalog != NULL) || !CHECK(query != NULL)) {
        return false;
    }
    if ((query->category != LIST_ALL_CATEGORIES) && (BINDINGS[item.control].source == SOURCE_BONUS) &&
        ((int)catalog->bonuses[item.item].category != query->category)) {
        return false;
    }
    if (query->filter[0] == '\0') {
        return true;
    }
    row = list_item_text(catalog, item);
    return text_contains_folded(row.text, query->filter);
}

static void add_row(ListModel *model, int item)
{
    if (!CHECK(model != NULL) || !CHECK(model->count >= 0) || (model->count >= LIST_ROW_CAPACITY)) {
        return;
    }
    model->items[model->count] = item;
    ++model->count;
}

bool list_model_fill(ListModel *model, const Catalog *catalog, const ListQuery *query)
{
    ListItem item;
    int size;

    if (!CHECK(model != NULL) || !CHECK(catalog != NULL) || !CHECK(query != NULL)) {
        return false;
    }
    model->count = 0;
    item.control = query->control;
    size = source_size(catalog, BINDINGS[query->control].source);
    if (BINDINGS[query->control].source == SOURCE_HERO) {
        add_row(model, LIST_NO_ITEM);
    }
    for (item.item = 0; (item.item < size) && (item.item < LIST_ROW_CAPACITY); ++item.item) {
        if (item_belongs(catalog, item) && item_matches(catalog, item, query)) {
            add_row(model, item.item);
        }
    }
    return true;
}

static RowText row_of(const char *first, const char *second)
{
    RowText row;

    row.text[0] = '\0';
    if (!CHECK(first != NULL) || !CHECK(second != NULL)) {
        return row;
    }
    (void)text_append(row.text, sizeof row.text, first);
    (void)text_append(row.text, sizeof row.text, second);
    return row;
}

static RowText picked_row(const Catalog *catalog, ListItem item, ListSource source)
{
    const UnitEntry *unit;

    if (!CHECK(catalog != NULL) || !CHECK(item.item >= 0) ||
        !CHECK(item.item < LIST_ROW_CAPACITY)) {
        return row_of("", "");
    }
    if (pick_kind(source) == PICK_TECH) {
        return row_of(catalog->techs[item.item].name, "");
    }
    unit = &catalog->units[item.item];
    return row_of(unit->name, ((source == SOURCE_UNIT) && (unit->role == UNIT_ROLE_CUSTOM)) ? " (custom)" : "");
}

static RowText ticked_row(const Catalog *catalog, ListItem item, ListSource source)
{
    if (!CHECK(catalog != NULL) || !CHECK(item.item >= 0) ||
        !CHECK(item.item < LIST_ROW_CAPACITY)) {
        return row_of("", "");
    }
    if (source == SOURCE_TREE) {
        return row_of(TREE_KIND_NAMES[catalog->tree[item.item].kind], catalog->tree[item.item].name);
    }
    if (source == SOURCE_BONUS) {
        return row_of(catalog->bonuses[item.item].description, "");
    }
    if (source == SOURCE_TEAM_BONUS) {
        return row_of(catalog->team_bonuses[item.item].description, "");
    }
    return row_of((source == SOURCE_APPEARANCE) ? catalog->appearance[item.item].name : "", "");
}

RowText list_item_text(const Catalog *catalog, ListItem item)
{
    ListSource source;

    if (!CHECK(catalog != NULL) || !CHECK(item.item < LIST_ROW_CAPACITY) || (item.item == LIST_NO_ITEM)) {
        return row_of("No hero", "");
    }
    source = BINDINGS[item.control].source;
    return is_pick_source(source) ? picked_row(catalog, item, source) : ticked_row(catalog, item, source);
}

static bool unit_is_chosen(const CivDesign *design, const UnitEntry *unit)
{
    UnitOrigin origin;

    if (!CHECK(design != NULL) || !CHECK(unit != NULL)) {
        return false;
    }
    origin = (unit->role == UNIT_ROLE_CUSTOM) ? UNIT_ORIGIN_CUSTOM : UNIT_ORIGIN_VANILLA;
    return (design->unique_unit.origin == origin) && (design->unique_unit.id == unit->id);
}

static bool pick_is_chosen(const Catalog *catalog, const CivDesign *design, ListItem item, ListSource source)
{
    if (!CHECK(catalog != NULL) || !CHECK(design != NULL) ||
        !CHECK(item.item >= 0)) {
        return false;
    }
    if (source == SOURCE_HERO) {
        return design->hero_unit == catalog->units[item.item].id;
    }
    if (source == SOURCE_UNIT) {
        return unit_is_chosen(design, &catalog->units[item.item]);
    }
    return design->techs[tier_of(source)].km_index == catalog->techs[item.item].id;
}

static bool tick_is_chosen(const Catalog *catalog, const CivDesign *design, ListItem item, ListSource source)
{
    if (!CHECK(catalog != NULL) || !CHECK(design != NULL) ||
        !CHECK(item.item >= 0)) {
        return false;
    }
    if (source == SOURCE_TREE) {
        return civ_design_tree_has(design, catalog->tree[item.item].kind, catalog->tree[item.item].id);
    }
    if (source == SOURCE_BONUS) {
        return civ_design_has_bonus(design, catalog->bonuses[item.item].id);
    }
    if (source == SOURCE_TEAM_BONUS) {
        return civ_design_has_team_bonus(design, catalog->team_bonuses[item.item].id);
    }
    return (source == SOURCE_APPEARANCE) &&
           (design->choices[BINDINGS[item.control].choice] == catalog->appearance[item.item].id);
}

bool list_item_is_chosen(const Catalog *catalog, const CivDesign *design, ListItem item)
{
    ListSource source;

    if (!CHECK(catalog != NULL) || !CHECK(design != NULL)) {
        return false;
    }
    if (item.item == LIST_NO_ITEM) {
        return design->hero_unit == CIV_NO_CHOICE;
    }
    source = BINDINGS[item.control].source;
    return is_pick_source(source) ? pick_is_chosen(catalog, design, item, source)
                                  : tick_is_chosen(catalog, design, item, source);
}

static Command pick_command(const Catalog *catalog, ListItem item, ListSource source)
{
    const UnitEntry *unit;

    if (!CHECK(catalog != NULL) || !CHECK(item.item >= 0) ||
        !CHECK(item.item < LIST_ROW_CAPACITY)) {
        return command_nothing();
    }
    if (pick_kind(source) == PICK_TECH) {
        return command_of(COMMAND_PICK_TECH, tier_of(source), catalog->techs[item.item].id);
    }
    unit = &catalog->units[item.item];
    if (source == SOURCE_HERO) {
        return command_of(COMMAND_PICK_HERO, 0, unit->id);
    }
    return command_of(COMMAND_PICK_UNIT,
                      (unit->role == UNIT_ROLE_CUSTOM) ? (int)UNIT_ORIGIN_CUSTOM : (int)UNIT_ORIGIN_VANILLA, unit->id);
}

static Command tick_command(const Catalog *catalog, ListItem item, ListSource source)
{
    if (!CHECK(catalog != NULL) || !CHECK(item.item >= 0) ||
        !CHECK(item.item < LIST_ROW_CAPACITY)) {
        return command_nothing();
    }
    if (source == SOURCE_TREE) {
        return command_of(COMMAND_TOGGLE_TREE, (int)catalog->tree[item.item].kind, catalog->tree[item.item].id);
    }
    if (source == SOURCE_BONUS) {
        return command_of(COMMAND_TOGGLE_BONUS, 0, catalog->bonuses[item.item].id);
    }
    if (source == SOURCE_TEAM_BONUS) {
        return command_of(COMMAND_TOGGLE_TEAM_BONUS, 0, catalog->team_bonuses[item.item].id);
    }
    if (source == SOURCE_APPEARANCE) {
        return command_of(COMMAND_SET_CHOICE, BINDINGS[item.control].choice, catalog->appearance[item.item].id);
    }
    return command_nothing();
}

Command list_item_command(const Catalog *catalog, ListItem item)
{
    ListSource source;

    if (!CHECK(catalog != NULL) || !CHECK(item.item < LIST_ROW_CAPACITY) || (item.item == LIST_NO_ITEM)) {
        return command_of(COMMAND_PICK_HERO, 0, CIV_NO_CHOICE);
    }
    source = BINDINGS[item.control].source;
    return is_pick_source(source) ? pick_command(catalog, item, source) : tick_command(catalog, item, source);
}
