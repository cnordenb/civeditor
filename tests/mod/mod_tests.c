#include "mod_tests.h"

#include <string.h>

#include "../../src/civlist/civ_list.h"
#include "../../src/core/check.h"
#include "../../src/forge/forge.h"
#include "../../src/mod/mod_strings.h"
#include "../../src/techtree/tech_tree.h"
#include "../../src/unique/unique_aim.h"

enum { BRITONS_SLOT = 1, BRITONS_NAME_ID = 10271 };

static const char VANILLA[] = "120201 \"Khitans\"\r\n120200 \"Jurchens\"\r\n120150 \"Britons\"\r\n";
static const char NAME_LINE[] = "10271 \"Testonia\"\n";

static CivDesign design;
static Catalog catalog;
static ModStrings strings;
static ForgeResult result;

static ModCiv named_civ(void)
{
    ModCiv civ;

    civ_design_reset(&design);
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_NAME, "Testonia"));
    civ.design = &design;
    civ.catalog = &catalog;
    civ.slot = BRITONS_SLOT;
    civ.name_sid = BRITONS_NAME_ID;
    civ.hero_skipped = false;
    return civ;
}

bool test_the_name_is_written_under_the_replaced_civs_name_id(void)
{
    ModCiv civ = named_civ();

    if (!CHECK(mod_strings_write(&strings, &civ, 1, VANILLA))) {
        return false;
    }
    return strncmp(strings.text, NAME_LINE, strlen(NAME_LINE)) == 0;
}

bool test_the_civ_descriptions_are_written_in_id_order(void)
{
    ModCiv civ = named_civ();
    const char *jurchens;
    const char *khitans;

    if (!CHECK(mod_strings_write(&strings, &civ, 1, VANILLA))) {
        return false;
    }
    jurchens = strstr(strings.text, "\n120200 ");
    khitans = strstr(strings.text, "\n120201 ");
    return (jurchens != NULL) && (khitans != NULL) && (jurchens < khitans);
}

bool test_the_mod_folder_name_is_made_from_the_mod_name(void)
{
    char prefix[FORGE_PREFIX_CAPACITY];

    if (!CHECK(forge_prefix("\xC3\x86sir Kings 2!", prefix, sizeof prefix))) {
        return false;
    }
    return strcmp(prefix, "_sir_kings_2_") == 0;
}

bool test_a_mod_is_not_built_from_an_unfinished_civ(void)
{
    ForgeJob job;

    civ_design_reset(&design);
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_NAME, "Testonia"));
    job.designs[0] = &design;
    job.design_count = 1;
    job.mod_name = "Testonia";
    job.catalog = &catalog;
    job.game_folder = L".";
    job.mods_folder = L".";
    (void)forge_build(&job, &result);
    return strstr(result.problem, "not finished") != NULL;
}

enum {
    THROWING_AXEMAN = 281,
    ELITE_THROWING_AXEMAN = 531,
    LONGBOWMAN = 8,
    ELITE_LONGBOWMAN = 530,
    ADD_ATTRIBUTE = 4,
    NO_CLASS = -1,
    RANGE = 12
};

static const float ONE_MORE = 1.0F;

bool test_a_unique_tech_is_aimed_at_the_civs_own_unique_unit(void)
{
    UniqueSwap swap = { { THROWING_AXEMAN, ELITE_THROWING_AXEMAN }, { LONGBOWMAN, ELITE_LONGBOWMAN } };
    DatChange change = { ADD_ATTRIBUTE, THROWING_AXEMAN, NO_CLASS, RANGE, 0.0F };

    change.d = ONE_MORE;
    return (unique_aim(&swap, &change) == UNIQUE_SWAP) && (change.a == LONGBOWMAN);
}

enum { HIT_POINTS = 0, ARMOR = 8 };

static const float FIVE_MORE_PIERCE_ARMOR = (3.0F * 256.0F) + 5.0F;
static const float FIVE_MORE_MELEE_ARMOR = (4.0F * 256.0F) + 5.0F;
static const float TEN_MORE = 10.0F;
static const float THREE_HUNDRED_MORE = 300.0F;

