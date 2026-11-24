#include "number_edit_tests.h"

#include <math.h>
#include <string.h>

#include "../../../src/core/check.h"
#include "../../../src/core/text.h"
#include "../../../src/mod/mod_text.h"
#include "../../../src/numbers/number_edit.h"
#include "../../../src/present/summary.h"

enum {
    HALF_CUT = 7,
    ARCHERS = 8,
    CAVALRY = 9,
    ABSENT = 99,
    LINE_WIDTH = 12,
    NARROW_LINE = 10,
    LIST_LINE = 6,
    TINY_LINE = 3,
    WIDE_LINE = 1000,
    COST_PIECE = 4,
    CENTERS_PIECE = 2,
    LIST_PIECE = 2,
    CUT_NUMBER_PIECE = 6,
    CUT_NUMBER_WIDTH = 3,
    BARE_NUMBER_PIECE = 4,
    BARE_NUMBER_WIDTH = 2,
    CUT_PIECES = 10,
    WRAPPED_LINES = 3,
    MANY_WORDS = 100,
    TWO_AND_A_HALF = 250,
    SHORT_LINE = 4,
    FIELD_CENTER = 200,
    AREA_WIDTH = 400,
    NARROW_AREA = 200,
    TINY_AREA = 80,
    END_WIDTH = 40,
    TRACK_WIDTH = 160,
    CARD_GAP = 5,
    CENTERED_LEFT = 70,
    RIGHTMOST_LEFT = 140,
    NEAR_LEFT = 10,
    NEAR_RIGHT = 390,
    SHRUNK_TRACK = 100,
    TRACK_LEFT = 120,
    GROUP = 8
};

static const double CLOSE = 0.000001;
static const char LONGER_ROW[] = "aaa a/+1/b";

static Catalog catalog;
static CivDesign design;
static NumberLayout layout;
static BonusShown shown;

static void add_bonus(int index, int id, const char *description)
{
    catalog.bonuses[index].id = id;
    (void)CHECK(text_copy(catalog.bonuses[index].description, sizeof catalog.bonuses[index].description,
                          description));
}

static NumberPlace chosen(int id, double times)
{
    NumberPlace place = { &catalog, &design, { NUMBER_PANEL_BONUSES, 0, 0 } };

    add_bonus(0, HALF_CUT, "Town Centers cost -50% wood");
    add_bonus(1, ARCHERS, "Foot archers +1 range");
    add_bonus(2, CAVALRY, "Cavalry +20% hit points");
    catalog.bonus_count = 3;
    civ_design_reset(&design);
    (void)CHECK(civ_design_toggle_bonus(&design, id) &&
                civ_design_set_multiplier(&design, false, id, civ_multiplier_of(times)));
    return place;
}

static bool typed_counts(const char *typed, int millionths)
{
    Command command = number_typed_command(chosen(HALF_CUT, 1.0), typed);

    return command_apply(&design, &command) && (civ_design_multiplier(&design, HALF_CUT).millionths == millionths);
}

static bool most_taken(void)
{
    Command command = number_typed_command(chosen(HALF_CUT, 1.0), "99.9");

    return command_apply(&design, &command) &&
           (civ_design_multiplier(&design, HALF_CUT).millionths > (CIV_MULTIPLIER_LIMIT - 1) * CIV_MILLIONTHS);
}

static bool least_counts_once(void)
{
    Command command = number_typed_command(chosen(HALF_CUT, 2.0), "50");

    return command_apply(&design, &command) && (civ_design_multiplier(&design, HALF_CUT).millionths == CIV_MILLIONTHS);
}

static bool refuses(const char *typed)
{
    return number_typed_command(chosen(HALF_CUT, 1.0), typed).kind == COMMAND_KIND_COUNT;
}

static bool reads(int id, NumberText form, const char *expected)
{
    char text[NUMBER_TEXT_CAPACITY];
    NumberPlace place = chosen(id, 1.5);

    return number_text(place, number_times(place), form, text, sizeof text) && (strcmp(text, expected) == 0);
}

static bool past_the_last_has_no_value(void)
{
    char text[NUMBER_TEXT_CAPACITY];
    NumberPlace place = chosen(CAVALRY, 1.0);

    place.focus.number = 1;
    return !number_text(place, 1.0, NUMBER_WITH_UNIT, text, sizeof text);
}

