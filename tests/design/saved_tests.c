#include "saved_tests.h"

#include <string.h>

#include "../../src/core/check.h"
#include "../../src/persist/civ_file.h"

enum { A_UNIT = 1263, A_CUSTOM_UNIT = 5, BRITONS_SLOT = 1 };

static CivDesign design;
static JsonWriter saved;

static bool saved_text_has(const char *part)
{
    if (!CHECK(part != NULL) || !CHECK(civ_file_compose(&design, &saved))) {
        return false;
    }
    return strstr(saved.data, part) != NULL;
}

bool test_a_unit_of_the_game_is_saved_with_no_custom_units_index(void)
{
    civ_design_reset(&design);
    (void)CHECK(civ_design_pick_unit(&design, UNIT_ORIGIN_VANILLA, A_UNIT));
    return saved_text_has("\"km_idx\": null");
}

bool test_a_custom_unit_is_saved_with_no_id_of_the_game(void)
{
    civ_design_reset(&design);
    (void)CHECK(civ_design_pick_unit(&design, UNIT_ORIGIN_CUSTOM, A_CUSTOM_UNIT));
    return saved_text_has("\"vanilla_id\": null");
}

bool test_the_language_of_the_britons_is_saved_as_index_0(void)
{
    civ_design_reset(&design);
    (void)CHECK(civ_design_set_choice(&design, CHOICE_LANGUAGE, BRITONS_SLOT));
    return saved_text_has("\"language\": 0");
}
