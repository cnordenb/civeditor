#include "access_tests.h"

#include "../../src/core/text.h"
#include "../../src/data/tsv.h"
#include "../../src/present/details.h"
#include "../../src/view/layout.h"
#include "../../src/view/palette.h"
#include "../test_outcome.h"

enum { BOX_CAPACITY = 2 * CONTROL_COUNT, CAPITALS_MOST = 5, PLAIN_COUNT = 2, CATALOG_FILE_COUNT = 5, PATH_WIDTH = 24 };

static const wchar_t CATALOG_FILES[CATALOG_FILE_COUNT][PATH_WIDTH] = {
    L"data/bonuses.txt", L"data/team_bonuses.txt", L"data/techs.txt", L"data/units.txt", L"data/tree.txt"
};
static const char PLAIN_CAPITALS[PLAIN_COUNT][CAPITALS_MOST + 1] = { "FREE", "OFF" };

static Box boxes[BOX_CAPACITY];
static TsvFile catalog_file;
static TsvRecord catalog_line;

bool test_every_text_contrasts_seven_to_one(void)
{
    ColorPair pair;
    int index;

    for (index = 0; index < PALETTE_PAIR_COUNT; ++index) {
        pair = palette_pair(index);
        if (palette_contrast_hundredths(palette_color(pair.text), palette_color(pair.background)) <
            PALETTE_AAA_CONTRAST) {
            return false;
        }
    }
    return true;
}

static bool needs_label(WidgetKind kind)
{
    return (kind == WIDGET_LINE) || (kind == WIDGET_PARAGRAPH) || (kind == WIDGET_CHOICE) || (kind == WIDGET_LIST) ||
           (kind == WIDGET_CHECKLIST) || (kind == WIDGET_REPORT);
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
        box = layout_box((ControlId)control, LAYOUT_MIN_WIDTH, LAYOUT_MIN_HEIGHT);
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
        boxes[count] = layout_box((ControlId)control, LAYOUT_MIN_WIDTH, LAYOUT_MIN_HEIGHT);
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
                    test_note(wizard_title((WizardStep)step));
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
    title = details_window_title(&titled, STEP_TECH_TREE);
    return text_equal(title.text, "Testonia - Step 2 of 9: Tech tree - civbuilder");
}

bool test_the_steps_and_their_buttons_are_on_every_step(void)
{
    WizardStep at;
    int step;

    for (step = 0; step < STEP_COUNT; ++step) {
        at = (WizardStep)step;
        if (!layout_shows(CONTROL_STEPS, at) || (layout_shows(CONTROL_BACK, at) != (wizard_move(at, -1) != at)) ||
            (layout_shows(CONTROL_NEXT, at) != (wizard_move(at, 1) != at))) {
            test_note(wizard_title(at));
            return false;
        }
    }
    return true;
}

bool test_the_layout_grows_with_the_text_size_of_windows(void)
{
    return layout_text_dpi(LAYOUT_BASE_DPI, 400) == 216;
}

static bool is_letter_between(char letter, char first, char last)
{
    return (letter >= first) && (letter <= last);
}

static bool is_explained(const char *word)
{
    GlossaryEntry entry;
    int index;

    for (index = 0; index < LAYOUT_GLOSSARY_COUNT; ++index) {
        entry = layout_glossary(index);
        if (text_equal(word, entry.word) && (entry.meaning[0] != '\0')) {
            return true;
        }
    }
    for (index = 0; index < PLAIN_COUNT; ++index) {
        if (text_equal(word, PLAIN_CAPITALS[index])) {
            return true;
        }
    }
    return false;
}

static bool capitals_are_explained(const char *text, size_t at)
{
    char word[CAPITALS_MOST + 2] = "";
    size_t length;

    if ((at > 0U) && (is_letter_between(text[at - 1U], 'A', 'Z') || is_letter_between(text[at - 1U], 'a', 'z'))) {
        return true;
    }
    for (length = 0U; (length <= CAPITALS_MOST) && is_letter_between(text[at + length], 'A', 'Z'); ++length) {
        word[length] = text[at + length];
    }
    word[length] = '\0';
    if ((length < 2U) || (length > CAPITALS_MOST) || is_letter_between(text[at + length], 'a', 'z')) {
        return true;
    }
    test_note(word);
    return is_explained(word);
}

bool test_the_glossary_explains_every_short_form(void)
{
    size_t at;
    int file;
    int row;

    for (file = 0; file < CATALOG_FILE_COUNT; ++file) {
        if (!tsv_open(&catalog_file, CATALOG_FILES[file])) {
            return false;
        }
        for (row = 0; (row < TSV_FILE_CAPACITY) && tsv_next(&catalog_file, &catalog_line); ++row) {
            for (at = 0U; (at < TSV_LINE_CAPACITY) && (catalog_line.text[at] != '\0'); ++at) {
                if (is_letter_between(catalog_line.text[at], 'A', 'Z') &&
                    !capitals_are_explained(catalog_line.text, at)) {
                    return false;
                }
            }
        }
    }
    return true;
}