static bool range_named(int id, const char *expected)
{
    char text[NUMBER_TEXT_CAPACITY];

    return number_range_text(chosen(id, 1.0), "Slide from ", text, sizeof text) && (strcmp(text, expected) == 0);
}

static bool slid_counts(void)
{
    Command command = number_slid_command(chosen(CAVALRY, 1.0), 250);

    return command_apply(&design, &command) && (civ_design_multiplier(&design, CAVALRY).millionths == 2500000);
}

static bool row_past_the_last(void)
{
    NumberPlace place = chosen(CAVALRY, 1.0);

    place.focus.pick = design.bonus_count;
    return !number_row(place, &shown);
}

static bool absent_row_has_no_number(void)
{
    return number_row(chosen(ABSENT, 1.0), &shown) && (shown.count == 0);
}

static bool row_as_it_counts(void)
{
    return number_row(chosen(HALF_CUT, 2.0), &shown) && (strcmp(shown.text, "Town Centers cost -75% wood") == 0);
}

static NumberPlace team_chosen(double times)
{
    NumberPlace place = chosen(HALF_CUT, 1.0);

    place.focus.panel = NUMBER_PANEL_TEAM;
    catalog.team_bonuses[0].id = HALF_CUT;
    (void)CHECK(text_copy(catalog.team_bonuses[0].description, sizeof catalog.team_bonuses[0].description,
                          "Knights +2 LOS"));
    catalog.team_bonus_count = 1;
    (void)CHECK(civ_design_toggle_team_bonus(&design, HALF_CUT) &&
                civ_design_set_multiplier(&design, true, HALF_CUT, civ_multiplier_of(times)));
    return place;
}

static bool team_row_as_it_counts(void)
{
    return number_row(team_chosen(2.0), &shown) && (strcmp(shown.text, "Knights +4 LOS") == 0);
}

static bool team_typed_counts(void)
{
    Command command = number_typed_command(team_chosen(1.0), "4");

    return command_apply(&design, &command) && (design.team_bonuses[0].multiplier.millionths == 2 * CIV_MILLIONTHS);
}

static bool no_team_row(void)
{
    NumberPlace place = team_chosen(1.0);

    return civ_design_toggle_team_bonus(&design, HALF_CUT) &&
           (number_rows(place.catalog, place.design, NUMBER_PANEL_TEAM) == 0);
}

static bool team_described_as_it_counts(void)
{
    static char text[MOD_TEXT_CAPACITY];
    NumberPlace place = team_chosen(2.0);
    ModCiv civ = { place.design, place.catalog, 1, 0L, false };

    return mod_description(&civ, text, sizeof text) && (strstr(text, "Knights +4 LOS") != NULL);
}

static bool team_summed_as_it_counts(void)
{
    static SummaryText summary;
    NumberPlace place = team_chosen(2.0);

    return summary_write(&summary, place.design, place.catalog) &&
           (strstr(summary.text, "Team bonus: Knights +4 LOS") != NULL);
}

static NumberLayout *cut(const char *text, int width)
{
    int piece;

    (void)CHECK(bonus_text_show(text, 1.0, &shown));
    number_layout_cut(&layout, &shown);
    for (piece = 0; (piece < layout.count) && (piece < NUMBER_PIECE_CAPACITY); ++piece) {
        layout.widths[piece] = (int)(layout.pieces[piece].end - layout.pieces[piece].start);
    }
    number_layout_place(&layout, width);
    return &layout;
}

static NumberLayout *cut_many_words(void)
{
    static char text[BONUS_TEXT_CAPACITY];
    int word;

    text[0] = '\0';
    for (word = 0; word < MANY_WORDS; ++word) {
        (void)CHECK(text_append(text, sizeof text, "a "));
    }
    return cut(text, WIDE_LINE);
}

static bool wraps(void)
{
    NumberLayout *placed = cut("Town Centers cost -50% wood", LINE_WIDTH);

    return (placed->count > COST_PIECE) && (placed->line[COST_PIECE] == 1) && (placed->x[COST_PIECE] == 0);
}

