#include "civ_tests.h"

#include "../../src/base/base_civ.h"
#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../src/present/binding.h"
#include "../../src/roster/civ_roster.h"
#include "../../src/view/wizard.h"

enum { BRITONS = 1, FRANKS = 2, LONGBOWMAN = 0, AXEMAN = 1, JOAN = 7, YEOMEN = 2, BEARDED_AXE = 3, WARWOLF = 2 };
enum { AXEMAN_ROW = 1, JOAN_ROW = 2, BEARDED_AXE_ROW = 2, FIRST_RESOURCE = 0, FIRST_BONUS = 100, SECOND_BONUS = 101 };

static Catalog game;
static CivDesign design;
static CivDesign other;
static CivDesign as_game;
static CivRoster roster;

static void add_civ(int index, int slot, const char *name, int unit, int castle_tech)
{
    BaseCiv base = { 0, { { 0 }, 0 }, { -1, 0, 0, WARWOLF, 0, -1, -1 }, "Foot Archer", "An archer civilization" };

    game.appearance[index].kind = APPEARANCE_BASE;
    game.appearance[index].id = slot;
    (void)text_copy(game.appearance[index].name, sizeof game.appearance[index].name, name);
    base.id = slot;
    base.values[BASE_UNIT] = unit;
    base.values[BASE_CASTLE_TECH] = castle_tech;
    game.bases[index] = base;
}

static void add_entry(int index, UnitRole role, int id, const char *name)
{
    game.units[index].role = role;
    game.units[index].id = id;
    (void)text_copy(game.units[index].name, sizeof game.units[index].name, name);
}

static void add_tech(int index, TechTier tier, int id, const char *name)
{
    game.techs[index].tier = tier;
    game.techs[index].id = id;
    (void)text_copy(game.techs[index].name, sizeof game.techs[index].name, name);
}

static void give_bonuses(BaseCiv *base, int count)
{
    int bonus;

    for (bonus = 0; bonus < count; ++bonus) {
        base->bonuses.values[bonus] = FIRST_BONUS + bonus;
    }
    base->bonuses.count = count;
}

static const Catalog *made_game(void)
{
    add_civ(0, BRITONS, "Britons", LONGBOWMAN, YEOMEN);
    add_civ(1, FRANKS, "Franks", AXEMAN, BEARDED_AXE);
    give_bonuses(&game.bases[0], 1);
    give_bonuses(&game.bases[1], 2);
    game.appearance_count = 2;
    game.base_count = 2;
    add_entry(0, UNIT_ROLE_VANILLA, LONGBOWMAN, "Longbowman");
    add_entry(1, UNIT_ROLE_VANILLA, AXEMAN, "Throwing Axeman");
    add_entry(2, UNIT_ROLE_HERO, JOAN, "Joan of Arc");
    game.unit_count = 3;
    add_tech(0, TECH_TIER_CASTLE, YEOMEN, "Yeomen");
    add_tech(1, TECH_TIER_IMPERIAL, WARWOLF, "Warwolf");
    add_tech(2, TECH_TIER_CASTLE, BEARDED_AXE, "Bearded Axe");
    game.tech_count = 3;
    game.tree_count = 0;
    return &game;
}

static CivDesign *civ_of_the_game(int slot)
{
    civ_design_reset(&design);
    return base_civ_apply(&design, made_game(), slot) ? &design : &other;
}

static const char *text_of(TextField field)
{
    return civ_design_text(&design, field);
}

static bool picked(ControlId list, int row)
{
    ListItem item = { CONTROL_TITLE, 0 };
    Command command;

    item.control = list;
    item.item = row;
    command = list_item_command(&game, &design, item);
    return command_apply(&design, &command);
}

static const char *named_after_pick(ControlId list, int row, TextField field)
{
    (void)civ_of_the_game(BRITONS);
    return picked(list, row) ? civ_design_text(&design, field) : "";
}

bool test_a_civ_of_the_game_gives_the_design_its_name(void)
{
    (void)civ_of_the_game(BRITONS);
    return text_equal(text_of(TEXT_FIELD_NAME), "Britons");
}

bool test_a_civ_of_the_game_names_its_unique_unit_as_the_game_does(void)
{
    (void)civ_of_the_game(BRITONS);
    return text_equal(text_of(TEXT_FIELD_UNIT_NAME), "Longbowman");
}

bool test_a_civ_of_the_game_names_its_castle_tech_as_the_game_does(void)
{
    (void)civ_of_the_game(FRANKS);
    return text_equal(text_of(TEXT_FIELD_CASTLE_TECH_NAME), "Bearded Axe");
}

