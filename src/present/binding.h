#ifndef BINDING_H
#define BINDING_H

#include <stdbool.h>

#include "../data/catalog.h"
#include "../model/command.h"
#include "../view/layout.h"

enum {
    LIST_ROW_CAPACITY = 640,
    LIST_FILTER_CAPACITY = 64,
    LIST_TEXT_CAPACITY = 400,
    LIST_ALL_CATEGORIES = -1,
    BINDING_NO_FIELD = -1,
    HERO_AT_FIRST = 777
};

typedef enum {
    SOURCE_NONE,
    SOURCE_TREE,
    SOURCE_BONUS,
    SOURCE_TEAM_BONUS,
    SOURCE_UNIT,
    SOURCE_CASTLE_TECH,
    SOURCE_IMPERIAL_TECH,
    SOURCE_HERO,
    SOURCE_APPEARANCE,
    SOURCE_COUNT
} ListSource;

typedef struct {
    ListSource source;
    int text_field;
    int choice;
    int appearance;
} Binding;

typedef struct {
    ControlId control;
    int item;
} ListItem;

typedef struct {
    ControlId control;
    char filter[LIST_FILTER_CAPACITY];
    int category;
} ListQuery;

typedef struct {
    int items[LIST_ROW_CAPACITY];
    int count;
} ListModel;

typedef struct {
    char text[LIST_TEXT_CAPACITY];
} RowText;

const Binding *binding_of(ControlId control);
bool list_model_fill(ListModel *model, const Catalog *catalog, const ListQuery *query);
RowText list_item_text(const Catalog *catalog, ListItem item);
bool list_item_is_chosen(const Catalog *catalog, const CivDesign *design, ListItem item);
Command list_item_command(const Catalog *catalog, ListItem item);
Command hero_box_command(const Catalog *catalog, bool ticked);

#endif
