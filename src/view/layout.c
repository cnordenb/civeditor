#include "layout.h"

#include "../core/check.h"

enum {
    MARGIN = 16,
    GAP = 8,
    LABEL_HEIGHT = 22,
    LABEL_GAP = 4,
    STEP_LIST_WIDTH = 220,
    TITLE_HEIGHT = 36,
    HELP_TOP = 56,
    HELP_HEIGHT = 28,
    CONTENT_TOP = 96,
    PLACE_SLOTS = 3,
    BUTTON_WIDTH = 140,
    PARAGRAPH_HEIGHT = 132,
    STRETCH = 0
};

static const ControlSpec SPECS[CONTROL_COUNT] = {
    { STEP_COUNT, WIDGET_LIST, PLACE_FRAME, STRETCH, "Steps" },
    { STEP_COUNT, WIDGET_TEXT, PLACE_FRAME, TITLE_HEIGHT, "" },
    { STEP_COUNT, WIDGET_TEXT, PLACE_FRAME, LAYOUT_TARGET_SIZE, "" },
    { STEP_COUNT, WIDGET_TEXT, PLACE_FRAME, LAYOUT_TARGET_SIZE, "" },
    { STEP_COUNT, WIDGET_BUTTON, PLACE_FRAME, LAYOUT_TARGET_SIZE, "< Back" },
    { STEP_COUNT, WIDGET_BUTTON, PLACE_FRAME, LAYOUT_TARGET_SIZE, "Next >" },
    { STEP_IDENTITY, WIDGET_CHOICE, PLACE_LEFT, LAYOUT_TARGET_SIZE, "Base civ" },
    { STEP_IDENTITY, WIDGET_LINE, PLACE_LEFT, LAYOUT_TARGET_SIZE, "Name" },
    { STEP_IDENTITY, WIDGET_LINE, PLACE_LEFT, LAYOUT_TARGET_SIZE, "Tagline" },
    { STEP_IDENTITY, WIDGET_PARAGRAPH, PLACE_LEFT, STRETCH, "Description" },
    { STEP_IDENTITY, WIDGET_CHOICE, PLACE_MIDDLE, LAYOUT_TARGET_SIZE, "Architecture" },
    { STEP_IDENTITY, WIDGET_CHOICE, PLACE_MIDDLE, LAYOUT_TARGET_SIZE, "Language" },
    { STEP_IDENTITY, WIDGET_CHOICE, PLACE_MIDDLE, LAYOUT_TARGET_SIZE, "Wonder" },
    { STEP_IDENTITY, WIDGET_CHOICE, PLACE_RIGHT, LAYOUT_TARGET_SIZE, "Castle" },
    { STEP_IDENTITY, WIDGET_CHOICE, PLACE_RIGHT, LAYOUT_TARGET_SIZE, "Monk" },
    { STEP_IDENTITY, WIDGET_CHOICE, PLACE_RIGHT, LAYOUT_TARGET_SIZE, "Scout" },
    { STEP_TECH_TREE, WIDGET_LINE, PLACE_LEFT, LAYOUT_TARGET_SIZE, "Filter" },
    { STEP_TECH_TREE, WIDGET_CHECKLIST, PLACE_LEFT, STRETCH, "Units, buildings and techs" },
    { STEP_TECH_TREE, WIDGET_TEXT, PLACE_RIGHT, STRETCH, "In the tree" },
    { STEP_BONUSES, WIDGET_LINE, PLACE_LEFT, LAYOUT_TARGET_SIZE, "Filter" },
    { STEP_BONUSES, WIDGET_CHOICE, PLACE_LEFT, LAYOUT_TARGET_SIZE, "Category" },
    { STEP_BONUSES, WIDGET_CHECKLIST, PLACE_LEFT, STRETCH, "Bonuses" },
    { STEP_BONUSES, WIDGET_TEXT, PLACE_RIGHT, STRETCH, "Chosen bonuses" },
    { STEP_TEAM_BONUS, WIDGET_LIST, PLACE_LEFT, STRETCH, "Team bonuses" },
    { STEP_TEAM_BONUS, WIDGET_TEXT, PLACE_RIGHT, STRETCH, "The selected team bonus" },
    { STEP_UNIQUE_UNIT, WIDGET_LIST, PLACE_LEFT, STRETCH, "Units" },
    { STEP_UNIQUE_UNIT, WIDGET_LINE, PLACE_RIGHT, LAYOUT_TARGET_SIZE, "Name of the unique unit" },
    { STEP_UNIQUE_UNIT, WIDGET_PARAGRAPH, PLACE_RIGHT, PARAGRAPH_HEIGHT, "Its description" },
    { STEP_UNIQUE_UNIT, WIDGET_TEXT, PLACE_RIGHT, STRETCH, "Its base unit" },
    { STEP_CASTLE_TECH, WIDGET_LIST, PLACE_LEFT, STRETCH, "Castle techs" },
    { STEP_CASTLE_TECH, WIDGET_LINE, PLACE_RIGHT, LAYOUT_TARGET_SIZE, "Name of the castle tech" },
    { STEP_CASTLE_TECH, WIDGET_TEXT, PLACE_RIGHT, STRETCH, "What it does" },
    { STEP_IMPERIAL_TECH, WIDGET_LIST, PLACE_LEFT, STRETCH, "Imperial techs" },
    { STEP_IMPERIAL_TECH, WIDGET_LINE, PLACE_RIGHT, LAYOUT_TARGET_SIZE, "Name of the imperial tech" },
    { STEP_IMPERIAL_TECH, WIDGET_TEXT, PLACE_RIGHT, STRETCH, "What it does" },
    { STEP_HERO, WIDGET_LIST, PLACE_LEFT, STRETCH, "Heroes" },
    { STEP_HERO, WIDGET_LINE, PLACE_RIGHT, LAYOUT_TARGET_SIZE, "Name of the hero" },
    { STEP_REVIEW, WIDGET_REPORT, PLACE_LEFT, STRETCH, "The civ" },
    { STEP_REVIEW, WIDGET_CHOICE, PLACE_RIGHT, LAYOUT_TARGET_SIZE, "Civ it replaces in the game" },
    { STEP_REVIEW, WIDGET_BUTTON, PLACE_RIGHT, LAYOUT_TARGET_SIZE, "Save civ file..." },
    { STEP_REVIEW, WIDGET_BUTTON, PLACE_RIGHT, LAYOUT_TARGET_SIZE, "Open civ file..." },
    { STEP_REVIEW, WIDGET_BUTTON, PLACE_RIGHT, LAYOUT_TARGET_SIZE, "Build mod..." }
};

