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
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_APPEARANCE, NO, CHOICE_REPLACE, APPEARANCE_BASE, NO },
    { SOURCE_NONE, TEXT_FIELD_NAME, NO, NO, NO },
    { SOURCE_NONE, TEXT_FIELD_TAGLINE, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_APPEARANCE, NO, CHOICE_ARCHITECTURE, APPEARANCE_ARCHITECTURE, NO },
    { SOURCE_APPEARANCE, NO, CHOICE_LANGUAGE, APPEARANCE_CIV, NO },
    { SOURCE_APPEARANCE, NO, CHOICE_WONDER, APPEARANCE_CIV, NO },
    { SOURCE_APPEARANCE, NO, CHOICE_CASTLE, APPEARANCE_CIV, NO },
    { SOURCE_APPEARANCE, NO, CHOICE_MONK, APPEARANCE_MONK, NO },
    { SOURCE_APPEARANCE, NO, CHOICE_SCOUT, APPEARANCE_SCOUT, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_TREE, NO, NO, NO, CONTROL_TREE_FILTER },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_BONUS, NO, NO, NO, CONTROL_BONUS_FILTER },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_TEAM_BONUS, NO, NO, NO, CONTROL_TEAM_FILTER },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_UNIT, NO, NO, NO, CONTROL_UNIT_FILTER },
    { SOURCE_NONE, TEXT_FIELD_UNIT_NAME, NO, NO, NO },
    { SOURCE_NONE, TEXT_FIELD_UNIT_DESCRIPTION, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_CASTLE_TECH, NO, NO, NO, CONTROL_CASTLE_TECH_FILTER },
    { SOURCE_NONE, TEXT_FIELD_CASTLE_TECH_NAME, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_IMPERIAL_TECH, NO, NO, NO, CONTROL_IMPERIAL_TECH_FILTER },
    { SOURCE_NONE, TEXT_FIELD_IMPERIAL_TECH_NAME, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_HERO, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO },
    { SOURCE_NONE, NO, NO, NO, NO }
};

static const char TREE_KIND_NAMES[TREE_KIND_COUNT][KIND_NAME_WIDTH] = { "Unit: ", "Building: ", "Tech: " };

const Binding *binding_of(ControlId control)
{
    static const Binding NONE = { SOURCE_NONE, NO, NO, NO, NO };

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
    if (pick_kind(source) == PICK_UNIT) {
        return catalog->unit_count;
    }
    return (pick_kind(source) == PICK_TECH) ? catalog->tech_count : ticked_size(catalog, source);
}

static bool item_belongs(const Catalog *catalog, ListItem item)
{
    ListSource source;

    source = BINDINGS[item.control].source;
    if (pick_kind(source) == PICK_UNIT) {
        return (catalog->units[item.item].role == UNIT_ROLE_HERO) == (source == SOURCE_HERO);
    }
    if (pick_kind(source) == PICK_TECH) {
        return catalog->techs[item.item].tier == tier_of(source);
    }
    if (source == SOURCE_APPEARANCE) {
        return ((int)catalog->appearance[item.item].kind == BINDINGS[item.control].appearance) &&
               ((BINDINGS[item.control].appearance != (int)APPEARANCE_BASE) ||
                catalog_is_ranked(catalog, catalog->appearance[item.item].id));
    }
    return source != SOURCE_NONE;
}

typedef struct {
    bool civs[CATALOG_BASE_CAPACITY];
} NamedCivs;

static bool civ_has(const Catalog *catalog, const BaseCiv *civ, ListSource source, int item)
{
    switch ((int)source) {
    case SOURCE_TREE:
        return ((catalog->tree[item].civs >> civ->id) & 1U) != 0U;
    case SOURCE_BONUS:
        return tsv_list_holds(&civ->bonuses, catalog->bonuses[item].id);
    case SOURCE_TEAM_BONUS:
        return civ->values[BASE_TEAM_BONUS] == catalog->team_bonuses[item].id;
    case SOURCE_UNIT:
        return (catalog->units[item].role == UNIT_ROLE_VANILLA) && (civ->values[BASE_UNIT] == catalog->units[item].id);
    case SOURCE_CASTLE_TECH:
        return civ->values[BASE_CASTLE_TECH] == catalog->techs[item].id;
    case SOURCE_IMPERIAL_TECH:
        return civ->values[BASE_IMPERIAL_TECH] == catalog->techs[item].id;
    default:
        return false;
    }
}

static void name_civs(const Catalog *catalog, const char *filter, NamedCivs *named)
{
    int civ;

    for (civ = 0; (civ < catalog->base_count) && (civ < CATALOG_BASE_CAPACITY); ++civ) {
        named->civs[civ] = (filter[0] != '\0') &&
                           text_starts_folded(catalog_appearance_name(catalog, APPEARANCE_BASE, catalog->bases[civ].id),
                                              filter);
    }
}

static bool civ_matches(const Catalog *catalog, ListItem item, const NamedCivs *named)
{
    ListSource source = BINDINGS[item.control].source;
    int civ;

    for (civ = 0; (civ < catalog->base_count) && (civ < CATALOG_BASE_CAPACITY); ++civ) {
        if (named->civs[civ] && civ_has(catalog, &catalog->bases[civ], source, item.item)) {
            return true;
        }
    }
    return false;
}

