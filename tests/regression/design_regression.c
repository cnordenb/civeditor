#include "design_regression.h"

#include <string.h>

#include "../../src/bonus/bonus_text.h"
#include "../../src/core/check.h"
#include "../../src/persist/civ_file.h"
#include "../../src/view/layout.h"
#include "../../src/view/wizard.h"
#include "../base/base_tests.h"
#include "../functional/model_tests.h"

enum { JAPANESE_SLOT = 5, BARRACKS = 12, OTHER_CIV = 2, TWICE = 2 };
enum { A_BONUS = 4, A_TEAM_BONUS = 8, A_UNIT = 1 };

static const wchar_t EXAMPLE_PATH[] = L"tests/fixtures/example.civbuilder.json";
static const char COMMA_LIST[] = "Military units cost -10, 15, 20, 25% food in Dark, Feudal, Castle, Imperial";
static const char COMMA_LIST_TWICE[] = "Military units cost -19, 27.8, 36, 43.8% food "
                                       "in Dark, Feudal, Castle, Imperial";

static CivDesign loaded;
static CivDesign design;
static CivDesign blank;

static CivDesign *fresh(void)
{
    civ_design_reset(&design);
    civ_design_reset(&blank);
    return &design;
}

static bool is_done(WizardStep step)
{
    return wizard_step_is_done(&design, &blank, step);
}

bool test_original_example_civ_keeps_its_language(void)
{
    return civ_file_load(&loaded, EXAMPLE_PATH) && (loaded.choices[CHOICE_LANGUAGE] == JAPANESE_SLOT);
}

bool test_summary_labels_the_name(void)
{
    return model_tests_summary_holds("Name: Vikings\r\n");
}

bool test_the_last_step_shows_no_next_button(void)
{
    return !layout_shows(CONTROL_NEXT, STEP_FINISH);
}

bool test_a_new_civ_starts_with_the_tech_tree_of_its_civ(void)
{
    const CivDesign *based = base_tests_design();

    return base_tests_apply(OTHER_CIV) && (based->tree[TREE_BUILDINGS].count == 1) &&
           civ_design_tree_has(based, TREE_BUILDINGS, BARRACKS);
}

bool test_a_tree_that_swaps_a_node_of_the_base_civ_is_changed(void)
{
    (void)CHECK(civ_design_toggle_tree(fresh(), TREE_UNITS, A_UNIT));
    (void)CHECK(civ_design_toggle_tree(&blank, TREE_UNITS, A_UNIT + 1));
    return is_done(STEP_NAME_AND_TREE);
}

bool test_bonuses_that_swap_one_of_the_base_civ_are_changed(void)
{
    (void)CHECK(civ_design_toggle_bonus(fresh(), A_BONUS));
    (void)CHECK(civ_design_toggle_bonus(&blank, A_BONUS + 1));
    return is_done(STEP_BONUSES);
}

bool test_a_team_bonus_other_than_the_base_civs_is_a_change(void)
{
    (void)CHECK(civ_design_toggle_team_bonus(fresh(), A_TEAM_BONUS));
    (void)CHECK(civ_design_toggle_team_bonus(&blank, A_TEAM_BONUS + 1));
    return is_done(STEP_BONUSES);
}

bool test_the_base_civs_own_team_bonus_is_no_change(void)
{
    (void)CHECK(civ_design_toggle_team_bonus(fresh(), A_TEAM_BONUS));
    (void)CHECK(civ_design_toggle_team_bonus(&blank, A_TEAM_BONUS));
    return !is_done(STEP_BONUSES);
}

bool test_a_list_joined_by_commas_is_scaled_as_one(void)
{
    static char scaled[BONUS_TEXT_CAPACITY];

    return CHECK(bonus_text_scaled(COMMA_LIST, TWICE, scaled, sizeof scaled)) &&
           (strcmp(scaled, COMMA_LIST_TWICE) == 0);
}
