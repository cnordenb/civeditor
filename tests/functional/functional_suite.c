#include "../test_outcome.h"

#include "../../src/core/check.h"
#include "civ_file_tests.h"
#include "core_tests.h"
#include "model_tests.h"
#include "../base/base_tests.h"
#include "../dat/dat_tests.h"
#include "../mod/mod_tests.h"
#include "../pack/pack_tests.h"

enum { FILE_GROUP_SIZE = 6, MODEL_GROUP_SIZE = 8, PACK_GROUP_SIZE = 5, MOD_GROUP_SIZE = 8, CIV_GROUP_SIZE = 12 };
enum {
    MODEL_START = FILE_GROUP_SIZE,
    PACK_START = MODEL_START + MODEL_GROUP_SIZE,
    MOD_START = PACK_START + PACK_GROUP_SIZE,
    CIV_START = MOD_START + MOD_GROUP_SIZE,
    BASE_START = CIV_START + CIV_GROUP_SIZE
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
        return test_outcome_of("the original's example civ keeps its language",
                               test_original_example_civ_keeps_its_language());
    case 6:
        return test_outcome_of("a saved civ loads back unchanged", test_saved_civ_loads_back_unchanged());
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
        return test_outcome_of("undo brings back the civ before a change",
                               test_undo_brings_back_the_civ_before_a_change());
    case 2:
        return test_outcome_of("typing a name is undone at once", test_typing_a_name_is_undone_at_once());
    case 3:
        return test_outcome_of("redo makes an undone change again", test_redo_makes_an_undone_change_again());
    case 4:
        return test_outcome_of("a 25th bonus is refused", test_a_25th_bonus_is_refused());
    case 5:
        return test_outcome_of("next walks every step in order", test_next_walks_every_step_in_order());
    case 6:
        return test_outcome_of("the summary names what is still needed", test_summary_names_what_is_still_needed());
    case 7:
        return test_outcome_of("the summary labels the civ's name", test_summary_labels_the_name());
    case 8:
        return test_outcome_of("the last step shows no Next button", test_the_last_step_shows_no_next_button());
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
        return test_outcome_of("the name is written under the replaced civ's name id",
                               test_the_name_is_written_under_the_replaced_civs_name_id());
    case 6:
        return test_outcome_of("the civ descriptions are written in id order",
                               test_the_civ_descriptions_are_written_in_id_order());
    case 7:
        return test_outcome_of("the mod's folder name is made from the civ's name",
                               test_the_mod_folder_name_is_made_from_the_civ_name());
    case 8:
        return test_outcome_of("a mod is not built from an unfinished civ",
                               test_a_mod_is_not_built_from_an_unfinished_civ());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_civ_test(int index)
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
        return test_outcome_of("a hero's name is written under the string id of its slot",
                               test_a_heros_name_is_written_under_the_string_id_of_its_slot());
    case 8:
        return test_outcome_of("a skipped hero's name is not written", test_a_skipped_heros_name_is_not_written());
    case 9:
        return test_outcome_of("the civ list names the civ's own unique unit",
                               test_the_civ_list_names_the_civs_own_unique_unit());
    case 10:
        return test_outcome_of("a unique unit missing from the civ list takes its own icon and texts",
                               test_a_unique_unit_missing_from_the_civ_list_takes_its_own_icon_and_texts());
    case 11:
        return test_outcome_of("a unit patch changes its civ's unit", test_a_unit_patch_changes_its_civs_unit());
    case 12:
        return test_outcome_of("the civ list points at the civ's own description",
                               test_the_civ_list_points_at_the_civs_own_description());
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
        return test_outcome_of("the Generic base civ has exactly the universal nodes",
                               test_the_generic_base_civ_has_exactly_the_universal_nodes());
    case 2:
        return test_outcome_of("choosing Generic takes away a regional node",
                               test_choosing_generic_takes_away_a_regional_node());
    case 3:
        return test_outcome_of("a base civ has the nodes its civ has in the game",
                               test_a_base_civ_has_the_nodes_its_civ_has_in_the_game());
    case 4:
        return test_outcome_of("a base civ gives the civ exactly its bonuses",
                               test_a_base_civ_gives_the_civ_exactly_its_bonuses());
    case 5:
        return test_outcome_of("a base civ gives the civ its team bonus",
                               test_a_base_civ_gives_the_civ_its_team_bonus());
    case 6:
        return test_outcome_of("a base civ gives the civ its unique unit",
                               test_a_base_civ_gives_the_civ_its_unique_unit());
    case 7:
        return test_outcome_of("a building the civ lacks is not available in its tech tree",
                               test_a_building_the_civ_lacks_is_not_available_in_its_tech_tree());
    case 8:
        return test_outcome_of("a building the civ has is available in its tech tree",
                               test_a_building_the_civ_has_is_available_in_its_tech_tree());
    case 9:
        return test_outcome_of("a civ with an empty tree keeps the game's tech tree",
                               test_a_civ_with_an_empty_tree_keeps_the_games_tech_tree());
    case 10:
        return test_outcome_of("a civ keeps its villagers in its tech tree",
                               test_a_civ_keeps_its_villagers_in_its_tech_tree());
    case 11:
        return test_outcome_of("a building the civ has keeps the research it needs in its tech tree",
                               test_a_building_the_civ_has_keeps_the_research_it_needs_in_its_tech_tree());
    case 12:
        return test_outcome_of("only the tech of a unit the civ lacks is disabled",
                               test_only_the_tech_of_a_unit_the_civ_lacks_is_disabled());
    case 13:
        return test_outcome_of("only a tech the civ lacks is disabled", test_only_a_tech_the_civ_lacks_is_disabled());
    case 14:
        return test_outcome_of("a civ keeps its villagers in the dat file",
                               test_a_civ_keeps_its_villagers_in_the_dat_file());
    default:
        return test_outcome_missing();
    }
}

TestOutcome functional_suite_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    if (index <= MODEL_START) {
        return run_file_test(index);
    }
    if (index <= PACK_START) {
        return run_model_test(index - MODEL_START);
    }
    if (index <= MOD_START) {
        return run_pack_test(index - PACK_START);
    }
    if (index <= CIV_START) {
        return run_mod_test(index - MOD_START);
    }
    if (index <= BASE_START) {
        return run_civ_test(index - CIV_START);
    }
    return run_base_test(index - BASE_START);
}
