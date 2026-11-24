#include "../test_outcome.h"

#include "../../src/core/check.h"
#include "civ_file_tests.h"
#include "core_tests.h"
#include "model_tests.h"
#include "../base/base_tests.h"
#include "../dat/dat_tests.h"
#include "../suites/suites.h"
#include "../mod/mod_tests.h"
#include "../pack/pack_tests.h"

enum { FILE_GROUP_SIZE = 6, MODEL_GROUP_SIZE = 4, PACK_GROUP_SIZE = 5, MOD_GROUP_SIZE = 7, UNIQUE_GROUP_SIZE = 8 };
enum { CIV_LIST_GROUP_SIZE = 10, BASE_GROUP_SIZE = 5, TREE_GROUP_SIZE = 7 };
enum {
    MODEL_START = FILE_GROUP_SIZE,
    PACK_START = MODEL_START + MODEL_GROUP_SIZE,
    MOD_START = PACK_START + PACK_GROUP_SIZE,
    UNIQUE_START = MOD_START + MOD_GROUP_SIZE,
    CIV_LIST_START = UNIQUE_START + UNIQUE_GROUP_SIZE,
    BASE_START = CIV_LIST_START + CIV_LIST_GROUP_SIZE,
    TREE_START = BASE_START + BASE_GROUP_SIZE,
    BUILD_START = TREE_START + TREE_GROUP_SIZE
};