bool test_a_civ_of_the_game_names_its_imperial_tech_as_the_game_does(void)
{
    (void)civ_of_the_game(BRITONS);
    return text_equal(text_of(TEXT_FIELD_IMPERIAL_TECH_NAME), "Warwolf");
}

bool test_a_civ_of_the_game_gives_the_design_its_tagline(void)
{
    (void)civ_of_the_game(BRITONS);
    return text_equal(text_of(TEXT_FIELD_TAGLINE), "Foot Archer");
}

bool test_a_civ_of_the_game_gives_the_design_its_description(void)
{
    (void)civ_of_the_game(BRITONS);
    return text_equal(text_of(TEXT_FIELD_DESCRIPTION), "An archer civilization");
}

bool test_a_civ_of_the_game_takes_away_the_hero_of_the_civ_before(void)
{
    civ_design_reset(&design);
    (void)CHECK(civ_design_pick_hero(&design, JOAN));
    return base_civ_apply(&design, made_game(), BRITONS) && (design.hero_unit == CIV_NO_CHOICE);
}

bool test_picking_a_unique_unit_names_it_as_the_game_does(void)
{
    return text_equal(named_after_pick(CONTROL_UNIT_LIST, AXEMAN_ROW, TEXT_FIELD_UNIT_NAME), "Throwing Axeman");
}

bool test_picking_a_castle_tech_names_it_as_the_game_does(void)
{
    return text_equal(named_after_pick(CONTROL_CASTLE_TECH_LIST, BEARDED_AXE_ROW, TEXT_FIELD_CASTLE_TECH_NAME),
                      "Bearded Axe");
}

bool test_picking_a_hero_names_it_as_the_game_does(void)
{
    return text_equal(named_after_pick(CONTROL_HERO_CHOOSE, JOAN_ROW, TEXT_FIELD_HERO_NAME), "Joan of Arc");
}

bool test_unticking_hero_takes_away_the_heros_name(void)
{
    Command command;

    (void)CHECK(text_equal(named_after_pick(CONTROL_HERO_CHOOSE, JOAN_ROW, TEXT_FIELD_HERO_NAME), "Joan of Arc"));
    command = hero_box_command(&game, false);
    return command_apply(&design, &command) && text_equal(civ_design_text(&design, TEXT_FIELD_HERO_NAME), "");
}

bool test_a_hero_whose_name_is_emptied_lacks_a_name(void)
{
    (void)CHECK(text_equal(named_after_pick(CONTROL_HERO_CHOOSE, JOAN_ROW, TEXT_FIELD_HERO_NAME), "Joan of Arc"));
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_HERO_NAME, ""));
    return civ_design_hero_lacks_name(&design);
}

bool test_a_civ_of_the_game_shows_no_step_done(void)
{
    int done = 0;
    int step;

    as_game = *civ_of_the_game(BRITONS);
    for (step = 0; step < STEP_COUNT; ++step) {
        done += wizard_step_is_done(&design, &as_game, (WizardStep)step) ? 1 : 0;
    }
    return done == 0;
}

bool test_finish_is_done_once_a_whole_civ_differs_from_the_games(void)
{
    as_game = *civ_of_the_game(BRITONS);
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_NAME, "Testonia"));
    return wizard_step_is_done(&design, &as_game, STEP_FINISH);
}

static const CivDesign *changed_civ(int slot, const char *name)
{
    as_game = *civ_of_the_game(slot);
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_NAME, name));
    roster_start(&roster);
    return &design;
}

bool test_a_changed_civ_is_counted(void)
{
    (void)roster_keep(&roster, changed_civ(BRITONS, "Testonia"), &as_game);
    return roster_count(&roster) == 1;
}

bool test_a_civ_as_the_game_has_it_is_left_out_of_the_mod(void)
{
    (void)changed_civ(BRITONS, "Britons");
    return !roster_keep(&roster, &design, &as_game) && (roster_count(&roster) == 0);
}

bool test_a_civ_changed_again_holds_its_newer_edits_in_the_mod(void)
{
    (void)roster_keep(&roster, changed_civ(BRITONS, "Testonia"), &as_game);
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_NAME, "Newer"));
    return !roster_keep(&roster, &design, &as_game) &&
           text_equal(civ_design_text(roster_civ(&roster, 0), TEXT_FIELD_NAME), "Newer");
}

