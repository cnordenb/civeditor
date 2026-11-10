#include "dat_edit.h"

#include <limits.h>
#include <stdint.h>
#include <string.h>

#include "../core/check.h"
#include "dat_parts.h"

bool dat_edit_start(DatEdit *edit, const DatMap *map)
{
    if (!CHECK(edit != NULL) || !CHECK(map != NULL) ||
        !CHECK(map->effect_count_at + DAT_LONG_WIDTH <= map->unit_headers_at) ||
        !CHECK(map->unit_headers_at <= map->tech_count_at) ||
        !CHECK(map->tech_count_at + DAT_SHORT_WIDTH <= map->tail_at)) {
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

static bool range_fits(const DatEdit *edit, size_t at, size_t width)
{
    size_t end;

    if (!CHECK(edit != NULL) || !CHECK(width > 0U) || !CHECK(at <= SIZE_MAX - width)) {
        return false;
    }
    end = at + width;
    return CHECK((end <= edit->effect_count_at) || (at >= edit->effect_count_at + DAT_LONG_WIDTH)) &&
           CHECK((end <= edit->tech_count_at) || (at >= edit->tech_count_at + DAT_SHORT_WIDTH)) &&
           CHECK((end <= edit->effects_end_at) || (at >= edit->effects_end_at)) &&
           CHECK((end <= edit->techs_end_at) || (at >= edit->techs_end_at));
}

bool dat_edit_patch(DatEdit *edit, const DatPatch *patch)
{
    if (!CHECK(edit != NULL) || !CHECK(patch != NULL) || !CHECK(edit->patch_count < DAT_PATCH_CAPACITY) ||
        !CHECK(patch->width <= DAT_PATCH_WIDTH_LIMIT) || !range_fits(edit, patch->at, patch->width)) {
        return false;
    }
    edit->patches[edit->patch_count] = *patch;
    ++edit->patch_count;
    return true;
}

bool dat_edit_copy(DatEdit *edit, const DatCopy *copy)
{
    if (!CHECK(edit != NULL) || !CHECK(copy != NULL) || !CHECK(edit->copy_count < DAT_COPY_CAPACITY) ||
        !range_fits(edit, copy->at, copy->width) || !range_fits(edit, copy->from, copy->width)) {
        return false;
    }
    edit->copies[edit->copy_count] = *copy;
    ++edit->copy_count;
    return true;
}

bool dat_edit_patch_u8(DatEdit *edit, size_t at, int value)
{
    DatPatch patch = { 0U, 1U, { 0U } };

    if (!CHECK(edit != NULL) || !CHECK((value >= 0) && (value <= UCHAR_MAX))) {
        return false;
    }
    patch.at = at;
    patch.bytes[0] = (unsigned char)value;
    return dat_edit_patch(edit, &patch);
}

bool dat_edit_patch_sunk(DatEdit *edit, size_t at, DatPatch *patch, const DatSink *sink)
{
    if (!CHECK(edit != NULL) || !CHECK(patch != NULL) || !CHECK(sink != NULL) || !CHECK(sink->fine)) {
        return false;
    }
    patch->at = at;
    patch->width = sink->length;
    return dat_edit_patch(edit, patch);
}

bool dat_edit_patch_i16(DatEdit *edit, size_t at, int value)
{
    DatPatch patch = { 0U, 0U, { 0U } };
    DatSink sink = dat_sink_over(patch.bytes, sizeof patch.bytes, 0U);

    dat_sink_i16(&sink, value);
    return dat_edit_patch_sunk(edit, at, &patch, &sink);
}

bool dat_edit_patch_i32(DatEdit *edit, size_t at, long value)
{
    DatPatch patch = { 0U, 0U, { 0U } };
    DatSink sink = dat_sink_over(patch.bytes, sizeof patch.bytes, 0U);

    dat_sink_i32(&sink, value);
    return dat_edit_patch_sunk(edit, at, &patch, &sink);
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

    if (!CHECK(edit != NULL) || !CHECK(name != NULL) ||
        !CHECK(edit->old_effect_count + edit->effect_count < DAT_EFFECT_CAPACITY)) {
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

bool dat_edit_add_change(DatEdit *edit, const DatChange *change)
{
    DatSink sink;
    DatSink count;

    if (!CHECK(edit != NULL) || !CHECK(change != NULL) || !CHECK(edit->effect_count > 0)) {
        return false;
    }
    sink = dat_sink_over(edit->effects, DAT_ADDED_EFFECTS_CAPACITY, edit->effects_length);
    dat_sink_u8(&sink, change->type);
    dat_sink_i16(&sink, change->a);
    dat_sink_i16(&sink, change->b);
    dat_sink_i16(&sink, change->c);
    dat_sink_f32(&sink, change->d);
    if (!CHECK(sink.fine)) {
        return false;
    }
    count = dat_sink_over(edit->effects, DAT_ADDED_EFFECTS_CAPACITY, edit->change_count_at);
    dat_sink_i16(&count, edit->change_count + 1);
    if (!CHECK(count.fine)) {
        return false;
    }
    edit->effects_length = sink.length;
    ++edit->change_count;
    return true;
}

static void sink_tech_head(DatSink *sink, const DatTech *tech)
{
    int slot;

    if (!CHECK(sink != NULL) || !CHECK(tech != NULL)) {
        return;
    }
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
    if (!CHECK(sink != NULL) || !CHECK(location != NULL)) {
        return;
    }
    dat_sink_i16(sink, location->location_id);
    dat_sink_i16(sink, location->research_time);
    dat_sink_u8(sink, location->button_id);
    dat_sink_i32(sink, location->hot_key_id);
}

int dat_edit_add_tech(DatEdit *edit, const DatTech *tech)
{
    DatSink sink;
    int location;

    if (!CHECK(edit != NULL) || !CHECK(tech != NULL) ||
        !CHECK(edit->old_tech_count + edit->tech_count < DAT_TECH_CAPACITY) ||
        !CHECK((tech->research_location_count >= 0) &&
               (tech->research_location_count <= DAT_RESEARCH_LOCATION_CAPACITY))) {
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

static bool edit_fits(const DatEdit *edit, const DatBytes *original)
{
    DatCursor effects;
    DatCursor techs;

    if (!CHECK(edit != NULL) || !CHECK(original != NULL) || !CHECK(edit->techs_end_at <= original->size) ||
        !dat_cursor_start(&effects, original, edit->effect_count_at) ||
        !dat_cursor_start(&techs, original, edit->tech_count_at)) {
        return false;
    }
    return CHECK(dat_cursor_i32(&effects) == edit->old_effect_count) &&
           CHECK(dat_cursor_i16(&techs) == edit->old_tech_count) && CHECK(effects.fine) && CHECK(techs.fine);
}

static bool output_append(DatOutput *output, const unsigned char *from, size_t length)
{
    unsigned char *data;

    if (!CHECK(output != NULL) || !CHECK(from != NULL) || !CHECK(output->length <= output->capacity) ||
        !CHECK(length <= output->capacity - output->length)) {
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

    if (!CHECK(edit != NULL) || !CHECK(original != NULL) || !CHECK(output != NULL)) {
        return false;
    }
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

    if (!CHECK(edit != NULL) || !CHECK(edit->effects_end_at <= edit->techs_end_at)) {
        return at;
    }
    shift += (at >= edit->effects_end_at) ? edit->effects_length : 0U;
    shift += (at >= edit->techs_end_at) ? edit->techs_length : 0U;
    return at + shift;
}

static bool output_counts(const DatEdit *edit, DatOutput *output)
{
    DatSink effects;
    DatSink techs;

    if (!CHECK(edit != NULL) || !CHECK(output != NULL)) {
        return false;
    }
    effects = dat_sink_over(output->data, output->length, edit->effect_count_at);
    dat_sink_i32(&effects, (long)edit->old_effect_count + (long)edit->effect_count);
    techs = dat_sink_over(output->data, output->length, shifted_at(edit, edit->tech_count_at));
    dat_sink_i16(&techs, edit->old_tech_count + edit->tech_count);
    return CHECK(effects.fine) && CHECK(techs.fine);
}

static bool output_patch(DatOutput *output, const DatPatch *patch, size_t at)
{
    unsigned char *data;

    if (!CHECK(output != NULL) || !CHECK(patch != NULL) || !CHECK(patch->width <= DAT_PATCH_WIDTH_LIMIT) ||
        !CHECK(at <= output->length) || !CHECK(patch->width <= output->length - at)) {
        return false;
    }
    data = output->data;
    (void)memcpy(&data[at], patch->bytes, patch->width);
    return true;
}

static bool output_patches(const DatEdit *edit, DatOutput *output)
{
    int index;

    if (!CHECK(edit != NULL) || !CHECK(edit->patch_count <= DAT_PATCH_CAPACITY)) {
        return false;
    }
    for (index = 0; (index < edit->patch_count) && (index < DAT_PATCH_CAPACITY); ++index) {
        if (!output_patch(output, &edit->patches[index], shifted_at(edit, edit->patches[index].at))) {
            return false;
        }
    }
    return true;
}

static bool output_copies(const DatEdit *edit, const DatBytes *original, DatOutput *output)
{
    const unsigned char *from;
    unsigned char *data;
    const DatCopy *copy;
    size_t at;
    int index;

    if (!CHECK(edit != NULL) || !CHECK(original != NULL) || !CHECK(output != NULL)) {
        return false;
    }
    from = original->data;
    data = output->data;
    for (index = 0; (index < edit->copy_count) && (index < DAT_COPY_CAPACITY); ++index) {
        copy = &edit->copies[index];
        at = shifted_at(edit, copy->at);
        if (!CHECK(copy->from <= original->size) || !CHECK(copy->width <= original->size - copy->from) ||
            !CHECK(at <= output->length) || !CHECK(copy->width <= output->length - at)) {
            return false;
        }
        (void)memcpy(&data[at], &from[copy->from], copy->width);
    }
    return true;
}

bool dat_edit_write(const DatEdit *edit, const DatBytes *original, DatOutput *output)
{
    if (!CHECK(output != NULL) || !CHECK(output->data != NULL) || !edit_fits(edit, original)) {
        return false;
    }
    if (output_pieces(edit, original, output) && output_counts(edit, output) &&
        output_copies(edit, original, output) && output_patches(edit, output)) {
        return true;
    }
    output->length = 0U;
    return false;
}
