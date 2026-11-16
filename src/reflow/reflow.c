#include "reflow.h"

#include "../core/check.h"

enum { MARGIN = 16, GAP = 8, LABEL_HEIGHT = 22, LABEL_GAP = 4, STEPS_HEIGHT = 176, NOTE_HEIGHT = 66 };

static const int KIND_HEIGHTS[WIDGET_KIND_COUNT] = { 132, 44, 132, 44, 220, 220, 44, 264 };

bool reflow_is_needed(int width, int height)
{
    if (!CHECK(width >= 0) || !CHECK(height >= 0)) {
        return true;
    }
    return (width < LAYOUT_MIN_WIDTH) || (height < LAYOUT_MIN_HEIGHT);
}

static int own_height(ControlId control)
{
    ControlSpec spec = layout_spec(control);

    if (!CHECK((int)control >= 0) || !CHECK(spec.kind < WIDGET_KIND_COUNT)) {
        return 0;
    }
    if (control == CONTROL_STEPS) {
        return STEPS_HEIGHT;
    }
    if ((control == CONTROL_HELP) || (control == CONTROL_STATUS)) {
        return NOTE_HEIGHT;
    }
    return (spec.height > 0) ? spec.height : KIND_HEIGHTS[spec.kind];
}

static int label_height(ControlId control)
{
    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT)) {
        return 0;
    }
    return layout_has_label(control) ? LABEL_HEIGHT + LABEL_GAP : 0;
}

static int slot_height(ControlId control)
{
    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT)) {
        return 0;
    }
    return label_height(control) + own_height(control) + GAP;
}

static bool is_foot(int control)
{
    return (control >= CONTROL_STATUS) && (control <= CONTROL_NEXT);
}

static int slots_of(WizardStep step, int before)
{
    int total = 0;
    int control;

    if (!CHECK(before >= 0) || !CHECK(before <= CONTROL_COUNT)) {
        return 0;
    }
    for (control = 0; (control < before) && (control < CONTROL_COUNT); ++control) {
        if (!is_foot(control) && (layout_spec((ControlId)control).step == step)) {
            total += slot_height((ControlId)control);
        }
    }
    return total;
}

static int slot_top(ControlId control, WizardStep step)
{
    int head = MARGIN + slots_of(STEP_COUNT, CONTROL_COUNT);

    if (!CHECK(control < CONTROL_COUNT) || !CHECK(step < STEP_COUNT)) {
        return head;
    }
    if (control < CONTROL_STATUS) {
        return MARGIN + slots_of(STEP_COUNT, (int)control);
    }
    if (control > CONTROL_NEXT) {
        return head + slots_of(layout_spec(control).step, (int)control);
    }
    return head + slots_of(step, CONTROL_COUNT) + ((control == CONTROL_STATUS) ? 0 : slot_height(CONTROL_STATUS));
}

int reflow_height(WizardStep step)
{
    if (!CHECK((int)step >= 0) || !CHECK(step < STEP_COUNT)) {
        return 0;
    }
    return slot_top(CONTROL_NEXT, step) + LAYOUT_TARGET_SIZE + MARGIN;
}

Box reflow_box(ControlId control, WizardStep step, int width)
{
    Box box = { MARGIN, 0, 0, 0 };
    int inner = width - (2 * MARGIN);
    int half = (inner - GAP) / 2;

    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT) || !CHECK(width >= REFLOW_MIN_WIDTH)) {
        return box;
    }
    box.top = slot_top(control, step) + label_height(control);
    box.height = own_height(control);
    box.width = ((control == CONTROL_BACK) || (control == CONTROL_NEXT)) ? half : inner;
    box.left += (control == CONTROL_NEXT) ? half + GAP : 0;
    return box;
}

Box reflow_label_box(ControlId control, WizardStep step, int width)
{
    Box box = { MARGIN, 0, 0, 0 };

    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT) || !CHECK(width >= REFLOW_MIN_WIDTH) ||
        !layout_has_label(control)) {
        return box;
    }
    box.top = slot_top(control, step);
    box.width = width - (2 * MARGIN);
    box.height = LABEL_HEIGHT;
    return box;
}

int reflow_scroll_limit(int content, int view)
{
    if (!CHECK(content >= 0) || !CHECK(view >= 0)) {
        return 0;
    }
    return (content > view) ? content - view : 0;
}

int reflow_scroll_within(int scroll, int content, int view)
{
    int limit = reflow_scroll_limit(content, view);

    if (!CHECK(limit >= 0) || !CHECK(content >= 0)) {
        return 0;
    }
    if (scroll > limit) {
        return limit;
    }
    return (scroll < 0) ? 0 : scroll;
}

int reflow_scroll_to_show(Box box, int scroll, int view)
{
    int top = box.top - LABEL_HEIGHT - LABEL_GAP - MARGIN;
    int bottom = box.top + box.height + MARGIN;
    int target = scroll;

    if (!CHECK(box.height >= 0) || !CHECK(view > 0)) {
        return scroll;
    }
    if (bottom > target + view) {
        target = bottom - view;
    }
    if (top < target) {
        target = top;
    }
    return (target < 0) ? 0 : target;
}
