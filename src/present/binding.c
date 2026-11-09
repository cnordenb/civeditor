#include "binding.h"

#include "../core/check.h"
#include "../core/text.h"

enum { NO = -1, KIND_NAME_WIDTH = 12 };

typedef struct {
    ListSource source;
    int text_field;
    int choice;
    int appearance;
} Binding;

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

int binding_text_field(ControlId control)
{
    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT)) {
        return BINDING_NO_FIELD;
    }
    return BINDINGS[control].text_field;
}

ListSource binding_source(ControlId control)
{
    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT)) {
        return SOURCE_NONE;
    }
    return BINDINGS[control].source;
}

static int source_size(const Catalog *catalog, ListSource source)
{
    if (!CHECK(catalog != NULL) || !CHECK(source < SOURCE_COUNT)) {
        return 0;
    }
    switch (source) {
    case SOURCE_TREE:
        return catalog->tree_count;
    case SOURCE_BONUS:
        return catalog->bonus_count;
    case SOURCE_TEAM_BONUS:
        return catalog->team_bonus_count;
    case SOURCE_UNIT:
    case SOURCE_HERO:
        return catalog->unit_count;
    case SOURCE_CASTLE_TECH:
    case SOURCE_IMPERIAL_TECH:
        return catalog->tech_count;
    case SOURCE_APPEARANCE:
        return catalog->appearance_count;
    case SOURCE_NONE:
    case SOURCE_COUNT:
    default:
        return 0;
    }
}

