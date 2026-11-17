#include "mod_strings.h"

#include <string.h>

#include "../core/check.h"
#include "../core/text.h"

enum {
    PICKER_OFFSET = 80000,
    DESCRIPTION_OFFSET = 109879,
    FIRST_DESCRIPTION = 120150,
    DESCRIPTION_SPAN = 128,
    LINE_LIMIT = 400000,
    ID_DIGITS = 6,
    KEY_CAPACITY = 32
};

static char part[MOD_TEXT_CAPACITY];
static long description_at[DESCRIPTION_SPAN];
static char elite_name[CIV_LINE_CAPACITY];

static void put_span(ModStrings *strings, const char *text, size_t length)
{
    if (length >= sizeof strings->text - strings->length) {
        strings->failed = true;
        return;
    }
    (void)memcpy(strings->text + strings->length, text, length);
    strings->length += length;
    strings->text[strings->length] = '\0';
}

static void put(ModStrings *strings, const char *text)
{
    put_span(strings, text, strlen(text));
}

static void put_escaped_letter(ModStrings *strings, const char *text, size_t index)
{
    if (text_is_line_break(text[index])) {
        put(strings, "\\n");
    } else if ((text[index] == '"') && ((index == 0U) || (text[index - 1U] != '\\'))) {
        put(strings, "\\\"");
    } else {
        put_span(strings, &text[index], 1U);
    }
}

static void put_escaped(ModStrings *strings, const char *text)
{
    size_t index;

    for (index = 0U; (index < (size_t)MOD_TEXT_CAPACITY) && (text[index] != '\0'); ++index) {
        if ((text[index] != '\r') || (text[index + 1U] != '\n')) {
            put_escaped_letter(strings, text, index);
        }
    }
}

static void put_entry(ModStrings *strings, const char *key, const char *text)
{
    put(strings, key);
    put(strings, " \"");
    put_escaped(strings, text);
    put(strings, "\"\n");
}

static void put_numbered(ModStrings *strings, long id, const char *text)
{
    char key[KEY_CAPACITY] = "";

    (void)text_append_number(key, sizeof key, id);
    put_entry(strings, key, text);
}

static long line_id(const char *line)
{
    TextNumber number;
    size_t length = 0U;

    for (length = 0U; (length < (size_t)ID_DIGITS) && text_is_digit(line[length]); ++length) {
        continue;
    }
    if ((length != (size_t)ID_DIGITS) || (line[length] != ' ')) {
        return -1L;
    }
    number = text_to_number(line, length);
    return number.valid ? number.value : -1L;
}

static void put_vanilla(ModStrings *strings, const char *line)
{
    const char *end;
    size_t length;

    end = strchr(line, '\n');
    length = (end == NULL) ? strlen(line) : (size_t)(end - line);
    length -= ((length > 0U) && (line[length - 1U] == '\r')) ? 1U : 0U;
    put_span(strings, line, length);
    put(strings, "\n");
}

static void note_description(long id, long offset)
{
    long index = id - FIRST_DESCRIPTION;

    if (!CHECK(offset >= 0L) || (index < 0L) || (index >= DESCRIPTION_SPAN)) {
        return;
    }
    if (description_at[index] < 0L) {
        description_at[index] = offset;
    }
}

static void find_descriptions(const char *vanilla)
{
    const char *line = vanilla;
    int count;

    for (count = 0; count < DESCRIPTION_SPAN; ++count) {
        description_at[count] = -1L;
    }
    for (count = 0; (count < LINE_LIMIT) && (line != NULL) && (line[0] != '\0'); ++count) {
        note_description(line_id(line), (long)(line - vanilla));
        line = strchr(line, '\n');
        line = (line == NULL) ? NULL : line + 1;
    }
}

static void put_descriptions(ModStrings *strings, const char *vanilla, long ours)
{
    int index;

    find_descriptions(vanilla);
    for (index = 0; index < DESCRIPTION_SPAN; ++index) {
        if (FIRST_DESCRIPTION + index == ours) {
            put_numbered(strings, ours, part);
        } else if (description_at[index] >= 0L) {
            put_vanilla(strings, &vanilla[description_at[index]]);
        }
    }
    if ((ours < FIRST_DESCRIPTION) || (ours >= FIRST_DESCRIPTION + DESCRIPTION_SPAN)) {
        put_numbered(strings, ours, part);
    }
}

static bool make_tip_key(char *key, size_t capacity, int slot, int index)
{
    if (!CHECK(capacity > 0U)) {
        return false;
    }
    key[0] = '\0';
    return text_append(key, capacity, "IDS_CIVTIPS_") && text_append_number(key, capacity, slot) &&
           text_append(key, capacity, "_") && text_append_number(key, capacity, index);
}

static void put_tips(ModStrings *strings, const ModCiv *civ)
{
    char key[KEY_CAPACITY];
    int index;

    for (index = 0; index < MOD_TIP_COUNT; ++index) {
        if (!make_tip_key(key, sizeof key, civ->slot, index) || !mod_tip(civ, index, part, sizeof part)) {
            strings->failed = true;
            return;
        }
        put_entry(strings, key, part);
    }
}

