#include "civ_file_tests.h"

#include "../../src/core/text.h"
#include "../../src/persist/civ_file.h"

enum { TEXT_WIDTH = 48, FIRST_CHOICE = 3 };

static const wchar_t EXAMPLE_PATH[] = L"tests/fixtures/example.civbuilder.json";

static CivDesign loaded;
static CivDesign reloaded;
static JsonWriter first_text;
static JsonWriter second_text;
static JsonDocument document;

bool test_original_example_civ_keeps_its_name(void)
{
    return civ_file_load(&loaded, EXAMPLE_PATH) && text_equal(civ_design_text(&loaded, TEXT_FIELD_NAME), "Vanguards");
}

bool test_original_example_civ_keeps_its_bonuses(void)
{
    return civ_file_load(&loaded, EXAMPLE_PATH) && (loaded.bonus_count == 2) &&
           civ_design_has_bonus(&loaded, 9) && civ_design_has_bonus(&loaded, 29);
}

bool test_original_example_civ_keeps_its_unique_unit(void)
{
    return civ_file_load(&loaded, EXAMPLE_PATH) && (loaded.unique_unit.origin == UNIT_ORIGIN_CUSTOM) &&
           (loaded.unique_unit.id == 5);
}

static void set_every_text_and_choice(CivDesign *design)
{
    static const char TEXTS[TEXT_FIELD_COUNT][TEXT_WIDTH] = { "Jurchen \"Iron\" Riders", "Cavalry",
                                                               "Riders of the north.\nStrong cavalry.",
                                                               "Iron Rider", "A rider in iron.", "Iron Stirrups",
                                                               "Grand Hunt", "Aguda" };
    int field;
    int choice;

    for (field = 0; field < TEXT_FIELD_COUNT; ++field) {
        (void)civ_design_set_text(design, (TextField)field, TEXTS[field]);
    }
    for (choice = 0; choice < CHOICE_COUNT; ++choice) {
        (void)civ_design_set_choice(design, (ChoiceField)choice, FIRST_CHOICE + choice);
    }
}

static void design_a_civ(CivDesign *design)
{
    civ_design_reset(design);
    set_every_text_and_choice(design);
    (void)civ_design_toggle_bonus(design, 4);
    (void)civ_design_toggle_bonus(design, 17);
    (void)civ_design_toggle_team_bonus(design, 8);
    (void)civ_design_toggle_tree(design, TREE_UNITS, 74);
    (void)civ_design_toggle_tree(design, TREE_BUILDINGS, 12);
    (void)civ_design_toggle_tree(design, TREE_TECHS, 45);
    (void)civ_design_pick_unit(design, UNIT_ORIGIN_VANILLA, 1263);
    (void)civ_design_pick_tech(design, TECH_TIER_CASTLE, 3);
    (void)civ_design_pick_tech(design, TECH_TIER_IMPERIAL, 6);
    (void)civ_design_set_tech_value(design, TECH_TIER_CASTLE, TECH_VALUE_FOOD, 250);
    (void)civ_design_set_tech_value(design, TECH_TIER_CASTLE, TECH_VALUE_TIME, 45);
    (void)civ_design_set_tech_value(design, TECH_TIER_IMPERIAL, TECH_VALUE_WOOD, 600);
    (void)civ_design_set_tech_value(design, TECH_TIER_IMPERIAL, TECH_VALUE_TIME, 70);
    (void)civ_design_pick_hero(design, 1911);
}

static bool composed_civ_is_parsed(void)
{
    if (!civ_file_compose(&loaded, &first_text)) {
        return false;
    }
    document.length = (int)first_text.length;
    return text_copy(document.text, sizeof document.text, first_text.data) && json_parse(&document);
}

static bool texts_match(const CivDesign *first, const CivDesign *second)
{
    int field;

    for (field = 0; field < TEXT_FIELD_COUNT; ++field) {
        if (!text_equal(civ_design_text(first, (TextField)field), civ_design_text(second, (TextField)field))) {
            return false;
        }
    }
    return true;
}

bool test_saved_civ_loads_back_unchanged(void)
{
    design_a_civ(&loaded);
    return composed_civ_is_parsed() && civ_file_interpret(&document, &reloaded) && texts_match(&loaded, &reloaded) &&
           civ_file_compose(&reloaded, &second_text) && text_equal(first_text.data, second_text.data);
}

bool test_a_bonus_multiplier_is_read_back_from_a_civ_file(void)
{
    design_a_civ(&loaded);
    (void)civ_design_set_multiplier(&loaded, false, 17, civ_multiplier_of(CIV_MULTIPLIER_LIMIT));
    return composed_civ_is_parsed() && civ_file_interpret(&document, &reloaded) &&
           (reloaded.bonuses[1].multiplier.millionths == CIV_MULTIPLIER_LIMIT * CIV_MILLIONTHS);
}