static bool one_target(int attribute, float first, float second)
{
    DatChange left = { ADD_ATTRIBUTE, LONGBOWMAN, NO_CLASS, HIT_POINTS, 0.0F };
    DatChange right = { ADD_ATTRIBUTE, LONGBOWMAN, NO_CLASS, HIT_POINTS, 0.0F };

    left.c = attribute;
    right.c = attribute;
    left.d = first;
    right.d = second;
    return unique_same_target(&left, &right);
}

bool test_a_unique_techs_changes_of_one_attribute_by_two_amounts_are_one_target(void)
{
    return one_target(HIT_POINTS, TEN_MORE, THREE_HUNDRED_MORE);
}

bool test_a_unique_techs_changes_of_two_armor_classes_are_two_targets(void)
{
    return !one_target(ARMOR, FIVE_MORE_PIERCE_ARMOR, FIVE_MORE_MELEE_ARMOR);
}

enum { CASTLE_TECH = 11, CASTLE_TECH_SID = 44008 };

static const char CASTLE_TECH_LINE[] = "\n44008 \"Testing Axes\"\n";

static bool written_under(CivString kind, int sid, const char *line)
{
    ModCiv civ = { &design, &catalog, BRITONS_SLOT, BRITONS_NAME_ID, false };
    bool written;

    catalog.appearance[0].kind = APPEARANCE_CIV;
    catalog.appearance[0].id = BRITONS_SLOT;
    catalog.appearance[0].string_ids[CIV_STRING_IMPERIAL_TECH] = CATALOG_MISSING;
    catalog.appearance[0].string_ids[kind] = sid;
    catalog.appearance_count = 1;
    written = CHECK(mod_strings_write(&strings, &civ, 1, VANILLA)) && (strstr(strings.text, line) != NULL);
    catalog.appearance_count = 0;
    catalog.tech_count = 0;
    return written;
}

bool test_a_unique_techs_name_is_written_under_its_slots_string_id(void)
{
    (void)named_civ();
    catalog.techs[0].tier = TECH_TIER_CASTLE;
    catalog.techs[0].id = CASTLE_TECH;
    catalog.tech_count = 1;
    (void)CHECK(civ_design_pick_tech(&design, TECH_TIER_CASTLE, CASTLE_TECH));
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_CASTLE_TECH_NAME, "Testing Axes"));
    return written_under(CIV_STRING_CASTLE_TECH, CASTLE_TECH_SID, CASTLE_TECH_LINE);
}

enum { UNIT_SID = 69802 };

static const char UNIT_LINE[] = "\n69802 \"Testbowman\"\n";

bool test_a_unique_units_name_is_written_under_the_string_id_of_its_slot(void)
{
    (void)named_civ();
    (void)CHECK(civ_design_pick_unit(&design, UNIT_ORIGIN_VANILLA, 0));
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_UNIT_NAME, "Testbowman"));
    return written_under(CIV_STRING_UNIT, UNIT_SID, UNIT_LINE);
}

enum { LIU_BEI = 1966, HERO_SID = 44500 };

static const char HERO_LINE[] = "\n44500 \"Testhero\"\n";

static bool write_hero_strings(bool hero_skipped)
{
    ModCiv civ = named_civ();
    bool written;

    civ.hero_skipped = hero_skipped;
    catalog.appearance[0].kind = APPEARANCE_CIV;
    catalog.appearance[0].id = BRITONS_SLOT;
    catalog.appearance[0].string_ids[CIV_STRING_HERO] = HERO_SID;
    catalog.appearance_count = 1;
    (void)CHECK(civ_design_pick_hero(&design, LIU_BEI));
    (void)CHECK(civ_design_set_text(&design, TEXT_FIELD_HERO_NAME, "Testhero"));
    written = CHECK(mod_strings_write(&strings, &civ, 1, VANILLA));
    catalog.appearance_count = 0;
    return written;
}

bool test_a_heros_name_is_written_under_the_string_id_of_its_slot(void)
{
    return write_hero_strings(false) && (strstr(strings.text, HERO_LINE) != NULL);
}

bool test_a_skipped_heros_name_is_not_written(void)
{
    return write_hero_strings(true) && (strstr(strings.text, HERO_LINE) == NULL);
}

