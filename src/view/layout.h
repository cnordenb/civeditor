#ifndef LAYOUT_H
#define LAYOUT_H

#include <stdbool.h>

#include "wizard.h"

enum {
    LAYOUT_LABEL_WIDTH = 48,
    LAYOUT_MIN_WIDTH = 960,
    LAYOUT_MIN_HEIGHT = 640,
    LAYOUT_TARGET_SIZE = 44,
    LAYOUT_ROW_HEIGHT = 44,
    LAYOUT_BASE_DPI = 96,
    LAYOUT_HELP_WIDTH = 112,
    LAYOUT_POPUP_WIDTH = 320,
    LAYOUT_POPUP_HEIGHT = 544
};

typedef enum {
    CONTROL_STEPS,
    CONTROL_TITLE,
    CONTROL_HELP,
    CONTROL_STATUS,
    CONTROL_BACK,
    CONTROL_NEXT,
    CONTROL_CIV,
    CONTROL_NAME,
    CONTROL_TAGLINE,
    CONTROL_APPEARANCE,
    CONTROL_ARCHITECTURE,
    CONTROL_LANGUAGE,
    CONTROL_WONDER,
    CONTROL_CASTLE,
    CONTROL_MONK,
    CONTROL_SCOUT,
    CONTROL_APPEARANCE_CLOSE,
    CONTROL_TREE_FILTER,
    CONTROL_TREE_LIST,
    CONTROL_TREE_INFO,
    CONTROL_BONUS_FILTER,
    CONTROL_BONUS_CATEGORY,
    CONTROL_BONUS_LIST,
    CONTROL_BONUS_CLEAR,
    CONTROL_BONUS_INFO,
    CONTROL_TEAM_FILTER,
    CONTROL_TEAM_LIST,
    CONTROL_TEAM_INFO,
    CONTROL_UNIT_FILTER,
    CONTROL_UNIT_LIST,
    CONTROL_UNIT_NAME,
    CONTROL_UNIT_DESCRIPTION,
    CONTROL_UNIT_INFO,
    CONTROL_CASTLE_TECH_FILTER,
    CONTROL_CASTLE_TECH_LIST,
    CONTROL_CASTLE_TECH_NAME,
    CONTROL_CASTLE_TECH_INFO,
    CONTROL_IMPERIAL_TECH_FILTER,
    CONTROL_IMPERIAL_TECH_LIST,
    CONTROL_IMPERIAL_TECH_NAME,
    CONTROL_IMPERIAL_TECH_INFO,
    CONTROL_HERO_NAME,
    CONTROL_HERO_CHOOSE,
    CONTROL_HERO,
    CONTROL_SUMMARY,
    CONTROL_CHANGED,
    CONTROL_BUILD,
    CONTROL_BUILT,
    CONTROL_COUNT
} ControlId;

typedef enum {
    WIDGET_TEXT,
    WIDGET_LINE,
    WIDGET_PARAGRAPH,
    WIDGET_CHOICE,
    WIDGET_LIST,
    WIDGET_CHECKLIST,
    WIDGET_BUTTON,
    WIDGET_REPORT,
    WIDGET_CHECKBOX,
    WIDGET_MARK,
    WIDGET_KIND_COUNT
} WidgetKind;

typedef enum {
    PLACE_FRAME,
    PLACE_LEFT,
    PLACE_MIDDLE,
    PLACE_RIGHT,
    PLACE_POPUP
} Placement;

typedef struct {
    int left;
    int top;
    int width;
    int height;
} Box;

typedef struct {
    WizardStep step;
    WidgetKind kind;
    Placement place;
    int height;
    char label[LAYOUT_LABEL_WIDTH];
} ControlSpec;

ControlSpec layout_spec(ControlId control);
bool layout_shows(ControlId control, WizardStep step);
bool layout_is_focusable(ControlId control);
bool layout_has_label(ControlId control);
bool layout_names_itself(ControlId control);
bool layout_needs_hero(ControlId control);
bool layout_in_popup(ControlId control);
Box layout_box(ControlId control, WizardStep step, int width, int height);
Box layout_label_box(ControlId control, int width, int height);
bool layout_boxes_overlap(Box first, Box second);
const char *layout_help(ControlId control);
int layout_text_dpi(int dpi, int text_percent);

#endif
