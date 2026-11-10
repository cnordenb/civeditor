#include "mod_text.h"

#include "../core/check.h"
#include "../core/text.h"

static const char BULLET[] = "\xE2\x80\xA2 ";

static const TextField TECH_NAMES[TECH_TIER_COUNT] = { TEXT_FIELD_CASTLE_TECH_NAME, TEXT_FIELD_IMPERIAL_TECH_NAME };

const char *mod_unit_name(const ModCiv *civ)
{
    const CivDesign *design;
    const Catalog *catalog;
    UnitRole role;
    int found;

    if (!CHECK(civ != NULL) || !CHECK(civ->design != NULL)) {
        return "";
    }
    design = civ->design;
    catalog = civ->catalog;
    if (design->texts[TEXT_FIELD_UNIT_NAME][0] != '\0') {
        return design->texts[TEXT_FIELD_UNIT_NAME];
    }
    role = (design->unique_unit.origin == UNIT_ORIGIN_CUSTOM) ? UNIT_ROLE_CUSTOM : UNIT_ROLE_VANILLA;
    found = catalog_find_unit(catalog, role, design->unique_unit.id);
    return (found == CATALOG_MISSING) ? "" : catalog->units[found].name;
}

bool mod_unit_has_own_texts(const CivDesign *design)
{
    bool named;
    bool described;

    if (!CHECK(design != NULL) || !CHECK(design->unique_unit.origin <= UNIT_ORIGIN_CUSTOM)) {
        return false;
    }
    named = design->texts[TEXT_FIELD_UNIT_NAME][0] != '\0';
    described = design->texts[TEXT_FIELD_UNIT_DESCRIPTION][0] != '\0';
    return (design->unique_unit.origin == UNIT_ORIGIN_VANILLA) && (named || described);
}

const char *mod_tech_text(const ModCiv *civ, TechTier tier, bool want_name)
{
    const CivDesign *design;
    const Catalog *catalog;
    int found;

    if (!CHECK(civ != NULL) || !CHECK((tier == TECH_TIER_CASTLE) || (tier == TECH_TIER_IMPERIAL))) {
        return "";
    }
    design = civ->design;
    catalog = civ->catalog;
    if (want_name && (design->texts[TECH_NAMES[tier]][0] != '\0')) {
        return design->texts[TECH_NAMES[tier]];
    }
    found = catalog_find_tech(catalog, tier, design->techs[tier].km_index);
    if (found == CATALOG_MISSING) {
        return "";
    }
    return want_name ? catalog->techs[found].name : catalog->techs[found].description;
}

static const char *bonus_text(const ModCiv *civ, int pick)
{
    const CivDesign *design;
    const Catalog *catalog;
    int found;

    if (!CHECK(civ != NULL) || !CHECK((pick >= 0) && (pick < CIV_BONUS_CAPACITY))) {
        return "";
    }
    design = civ->design;
    catalog = civ->catalog;
    found = catalog_find_bonus(catalog, false, design->bonuses[pick].id);
    return (found == CATALOG_MISSING) ? "" : catalog->bonuses[found].description;
}

static const char *team_text(const ModCiv *civ)
{
    const CivDesign *design;
    const Catalog *catalog;
    int found;

    if (!CHECK(civ != NULL) || !CHECK(civ->design != NULL)) {
        return "";
    }
    design = civ->design;
    catalog = civ->catalog;
    if (design->team_bonus_count == 0) {
        return "";
    }
    found = catalog_find_bonus(catalog, true, design->team_bonuses[0].id);
    return (found == CATALOG_MISSING) ? "" : catalog->team_bonuses[found].description;
}

static const char *head_text(const CivDesign *design)
{
    if (!CHECK(design != NULL) || !CHECK(design->texts[TEXT_FIELD_NAME][0] != '\0')) {
        return "";
    }
    if (design->texts[TEXT_FIELD_TAGLINE][0] != '\0') {
        return design->texts[TEXT_FIELD_TAGLINE];
    }
    if (design->texts[TEXT_FIELD_DESCRIPTION][0] != '\0') {
        return design->texts[TEXT_FIELD_DESCRIPTION];
    }
    return design->texts[TEXT_FIELD_NAME];
}

static bool add_tech_line(const ModCiv *civ, TechTier tier, char *text, size_t capacity)
{
    const char *detail = mod_tech_text(civ, tier, false);

    if (!CHECK(text != NULL) || !CHECK(capacity > 0U)) {
        return false;
    }
    return text_append(text, capacity, "\n") && text_append(text, capacity, BULLET) &&
           text_append(text, capacity, mod_tech_text(civ, tier, true)) &&
           ((detail[0] == '\0') || (text_append(text, capacity, " (") && text_append(text, capacity, detail) &&
                                    text_append(text, capacity, ")")));
}

