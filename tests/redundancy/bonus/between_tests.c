#include "between_tests.h"

#include <string.h>

#include "../../../src/bonus/bonus_scale.h"
#include "../../../src/bonus/bonus_text.h"
#include "../../../src/core/check.h"
#include "../../../src/core/text.h"
#include "../../../src/persist/civ_file.h"

enum {
    A_BONUS = 17,
    ADD = 4,
    MULTIPLY = 5,
    SPAWN = 7,
    NONE = -1,
    INFANTRY = 6,
    SPEED = 5,
    ARMOR = 8,
    ATTACK = 9,
    PIERCE = 768,
    MILITIA = 74,
    BARRACKS = 12
};

static const double HALF_AGAIN = 1.5;
static const float CLOSE = 0.001F;

static JsonWriter writer;
static JsonDocument document;
static CivDesign design;
static CivDesign reloaded;

static bool read_back_half_again(void)
{
    civ_design_reset(&design);
    civ_design_reset(&reloaded);
    (void)CHECK(civ_design_toggle_bonus(&design, A_BONUS) &&
                civ_design_set_multiplier(&design, false, A_BONUS, civ_multiplier_of(HALF_AGAIN)));
    if (!civ_file_compose(&design, &writer)) {
        return false;
    }
    document.length = (int)writer.length;
    return text_copy(document.text, sizeof document.text, writer.data) && json_parse(&document) &&
           civ_file_interpret(&document, &reloaded) &&
           (reloaded.bonuses[0].multiplier.millionths == civ_multiplier_of(HALF_AGAIN).millionths);
}

static bool scaled_is(const char *text, const char *expected)
{
    static char scaled[BONUS_TEXT_CAPACITY];

    return CHECK(bonus_text_scaled(text, HALF_AGAIN, scaled, sizeof scaled)) && (strcmp(scaled, expected) == 0);
}

static bool first_is(DatChange change, float expected)
{
    float amount = bonus_scale(&change, HALF_AGAIN).changes[0].d;

    return (amount > expected - CLOSE) && (amount < expected + CLOSE);
}

static bool spawns(int count, int expected)
{
    DatChange spawn = { SPAWN, MILITIA, BARRACKS, count, 0.0F };

    return bonus_scale(&spawn, HALF_AGAIN).changes[0].c == expected;
}

static bool written_as(long value, unsigned places, const char *expected)
{
    char text[TEXT_NUMBER_CAPACITY] = "";

    return text_append_decimal(text, sizeof text, value, places) && (strcmp(text, expected) == 0);
}

static bool refused(long value, unsigned places)
{
    char text[TEXT_NUMBER_CAPACITY] = "";

    return !text_append_decimal(text, sizeof text, value, places) && (text[0] == '\0');
}

static bool read_whole(const char *text, long long expected)
{
    TextDecimal number = text_to_decimal(text, strlen(text));

    return number.valid && ((long long)number.value == expected);
}

static bool text_is_no_decimal(void)
{
    json_writer_start(&writer);
    json_begin(&writer, NULL, '{');
    json_write_string(&writer, "multiplier", "2");
    json_end(&writer, '}');
    document.length = (int)writer.length;
    return text_copy(document.text, sizeof document.text, writer.data) && json_parse(&document) &&
           !json_decimal(&document, json_member(&document, 0, "multiplier")).valid;
}

static bool nothing_refused(void)
{
    Multiplier none = { 0 };

    civ_design_reset(&design);
    (void)CHECK(civ_design_toggle_bonus(&design, A_BONUS));
    return !civ_design_set_multiplier(&design, false, A_BONUS, none);
}

static TestOutcome run_decimal_test(int index)
{
    switch (index) {
    case 8:
        return test_outcome_of("a decimal of nine places is written whole", written_as(5L, 9U, "0.000000005"));
    case 9:
        return test_outcome_of("a decimal of ten places is refused", refused(5L, 10U));
    case 10:
        return test_outcome_of("a negative whole number is written with its sign", written_as(-5L, 0U, "-5"));
    case 11:
        return test_outcome_of("a number as long as the room for it is read",
                               read_whole("000000000000000000000001", 1LL));
    case 12:
        return test_outcome_of("a number as large as ten to the eighteenth is read",
                               read_whole("1000000000000000000", 1000000000000000000LL));
    case 13:
        return test_outcome_of("a text in the place of a number is no decimal", text_is_no_decimal());
    case 14:
        return test_outcome_of("a count past ten is no multiplier",
                               civ_multiplier_of((double)(CIV_MULTIPLIER_LIMIT + 1)).millionths == 0);
    case 15:
        return test_outcome_of("a multiplier of nothing is refused", nothing_refused());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome between_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a bonus counted 1.5 times is read back from a civ file as 1.5", read_back_half_again());
    case 2:
        return test_outcome_of("a percent of a bonus counted 1.5 times is raised to 1.5",
                               scaled_is("Cavalry +20% hit points", "Cavalry +31.5% hit points"));
    case 3:
        return test_outcome_of("a percent without a sign before cheaper counted 1.5 times is a cut raised to 1.5",
                               scaled_is("Siege Workshop units 15% cheaper", "Siege Workshop units 21.6% cheaper"));
    case 4:
        return test_outcome_of("a change multiplied and counted 1.5 times is raised to 1.5",
                               first_is((DatChange){ MULTIPLY, NONE, INFANTRY, SPEED, 1.21F }, 1.331F));
    case 5:
        return test_outcome_of("an armor added and counted 1.5 times is rounded to the nearest",
                               first_is((DatChange){ ADD, NONE, INFANTRY, ARMOR, (float)(PIERCE + 5) },
                                        (float)(PIERCE + 8)));
    case 6:
        return test_outcome_of("an attack multiplied and counted 1.5 times is raised to 1.5",
                               first_is((DatChange){ MULTIPLY, NONE, INFANTRY, ATTACK, (float)(PIERCE + 121) },
                                        (float)(PIERCE + 133)));
    case 7:
        return test_outcome_of("a spawn counted 1.5 times makes the nearest whole number of units", spawns(5, 8));
    default:
        return run_decimal_test(index);
    }
}

TestOutcome bonus_tests_run(int index)
{
    if (index <= SCALED_TEXT_TEST_COUNT) {
        return scaled_text_tests_run(index);
    }
    if (index <= SCALED_TEXT_TEST_COUNT + BETWEEN_TEST_COUNT) {
        return between_tests_run(index - SCALED_TEXT_TEST_COUNT);
    }
    return number_tests_run(index - SCALED_TEXT_TEST_COUNT - BETWEEN_TEST_COUNT);
}
