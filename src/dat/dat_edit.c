#include "dat_edit.h"

#include <limits.h>
#include <string.h>

#include "../core/check.h"
#include "dat_parts.h"

static bool map_is_in_order(const DatMap *map)
{
    return CHECK(map->effect_count_at + DAT_LONG_WIDTH <= map->unit_headers_at) &&
           CHECK(map->unit_headers_at <= map->tech_count_at) &&
           CHECK(map->tech_count_at + DAT_SHORT_WIDTH <= map->tail_at);
}

bool dat_edit_start(DatEdit *edit, const DatMap *map)
{
    if (!map_is_in_order(map)) {
        return false;
    }
    edit->effect_count_at = map->effect_count_at;
    edit->old_effect_count = map->effect_count;
    edit->effects_end_at = map->unit_headers_at;
    edit->tech_count_at = map->tech_count_at;
    edit->old_tech_count = map->tech_count;
    edit->techs_end_at = map->tail_at;
    edit->patch_count = 0;
    edit->copy_count = 0;
    edit->effects_length = 0U;
    edit->effect_count = 0;
    edit->change_count_at = 0U;
    edit->change_count = 0;
    edit->techs_length = 0U;
    edit->tech_count = 0;
    return true;
}

static bool is_clear_of(size_t at, size_t end, size_t mark, size_t width)
{
    return (end <= mark) || (at >= mark + width);
}

static bool counts_are_clear(const DatEdit *edit, size_t at, size_t end)
{
    return CHECK(is_clear_of(at, end, edit->effect_count_at, DAT_LONG_WIDTH)) &&
           CHECK(is_clear_of(at, end, edit->tech_count_at, DAT_SHORT_WIDTH)) &&
           CHECK(is_clear_of(at, end, edit->effects_end_at, 0U)) && CHECK(is_clear_of(at, end, edit->techs_end_at, 0U));
}

static bool range_fits(const DatEdit *edit, size_t at, size_t width)
{
    return counts_are_clear(edit, at, at + width);
}

bool dat_edit_patch(DatEdit *edit, const DatPatch *patch)
{
    if (!CHECK(edit->patch_count < DAT_PATCH_CAPACITY) || !range_fits(edit, patch->at, patch->width)) {
        return false;
    }
    edit->patches[edit->patch_count] = *patch;
    ++edit->patch_count;
    return true;
}

bool dat_edit_is_patched(const DatEdit *edit, size_t at)
{
    int index;

    for (index = 0; index < edit->patch_count; ++index) {
        if (edit->patches[index].at == at) {
            return true;
        }
    }
    return false;
}

static bool copy_fits(const DatEdit *edit, const DatCopy *copy)
{
    return CHECK(edit->copy_count < DAT_COPY_CAPACITY) && range_fits(edit, copy->at, copy->width) &&
           range_fits(edit, copy->from, copy->width);
}

bool dat_edit_copy(DatEdit *edit, const DatCopy *copy)
{
    if (!copy_fits(edit, copy)) {
        return false;
    }
    edit->copies[edit->copy_count] = *copy;
    ++edit->copy_count;
    return true;
}

bool dat_edit_patch_u8(DatEdit *edit, size_t at, int value)
{
    DatPatch patch = { 0U, 1U, { 0U } };

    if (!CHECK((value >= 0) && (value <= UCHAR_MAX))) {
        return false;
    }
    patch.at = at;
    patch.bytes[0] = (unsigned char)value;
    return dat_edit_patch(edit, &patch);
}

bool dat_edit_patch_sunk(DatEdit *edit, size_t at, DatPatch *patch, const DatSink *sink)
{
    if (!CHECK(sink->fine)) {
        return false;
    }
    patch->at = at;
    patch->width = sink->length;
    return dat_edit_patch(edit, patch);
}

static bool patch_whole(DatEdit *edit, size_t at, long value, size_t width)
{
    DatPatch patch = { 0U, 0U, { 0U } };
    DatSink sink = dat_sink_over(patch.bytes, sizeof patch.bytes, 0U);

    if (!CHECK((width == DAT_SHORT_WIDTH) || (width == DAT_LONG_WIDTH))) {
        return false;
    }
    if (width == DAT_SHORT_WIDTH) {
        dat_sink_i16(&sink, (int)value);
    } else {
        dat_sink_i32(&sink, value);
    }
    return dat_edit_patch_sunk(edit, at, &patch, &sink);
}

bool dat_edit_patch_i16(DatEdit *edit, size_t at, int value)
{
    return patch_whole(edit, at, value, DAT_SHORT_WIDTH);
}

bool dat_edit_patch_i32(DatEdit *edit, size_t at, long value)
{
    return patch_whole(edit, at, value, DAT_LONG_WIDTH);
}