static bool make_part(const char *first, const char *second, const char *third)
{
    part[0] = '\0';
    return text_append_three(part, sizeof part, first, second, third);
}

static bool make_tech_label(const char *name, const char *description)
{
    size_t length;

    if (!make_part("Research ", name, "")) {
        return false;
    }
    if (description[0] == '\0') {
        return true;
    }
    if (!text_append_two(part, sizeof part, " (", description)) {
        return false;
    }
    length = strlen(part);
    if (part[length - 1U] == '.') {
        part[length - 1U] = '\0';
    }
    return text_append(part, sizeof part, ")");
}

static bool make_tooltip(const char *verb, const char *name, const char *description)
{
    if (!make_part(verb, " <b>", name) || !text_append(part, sizeof part, "<b> (<cost>)")) {
        return false;
    }
    return (description[0] == '\0') || text_append_two(part, sizeof part, "\n", description);
}

static bool put_tech_lines(ModStrings *strings, int sid, const char *name, const char *description)
{
    put_numbered(strings, sid, name);
    put_numbered(strings, sid + MOD_TECH_TREE_OFFSET, name);
    if (!make_tech_label(name, description)) {
        return false;
    }
    put_numbered(strings, sid + MOD_LABEL_OFFSET, part);
    if (!make_tooltip("Research", name, description)) {
        return false;
    }
    put_numbered(strings, sid + MOD_TOOLTIP_OFFSET, part);
    return true;
}

static void put_tech_names(ModStrings *strings, const ModCiv *civ)
{
    const char *name;
    int tier;
    int sid;

    for (tier = 0; tier < TECH_TIER_COUNT; ++tier) {
        sid = catalog_string_id(civ->catalog, civ->slot, (CivString)tier);
        name = mod_tech_text(civ, (TechTier)tier, true);
        if ((sid == CATALOG_MISSING) || (name[0] == '\0')) {
            continue;
        }
        if (!put_tech_lines(strings, sid, name, mod_tech_text(civ, (TechTier)tier, false))) {
            strings->failed = true;
            return;
        }
    }
}

static void put_unit_lines(ModStrings *strings, int sid, const char *name, const char *description)
{
    if ((sid == CATALOG_MISSING) || (name[0] == '\0')) {
        return;
    }
    put_numbered(strings, sid, name);
    if (!make_part("Create ", name, "")) {
        strings->failed = true;
        return;
    }
    put_numbered(strings, sid + MOD_LABEL_OFFSET, part);
    if (!make_tooltip("Create", name, description)) {
        strings->failed = true;
        return;
    }
    put_numbered(strings, sid + MOD_TOOLTIP_OFFSET, part);
    put_numbered(strings, sid + MOD_HELP_OFFSET, part);
}

static void put_unit_names(ModStrings *strings, const ModCiv *civ)
{
    const CivDesign *design;
    const char *name;

    if (!mod_unit_has_own_texts(civ->design)) {
        return;
    }
    design = civ->design;
    name = mod_unit_name(civ);
    elite_name[0] = '\0';
    if (!text_append_two(elite_name, sizeof elite_name, "Elite ", name)) {
        strings->failed = true;
        return;
    }
    put_unit_lines(strings, catalog_string_id(civ->catalog, civ->slot, CIV_STRING_UNIT), name,
                   design->texts[TEXT_FIELD_UNIT_DESCRIPTION]);
    put_unit_lines(strings, catalog_string_id(civ->catalog, civ->slot, CIV_STRING_ELITE_UNIT), elite_name,
                   design->texts[TEXT_FIELD_UNIT_DESCRIPTION]);
}

static void put_hero_name(ModStrings *strings, const ModCiv *civ)
{
    const CivDesign *design;

    design = civ->design;
    if ((design->hero_unit >= 0) && !civ->hero_skipped) {
        put_unit_lines(strings, catalog_string_id(civ->catalog, civ->slot, CIV_STRING_HERO),
                       design->texts[TEXT_FIELD_HERO_NAME], "");
    }
}

bool mod_strings_write(ModStrings *strings, const ModCiv *civ, const char *vanilla)
{
    const CivDesign *design;
    const char *alias;

    design = civ->design;
    alias = design->texts[TEXT_FIELD_NAME];
    strings->length = 0U;
    strings->text[0] = '\0';
    strings->failed = false;
    put_numbered(strings, civ->name_sid, alias);
    if (!make_part("Click to play as ", alias, ".")) {
        return false;
    }
    put_numbered(strings, civ->name_sid + PICKER_OFFSET, part);
    if (!mod_description(civ, part, sizeof part)) {
        return false;
    }
    put_descriptions(strings, vanilla, civ->name_sid + DESCRIPTION_OFFSET);
    if (mod_description_sid(civ->slot) >= 0L) {
        put_numbered(strings, mod_description_sid(civ->slot), part);
    }
    put_tips(strings, civ);
    put_tech_names(strings, civ);
    put_unit_names(strings, civ);
    put_hero_name(strings, civ);
    return !strings->failed;
}