static bool item_matches(const Catalog *catalog, ListItem item, const ListQuery *query, const NamedCivs *named)
{
    RowText row;

    if ((query->category != LIST_ALL_CATEGORIES) && (BINDINGS[item.control].source == SOURCE_BONUS) &&
        ((int)catalog->bonuses[item.item].category != query->category)) {
        return false;
    }
    if (query->filter[0] == '\0') {
        return true;
    }
    row = list_item_text(catalog, item);
    return text_contains_folded(row.text, query->filter) || civ_matches(catalog, item, named);
}

ControlId binding_filtered_list(ControlId filter)
{
    int control;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        if (BINDINGS[control].filter == (int)filter) {
            return (ControlId)control;
        }
    }
    return CONTROL_COUNT;
}

static void add_row(ListModel *model, int item)
{
    if (model->count >= LIST_ROW_CAPACITY) {
        return;
    }
    model->items[model->count] = item;
    ++model->count;
}

bool list_model_fill(ListModel *model, const Catalog *catalog, const ListQuery *query)
{
    static NamedCivs named;
    ListItem item;
    int size;

    name_civs(catalog, query->filter, &named);
    model->count = 0;
    item.control = query->control;
    size = source_size(catalog, BINDINGS[query->control].source);
    for (item.item = 0; (item.item < size) && (item.item < LIST_ROW_CAPACITY); ++item.item) {
        if (item_belongs(catalog, item) && item_matches(catalog, item, query, &named)) {
            add_row(model, item.item);
        }
    }
    return true;
}

static RowText row_of(const char *first, const char *second)
{
    RowText row;

    row.text[0] = '\0';
    (void)text_append(row.text, sizeof row.text, first);
    (void)text_append(row.text, sizeof row.text, second);
    return row;
}

static RowText picked_row(const Catalog *catalog, ListItem item, ListSource source)
{
    const UnitEntry *unit;

    if (pick_kind(source) == PICK_TECH) {
        return row_of(catalog->techs[item.item].name, "");
    }
    unit = &catalog->units[item.item];
    return row_of(unit->name, ((source == SOURCE_UNIT) && (unit->role == UNIT_ROLE_CUSTOM)) ? " (custom)" : "");
}

static RowText ticked_row(const Catalog *catalog, ListItem item, ListSource source)
{
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
    ListSource source = BINDINGS[item.control].source;

    return is_pick_source(source) ? picked_row(catalog, item, source) : ticked_row(catalog, item, source);
}

static bool unit_is_chosen(const CivDesign *design, const UnitEntry *unit)
{
    UnitOrigin origin;

    origin = (unit->role == UNIT_ROLE_CUSTOM) ? UNIT_ORIGIN_CUSTOM : UNIT_ORIGIN_VANILLA;
    return (design->unique_unit.origin == origin) && (design->unique_unit.id == unit->id);
}

static bool pick_is_chosen(const Catalog *catalog, const CivDesign *design, ListItem item, ListSource source)
{
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
    ListSource source = BINDINGS[item.control].source;

    return is_pick_source(source) ? pick_is_chosen(catalog, design, item, source)
                                  : tick_is_chosen(catalog, design, item, source);
}

static Command pick_command(const Catalog *catalog, ListItem item, ListSource source)
{
    const UnitEntry *unit;

    if (pick_kind(source) == PICK_TECH) {
        return command_pick(COMMAND_PICK_TECH, tier_of(source), catalog->techs[item.item].id,
                            catalog->techs[item.item].name);
    }
    unit = &catalog->units[item.item];
    if (source == SOURCE_HERO) {
        return command_pick(COMMAND_PICK_HERO, 0, unit->id, unit->name);
    }
    return command_pick(COMMAND_PICK_UNIT,
                        (unit->role == UNIT_ROLE_CUSTOM) ? (int)UNIT_ORIGIN_CUSTOM : (int)UNIT_ORIGIN_VANILLA, unit->id,
                        unit->name);
}

static Command tick_command(const Catalog *catalog, const CivDesign *design, ListItem item, ListSource source)
{
    if (source == SOURCE_TREE) {
        return tree_command(catalog, design, item.item);
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

Command list_item_command(const Catalog *catalog, const CivDesign *design, ListItem item)
{
    ListSource source = BINDINGS[item.control].source;

    return is_pick_source(source) ? pick_command(catalog, item, source)
                                  : tick_command(catalog, design, item, source);
}

Command hero_box_command(const Catalog *catalog, bool ticked)
{
    ListItem item = { CONTROL_HERO_CHOOSE, CATALOG_MISSING };

    item.item = ticked ? catalog_find(catalog, CATALOG_UNITS, UNIT_ROLE_HERO, HERO_AT_FIRST) : CATALOG_MISSING;
    if (item.item == CATALOG_MISSING) {
        return command_of(COMMAND_PICK_HERO, 0, CIV_NO_CHOICE);
    }
    return pick_command(catalog, item, SOURCE_HERO);
}