bool dat_edit_patch_f32(DatEdit *edit, size_t at, float value)
{
    DatPatch patch = { 0U, 0U, { 0U } };
    DatSink sink = dat_sink_over(patch.bytes, sizeof patch.bytes, 0U);

    dat_sink_f32(&sink, value);
    return dat_edit_patch_sunk(edit, at, &patch, &sink);
}

int dat_edit_add_effect(DatEdit *edit, const char *name)
{
    DatSink sink;
    size_t count_at;

    if (!CHECK(edit->old_effect_count + edit->effect_count < DAT_EFFECT_CAPACITY)) {
        return DAT_NO_ID;
    }
    sink = dat_sink_over(edit->effects, DAT_ADDED_EFFECTS_CAPACITY, edit->effects_length);
    dat_sink_text(&sink, name);
    count_at = sink.length;
    dat_sink_i16(&sink, 0);
    if (!CHECK(sink.fine)) {
        return DAT_NO_ID;
    }
    edit->effects_length = sink.length;
    edit->change_count_at = count_at;
    edit->change_count = 0;
    ++edit->effect_count;
    return edit->old_effect_count + edit->effect_count - 1;
}

static bool count_change(DatEdit *edit)
{
    DatSink count = dat_sink_over(edit->effects, DAT_ADDED_EFFECTS_CAPACITY, edit->change_count_at);

    dat_sink_i16(&count, edit->change_count + 1);
    return CHECK(count.fine);
}

bool dat_edit_add_change(DatEdit *edit, const DatChange *change)
{
    DatSink sink;

    sink = dat_sink_over(edit->effects, DAT_ADDED_EFFECTS_CAPACITY, edit->effects_length);
    dat_sink_u8(&sink, change->type);
    dat_sink_i16(&sink, change->a);
    dat_sink_i16(&sink, change->b);
    dat_sink_i16(&sink, change->c);
    dat_sink_f32(&sink, change->d);
    if (!CHECK(sink.fine) || !count_change(edit)) {
        return false;
    }
    edit->effects_length = sink.length;
    ++edit->change_count;
    return true;
}

static void sink_tech_head(DatSink *sink, const DatTech *tech)
{
    int slot;

    for (slot = 0; slot < DAT_REQUIRED_TECH_SLOTS; ++slot) {
        dat_sink_i16(sink, tech->required_techs[slot]);
    }
    for (slot = 0; slot < DAT_TECH_COST_SLOTS; ++slot) {
        dat_sink_i16(sink, tech->costs[slot].type);
        dat_sink_i16(sink, tech->costs[slot].amount);
        dat_sink_u8(sink, tech->costs[slot].flag);
    }
    dat_sink_i16(sink, tech->required_tech_count);
    dat_sink_i16(sink, tech->civ);
    dat_sink_i16(sink, tech->full_tech_mode);
    dat_sink_i32(sink, tech->language_dll_name);
    dat_sink_i32(sink, tech->language_dll_description);
    dat_sink_i16(sink, tech->effect_id);
    dat_sink_i16(sink, tech->type);
    dat_sink_i16(sink, tech->icon_id);
    dat_sink_i32(sink, tech->language_dll_help);
    dat_sink_i32(sink, tech->language_dll_tech_tree);
}

static void sink_research_location(DatSink *sink, const DatResearchLocation *location)
{
    dat_sink_i16(sink, location->location_id);
    dat_sink_i16(sink, location->research_time);
    dat_sink_u8(sink, location->button_id);
    dat_sink_i32(sink, location->hot_key_id);
}

static bool tech_fits(const DatEdit *edit, const DatTech *tech)
{
    return CHECK(edit->old_tech_count + edit->tech_count < DAT_TECH_CAPACITY) &&
           CHECK(tech->research_location_count >= 0) &&
           CHECK(tech->research_location_count <= DAT_RESEARCH_LOCATION_CAPACITY);
}

int dat_edit_add_tech(DatEdit *edit, const DatTech *tech)
{
    DatSink sink;
    int location;

    if (!tech_fits(edit, tech)) {
        return DAT_NO_ID;
    }
    sink = dat_sink_over(edit->techs, DAT_ADDED_TECHS_CAPACITY, edit->techs_length);
    sink_tech_head(&sink, tech);
    dat_sink_text(&sink, tech->name);
    dat_sink_u8(&sink, tech->repeatable);
    dat_sink_i16(&sink, tech->research_location_count);
    for (location = 0; (location < tech->research_location_count) && (location < DAT_RESEARCH_LOCATION_CAPACITY);
         ++location) {
        sink_research_location(&sink, &tech->research_locations[location]);
    }
    if (!CHECK(sink.fine)) {
        return DAT_NO_ID;
    }
    edit->techs_length = sink.length;
    ++edit->tech_count;
    return edit->old_tech_count + edit->tech_count - 1;
}