static bool add_bonus_lines(const ModCiv *civ, char *text, size_t capacity)
{
    const CivDesign *design;
    bool fits = true;
    int pick;

    if (!CHECK(civ != NULL) || !CHECK(text != NULL)) {
        return false;
    }
    design = civ->design;
    for (pick = 0; (pick < design->bonus_count) && (pick < CIV_BONUS_CAPACITY); ++pick) {
        fits = fits && text_append(text, capacity, (pick == 0) ? "" : "\n") && text_append(text, capacity, BULLET) &&
               text_append(text, capacity, bonus_text(civ, pick));
    }
    return fits;
}

static void trim_end(char *text, size_t capacity)
{
    size_t length = text_length(text, capacity);
    size_t step;

    if (!CHECK(text != NULL) || !CHECK(length < capacity)) {
        return;
    }
    for (step = 0U; (step < (size_t)MOD_TEXT_CAPACITY) && (length > 0U); ++step) {
        if ((text[length - 1U] != '\n') && (text[length - 1U] != ' ') && (text[length - 1U] != '\r')) {
            return;
        }
        --length;
        text[length] = '\0';
    }
}

bool mod_description(const ModCiv *civ, char *text, size_t capacity)
{
    bool fits;

    if (!CHECK(civ != NULL) || !CHECK(text != NULL) || !CHECK(capacity > 0U)) {
        return false;
    }
    text[0] = '\0';
    fits = text_append(text, capacity, head_text(civ->design)) && text_append(text, capacity, " civilization\n\n") &&
           add_bonus_lines(civ, text, capacity) && text_append(text, capacity, "\n\n<b>Unique Unit:<b> \n") &&
           text_append(text, capacity, mod_unit_name(civ)) &&
           text_append(text, capacity, "\n\n<b>Unique Techs:<b> ") &&
           add_tech_line(civ, TECH_TIER_CASTLE, text, capacity) &&
           add_tech_line(civ, TECH_TIER_IMPERIAL, text, capacity) &&
           text_append(text, capacity, "\n\n<b>Team Bonus:<b> \n") &&
           text_append(text, capacity, (team_text(civ)[0] != '\0') ? team_text(civ) : "None");
    trim_end(text, capacity);
    return fits;
}

static bool tip_of_tech(const ModCiv *civ, TechTier tier, char *text, size_t capacity)
{
    const char *detail = mod_tech_text(civ, tier, false);

    if (!CHECK(text != NULL) || !CHECK(capacity > 0U)) {
        return false;
    }
    return text_copy(text, capacity, "Research ") && text_append(text, capacity, mod_tech_text(civ, tier, true)) &&
           text_append(text, capacity, (tier == TECH_TIER_CASTLE) ? " in your Castle"
                                                                  : " in your Castle in the Imperial Age") &&
           ((detail[0] == '\0') || (text_append(text, capacity, ": ") && text_append(text, capacity, detail)));
}

static bool tip_text(const ModCiv *civ, int index, char *text, size_t capacity)
{
    const CivDesign *design;

    if (!CHECK(civ != NULL) || !CHECK(text != NULL)) {
        return false;
    }
    design = civ->design;
    if ((index == 0) && (design->bonus_count > 0)) {
        return text_copy(text, capacity, "Civilization bonus: ") && text_append(text, capacity, bonus_text(civ, 0));
    }
    if ((index == 0) && (team_text(civ)[0] != '\0')) {
        return text_copy(text, capacity, "Team bonus: ") && text_append(text, capacity, team_text(civ));
    }
    if (index <= 1) {
        return text_copy(text, capacity, "Your unique unit is the ") && text_append(text, capacity, mod_unit_name(civ));
    }
    return tip_of_tech(civ, (index == 2) ? TECH_TIER_CASTLE : TECH_TIER_IMPERIAL, text, capacity);
}

bool mod_tip(const ModCiv *civ, int index, char *text, size_t capacity)
{
    size_t length;
    size_t at;

    if (!CHECK((index >= 0) && (index < MOD_TIP_COUNT)) || !CHECK(capacity > 1U) ||
        !tip_text(civ, index, text, capacity)) {
        return false;
    }
    for (at = 0U; (at < (size_t)MOD_TEXT_CAPACITY) && (text[at] != '\0'); ++at) {
        text[at] = ((text[at] == '\n') || (text[at] == '\r')) ? ' ' : text[at];
    }
    trim_end(text, capacity);
    length = text_length(text, capacity);
    if ((length > 0U) && (text[length - 1U] != '.') && (text[length - 1U] != '!') && (text[length - 1U] != '?')) {
        return text_append(text, capacity, ".");
    }
    return true;
}

long mod_description_sid(int slot)
{
    if (!CHECK(slot >= 0) || !CHECK(slot < MOD_OWN_DESCRIPTION_ROOM)) {
        return -1L;
    }
    return (long)MOD_OWN_DESCRIPTION_BASE + (long)slot;
}