static TestOutcome run_file_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= FILE_GROUP_SIZE + 1)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a failed check names its file, line and condition",
                               test_failed_check_names_its_condition());
    case 2:
        return test_outcome_of("the original's example civ keeps its name", test_original_example_civ_keeps_its_name());
    case 3:
        return test_outcome_of("the original's example civ keeps its bonuses",
                               test_original_example_civ_keeps_its_bonuses());
    case 4:
        return test_outcome_of("the original's example civ keeps its unique unit",
                               test_original_example_civ_keeps_its_unique_unit());
    case 5:
        return test_outcome_of("a saved civ loads back unchanged", test_saved_civ_loads_back_unchanged());
    case 6:
        return test_outcome_of("a bonus's multiplier is read back from a civ file",
                               test_a_bonus_multiplier_is_read_back_from_a_civ_file());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_model_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("typing a name is undone at once", test_typing_a_name_is_undone_at_once());
    case 2:
        return test_outcome_of("redo makes an undone change again", test_redo_makes_an_undone_change_again());
    case 3:
        return test_outcome_of("next walks every step in order", test_next_walks_every_step_in_order());
    case 4:
        return test_outcome_of("the summary names what is still needed", test_summary_names_what_is_still_needed());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_pack_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a dynamic Huffman stream inflates", test_a_dynamic_huffman_stream_inflates());
    case 2:
        return test_outcome_of("a fixed Huffman stream inflates", test_a_fixed_huffman_stream_inflates());
    case 3:
        return test_outcome_of("a stored stream inflates back", test_a_stored_stream_inflates_back());
    case 4:
        return test_outcome_of("packed data unpacks to the same bytes", test_packed_data_unpacks_to_the_same_bytes());
    case 5:
        return test_outcome_of("repeated data packs to a tenth of its size", test_repeated_data_packs_small());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_mod_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("an unchanged dat file is written back byte for byte",
                               test_an_unchanged_dat_file_is_written_back_byte_for_byte());
    case 2:
        return test_outcome_of("an added effect is written as the game holds it",
                               test_an_added_effect_is_written_as_the_game_holds_it());
    case 3:
        return test_outcome_of("an added tech is written as the game holds it",
                               test_an_added_tech_is_written_as_the_game_holds_it());
    case 4:
        return test_outcome_of("a bonus without techs is built from its changes",
                               test_a_bonus_without_techs_is_built_from_its_changes());
    case 5:
        return test_outcome_of("a bonus taken twice is built with its changes doubled",
                               test_a_bonus_taken_twice_is_built_with_its_changes_doubled());
    case 6:
        return test_outcome_of("the name is written under the replaced civ's name id",
                               test_the_name_is_written_under_the_replaced_civs_name_id());
    case 7:
        return test_outcome_of("a mod is not built from an unfinished civ",
                               test_a_mod_is_not_built_from_an_unfinished_civ());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_unique_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a unique tech is aimed at the civ's own unique unit",
                               test_a_unique_tech_is_aimed_at_the_civs_own_unique_unit());
    case 2:
        return test_outcome_of("a unique tech's name is written under the string id of its slot",
                               test_a_unique_techs_name_is_written_under_its_slots_string_id());
    case 3:
        return test_outcome_of("a unique unit's name is written under the string id of its slot",
                               test_a_unique_units_name_is_written_under_the_string_id_of_its_slot());
    case 4:
        return test_outcome_of("a building takes the look of its architecture's civ",
                               test_a_building_takes_the_look_of_its_architectures_civ());
    case 5:
        return test_outcome_of("a monk takes the look of its monk skin", test_a_monk_takes_the_look_of_its_monk_skin());
    case 6:
        return test_outcome_of("a hero trains at the castle", test_a_hero_trains_at_the_castle());
    case 7:
        return test_outcome_of("a unique tech's changes of one attribute by two amounts are one target",
                               test_a_unique_techs_changes_of_one_attribute_by_two_amounts_are_one_target());
    case 8:
        return test_outcome_of("a unique tech's changes of two armor classes are two targets",
                               test_a_unique_techs_changes_of_two_armor_classes_are_two_targets());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_civ_list_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a hero's name is written under the string id of its slot",
                               test_a_heros_name_is_written_under_the_string_id_of_its_slot());
    case 2:
        return test_outcome_of("the civ list names the civ's own unique unit",
                               test_the_civ_list_names_the_civs_own_unique_unit());
    case 3:
        return test_outcome_of("a unique unit missing from the civ list takes its own icon and texts",
                               test_a_unique_unit_missing_from_the_civ_list_takes_its_own_icon_and_texts());
    case 4:
        return test_outcome_of("a unit patch changes its civ's unit", test_a_unit_patch_changes_its_civs_unit());
    case 5:
        return test_outcome_of("a train line moves where its civ's unit is trained",
                               test_a_train_line_moves_where_its_civs_unit_is_trained());
    case 6:
        return test_outcome_of("a train line sets the button of its civ's unit",
                               test_a_train_line_sets_the_button_of_its_civs_unit());
    case 7:
        return test_outcome_of("a unit with nowhere to train leaves its bonus skipped",
                               test_a_unit_with_nowhere_to_train_leaves_its_bonus_skipped());
    case 8:
        return test_outcome_of("a cost line sets a cost of its civ's unit",
                               test_a_cost_line_sets_a_cost_of_its_civs_unit());
    case 9:
        return test_outcome_of("a cost line reaches the last field of the costs",
                               test_a_cost_line_reaches_the_last_field_of_the_costs());
    case 10:
        return test_outcome_of("a value the civ keeps stays the game's in the civ list",
                               test_a_value_the_civ_keeps_stays_the_games_in_the_civ_list());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_base_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("choosing another civ takes away a regional node",
                               test_choosing_another_civ_takes_away_a_regional_node());
    case 2:
        return test_outcome_of("a base civ has the nodes its civ has in the game",
                               test_a_base_civ_has_the_nodes_its_civ_has_in_the_game());
    case 3:
        return test_outcome_of("a base civ gives the civ exactly its bonuses",
                               test_a_base_civ_gives_the_civ_exactly_its_bonuses());
    case 4:
        return test_outcome_of("a base civ gives the civ its team bonus",
                               test_a_base_civ_gives_the_civ_its_team_bonus());
    case 5:
        return test_outcome_of("a base civ gives the civ its unique unit",
                               test_a_base_civ_gives_the_civ_its_unique_unit());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_tree_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a civ with an empty tree keeps the game's tech tree",
                               test_a_civ_with_an_empty_tree_keeps_the_games_tech_tree());
    case 2:
        return test_outcome_of("a civ keeps its villagers in its tech tree",
                               test_a_civ_keeps_its_villagers_in_its_tech_tree());
    case 3:
        return test_outcome_of("a building the civ has keeps the research it needs in its tech tree",
                               test_a_building_the_civ_has_keeps_the_research_it_needs_in_its_tech_tree());
    case 4:
        return test_outcome_of("only the tech of a unit the civ lacks is disabled",
                               test_only_the_tech_of_a_unit_the_civ_lacks_is_disabled());
    case 5:
        return test_outcome_of("only a tech the civ lacks is disabled", test_only_a_tech_the_civ_lacks_is_disabled());
    case 6:
        return test_outcome_of("a civ keeps its villagers in the dat file",
                               test_a_civ_keeps_its_villagers_in_the_dat_file());
    case 7:
        return test_outcome_of("a building the replaced civ lacked takes the status most civs give it",
                               test_a_building_the_replaced_civ_lacked_takes_its_usual_status());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_suite_test(int index)
{
    if (index <= BUILD_SUITE_SIZE) {
        return build_suite_run(index);
    }
    if (index <= BUILD_SUITE_SIZE + MOD_SUITE_SIZE) {
        return mod_suite_run(index - BUILD_SUITE_SIZE);
    }
    if (index <= BUILD_SUITE_SIZE + MOD_SUITE_SIZE + CORE_SUITE_SIZE) {
        return core_suite_run(index - BUILD_SUITE_SIZE - MOD_SUITE_SIZE);
    }
    return model_suite_run(index - BUILD_SUITE_SIZE - MOD_SUITE_SIZE - CORE_SUITE_SIZE);
}

TestOutcome functional_suite_run(int index)
{
    if (index <= MODEL_START) {
        return run_file_test(index);
    }
    if (index <= PACK_START) {
        return run_model_test(index - MODEL_START);
    }
    if (index <= MOD_START) {
        return run_pack_test(index - PACK_START);
    }
    if (index <= UNIQUE_START) {
        return run_mod_test(index - MOD_START);
    }
    if (index <= CIV_LIST_START) {
        return run_unique_test(index - UNIQUE_START);
    }
    if (index <= BASE_START) {
        return run_civ_list_test(index - CIV_LIST_START);
    }
    if (index <= TREE_START) {
        return run_base_test(index - BASE_START);
    }
    if (index <= BUILD_START) {
        return run_tree_test(index - TREE_START);
    }
    return run_suite_test(index - BUILD_START);
}