static bool counts_match(const DatEdit *edit, DatCursor *effects, DatCursor *techs)
{
    return CHECK(dat_cursor_i32(effects) == edit->old_effect_count) &&
           CHECK(dat_cursor_i16(techs) == edit->old_tech_count) && CHECK(effects->fine) && CHECK(techs->fine);
}

static bool edit_fits(const DatEdit *edit, const DatBytes *original)
{
    DatCursor effects;
    DatCursor techs;

    if (!dat_cursor_start(&effects, original, edit->effect_count_at) ||
        !dat_cursor_start(&techs, original, edit->tech_count_at)) {
        return false;
    }
    return counts_match(edit, &effects, &techs);
}

static bool output_append(DatOutput *output, const unsigned char *from, size_t length)
{
    unsigned char *data;

    if (!CHECK(length <= output->capacity - output->length)) {
        return false;
    }
    data = output->data;
    (void)memcpy(&data[output->length], from, length);
    output->length += length;
    return true;
}

static bool output_pieces(const DatEdit *edit, const DatBytes *original, DatOutput *output)
{
    const unsigned char *data;

    data = original->data;
    output->length = 0U;
    return output_append(output, data, edit->effects_end_at) &&
           output_append(output, edit->effects, edit->effects_length) &&
           output_append(output, &data[edit->effects_end_at], edit->techs_end_at - edit->effects_end_at) &&
           output_append(output, edit->techs, edit->techs_length) &&
           output_append(output, &data[edit->techs_end_at], original->size - edit->techs_end_at);
}

static size_t shifted_at(const DatEdit *edit, size_t at)
{
    size_t shift = 0U;

    shift += (at >= edit->effects_end_at) ? edit->effects_length : 0U;
    shift += (at >= edit->techs_end_at) ? edit->techs_length : 0U;
    return at + shift;
}

static bool output_counts(const DatEdit *edit, DatOutput *output)
{
    DatSink effects;
    DatSink techs;

    effects = dat_sink_over(output->data, output->length, edit->effect_count_at);
    dat_sink_i32(&effects, (long)edit->old_effect_count + (long)edit->effect_count);
    techs = dat_sink_over(output->data, output->length, shifted_at(edit, edit->tech_count_at));
    dat_sink_i16(&techs, edit->old_tech_count + edit->tech_count);
    return CHECK(effects.fine) && CHECK(techs.fine);
}

static bool patch_lands(const DatOutput *output, const DatPatch *patch, size_t at)
{
    return CHECK(at <= output->length) && CHECK(patch->width <= output->length - at);
}

static bool output_patch(DatOutput *output, const DatPatch *patch, size_t at)
{
    if (!patch_lands(output, patch, at)) {
        return false;
    }
    (void)memcpy(&output->data[at], patch->bytes, patch->width);
    return true;
}

static bool output_patches(const DatEdit *edit, DatOutput *output)
{
    int index;

    for (index = 0; index < edit->patch_count; ++index) {
        if (!output_patch(output, &edit->patches[index], shifted_at(edit, edit->patches[index].at))) {
            return false;
        }
    }
    return true;
}

static bool copy_lands(const DatCopy *copy, const DatBytes *original, const DatOutput *output, size_t at)
{
    return CHECK(copy->from <= original->size) && CHECK(copy->width <= original->size - copy->from) &&
           CHECK(at <= output->length) && CHECK(copy->width <= output->length - at);
}

static bool output_copies(const DatEdit *edit, const DatBytes *original, DatOutput *output)
{
    const DatCopy *copy;
    size_t at;
    int index;

    for (index = 0; (index < edit->copy_count) && (index < DAT_COPY_CAPACITY); ++index) {
        copy = &edit->copies[index];
        at = shifted_at(edit, copy->at);
        if (!copy_lands(copy, original, output, at)) {
            return false;
        }
        (void)memcpy(&output->data[at], &original->data[copy->from], copy->width);
    }
    return true;
}

static bool output_all(const DatEdit *edit, const DatBytes *original, DatOutput *output)
{
    return output_pieces(edit, original, output) && output_counts(edit, output) &&
           output_copies(edit, original, output) && output_patches(edit, output);
}

bool dat_edit_write(const DatEdit *edit, const DatBytes *original, DatOutput *output)
{
    if (!edit_fits(edit, original)) {
        return false;
    }
    if (output_all(edit, original, output)) {
        return true;
    }
    output->length = 0U;
    return false;
}
