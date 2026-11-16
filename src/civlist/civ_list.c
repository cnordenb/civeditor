#include "civ_list.h"

#include "../core/check.h"
#include "../core/text.h"

enum { VALUE_CAPACITY = 256, CIV_LIMIT = 256, ICON_DIGITS_TWO = 10, ICON_DIGITS_FULL = 100 };

static const JsonDocument *document;
static JsonPatch patch;

int civ_list_entry(const JsonDocument *list, int slot)
{
    int civs;
    int civ;
    int index;

    if (!CHECK(list != NULL) || !CHECK((slot >= 0) && (slot < CIV_LIMIT))) {
        return JSON_NONE;
    }
    civs = json_member(list, 0, "civilization_list");
    civ = (civs == JSON_NONE) ? JSON_NONE : json_first_child(list, civs);
    for (index = 0; (index < slot) && (index < CIV_LIMIT) && (civ != JSON_NONE); ++index) {
        civ = json_next_sibling(list, civs, civ);
    }
    return civ;
}

static int entry_with_unit(int unit)
{
    TextNumber number;
    int civs;
    int civ;
    int index;

    if (!CHECK(document != NULL) || !CHECK(unit >= 0)) {
        return JSON_NONE;
    }
    civs = json_member(document, 0, "civilization_list");
    civ = (civs == JSON_NONE) ? JSON_NONE : json_first_child(document, civs);
    for (index = 0; (index < CIV_LIMIT) && (civ != JSON_NONE); ++index) {
        number = json_number(document, json_member(document, civ, "unique_unit_id"));
        if (number.valid && (number.value == (long)unit)) {
            return civ;
        }
        civ = json_next_sibling(document, civs, civ);
    }
    return JSON_NONE;
}

static void set_number(int civ, const char *key, long value)
{
    char text[VALUE_CAPACITY] = "";
    int token;

    if (!CHECK(document != NULL) || !CHECK(key != NULL) || (civ == JSON_NONE) || (value < 0L)) {
        return;
    }
    token = json_member(document, civ, key);
    if (json_is_kind(document, token, JSON_KIND_NUMBER) && text_append_number(text, sizeof text, value)) {
        json_patch_replace(&patch, token, text);
    }
}

static void copy_member(int civ, int source, const char *key)
{
    char text[VALUE_CAPACITY] = "";
    const JsonToken *from;
    int to;
    int found;

    if (!CHECK(document != NULL) || !CHECK(key != NULL) || (civ == JSON_NONE) || (source == JSON_NONE)) {
        return;
    }
    to = json_member(document, civ, key);
    found = json_member(document, source, key);
    if ((found == JSON_NONE) || json_is_kind(document, found, JSON_KIND_STRING) ||
        !json_is_kind(document, to, document->tokens[found].kind)) {
        return;
    }
    from = &document->tokens[found];
    if (text_copy_span(text, sizeof text, &document->text[from->start], (size_t)(from->end - from->start))) {
        json_patch_replace(&patch, to, text);
    }
}

static bool make_string_ids(char *text, size_t capacity, const CivListEntry *entry)
{
    if (!CHECK(text != NULL) || !CHECK(entry != NULL)) {
        return false;
    }
    return text_append_number(text, capacity, entry->unit_name_sid) &&
           text_append(text, capacity, ",\"description\":") &&
           text_append_number(text, capacity, entry->unit_description_sid) && text_append(text, capacity, "}]");
}

static void set_string_ids(int civ, const CivListEntry *entry)
{
    char text[VALUE_CAPACITY] = "[{\"name\":";

    if (!CHECK(document != NULL) || !CHECK(entry != NULL) || (civ == JSON_NONE) || (entry->unit_name_sid < 0L) ||
        (entry->unit_description_sid < 0L)) {
        return;
    }
    if (make_string_ids(text, sizeof text, entry)) {
        json_patch_replace(&patch, json_member(document, civ, "unique_unit_string_ids"), text);
    }
}

static const char *icon_zeros(int icon)
{
    if (icon >= ICON_DIGITS_FULL) {
        return "";
    }
    return (icon >= ICON_DIGITS_TWO) ? "0" : "00";
}

static bool make_icon_path(char *text, size_t capacity, int icon)
{
    if (!CHECK(text != NULL) || !CHECK(icon >= 0)) {
        return false;
    }
    return text_append(text, capacity, icon_zeros(icon)) && text_append_number(text, capacity, icon) &&
           text_append(text, capacity, "_50730.png\"]");
}

static void set_icon(int civ, int icon)
{
    char text[VALUE_CAPACITY] = "[\"/resources/uniticons/";

    if (!CHECK(document != NULL) || (civ == JSON_NONE) || (icon < 0)) {
        return;
    }
    if (make_icon_path(text, sizeof text, icon)) {
        json_patch_replace(&patch, json_member(document, civ, "unique_unit_image_paths"), text);
    }
}

static void name_unit(int civ, int source, const CivListEntry *entry)
{
    if (!CHECK(entry != NULL) || (civ == JSON_NONE)) {
        return;
    }
    if (source == JSON_NONE) {
        set_icon(civ, entry->icon);
    } else {
        copy_member(civ, source, "unique_unit_image_paths");
    }
    if (entry->unit_named || (source == JSON_NONE)) {
        set_string_ids(civ, entry);
    } else {
        copy_member(civ, source, "unique_unit_string_ids");
    }
}

bool civ_list_write(const JsonDocument *list, const CivListEntry *entry, CivListOutput *output)
{
    int civ;
    int source;

    if (!CHECK(list != NULL) || !CHECK(entry != NULL) || !CHECK(output != NULL)) {
        return false;
    }
    document = list;
    json_patch_start(&patch, list);
    civ = civ_list_entry(list, entry->slot);
    if (civ == JSON_NONE) {
        return false;
    }
    set_number(civ, "unique_tech_id_1", entry->imperial_tech);
    set_number(civ, "unique_tech_id_2", entry->castle_tech);
    set_number(civ, "tech_tree_description_string_id", entry->description_sid);
    if (entry->unit >= 0) {
        source = entry_with_unit(entry->unit);
        set_number(civ, "unique_unit_id", entry->unit);
        set_number(civ, "elite_unique_unit_id", entry->elite_unit);
        set_number(civ, "unique_unit_upgrade_id", entry->elite_tech);
        copy_member(civ, source, "unique_unit_line");
        name_unit(civ, source, entry);
    }
    return json_patch_write(&patch, output);
}
