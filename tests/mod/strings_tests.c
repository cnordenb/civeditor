#include "strings_tests.h"

#include <string.h>

#include "../../src/core/check.h"
#include "../../src/mod/mod_strings.h"
#include "../dat/recipe_tests.h"

enum { NAME_SID = 10271 };

static const char NO_VANILLA[] = "";

static CivDesign own;
static ModStrings strings;

static bool written_with(Scenario scenario, TextField field, const char *text)
{
    ModCiv civ = { &own, recipe_game_catalog(), SLOT, NAME_SID, false };

    if (!CHECK(text != NULL) || !CHECK(civ.catalog != NULL)) {
        return false;
    }
    own = *recipe_game_design(scenario);
    return civ_design_set_text(&own, field, text) && mod_strings_write(&strings, &civ, NO_VANILLA);
}

static bool strings_hold(Scenario scenario, TextField field, const char *text, const char *expected)
{
    if (!CHECK(expected != NULL) || !written_with(scenario, field, text)) {
        return false;
    }
    return strstr(strings.text, expected) != NULL;
}

bool test_a_line_break_in_a_text_is_written_as_an_escape(void)
{
    return strings_hold(FULL_CIV, TEXT_FIELD_TAGLINE, "North\nSouth", "\"North\\nSouth civilization");
}

bool test_a_carriage_return_before_a_line_break_is_left_out(void)
{
    return strings_hold(FULL_CIV, TEXT_FIELD_TAGLINE, "North\r\nSouth", "\"North\\nSouth civilization");
}

bool test_a_quote_in_a_text_is_escaped(void)
{
    return strings_hold(FULL_CIV, TEXT_FIELD_TAGLINE, "The \"best\"", "\"The \\\"best\\\" civilization");
}

bool test_a_quote_that_is_escaped_already_is_left_as_it_is(void)
{
    return strings_hold(FULL_CIV, TEXT_FIELD_TAGLINE, "The \\\"best", "\"The \\\"best civilization");
}

bool test_a_unique_techs_label_ends_its_description_without_a_full_stop(void)
{
    return strings_hold(FULL_CIV, TEXT_FIELD_TAGLINE, "North", "\"Research Bulwark (Walls are stronger)\"");
}

bool test_a_unique_tech_without_a_description_has_a_plain_label(void)
{
    return strings_hold(FULL_CIV, TEXT_FIELD_IMPERIAL_TECH_NAME, "Zeal", "\"Research Zeal\"");
}

bool test_a_units_tooltip_carries_its_description(void)
{
    return strings_hold(FULL_CIV, TEXT_FIELD_UNIT_DESCRIPTION, "It tests.",
                        "\"Create <b>Tester<b> (<cost>)\\nIt tests.\"");
}

bool test_the_first_tip_names_the_first_civ_bonus(void)
{
    return strings_hold(FULL_CIV, TEXT_FIELD_TAGLINE, "North",
                        "IDS_CIVTIPS_1_0 \"Civilization bonus: Infantry is stronger.\"");
}

bool test_the_first_tip_names_the_team_bonus_of_a_civ_without_civ_bonuses(void)
{
    return strings_hold(BARE_CIV, TEXT_FIELD_TAGLINE, "North", "IDS_CIVTIPS_1_0 \"Team bonus: Allies are faster.\"");
}

bool test_the_tip_of_a_unique_tech_says_where_it_is_researched(void)
{
    return strings_hold(FULL_CIV, TEXT_FIELD_TAGLINE, "North",
                        "IDS_CIVTIPS_1_2 \"Research Bulwark in your Castle: Walls are stronger.\"");
}

bool test_the_second_tip_names_the_unique_unit(void)
{
    return strings_hold(FULL_CIV, TEXT_FIELD_TAGLINE, "North", "IDS_CIVTIPS_1_1 \"Your unique unit is the Tester.\"");
}

bool test_a_line_break_in_a_tip_is_written_as_a_space(void)
{
    return strings_hold(FULL_CIV, TEXT_FIELD_UNIT_NAME, "Test\ner",
                        "IDS_CIVTIPS_1_1 \"Your unique unit is the Test er.\"");
}
