#include "civ_file.h"

#include "../core/file_io.h"
#include "../core/text.h"

const char CIV_FILE_TREE_KEYS[TREE_KIND_COUNT][CIV_FILE_KEY_WIDTH] = { "units", "buildings", "techs" };
const char CIV_FILE_RESOURCE_KEYS[RESOURCE_KIND_COUNT][CIV_FILE_KEY_WIDTH] = { "food", "wood", "stone", "gold" };

static void write_optional(JsonWriter *writer, const char *key, int value)
{
    if (value == CIV_NO_CHOICE) {
        json_write_null(writer, key);
    } else {
        json_write_number(writer, key, value);
    }
}

static int civ_index_of_slot(int slot)
{
    return (slot > 0) ? slot - 1 : CIV_NO_CHOICE;
}

static void write_empty(JsonWriter *writer, const char *key, const char *brackets)
{
    json_begin(writer, key, brackets[0]);
    json_end(writer, brackets[1]);
}

static void write_identity(const CivDesign *design, JsonWriter *writer)
{
    json_write_string(writer, "format", "empireforge_v2");
    json_write_number(writer, "schema_version", CIV_FILE_SCHEMA_VERSION);
    json_write_number(writer, "_draftVer", CIV_FILE_DRAFT_VERSION);
    json_write_string(writer, "alias", design->texts[TEXT_FIELD_NAME]);
    json_write_string(writer, "tagline", design->texts[TEXT_FIELD_TAGLINE]);
    json_write_string(writer, "description", design->texts[TEXT_FIELD_DESCRIPTION]);
    json_write_string(writer, "share_description", "");
    json_write_number(writer, "architecture", design->choices[CHOICE_ARCHITECTURE]);
    json_write_number(writer, "language", civ_index_of_slot(design->choices[CHOICE_LANGUAGE]));
    json_write_number(writer, "wonder_model", civ_index_of_slot(design->choices[CHOICE_WONDER]));
    json_write_number(writer, "castle_model", civ_index_of_slot(design->choices[CHOICE_CASTLE]));
    json_write_number(writer, "wonder", civ_index_of_slot(design->choices[CHOICE_WONDER]));
    json_write_number(writer, "castle", civ_index_of_slot(design->choices[CHOICE_CASTLE]));
    json_write_string(writer, "emblem", "");
}

static void write_hero(const CivDesign *design, JsonWriter *writer)
{
    if (design->hero_unit == CIV_NO_CHOICE) {
        json_write_null(writer, "hero_unit");
        return;
    }
    json_begin(writer, "hero_unit", '{');
    json_write_number(writer, "base_unit_id", design->hero_unit);
    json_write_string(writer, "name", design->texts[TEXT_FIELD_HERO_NAME]);
    json_write_string(writer, "description", "");
    write_empty(writer, "overrides", "{}");
    write_empty(writer, "flags", "{}");
    json_end(writer, '}');
}

static void write_unique_unit(const CivDesign *design, JsonWriter *writer)
{
    UnitOrigin origin;

    origin = design->unique_unit.origin;
    json_begin(writer, "unique_unit", '{');
    write_optional(writer, "km_idx", (origin == UNIT_ORIGIN_CUSTOM) ? design->unique_unit.id : CIV_NO_CHOICE);
    write_optional(writer, "vanilla_id", (origin == UNIT_ORIGIN_VANILLA) ? design->unique_unit.id : CIV_NO_CHOICE);
    json_write_string(writer, "name", design->texts[TEXT_FIELD_UNIT_NAME]);
    json_write_string(writer, "description", design->texts[TEXT_FIELD_UNIT_DESCRIPTION]);
    write_empty(writer, "overrides", "{}");
    write_empty(writer, "advanced_flags", "{}");
    json_end(writer, '}');
}