enum { LIST_SLOT = 1 };

static const char CIV_LIST[] = "{\"civilization_list\":[{\"unique_unit_id\":8,\"unique_unit_line\":-276},"
                               "{\"unique_unit_id\":725,\"unique_unit_line\":-267}]}";
static const char TWO_LONGBOWMEN[] = "{\"civilization_list\":[{\"unique_unit_id\":8,\"unique_unit_line\":-276},"
                                     "{\"unique_unit_id\":8,\"unique_unit_line\":-276}]}";

static JsonDocument civ_list;
static CivListOutput written_list;

static bool parsed(const char *text)
{
    civ_list.length = (int)strlen(text);
    return CHECK(text_copy(civ_list.text, sizeof civ_list.text, text)) && CHECK(json_parse(&civ_list));
}

static bool written_is(const char *expected)
{
    return (written_list.length == strlen(expected)) && (memcmp(written_list.text, expected, written_list.length) == 0);
}

static bool list_written_as(const char *list, const CivListEntry *entry, const char *expected)
{
    return parsed(list) && CHECK(civ_list_write(&civ_list, entry, &written_list)) && written_is(expected);
}

static const CivListEntry OWN_UNIT_ENTRY = { LIST_SLOT,     LONGBOWMAN,    CIV_LIST_KEEP, CIV_LIST_KEEP,
                                             CIV_LIST_KEEP, false,         CIV_LIST_KEEP, CIV_LIST_KEEP,
                                             CIV_LIST_KEEP, CIV_LIST_KEEP, CIV_LIST_KEEP };

bool test_the_civ_list_names_the_civs_own_unique_unit(void)
{
    return list_written_as(CIV_LIST, &OWN_UNIT_ENTRY, TWO_LONGBOWMEN);
}

enum { UNLISTED_UNIT = 9, UNLISTED_ICON = 7, UNLISTED_NAME = 5200, UNLISTED_DESCRIPTION = 26200 };

static const char LIST_WITH_TEXTS[] = "{\"civilization_list\":[{},{\"unique_unit_id\":725,"
                                      "\"unique_unit_image_paths\":[\"/resources/uniticons/110_50730.png\"],"
                                      "\"unique_unit_string_ids\":[{\"name\":5667,\"description\":26667}]}]}";
static const char UNLISTED_UNIT_SHOWN[] = "{\"civilization_list\":[{},{\"unique_unit_id\":9,"
                                          "\"unique_unit_image_paths\":[\"/resources/uniticons/007_50730.png\"],"
                                          "\"unique_unit_string_ids\":[{\"name\":5200,\"description\":26200}]}]}";

static const CivListEntry UNLISTED_ENTRY = { LIST_SLOT,     UNLISTED_UNIT, CIV_LIST_KEEP, CIV_LIST_KEEP,
                                             UNLISTED_ICON, false,         UNLISTED_NAME, UNLISTED_DESCRIPTION,
                                             CIV_LIST_KEEP, CIV_LIST_KEEP, CIV_LIST_KEEP };

bool test_a_unique_unit_missing_from_the_civ_list_takes_its_own_icon_and_texts(void)
{
    return list_written_as(LIST_WITH_TEXTS, &UNLISTED_ENTRY, UNLISTED_UNIT_SHOWN);
}

enum { OWN_DESCRIPTION_SID = 990101 };

static const char DESCRIBED_LIST[] = "{\"civilization_list\":[{},{\"tech_tree_description_string_id\":120150}]}";
static const char OWN_DESCRIPTION[] = "{\"civilization_list\":[{},{\"tech_tree_description_string_id\":990101}]}";

static const CivListEntry DESCRIBED_ENTRY = { LIST_SLOT,     CIV_LIST_KEEP, CIV_LIST_KEEP, CIV_LIST_KEEP,
                                              CIV_LIST_KEEP, false,         CIV_LIST_KEEP, CIV_LIST_KEEP,
                                              CIV_LIST_KEEP, CIV_LIST_KEEP, OWN_DESCRIPTION_SID };

bool test_the_civ_list_points_at_the_civs_own_description(void)
{
    return list_written_as(DESCRIBED_LIST, &DESCRIBED_ENTRY, OWN_DESCRIPTION);
}