static bool item_belongs(const Catalog *catalog, ListItem item)
{
    if (!CHECK(catalog != NULL) || !CHECK((item.item >= 0) && (item.item < LIST_ROW_CAPACITY))) {
        return false;
    }
    switch (BINDINGS[item.control].source) {
    case SOURCE_UNIT:
        return catalog->units[item.item].role != UNIT_ROLE_HERO;
    case SOURCE_HERO:
        return catalog->units[item.item].role == UNIT_ROLE_HERO;
    case SOURCE_CASTLE_TECH:
        return catalog->techs[item.item].tier == TECH_TIER_CASTLE;
    case SOURCE_IMPERIAL_TECH:
        return catalog->techs[item.item].tier == TECH_TIER_IMPERIAL;
    case SOURCE_APPEARANCE:
        return (int)catalog->appearance[item.item].kind == BINDINGS[item.control].appearance;
    case SOURCE_TREE:
    case SOURCE_BONUS:
    case SOURCE_TEAM_BONUS:
        return true;
    case SOURCE_NONE:
    case SOURCE_COUNT:
    default:
        return false;
    }
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
        model->items[0] = LIST_NO_ITEM;
        model->count = 1;
    }
    for (item.item = 0; (item.item < size) && (item.item < LIST_ROW_CAPACITY); ++item.item) {
        if (item_belongs(catalog, item) && item_matches(catalog, item, query) && (model->count < LIST_ROW_CAPACITY)) {
            model->items[model->count] = item.item;
            ++model->count;
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

RowText list_item_text(const Catalog *catalog, ListItem item)
{
    if (!CHECK(catalog != NULL) || !CHECK(item.item < LIST_ROW_CAPACITY) || (item.item == LIST_NO_ITEM)) {
        return row_of("No hero", "");
    }
    switch (BINDINGS[item.control].source) {
    case SOURCE_TREE:
        return row_of(TREE_KIND_NAMES[catalog->tree[item.item].kind], catalog->tree[item.item].name);
    case SOURCE_BONUS:
        return row_of(catalog->bonuses[item.item].description, "");
    case SOURCE_TEAM_BONUS:
        return row_of(catalog->team_bonuses[item.item].description, "");
    case SOURCE_UNIT:
        return row_of(catalog->units[item.item].name,
                      (catalog->units[item.item].role == UNIT_ROLE_CUSTOM) ? " (custom)" : "");
    case SOURCE_HERO:
        return row_of(catalog->units[item.item].name, "");
    case SOURCE_CASTLE_TECH:
    case SOURCE_IMPERIAL_TECH:
        return row_of(catalog->techs[item.item].name, "");
    case SOURCE_APPEARANCE:
        return row_of(catalog->appearance[item.item].name, "");
    case SOURCE_NONE:
    case SOURCE_COUNT:
    default:
        return row_of("", "");
    }
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

bool list_item_is_chosen(const Catalog *catalog, const CivDesign *design, ListItem item)
{
    if (!CHECK(catalog != NULL) || !CHECK(design != NULL) || (item.item == LIST_NO_ITEM)) {
        return (design != NULL) && (design->hero_unit == CIV_NO_CHOICE);
    }
    switch (BINDINGS[item.control].source) {
    case SOURCE_TREE:
        return civ_design_tree_has(design, catalog->tree[item.item].kind, catalog->tree[item.item].id);
    case SOURCE_BONUS:
        return civ_design_has_bonus(design, catalog->bonuses[item.item].id);
    case SOURCE_TEAM_BONUS:
        return civ_design_has_team_bonus(design, catalog->team_bonuses[item.item].id);
    case SOURCE_UNIT:
        return unit_is_chosen(design, &catalog->units[item.item]);
    case SOURCE_HERO:
        return design->hero_unit == catalog->units[item.item].id;
    case SOURCE_CASTLE_TECH:
        return design->techs[TECH_TIER_CASTLE].km_index == catalog->techs[item.item].id;
    case SOURCE_IMPERIAL_TECH:
        return design->techs[TECH_TIER_IMPERIAL].km_index == catalog->techs[item.item].id;
    case SOURCE_APPEARANCE:
        return design->choices[BINDINGS[item.control].choice] == catalog->appearance[item.item].id;
    case SOURCE_NONE:
    case SOURCE_COUNT:
    default:
        return false;
    }
}

Command list_item_command(const Catalog *catalog, ListItem item)
{
    if (!CHECK(catalog != NULL) || !CHECK(item.item < LIST_ROW_CAPACITY) || (item.item == LIST_NO_ITEM)) {
        return command_of(COMMAND_PICK_HERO, 0, CIV_NO_CHOICE);
    }
    switch (BINDINGS[item.control].source) {
    case SOURCE_TREE:
        return command_of(COMMAND_TOGGLE_TREE, (int)catalog->tree[item.item].kind, catalog->tree[item.item].id);
    case SOURCE_BONUS:
        return command_of(COMMAND_TOGGLE_BONUS, 0, catalog->bonuses[item.item].id);
    case SOURCE_TEAM_BONUS:
        return command_of(COMMAND_TOGGLE_TEAM_BONUS, 0, catalog->team_bonuses[item.item].id);
    case SOURCE_UNIT:
        return command_of(COMMAND_PICK_UNIT,
                          (catalog->units[item.item].role == UNIT_ROLE_CUSTOM) ? (int)UNIT_ORIGIN_CUSTOM
                                                                               : (int)UNIT_ORIGIN_VANILLA,
                          catalog->units[item.item].id);
    case SOURCE_HERO:
        return command_of(COMMAND_PICK_HERO, 0, catalog->units[item.item].id);
    case SOURCE_CASTLE_TECH:
        return command_of(COMMAND_PICK_TECH, TECH_TIER_CASTLE, catalog->techs[item.item].id);
    case SOURCE_IMPERIAL_TECH:
        return command_of(COMMAND_PICK_TECH, TECH_TIER_IMPERIAL, catalog->techs[item.item].id);
    case SOURCE_APPEARANCE:
        return command_of(COMMAND_SET_CHOICE, BINDINGS[item.control].choice, catalog->appearance[item.item].id);
    case SOURCE_NONE:
    case SOURCE_COUNT:
    default:
        return command_nothing();
    }
}