static void write_bonus_list(JsonWriter *writer, const char *key, const BonusPick *picks, int count)
{
    int index;

    json_begin(writer, key, '[');
    for (index = 0; (index < count) && (index < CIV_BONUS_CAPACITY); ++index) {
        json_begin(writer, NULL, '{');
        json_write_number(writer, "id", picks[index].id);
        json_write_number(writer, "multiplier", picks[index].multiplier);
        json_end(writer, '}');
    }
    json_end(writer, ']');
}

static void write_tech(JsonWriter *writer, const char *key, const TechPick *tech, const char *name)
{
    int resource;

    json_begin(writer, key, '{');
    json_write_string(writer, "mode", "vanilla");
    write_optional(writer, "vanilla_km_idx", tech->km_index);
    json_write_string(writer, "name", name);
    json_write_string(writer, "description", "");
    json_begin(writer, "cost", '{');
    for (resource = 0; resource < RESOURCE_KIND_COUNT; ++resource) {
        json_write_number(writer, CIV_FILE_RESOURCE_KEYS[resource], tech->cost[resource]);
    }
    json_end(writer, '}');
    json_write_number(writer, "time", tech->time);
    write_empty(writer, "effects", "[]");
    write_empty(writer, "custom_bonuses", "[]");
    json_end(writer, '}');
}

static void write_tree(const CivDesign *design, JsonWriter *writer)
{
    int kind;
    int index;

    json_begin(writer, "tree", '{');
    for (kind = 0; kind < (int)TREE_KIND_COUNT; ++kind) {
        json_begin(writer, CIV_FILE_TREE_KEYS[kind], '[');
        for (index = 0; (index < design->tree[kind].count) && (index < CIV_TREE_CAPACITY); ++index) {
            json_write_number(writer, NULL, design->tree[kind].ids[index]);
        }
        json_end(writer, ']');
    }
    json_end(writer, '}');
}

static void write_trailer(const CivDesign *design, JsonWriter *writer)
{
    json_write_null(writer, "long_range_ship");
    json_write_bool(writer, "long_range_ship_elite", true);
    write_optional(writer, "starting_scout", design->choices[CHOICE_SCOUT]);
    write_optional(writer, "monk_skin", design->choices[CHOICE_MONK]);
    json_write_null(writer, "monastery_skin_building");
    write_empty(writer, "unit_overrides", "[]");
    write_empty(writer, "button_moves", "[]");
    write_empty(writer, "free_techs", "[]");
    json_write_number(writer, "replace_civ", design->choices[CHOICE_REPLACE]);
    json_write_number(writer, "base_civ", design->choices[CHOICE_BASE]);
}

bool civ_file_compose(const CivDesign *design, JsonWriter *writer)
{
    json_writer_start(writer);
    json_begin(writer, NULL, '{');
    write_identity(design, writer);
    write_hero(design, writer);
    write_unique_unit(design, writer);
    json_write_null(writer, "second_uu");
    write_bonus_list(writer, "bonuses", design->bonuses, design->bonus_count);
    write_empty(writer, "custom_bonuses", "[]");
    write_bonus_list(writer, "team_bonuses", design->team_bonuses, design->team_bonus_count);
    write_empty(writer, "custom_team_bonuses", "[]");
    write_tech(writer, "castle_ut", &design->techs[TECH_TIER_CASTLE], design->texts[TEXT_FIELD_CASTLE_TECH_NAME]);
    write_tech(writer, "imperial_ut", &design->techs[TECH_TIER_IMPERIAL],
               design->texts[TEXT_FIELD_IMPERIAL_TECH_NAME]);
    write_tree(design, writer);
    write_trailer(design, writer);
    json_end(writer, '}');
    return json_writer_succeeded(writer);
}

bool civ_file_save(const CivDesign *design, const wchar_t *path)
{
    static JsonWriter writer;

    if (!civ_file_compose(design, &writer)) {
        return false;
    }
    return file_write_all(path, writer.data, writer.length);
}
