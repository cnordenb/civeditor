#include "game_tests.h"

#include <string.h>

#include "../../src/core/check.h"
#include "../../src/core/file_io.h"
#include "../../src/core/text.h"
#include "../../src/gametext/game_text.h"
#include "../../src/mod/mod_places.h"

enum { BRITONS = 1, FRANKS = 2, LONG_TAGLINE = 300, LONG_LINE_CAPACITY = 512 };

static const wchar_t ROOT[] = L"build";
static const wchar_t GAME[] = L"build\\described_game";
static const wchar_t LIST_FOLDER[] = L"described_game\\resources\\_common\\dat";
static const wchar_t STRINGS_FOLDER[] = L"described_game\\resources\\en\\strings\\key-value";
static const char CIV_LIST[] = "{\"civilization_list\":[{},{\"name_string_id\":10271,"
                               "\"tech_tree_description_string_id\":120150},{\"name_string_id\":10272,"
                               "\"tech_tree_description_string_id\":120151}]}";
static const char STRINGS[] = "10271 \"Britons\"\r\n"
                              "120150 \"Foot Archer civilization\\n\\n- Shepherds work faster\\n"
                              "Towers \\\"shoot\\\"\"\r\n";
static const char UNCLOSED[] = "120150 \"Foot Archer civilization\\n\\nShepherds\r\n10271 \"Britons\"\r\n";
static const char SPACED[] = "120150 \"Foot Archer civilization\\n\\nShepherds \\n\"\r\n";
static const char WHOLE[] = "120150 \"Archers of the north\\n\\nShepherds\"\r\n";

static Catalog described;

static bool write_file(const wchar_t *folder, const wchar_t *name, const char *text)
{
    wchar_t place[FILE_PATH_CAPACITY];
    wchar_t path[FILE_PATH_CAPACITY];

    return mod_make_folders(ROOT, folder) && file_join_path(place, FILE_PATH_CAPACITY, ROOT, folder) &&
           file_join_path(path, FILE_PATH_CAPACITY, place, name) && file_write_all(path, text, strlen(text));
}

static const BaseCiv *read_given(const char *strings, int base)
{
    described.base_count = 2;
    described.bases[0].id = BRITONS;
    described.bases[1].id = FRANKS;
    (void)text_copy(described.bases[0].tagline, sizeof described.bases[0].tagline, "stale");
    (void)text_copy(described.bases[1].tagline, sizeof described.bases[1].tagline, "stale");
    (void)text_copy(described.bases[0].description, sizeof described.bases[0].description, "stale");
    (void)text_copy(described.bases[1].description, sizeof described.bases[1].description, "stale");
    (void)CHECK(write_file(LIST_FOLDER, L"civilizations.json", CIV_LIST));
    (void)CHECK(write_file(STRINGS_FOLDER, L"key-value-strings-utf8.txt", strings));
    (void)game_text_read(&described, GAME);
    return &described.bases[base];
}

bool test_the_game_gives_a_civ_its_tagline(void)
{
    return text_equal(read_given(STRINGS, 0)->tagline, "Foot Archer");
}

bool test_the_description_of_a_civ_starts_after_its_tagline(void)
{
    return strncmp(read_given(STRINGS, 0)->description, "- Shepherds", strlen("- Shepherds")) == 0;
}

bool test_a_line_break_of_the_game_breaks_the_description(void)
{
    return strstr(read_given(STRINGS, 0)->description, "faster\r\nTowers") != NULL;
}

bool test_a_quote_of_the_game_is_kept_in_the_description(void)
{
    return strstr(read_given(STRINGS, 0)->description, "\"shoot\"") != NULL;
}

bool test_a_civ_the_game_does_not_describe_keeps_no_tagline(void)
{
    return text_equal(read_given(STRINGS, 1)->tagline, "");
}

bool test_texts_are_read_from_a_game_that_is_found(void)
{
    (void)read_given(STRINGS, 0);
    return game_text_read(&described, GAME);
}

bool test_a_civ_the_game_does_not_describe_keeps_no_description(void)
{
    return text_equal(read_given(STRINGS, 1)->description, "");
}

bool test_a_text_without_its_closing_quote_ends_with_its_line(void)
{
    return text_equal(read_given(UNCLOSED, 0)->description, "Shepherds");
}

bool test_the_spaces_and_breaks_at_the_end_of_a_text_are_cut(void)
{
    return text_equal(read_given(SPACED, 0)->description, "Shepherds");
}

bool test_a_first_line_without_civilization_is_the_whole_tagline(void)
{
    return text_equal(read_given(WHOLE, 0)->tagline, "Archers of the north");
}

bool test_a_tagline_longer_than_its_room_is_cut_to_fit(void)
{
    static char long_line[LONG_LINE_CAPACITY];
    int letter;

    (void)text_copy(long_line, sizeof long_line, "120150 \"");
    for (letter = 0; letter < LONG_TAGLINE; ++letter) {
        (void)text_append(long_line, sizeof long_line, "A");
    }
    (void)text_append(long_line, sizeof long_line, "\"\r\n");
    return strlen(read_given(long_line, 0)->tagline) == sizeof described.bases[0].tagline - 1U;
}
