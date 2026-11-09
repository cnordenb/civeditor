#include "civ_file_tests.h"

#include "../../src/core/text.h"
#include "../../src/persist/civ_file.h"

enum { JAPANESE_SLOT = 5 };

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

bool test_original_example_civ_keeps_its_language(void)
{
    return civ_file_load(&loaded, EXAMPLE_PATH) && (loaded.choices[CHOICE_LANGUAGE] == JAPANESE_SLOT);
}

static void design_a_civ(CivDesign *design)
{
    civ_design_reset(design);
    (void)civ_design_set_text(design, TEXT_FIELD_NAME, "Jurchen \"Iron\" Riders");
    (void)civ_design_set_text(design, TEXT_FIELD_TAGLINE, "Cavalry");
    (void)civ_design_set_choice(design, CHOICE_CASTLE, 12);
    (void)civ_design_toggle_bonus(design, 4);
    (void)civ_design_toggle_bonus(design, 17);
    (void)civ_design_toggle_team_bonus(design, 8);
    (void)civ_design_toggle_tree(design, TREE_UNITS, 74);
    (void)civ_design_toggle_tree(design, TREE_BUILDINGS, 12);
    (void)civ_design_pick_unit(design, UNIT_ORIGIN_VANILLA, 1263);
    (void)civ_design_pick_tech(design, TECH_TIER_CASTLE, 3);
    (void)civ_design_pick_hero(design, 1911);
}

bool test_saved_civ_loads_back_unchanged(void)
{
    design_a_civ(&loaded);
    if (!civ_file_compose(&loaded, &first_text)) {
        return false;
    }
    document.length = (int)first_text.length;
    if (!text_copy(document.text, sizeof document.text, first_text.data) || !json_parse(&document)) {
        return false;
    }
    return civ_file_interpret(&document, &reloaded) && civ_file_compose(&reloaded, &second_text) &&
           text_equal(first_text.data, second_text.data);
}