ControlSpec layout_spec(ControlId control)
{
    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT)) {
        return SPECS[CONTROL_TITLE];
    }
    return SPECS[control];
}

bool layout_shows(ControlId control, WizardStep step)
{
    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT) || (step >= STEP_COUNT)) {
        return false;
    }
    if (((control == CONTROL_NEXT) && (wizard_next(step) == step)) ||
        ((control == CONTROL_BACK) && (wizard_previous(step) == step))) {
        return false;
    }
    return (SPECS[control].step == STEP_COUNT) || (SPECS[control].step == step);
}

bool layout_is_focusable(ControlId control)
{
    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT)) {
        return false;
    }
    return SPECS[control].kind != WIDGET_TEXT;
}

bool layout_has_label(ControlId control)
{
    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT)) {
        return false;
    }
    return (SPECS[control].label[0] != '\0') && (SPECS[control].kind != WIDGET_BUTTON);
}

bool layout_boxes_overlap(Box first, Box second)
{
    if (!CHECK(first.width >= 0) || !CHECK(second.width >= 0)) {
        return false;
    }
    return (first.left < second.left + second.width) && (second.left < first.left + first.width) &&
           (first.top < second.top + second.height) && (second.top < first.top + first.height);
}

static Box box_of(int left, int top, int width, int height)
{
    Box box = { 0, 0, 0, 0 };

    if (!CHECK(width >= 0) || !CHECK(height >= 0)) {
        return box;
    }
    box.left = left;
    box.top = top;
    box.width = width;
    box.height = height;
    return box;
}

static Box frame_box(ControlId control, int width, int height)
{
    int content_left = MARGIN + STEP_LIST_WIDTH + MARGIN;
    int bottom_row = height - MARGIN - LAYOUT_TARGET_SIZE;
    int right_width = width - content_left - MARGIN;

    if (!CHECK(width >= LAYOUT_MIN_WIDTH) || !CHECK(height >= LAYOUT_MIN_HEIGHT)) {
        return box_of(0, 0, 0, 0);
    }
    switch ((int)control) {
    case CONTROL_STEPS:
        return box_of(MARGIN, MARGIN + LABEL_HEIGHT + LABEL_GAP, STEP_LIST_WIDTH,
                      height - (2 * MARGIN) - LABEL_HEIGHT - LABEL_GAP);
    case CONTROL_TITLE:
        return box_of(content_left, MARGIN, right_width, TITLE_HEIGHT);
    case CONTROL_HELP:
        return box_of(content_left, HELP_TOP, right_width, HELP_HEIGHT);
    case CONTROL_STATUS:
        return box_of(content_left, bottom_row, right_width - (2 * (BUTTON_WIDTH + GAP)), LAYOUT_TARGET_SIZE);
    case CONTROL_BACK:
        return box_of(width - MARGIN - (2 * BUTTON_WIDTH) - GAP, bottom_row, BUTTON_WIDTH, LAYOUT_TARGET_SIZE);
    case CONTROL_NEXT:
        return box_of(width - MARGIN - BUTTON_WIDTH, bottom_row, BUTTON_WIDTH, LAYOUT_TARGET_SIZE);
    default:
        return box_of(0, 0, 0, 0);
    }
}

