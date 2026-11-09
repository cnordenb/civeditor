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
    if (!CHECK(strings != NULL) || !CHECK(text != NULL)) {
        return;
    }
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
    if (!CHECK(strings != NULL) || !CHECK(text != NULL)) {
        return;
    }
    put_span(strings, text, strlen(text));
}

static void put_escaped(ModStrings *strings, const char *text)
{
    size_t index;

    if (!CHECK(strings != NULL) || !CHECK(text != NULL)) {
        return;
    }
    for (index = 0U; (index < (size_t)MOD_TEXT_CAPACITY) && (text[index] != '\0'); ++index) {
        if ((text[index] == '\r') && (text[index + 1U] == '\n')) {
            continue;
        }
        if ((text[index] == '\n') || (text[index] == '\r')) {
            put(strings, "\\n");
        } else if ((text[index] == '"') && ((index == 0U) || (text[index - 1U] != '\\'))) {
            put(strings, "\\\"");
        } else {
            put_span(strings, &text[index], 1U);
        }
    }
}

static void put_entry(ModStrings *strings, const char *key, const char *text)
{
    if (!CHECK(key != NULL) || !CHECK(text != NULL)) {
        return;
    }
    put(strings, key);
    put(strings, " \"");
    put_escaped(strings, text);
    put(strings, "\"\n");
}

static void put_numbered(ModStrings *strings, long id, const char *text)
{
    char key[KEY_CAPACITY] = "";

    if (!CHECK(strings != NULL) || !CHECK(id >= 0L)) {
        return;
    }
    (void)text_append_number(key, sizeof key, id);
    put_entry(strings, key, text);
}

static long line_id(const char *line)
{
    TextNumber number;
    size_t length = 0U;

    if (!CHECK(line != NULL) || !CHECK(line[0] != '\0')) {
        return -1L;
    }
    for (length = 0U; (length < (size_t)ID_DIGITS) && (line[length] >= '0') && (line[length] <= '9'); ++length) {
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

    if (!CHECK(strings != NULL) || !CHECK(line != NULL)) {
        return;
    }
    end = strchr(line, '\n');
    length = (end == NULL) ? strlen(line) : (size_t)(end - line);
    length -= ((length > 0U) && (line[length - 1U] == '\r')) ? 1U : 0U;
    put_span(strings, line, length);
    put(strings, "\n");
}

static void find_descriptions(const char *vanilla)
{
    const char *line = vanilla;
    long id;
    int count;

    if (!CHECK(vanilla != NULL) || !CHECK(DESCRIPTION_SPAN > 0)) {
        return;
    }
    for (count = 0; count < DESCRIPTION_SPAN; ++count) {
        description_at[count] = -1L;
    }
    for (count = 0; (count < LINE_LIMIT) && (line != NULL) && (line[0] != '\0'); ++count) {
        id = line_id(line);
        if ((id >= FIRST_DESCRIPTION) && (id < FIRST_DESCRIPTION + DESCRIPTION_SPAN) &&
            (description_at[id - FIRST_DESCRIPTION] < 0L)) {
            description_at[id - FIRST_DESCRIPTION] = (long)(line - vanilla);
        }
        line = strchr(line, '\n');
        line = (line == NULL) ? NULL : line + 1;
    }
}

static void put_descriptions(ModStrings *strings, const char *vanilla, long ours)
{
    int index;

    if (!CHECK(strings != NULL) || !CHECK(vanilla != NULL)) {
        return;
    }
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

static void put_tips(ModStrings *strings, const ModCiv *civ)
{
    char key[KEY_CAPACITY];
    int index;

    if (!CHECK(strings != NULL) || !CHECK(civ != NULL)) {
        return;
    }
    for (index = 0; index < MOD_TIP_COUNT; ++index) {
        if (!text_copy(key, sizeof key, "IDS_CIVTIPS_") || !text_append_number(key, sizeof key, civ->slot) ||
            !text_append(key, sizeof key, "_") || !text_append_number(key, sizeof key, index) ||
            !mod_tip(civ, index, part, sizeof part)) {
            strings->failed = true;
            return;
        }
        put_entry(strings, key, part);
    }
}

static bool make_tech_label(const char *name, const char *description)
{
    size_t length;

    if (!CHECK(name != NULL) || !CHECK(description != NULL) || !text_copy(part, sizeof part, "Research ") ||
        !text_append(part, sizeof part, name)) {
        return false;
    }
    if (description[0] == '\0') {
        return true;
    }
    if (!text_append(part, sizeof part, " (") || !text_append(part, sizeof part, description)) {
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
    if (!CHECK((verb != NULL) && (name != NULL)) || !CHECK(description != NULL) ||
        !text_copy(part, sizeof part, verb) || !text_append(part, sizeof part, " <b>") ||
        !text_append(part, sizeof part, name) || !text_append(part, sizeof part, "<b> (<cost>)")) {
        return false;
    }
    return (description[0] == '\0') ||
           (text_append(part, sizeof part, "\n") && text_append(part, sizeof part, description));
}

static void put_tech_names(ModStrings *strings, const ModCiv *civ)
{
    const char *name;
    const char *description;
    int tier;
    int sid;

    if (!CHECK(strings != NULL) || !CHECK(civ != NULL)) {
        return;
    }
    for (tier = 0; tier < TECH_TIER_COUNT; ++tier) {
        sid = catalog_string_id(civ->catalog, civ->slot, (CivString)tier);
        name = mod_tech_text(civ, (TechTier)tier, true);
        description = mod_tech_text(civ, (TechTier)tier, false);
        if ((sid == CATALOG_MISSING) || (name[0] == '\0')) {
            continue;
        }
        put_numbered(strings, sid, name);
        put_numbered(strings, sid + MOD_TECH_TREE_OFFSET, name);
        if (!make_tech_label(name, description) || !CHECK(sid + MOD_LABEL_OFFSET > 0)) {
            strings->failed = true;
            return;
        }
        put_numbered(strings, sid + MOD_LABEL_OFFSET, part);
        if (!make_tooltip("Research", name, description)) {
            strings->failed = true;
            return;
        }
        put_numbered(strings, sid + MOD_TOOLTIP_OFFSET, part);
    }
}

static void put_unit_lines(ModStrings *strings, int sid, const char *name, const char *description)
{
    if (!CHECK(strings != NULL) || !CHECK(name != NULL) || (sid == CATALOG_MISSING) || (name[0] == '\0')) {
        return;
    }
    put_numbered(strings, sid, name);
    if (!text_copy(part, sizeof part, "Create ") || !text_append(part, sizeof part, name)) {
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

    if (!CHECK(strings != NULL) || !CHECK(civ != NULL) || !mod_unit_has_own_texts(civ->design)) {
        return;
    }
    design = civ->design;
    name = mod_unit_name(civ);
    if (!text_copy(elite_name, sizeof elite_name, "Elite ") || !text_append(elite_name, sizeof elite_name, name)) {
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

    if (!CHECK(strings != NULL) || !CHECK(civ != NULL)) {
        return;
    }
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

    if (!CHECK(strings != NULL) || !CHECK(civ != NULL) || !CHECK(vanilla != NULL)) {
        return false;
    }
    design = civ->design;
    alias = design->texts[TEXT_FIELD_NAME];
    strings->length = 0U;
    strings->text[0] = '\0';
    strings->failed = false;
    put_numbered(strings, civ->name_sid, alias);
    if (!text_copy(part, sizeof part, "Click to play as ") || !text_append(part, sizeof part, alias) ||
        !text_append(part, sizeof part, ".")) {
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
