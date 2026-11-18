#include "civ_file.h"

#include "../core/check.h"
#include "../core/file_io.h"
#include "../core/text.h"

enum { FORMAT_CAPACITY = 32 };


static int read_number_or(const JsonDocument *document, int object, const char *key, int fallback)
{
    int token = json_member(document, object, key);
    TextNumber number;

    if (token == JSON_NONE) {
        return fallback;
    }
    number = json_number(document, token);
    return number.valid ? (int)number.value : fallback;
}

static int slot_of_civ_index(int civ_index)
{
    return (civ_index >= 0) ? civ_index + 1 : CIV_NO_CHOICE;
}

static void read_text(const JsonDocument *document, int object, const char *key, CivDesign *design,
                      TextField field)
{
    static char buffer[CIV_DESCRIPTION_CAPACITY];
    int token = json_member(document, object, key);

    if (token == JSON_NONE) {
        return;
    }
    if (json_copy_string(document, token, buffer, sizeof buffer)) {
        (void)civ_design_set_text(design, field, buffer);
    }
}

static bool has_known_format(const JsonDocument *document)
{
    char format[FORMAT_CAPACITY] = "";
    int token = json_member(document, 0, "format");

    if (token == JSON_NONE) {
        return false;
    }
    if (!json_copy_string(document, token, format, sizeof format)) {
        return false;
    }
    return text_equal(format, "empireforge_v2") || text_equal(format, "civbuilder_v1");
}

static void read_choices(const JsonDocument *document, CivDesign *design)
{
    int wonder;
    int castle;

    wonder = read_number_or(document, 0, "wonder", read_number_or(document, 0, "wonder_model", CIV_NO_CHOICE));
    castle = read_number_or(document, 0, "castle", read_number_or(document, 0, "castle_model", CIV_NO_CHOICE));
    (void)civ_design_set_choice(design, CHOICE_ARCHITECTURE,
                                read_number_or(document, 0, "architecture", CIV_DEFAULT_ARCHITECTURE));
    (void)civ_design_set_choice(design, CHOICE_LANGUAGE, slot_of_civ_index(read_number_or(document, 0, "language", 0)));
    (void)civ_design_set_choice(design, CHOICE_WONDER, slot_of_civ_index(wonder));
    (void)civ_design_set_choice(design, CHOICE_CASTLE, slot_of_civ_index(castle));
    (void)civ_design_set_choice(design, CHOICE_MONK, read_number_or(document, 0, "monk_skin", CIV_NO_CHOICE));
    (void)civ_design_set_choice(design, CHOICE_SCOUT, read_number_or(document, 0, "starting_scout", CIV_NO_CHOICE));
    (void)civ_design_set_choice(design, CHOICE_REPLACE,
                                read_number_or(document, 0, "replace_civ", CIV_DEFAULT_REPLACE));
}

static void read_bonuses(const JsonDocument *document, const char *key, CivDesign *design, bool team)
{
    int list = json_member(document, 0, key);
    int item = json_first_child(document, list);
    int step;
    int bonus;

    for (step = 0; (step < CIV_BONUS_CAPACITY) && (item != JSON_NONE); ++step) {
        bonus = read_number_or(document, item, "id", CIV_NO_CHOICE);
        if ((bonus != CIV_NO_CHOICE) && team) {
            (void)civ_design_toggle_team_bonus(design, bonus);
        } else if ((bonus != CIV_NO_CHOICE) && !civ_design_has_bonus(design, bonus)) {
            (void)civ_design_toggle_bonus(design, bonus);
        }
        civ_design_set_multiplier(design, team, bonus, read_number_or(document, item, "multiplier", 1));
        item = json_next_sibling(document, list, item);
    }
}

