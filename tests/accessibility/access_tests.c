#include "access_tests.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../src/present/details.h"
#include "../../src/view/layout.h"
#include "../../src/view/palette.h"
#include "../test_outcome.h"

enum { BOX_CAPACITY = 2 * CONTROL_COUNT };

static Box boxes[BOX_CAPACITY];

bool test_every_text_contrasts_four_and_a_half_to_one(void)
{
    ColorPair pair;
    int index;

    for (index = 0; index < PALETTE_PAIR_COUNT; ++index) {
        pair = palette_pair(index);
        if (palette_contrast_hundredths(palette_color(pair.text), palette_color(pair.background)) <
            PALETTE_AA_CONTRAST) {
            return false;
        }
    }
    return true;
}

enum { LABELLED_KIND_COUNT = 6 };

static const int LABELLED_KINDS[LABELLED_KIND_COUNT] = { WIDGET_LINE, WIDGET_PARAGRAPH, WIDGET_CHOICE,
                                                         WIDGET_LIST, WIDGET_CHECKLIST, WIDGET_REPORT };

static bool needs_label(WidgetKind kind)
{
    return check_is_among((int)kind, LABELLED_KINDS, LABELLED_KIND_COUNT);
}

bool test_every_input_has_a_visible_label(void)
{
    int control;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        if (needs_label(layout_spec((ControlId)control).kind) && !layout_has_label((ControlId)control)) {
            return false;
        }
    }
    return true;
}

bool test_every_target_is_44_by_44(void)
{
    Box box;
    int control;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        box = layout_box((ControlId)control, layout_spec((ControlId)control).step, LAYOUT_MIN_WIDTH, LAYOUT_MIN_HEIGHT);
        if (layout_is_focusable((ControlId)control) &&
            ((box.width < LAYOUT_TARGET_SIZE) || (box.height < LAYOUT_TARGET_SIZE))) {
            test_note(layout_spec((ControlId)control).label);
            return false;
        }
    }
    return LAYOUT_ROW_HEIGHT >= LAYOUT_TARGET_SIZE;
}

static bool fits(Box box)
{
    return (box.left >= 0) && (box.top >= 0) && (box.left + box.width <= LAYOUT_MIN_WIDTH) &&
           (box.top + box.height <= LAYOUT_MIN_HEIGHT);
}

static int collect_boxes(WizardStep step)
{
    int count = 0;
    int control;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        if (!layout_shows((ControlId)control, step)) {
            continue;
        }
        boxes[count] = layout_box((ControlId)control, step, LAYOUT_MIN_WIDTH, LAYOUT_MIN_HEIGHT);
        ++count;
        if (layout_has_label((ControlId)control)) {
            boxes[count] = layout_label_box((ControlId)control, LAYOUT_MIN_WIDTH, LAYOUT_MIN_HEIGHT);
            ++count;
        }
    }
    return count;
}

bool test_no_two_controls_of_a_step_overlap(void)
{
    int step;
    int count;
    int first;
    int second;

    for (step = 0; step < STEP_COUNT; ++step) {
        count = collect_boxes((WizardStep)step);
        for (first = 0; (first < count) && (first < BOX_CAPACITY); ++first) {
            if (!fits(boxes[first])) {
                return false;
            }
            for (second = first + 1; (second < count) && (second < BOX_CAPACITY); ++second) {
                if (layout_boxes_overlap(boxes[first], boxes[second])) {
                    test_note(wizard_page((WizardStep)step)->title);
                    return false;
                }
            }
        }
    }
    return true;
}

bool test_the_window_title_names_the_civ_and_the_step(void)
{
    static CivDesign titled;
    DetailText title;

    civ_design_reset(&titled);
    (void)civ_design_set_text(&titled, TEXT_FIELD_NAME, "Testonia");
    title = details_window_title(&titled, STEP_BONUSES);
    return text_equal(title.text, "Testonia - Step 2 of 4: Bonuses - civbuilder");
}

bool test_the_steps_and_their_buttons_are_on_every_step(void)
{
    WizardStep at;
    int step;

    for (step = 0; step < STEP_COUNT; ++step) {
        at = (WizardStep)step;
        if (!layout_shows(CONTROL_STEPS, at) || (layout_shows(CONTROL_BACK, at) != (wizard_move(at, -1) != at)) ||
            (layout_shows(CONTROL_NEXT, at) != (wizard_move(at, 1) != at))) {
            test_note(wizard_page(at)->title);
            return false;
        }
    }
    return true;
}

bool test_the_layout_grows_with_the_text_size_of_windows(void)
{
    return layout_text_dpi(LAYOUT_BASE_DPI, 400) == 216;
}