static int line_of(const char *text, int width, int piece)
{
    return cut(text, width)->line[piece];
}

static NumberLayout *cut_after_longer(const char *text, int width)
{
    (void)cut(LONGER_ROW, WIDE_LINE);
    return cut(text, width);
}

static bool number_piece_is_its_digits(const char *text, int index, size_t width)
{
    NumberPiece piece = cut(text, WIDE_LINE)->pieces[index];

    return (piece.number == 0) && (piece.end - piece.start == width);
}

static int piece_after_longer(void)
{
    (void)cut("Town Centers cost -50% wood", WIDE_LINE);
    return number_layout_piece(cut("Town Centers cost ", WIDE_LINE), 0);
}

static NumberCard card_at(int center, int area_width)
{
    NumberCardSizes sizes = { END_WIDTH, END_WIDTH, TRACK_WIDTH, CARD_GAP };

    return number_card_place(center, area_width, sizes);
}

static TestOutcome run_edit_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a value typed into a cost cut by half sets the count that shows it",
                               typed_counts("75", 2 * CIV_MILLIONTHS));
    case 2:
        return test_outcome_of("a value typed with its sign and percent is taken",
                               typed_counts("-75%", 2 * CIV_MILLIONTHS));
    case 3:
        return test_outcome_of("the most value of a number is taken", most_taken());
    case 4:
        return test_outcome_of("the value of a number is shown with its unit, as in its bonus",
                               reads(CAVALRY, NUMBER_WITH_UNIT, "+31.5%"));
    case 5:
        return test_outcome_of("the range of a number names its least and its most value",
                               range_named(HALF_CUT, "Slide from -50% to -99.9%"));
    case 6:
        return test_outcome_of("the slider stands at the count of the bonus in hundredths",
                               number_slider_position(chosen(CAVALRY, 1.5)) == 150);
    case 7:
        return test_outcome_of("the slider sets the count it stands at", slid_counts());
    case 8:
        return test_outcome_of("the field of a number holds its sign and digits without the percent",
                               reads(CAVALRY, NUMBER_DIGITS, "+31.5"));
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_row_test(int index)
{
    switch (index) {
    case 9:
        return test_outcome_of("the field of an amount holds its sign and digits",
                               reads(ARCHERS, NUMBER_DIGITS, "+1.5"));
    case 10:
        return test_outcome_of("a word too wide for its line goes on at the start of the next line", wraps());
    case 11:
        return test_outcome_of("the number of a row is a piece of its sign and digits alone",
                               number_piece_is_its_digits("Town Centers cost -50% wood", CUT_NUMBER_PIECE,
                                                          CUT_NUMBER_WIDTH));
    case 12:
        return test_outcome_of("the piece of a number is found in its row",
                               number_layout_piece(cut("+1/+2 food", WIDE_LINE), 1) == LIST_PIECE);
    case 13:
        return test_outcome_of("a row past the last chosen bonus is not shown", row_past_the_last());
    case 14:
        return test_outcome_of("the least value of a number sets its count back to 1", least_counts_once());
    case 15:
        return test_outcome_of("a number past the last of a row has no value", past_the_last_has_no_value());
    case 16:
        return test_outcome_of("a chosen bonus not in the catalog has no number to set", absent_row_has_no_number());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_card_test(int index)
{
    switch (index) {
    case 17:
        return test_outcome_of("a number past the last of a row has no piece in it",
                               number_layout_piece(cut("+1/+2 food", WIDE_LINE), 2) == NUMBER_NONE);
    case 18:
        return test_outcome_of("the slider's card is centered under its field",
                               card_at(FIELD_CENTER, AREA_WIDTH).left == CENTERED_LEFT);
    case 19:
        return test_outcome_of("the slider's card under a field near the left edge stays inside it",
                               card_at(NEAR_LEFT, AREA_WIDTH).left == 0);
    case 20:
        return test_outcome_of("the slider's card under a field near the right edge stays inside it",
                               card_at(NEAR_RIGHT, AREA_WIDTH).left == RIGHTMOST_LEFT);
    case 21:
        return test_outcome_of("a value with letters after it is refused", refuses("75abc"));
    case 22:
        return test_outcome_of("a value past the most of a number is refused", refuses("100"));
    case 23:
        return test_outcome_of("a place of the slider stands for its count in hundredths",
                               fabs(number_slider_times(TWO_AND_A_HALF) - 2.5) < CLOSE);
    case 24:
        return test_outcome_of("the slider shrinks to the room of a narrow window",
                               card_at(FIELD_CENTER, NARROW_AREA).track_width == SHRUNK_TRACK);
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_layout_test(int index)
{
    switch (index) {
    case 25:
        return test_outcome_of("a row is cut into its words, spaces, numbers and units",
                               cut("Town Centers cost -50% wood", WIDE_LINE)->count == CUT_PIECES);
    case 26:
        return test_outcome_of("a row of more pieces than it has room for is cut at the room",
                               cut_many_words()->count == NUMBER_PIECE_CAPACITY);
    case 27:
        return test_outcome_of("a word of several pieces goes on to the next line whole",
                               line_of("aaaa +1/+2", LIST_LINE, LIST_PIECE) == 1);
    case 28:
        return test_outcome_of("a word that would pass the end of its line goes on to the next",
                               line_of("Town Centers", NARROW_LINE, CENTERS_PIECE) == 1);
    case 29:
        return test_outcome_of("a first word wider than its line stays on the first line",
                               line_of("Centers", TINY_LINE, 0) == 0);
    case 30:
        return test_outcome_of("a row wrapped twice takes three lines",
                               cut("Town Centers cost -50% wood", LINE_WIDTH)->lines == WRAPPED_LINES);
    case 31:
        return test_outcome_of("the slider stands after the least value on its card",
                               card_at(FIELD_CENTER, AREA_WIDTH).track_left == TRACK_LEFT);
    case 32:
        return test_outcome_of("a word wider than its line is not broken between its pieces",
                               line_of("+1/+2", SHORT_LINE, 1) == 0);
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_after_test(int index)
{
    switch (index) {
    case 33:
        return test_outcome_of("the last word of a row that just fits its line stays on it",
                               cut_after_longer("+2 b", SHORT_LINE)->line[2] == 0);
    case 34:
        return test_outcome_of("a row that ends in a space takes only its own lines",
                               cut_after_longer("+2 b ", SHORT_LINE)->lines == 1);
    case 35:
        return test_outcome_of("a row of Chosen bonuses shows its bonus as it counts", row_as_it_counts());
    case 36:
        return test_outcome_of("the range of an amount names its least and its most value",
                               range_named(ARCHERS, "Slide from +1 to +10"));
    case 37:
        return test_outcome_of("a number without a sign is a piece of its digits alone",
                               number_piece_is_its_digits("Shepherds work 25% faster", BARE_NUMBER_PIECE,
                                                          BARE_NUMBER_WIDTH));
    case 38:
        return test_outcome_of("the piece of a number at the start of its row is its first",
                               number_layout_piece(cut("+1/+2 food", WIDE_LINE), 0) == 0);
    case 39:
        return test_outcome_of("a number of a row laid out before has no piece in a shorter row",
                               piece_after_longer() == NUMBER_NONE);
    case 40:
        return test_outcome_of("the slider of a card wider than its window has no width",
                               card_at(FIELD_CENTER, TINY_AREA).track_width == 0);
    case 41:
        return test_outcome_of("the row of the selected team bonus shows it as it counts", team_row_as_it_counts());
    case 42:
        return test_outcome_of("a value typed into the number of the team bonus sets how many times it counts",
                               team_typed_counts());
    case 43:
        return test_outcome_of("with no team bonus selected its panel has no row", no_team_row());
    case 44:
        return test_outcome_of("the mod describes the team bonus as it counts", team_described_as_it_counts());
    case 45:
        return test_outcome_of("the summary shows the team bonus as it counts", team_summed_as_it_counts());
    default:
        return test_outcome_missing();
    }
}

TestOutcome number_edit_tests_run(int index)
{
    if (index <= GROUP) {
        return run_edit_test(index);
    }
    if (index <= 2 * GROUP) {
        return run_row_test(index);
    }
    if (index <= 3 * GROUP) {
        return run_card_test(index);
    }
    return (index <= 4 * GROUP) ? run_layout_test(index) : run_after_test(index);
}
