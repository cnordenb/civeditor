#include "built_mod_tests.h"

#include <string.h>

#include "../../../src/core/check.h"
#include "../../../src/forge/forge.h"
#include "../../../src/mod/mod_strings.h"
#include "../../dat/recipe_tests.h"
#include "../../dat/small_dat.h"

enum {
    BRITONS_SLOT = 1,
    BRITONS_NAME_ID = 10271,
    FRANKS_NAME_ID = 10272,
    JURCHENS_NAME_ID = 10321,
    NAME_ID_PAST_THE_DESCRIPTIONS = 10399,
    CIVS_BUILT_BEFORE = 2,
    BONUS_TECH_ROOM = DAT_REQUIRED_TECH_SLOTS - FREE_SLOT,
    BONUS_LETTERS = 300
};

static const char VANILLA[] = "120201 \"Khitans\"\r\n120200 \"Jurchens\"\r\n120150 \"Britons\"\r\n";

static CivDesign design;
static Catalog catalog;
static ModStrings strings;
static ModCiv civs[CIVS_BUILT_BEFORE];

static void name_civ(ModCiv *civ, long name_sid)
{
    civ->design = &design;
    civ->catalog = &catalog;
    civ->slot = BRITONS_SLOT;
    civ->name_sid = name_sid;
    civ->hero_skipped = false;
}

static const char *written_for(long name_sid)
{
    civ_design_reset(&design);
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_NAME, "Testonia"));
    name_civ(&civs[0], name_sid);
    name_civ(&civs[1], FRANKS_NAME_ID);
    (void)CHECK(mod_strings_write(&strings, civs, 1, VANILLA));
    return strings.text;
}

bool test_a_job_with_no_civ_is_not_finished(void)
{
    static ForgeJob job;
    static ForgeResult result;

    job.design_count = 0;
    return !forge_civs_are_finished(&job, &result);
}

bool test_a_build_of_an_unfinished_civ_fails(void)
{
    static ForgeJob job;
    static ForgeResult result;

    civ_design_reset(&design);
    job.designs[0] = &design;
    job.design_count = 1;
    return !forge_build(&job, &result);
}

bool test_the_description_of_a_civ_takes_the_place_of_the_games_once(void)
{
    const char *text = written_for(JURCHENS_NAME_ID);
    const char *first = strstr(text, "\n120200 \"");

    return (strstr(text, "120200 \"Jurchens\"") == NULL) && (first != NULL) &&
           (strstr(first + 1, "\n120200 \"") == NULL);
}

bool test_the_other_descriptions_of_the_game_are_kept(void)
{
    return strstr(written_for(BRITONS_NAME_ID), "\n120200 \"Jurchens\"") != NULL;
}

bool test_a_civ_past_the_descriptions_of_the_game_gets_its_own(void)
{
    return strstr(written_for(NAME_ID_PAST_THE_DESCRIPTIONS), "\n120278 \"") != NULL;
}

bool test_a_civ_left_from_a_bigger_build_gets_no_description(void)
{
    return strstr(written_for(BRITONS_NAME_ID), "\n120151 \"") == NULL;
}

static bool commit_requirement(const RecipeInput *input)
{
    static const BonusRequirement LINE = { REQUIRE_BONUS, BONUS_TECH, LACKED_TECH };

    recipe_plan_reset();
    return recipe_plan_requirement(input, &LINE) && recipe_commit_plan(input, &small.edit);
}

bool test_a_tech_takes_no_more_requirements_than_it_has_room_for(void)
{
    static DatBytes game;
    const Catalog *recipe_catalog = recipe_game_catalog();
    RecipeInput input = { recipe_game_design(REQUIRED_CIV), recipe_catalog, &game, &small.written_map, SLOT };
    bool taken = recipe_game_make(&game) && CHECK(dat_edit_start(&small.edit, &small.written_map));
    int civ;

    for (civ = 0; taken && (civ < BONUS_TECH_ROOM); ++civ) {
        taken = commit_requirement(&input);
    }
    return taken && !commit_requirement(&input);
}

static bool strings_of_long_bonuses(const char *description)
{
    int bonus;

    civ_design_reset(&design);
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_NAME, "Testonia"));
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_DESCRIPTION, description));
    catalog.bonus_count = CIV_BONUS_CAPACITY;
    for (bonus = 0; bonus < CIV_BONUS_CAPACITY; ++bonus) {
        catalog.bonuses[bonus].id = bonus;
        (void)memset(catalog.bonuses[bonus].description, 'b', BONUS_LETTERS);
        catalog.bonuses[bonus].description[BONUS_LETTERS] = '\0';
        (void)CHECK(civ_design_toggle_bonus(&design, bonus));
    }
    name_civ(&civs[0], BRITONS_NAME_ID);
    return mod_strings_write(&strings, civs, 1, VANILLA);
}

bool test_the_strings_of_a_civ_fail_once_its_description_passes_its_room(void)
{
    static char description[CIV_DESCRIPTION_CAPACITY];

    (void)memset(description, 'd', sizeof description - 1U);
    description[sizeof description - 1U] = '\0';
    return strings_of_long_bonuses("Archers") && !strings_of_long_bonuses(description);
}

bool test_the_description_of_the_first_civ_described_is_written_once(void)
{
    const char *first = strstr(written_for(BRITONS_NAME_ID), "\n120150 \"");

    return (first != NULL) && (strstr(first + 1, "\n120150 \"") == NULL);
}
