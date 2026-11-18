#include "game_text.h"

#include <string.h>

#include "../civlist/civ_list.h"
#include "../core/check.h"
#include "../core/file_io.h"
#include "../core/text.h"
#include "../json/json_reader.h"
#include "../mod/mod_places.h"

enum { STRINGS_CAPACITY = 4194304, NOT_WANTED = -1 };

typedef struct {
    char *text;
    size_t capacity;
    size_t length;
} TextSink;

static const char TAIL[] = " civilization";

static JsonDocument civ_list;
static char strings[STRINGS_CAPACITY];
static long wanted[CATALOG_BASE_CAPACITY];

static bool read_strings(const wchar_t *game_folder)
{
    wchar_t path[FILE_PATH_CAPACITY];
    FileRead read;

    if (!mod_game_path(game_folder, "resources\\en\\strings\\key-value\\key-value-strings-utf8.txt", path)) {
        return false;
    }
    read = file_read_all(path, strings, sizeof strings);
    return read.succeeded;
}

static long description_id(int slot)
{
    TextNumber id;
    int civ = civ_list_entry(&civ_list, slot);

    if (civ == JSON_NONE) {
        return NOT_WANTED;
    }
    id = json_number(&civ_list, json_member(&civ_list, civ, "tech_tree_description_string_id"));
    return id.valid ? id.value : NOT_WANTED;
}

static void want_descriptions(Catalog *catalog)
{
    int base;

    for (base = 0; base < catalog->base_count; ++base) {
        catalog->bases[base].tagline[0] = '\0';
        catalog->bases[base].description[0] = '\0';
        wanted[base] = description_id(catalog->bases[base].id);
    }
}

static bool ends_line(char letter)
{
    return (letter == '\0') || text_is_line_break(letter);
}

static void put(TextSink *sink, char letter)
{
    if (sink->length + 1U < sink->capacity) {
        sink->text[sink->length] = letter;
        ++sink->length;
        sink->text[sink->length] = '\0';
    }
}

static void put_break(TextSink *sink)
{
    if (sink->length > 0U) {
        put(sink, '\r');
        put(sink, '\n');
    }
}

static void trim(TextSink *sink)
{
    while ((sink->length > 0U) && ((sink->text[sink->length - 1U] == ' ') ||
                                   text_is_line_break(sink->text[sink->length - 1U]))) {
        --sink->length;
        sink->text[sink->length] = '\0';
    }
}

static void cut_tail(TextSink *sink)
{
    size_t tail = sizeof TAIL - 1U;

    trim(sink);
    if ((sink->length >= tail) && (strcmp(&sink->text[sink->length - tail], TAIL) == 0)) {
        sink->length -= tail;
        sink->text[sink->length] = '\0';
    }
}

static int letter_length(const char *at)
{
    return ((at[0] == '\\') && !ends_line(at[1])) ? 2 : 1;
}

static void read_quoted(const char *at, BaseCiv *base)
{
    TextSink tagline = { base->tagline, sizeof base->tagline, 0U };
    TextSink description = { base->description, sizeof base->description, 0U };
    TextSink *sink = &tagline;

    while ((at[0] != '"') && !ends_line(at[0])) {
        if ((at[0] == '\\') && (at[1] == 'n')) {
            put_break(&description);
            sink = &description;
        } else {
            put(sink, at[letter_length(at) - 1]);
        }
        at += letter_length(at);
    }
    cut_tail(&tagline);
    trim(&description);
}

static int base_wanting(const Catalog *catalog, long id)
{
    int base;

    for (base = 0; base < catalog->base_count; ++base) {
        if (wanted[base] == id) {
            return base;
        }
    }
    return NOT_WANTED;
}

static void read_line(Catalog *catalog, const char *line)
{
    unsigned long id = 0UL;
    int digit;
    int base;
    const char *quote;

    for (digit = 0; text_is_digit(line[digit]); ++digit) {
        id = (id * 10UL) + (unsigned long)(line[digit] - '0');
    }
    base = base_wanting(catalog, (long)id);
    if (base == NOT_WANTED) {
        return;
    }
    quote = strchr(&line[digit], '"');
    if (quote != NULL) {
        read_quoted(quote + 1, &catalog->bases[base]);
    }
}

static void read_lines(Catalog *catalog)
{
    const char *line = strings;

    while (line != NULL) {
        read_line(catalog, line);
        line = strchr(line, '\n');
        line = (line == NULL) ? NULL : line + 1;
    }
}

static bool read_texts(Catalog *catalog, const wchar_t *game_folder)
{
    if (!civ_list_read(&civ_list, game_folder) || !read_strings(game_folder)) {
        return false;
    }
    want_descriptions(catalog);
    read_lines(catalog);
    return true;
}

bool game_text_read(Catalog *catalog, const wchar_t *game_folder)
{
    return CHECK(catalog != NULL) && CHECK(game_folder != NULL) && read_texts(catalog, game_folder);
}