static const CivListEntry KEEPING_ENTRY = { LIST_SLOT,     CIV_LIST_KEEP, CIV_LIST_KEEP, CIV_LIST_KEEP,
                                            CIV_LIST_KEEP, false,         CIV_LIST_KEEP, CIV_LIST_KEEP,
                                            CIV_LIST_KEEP, CIV_LIST_KEEP, CIV_LIST_KEEP };

bool test_a_value_the_civ_keeps_stays_the_games_in_the_civ_list(void)
{
    return list_written_as(DESCRIBED_LIST, &KEEPING_ENTRY, DESCRIBED_LIST);
}

enum { STABLE = 101, KEEP = 235, BARRACKS_NODE = 12, VILLAGER = 83, NOTHING_TICKED = -1 };

static const char OPEN_TREE[] = "{\"civ_techs_buildings\":[{\"Use Type\":\"Building\",\"Node ID\":101,"
                                "\"Node Status\":\"ResearchedCompleted\"}],\"civ_techs_units\":[]}";
static const char LOCKED_TREE[] = "{\"civ_techs_buildings\":[{\"Use Type\":\"Building\",\"Node ID\":101,"
                                  "\"Node Status\":\"NotAvailable\"}],\"civ_techs_units\":[]}";
static const char VILLAGER_TREE[] = "{\"civ_techs_buildings\":[],\"civ_techs_units\":[{\"Use Type\":\"Unit\","
                                    "\"Node ID\":83,\"Node Status\":\"ResearchedCompleted\"}]}";
static const char KEEP_TREE[] = "{\"civ_techs_buildings\":[{\"Use Type\":\"Building\",\"Node ID\":235,"
                                "\"Node Status\":\"ResearchRequired\"}],\"civ_techs_units\":[]}";

static bool tree_written_as(const char *tree, int ticked, const char *expected)
{
    TechTreeJob job = { &civ_list, &design, &catalog };

    civ_design_reset(&design);
    if (ticked != NOTHING_TICKED) {
        (void)CHECK(civ_design_toggle_tree(&design, TREE_BUILDINGS, ticked));
    }
    catalog.tree[0].kind = TREE_BUILDINGS;
    catalog.tree[0].id = STABLE;
    catalog.tree[1].kind = TREE_UNITS;
    catalog.tree[1].id = VILLAGER;
    catalog.tree[2].kind = TREE_BUILDINGS;
    catalog.tree[2].id = KEEP;
    catalog.tree_count = 3;
    return parsed(tree) && CHECK(tech_tree_write(&job, &written_list)) && written_is(expected);
}

bool test_a_building_the_civ_lacks_is_not_available_in_its_tech_tree(void)
{
    return tree_written_as(OPEN_TREE, BARRACKS_NODE, LOCKED_TREE);
}

bool test_a_building_the_civ_has_is_available_in_its_tech_tree(void)
{
    return tree_written_as(LOCKED_TREE, STABLE, OPEN_TREE);
}

bool test_a_civ_with_an_empty_tree_keeps_the_games_tech_tree(void)
{
    return tree_written_as(OPEN_TREE, NOTHING_TICKED, OPEN_TREE);
}

bool test_a_civ_keeps_its_villagers_in_its_tech_tree(void)
{
    return tree_written_as(VILLAGER_TREE, BARRACKS_NODE, VILLAGER_TREE);
}

bool test_a_building_the_civ_has_keeps_the_research_it_needs_in_its_tech_tree(void)
{
    return tree_written_as(KEEP_TREE, KEEP, KEEP_TREE);
}

static const char LOCKED_KEEP_TREE[] = "{\"civ_techs_buildings\":[{\"Use Type\":\"Building\",\"Node ID\":235,"
                                       "\"Node Status\":\"NotAvailable\"}],\"civ_techs_units\":[]}";

bool test_a_building_the_replaced_civ_lacked_takes_its_usual_status(void)
{
    bool written;

    catalog.tree[2].needs_research = true;
    written = tree_written_as(LOCKED_KEEP_TREE, KEEP, KEEP_TREE);
    catalog.tree[2].needs_research = false;
    return written;
}