static bool same_column(ControlId control, int other)
{
    if (!CHECK((int)control < CONTROL_COUNT) || !CHECK((other >= 0) && (other < CONTROL_COUNT))) {
        return false;
    }
    return (SPECS[other].step == SPECS[control].step) && (SPECS[other].place == SPECS[control].place);
}

static int step_columns(WizardStep step)
{
    bool used[PLACE_SLOTS] = { false, false, false };
    int other;

    if (!CHECK((int)step >= 0) || !CHECK(step < STEP_COUNT)) {
        return 1;
    }
    for (other = 0; other < CONTROL_COUNT; ++other) {
        if ((SPECS[other].step == step) && (SPECS[other].place != PLACE_FRAME)) {
            used[SPECS[other].place - PLACE_LEFT] = true;
        }
    }
    return (used[0] ? 1 : 0) + (used[1] ? 1 : 0) + (used[2] ? 1 : 0);
}

static int slot_height(int control, int stretch)
{
    int label;

    if (!CHECK((control >= 0) && (control < CONTROL_COUNT)) || !CHECK(stretch >= 0)) {
        return 0;
    }
    label = layout_has_label((ControlId)control) ? LABEL_HEIGHT + LABEL_GAP : 0;
    return label + ((SPECS[control].height == STRETCH) ? stretch : SPECS[control].height);
}

static Box column_of(ControlId control, int width, int height)
{
    int content_left = MARGIN + STEP_LIST_WIDTH + MARGIN;
    int content_width = width - content_left - MARGIN;
    int columns = step_columns(SPECS[control].step);
    int column_width;
    int index;

    if (!CHECK(width >= LAYOUT_MIN_WIDTH) || !CHECK(height >= LAYOUT_MIN_HEIGHT) || (columns < 1)) {
        return box_of(0, 0, 0, 0);
    }
    column_width = (content_width - ((columns - 1) * MARGIN)) / columns;
    index = (SPECS[control].place == PLACE_LEFT) ? 0 : ((SPECS[control].place == PLACE_MIDDLE) ? 1 : columns - 1);
    return box_of(content_left + (index * (column_width + MARGIN)), CONTENT_TOP, column_width,
                  height - MARGIN - LAYOUT_TARGET_SIZE - MARGIN - CONTENT_TOP);
}

static int stretch_height(ControlId control, Box column)
{
    int fixed = 0;
    int stretches = 0;
    int members = 0;
    int other;

    if (!CHECK((int)control < CONTROL_COUNT) || !CHECK(column.height > 0)) {
        return 0;
    }
    for (other = 0; other < CONTROL_COUNT; ++other) {
        if (same_column(control, other)) {
            ++members;
            fixed += slot_height(other, 0);
            stretches += (SPECS[other].height == STRETCH) ? 1 : 0;
        }
    }
    if (stretches == 0) {
        return 0;
    }
    return (column.height - fixed - ((members - 1) * GAP)) / stretches;
}

static int slot_top(ControlId control, Box column, int stretch)
{
    int top = column.top;
    int other;

    if (!CHECK((int)control < CONTROL_COUNT) || !CHECK(stretch >= 0)) {
        return top;
    }
    for (other = 0; (other < (int)control) && (other < CONTROL_COUNT); ++other) {
        if (same_column(control, other)) {
            top += slot_height(other, stretch) + GAP;
        }
    }
    return top;
}

Box layout_box(ControlId control, int width, int height)
{
    Box column;
    int stretch;
    int label;

    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT)) {
        return box_of(0, 0, 0, 0);
    }
    if (SPECS[control].place == PLACE_FRAME) {
        return frame_box(control, width, height);
    }
    column = column_of(control, width, height);
    stretch = stretch_height(control, column);
    label = layout_has_label(control) ? LABEL_HEIGHT + LABEL_GAP : 0;
    return box_of(column.left, slot_top(control, column, stretch) + label, column.width,
                  (SPECS[control].height == STRETCH) ? stretch : SPECS[control].height);
}

Box layout_label_box(ControlId control, int width, int height)
{
    Box column;

    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT) || !layout_has_label(control)) {
        return box_of(0, 0, 0, 0);
    }
    if (SPECS[control].place == PLACE_FRAME) {
        return box_of(MARGIN, MARGIN, STEP_LIST_WIDTH, LABEL_HEIGHT);
    }
    column = column_of(control, width, height);
    return box_of(column.left, slot_top(control, column, stretch_height(control, column)), column.width,
                  LABEL_HEIGHT);
}