bool test_a_civ_made_as_the_game_has_it_again_is_taken_out_of_the_mod(void)
{
    (void)roster_keep(&roster, changed_civ(BRITONS, "Testonia"), &as_game);
    return roster_keep(&roster, &as_game, &as_game) && (roster_count(&roster) == 0);
}

bool test_the_draft_of_a_civ_is_kept_while_another_is_edited(void)
{
    (void)roster_keep(&roster, changed_civ(BRITONS, "Testonia"), &as_game);
    other = *civ_of_the_game(FRANKS);
    (void)roster_keep(&roster, &other, &other);
    return roster_draft(&roster, BRITONS, &other) && text_equal(civ_design_text(&other, TEXT_FIELD_NAME), "Testonia");
}

bool test_a_changed_civ_is_marked_as_changed(void)
{
    return roster_keep(&roster, changed_civ(BRITONS, "Testonia"), &as_game) && roster_is_changed(&roster, BRITONS);
}

bool test_a_civ_as_the_game_has_it_is_not_marked_as_changed(void)
{
    (void)changed_civ(BRITONS, "Britons");
    (void)roster_keep(&roster, &design, &as_game);
    return !roster_is_changed(&roster, BRITONS) && !roster_is_changed(&roster, ROSTER_SLOT_LIMIT);
}

static void keep_changed_in_first_slot(void)
{
    (void)changed_civ(BRITONS, "Testonia");
    (void)CHECK(civ_design_set_choice(&design, CHOICE_REPLACE, 0));
    (void)roster_keep(&roster, &design, &as_game);
}

bool test_a_changed_civ_in_the_first_slot_is_counted(void)
{
    keep_changed_in_first_slot();
    return roster_count(&roster) == 1;
}

bool test_a_changed_civ_in_the_first_slot_is_given_to_the_mod(void)
{
    keep_changed_in_first_slot();
    return roster_civ(&roster, 0) != NULL;
}

bool test_a_changed_civ_in_the_first_slot_is_marked_as_changed(void)
{
    keep_changed_in_first_slot();
    return roster_is_changed(&roster, 0);
}

bool test_a_roster_started_anew_forgets_a_changed_civ_in_the_first_slot(void)
{
    keep_changed_in_first_slot();
    roster_start(&roster);
    return roster_count(&roster) == 0;
}

bool test_the_civs_of_the_mod_are_given_in_the_order_of_their_slots(void)
{
    (void)roster_keep(&roster, changed_civ(FRANKS, "Testonia"), &as_game);
    other = *civ_of_the_game(BRITONS);
    (void)CHECK(civ_design_set_text(&other, TEXT_FIELD_NAME, "Second"));
    (void)roster_keep(&roster, &other, &design);
    return (roster_civ(&roster, 0) != NULL) && (roster_civ(&roster, 0)->choices[CHOICE_REPLACE] == BRITONS);
}

bool test_a_roster_started_anew_holds_no_draft(void)
{
    (void)roster_keep(&roster, changed_civ(BRITONS, "Testonia"), &as_game);
    roster_start(&roster);
    return !roster_draft(&roster, BRITONS, &other);
}

bool test_a_civ_past_the_last_slot_is_left_out_of_the_mod(void)
{
    (void)changed_civ(BRITONS, "Testonia");
    (void)CHECK(civ_design_set_choice(&design, CHOICE_REPLACE, ROSTER_SLOT_LIMIT));
    return !roster_keep(&roster, &design, &as_game) && (roster_count(&roster) == 0);
}

bool test_a_civ_of_another_first_look_differs_from_its_civ_of_the_game(void)
{
    (void)changed_civ(BRITONS, "Britons");
    (void)CHECK(civ_design_set_choice(&design, CHOICE_ARCHITECTURE, design.choices[CHOICE_ARCHITECTURE] + 1));
    return !civ_design_same(&design, &as_game);
}

bool test_a_civ_of_another_first_cost_differs_from_its_civ_of_the_game(void)
{
    (void)changed_civ(BRITONS, "Britons");
    design.techs[TECH_TIER_CASTLE].values[FIRST_RESOURCE].set = true;
    return !civ_design_same(&design, &as_game);
}

bool test_a_civ_given_a_bonus_of_the_civ_before_differs_from_its_civ_of_the_game(void)
{
    (void)civ_of_the_game(FRANKS);
    as_game = *civ_of_the_game(BRITONS);
    (void)CHECK(civ_design_toggle_bonus(&design, FIRST_BONUS));
    (void)CHECK(civ_design_toggle_bonus(&design, SECOND_BONUS));
    return !civ_design_same(&design, &as_game);
}