static void read_tree(const JsonDocument *document, CivDesign *design)
{
    int tree = json_member(document, 0, "tree");
    int kind;
    int list;
    int item;
    int step;
    TextNumber number;

    for (kind = 0; kind < (int)TREE_KIND_COUNT; ++kind) {
        list = json_member(document, tree, CIV_FILE_TREE_KEYS[kind]);
        item = json_first_child(document, list);
        for (step = 0; (step < CIV_TREE_CAPACITY) && (item != JSON_NONE); ++step) {
            number = json_number(document, item);
            if (number.valid && !civ_design_tree_has(design, (TreeKind)kind, (int)number.value)) {
                (void)civ_design_toggle_tree(design, (TreeKind)kind, (int)number.value);
            }
            item = json_next_sibling(document, list, item);
        }
    }
}

static void read_unique_unit(const JsonDocument *document, CivDesign *design)
{
    int unit = json_member(document, 0, "unique_unit");
    int custom = read_number_or(document, unit, "km_idx", CIV_NO_CHOICE);
    int vanilla = read_number_or(document, unit, "vanilla_id", CIV_NO_CHOICE);

    if (vanilla != CIV_NO_CHOICE) {
        (void)civ_design_pick_unit(design, UNIT_ORIGIN_VANILLA, vanilla);
    } else if (custom != CIV_NO_CHOICE) {
        (void)civ_design_pick_unit(design, UNIT_ORIGIN_CUSTOM, custom);
    }
    read_text(document, unit, "name", design, TEXT_FIELD_UNIT_NAME);
    read_text(document, unit, "description", design, TEXT_FIELD_UNIT_DESCRIPTION);
}

static void read_tech(const JsonDocument *document, const char *key, CivDesign *design, TechTier tier)
{
    int tech = json_member(document, 0, key);
    int cost = json_member(document, tech, "cost");
    int resource;

    if (!CHECK((tier == TECH_TIER_CASTLE) || (tier == TECH_TIER_IMPERIAL))) {
        return;
    }
    (void)civ_design_pick_tech(design, tier, read_number_or(document, tech, "vanilla_km_idx", CIV_NO_CHOICE));
    for (resource = 0; resource < RESOURCE_KIND_COUNT; ++resource) {
        design->techs[tier].cost[resource] = read_number_or(document, cost, CIV_FILE_RESOURCE_KEYS[resource], 0);
    }
    design->techs[tier].time = read_number_or(document, tech, "time", 0);
    read_text(document, tech, "name", design,
              (tier == TECH_TIER_CASTLE) ? TEXT_FIELD_CASTLE_TECH_NAME : TEXT_FIELD_IMPERIAL_TECH_NAME);
}

static void read_hero(const JsonDocument *document, CivDesign *design)
{
    int hero = json_member(document, 0, "hero_unit");

    (void)civ_design_pick_hero(design, read_number_or(document, hero, "base_unit_id", CIV_NO_CHOICE));
    read_text(document, hero, "name", design, TEXT_FIELD_HERO_NAME);
}

bool civ_file_interpret(const JsonDocument *document, CivDesign *design)
{
    if (!has_known_format(document)) {
        return false;
    }
    civ_design_reset(design);
    read_text(document, 0, "alias", design, TEXT_FIELD_NAME);
    read_text(document, 0, "tagline", design, TEXT_FIELD_TAGLINE);
    read_text(document, 0, "description", design, TEXT_FIELD_DESCRIPTION);
    read_choices(document, design);
    read_bonuses(document, "bonuses", design, false);
    read_bonuses(document, "team_bonuses", design, true);
    read_tree(document, design);
    read_unique_unit(document, design);
    read_tech(document, "castle_ut", design, TECH_TIER_CASTLE);
    read_tech(document, "imperial_ut", design, TECH_TIER_IMPERIAL);
    read_hero(document, design);
    return true;
}

bool civ_file_load(CivDesign *design, const wchar_t *path)
{
    static JsonDocument document;
    FileRead read;

    read = file_read_all(path, document.text, sizeof document.text);
    if (!read.succeeded) {
        return false;
    }
    document.length = (int)read.length;
    return json_parse(&document) && civ_file_interpret(&document, design);
}
