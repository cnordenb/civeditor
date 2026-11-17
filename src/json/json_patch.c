#include "json_patch.h"

#include <string.h>

#include "../core/check.h"
#include "../core/text.h"

void json_patch_start(JsonPatch *patch, const JsonDocument *document)
{
    patch->document = document;
    patch->count = 0;
}

static void span_of(const JsonDocument *document, int token, JsonEdit *edit)
{
    int quote;

    quote = json_is_kind(document, token, JSON_KIND_STRING) ? 1 : 0;
    edit->start = document->tokens[token].start - quote;
    edit->end = document->tokens[token].end + quote;
}

void json_patch_replace(JsonPatch *patch, int token, const char *value)
{
    JsonEdit *edit;

    if ((token == JSON_NONE) || !CHECK(patch->count < JSON_PATCH_EDIT_CAPACITY)) {
        return;
    }
    edit = &patch->edits[patch->count];
    span_of(patch->document, token, edit);
    if (text_copy(edit->value, JSON_PATCH_VALUE_CAPACITY, value)) {
        ++patch->count;
    }
}

static void sort_edits(JsonPatch *patch)
{
    JsonEdit moved;
    int index;
    int place;

    for (index = 1; index < patch->count; ++index) {
        moved = patch->edits[index];
        for (place = index; (place > 0) && (index - place < JSON_PATCH_EDIT_CAPACITY) &&
                            (patch->edits[place - 1].start > moved.start);
             --place) {
            patch->edits[place] = patch->edits[place - 1];
        }
        patch->edits[place] = moved;
    }
}

static bool put_span(JsonPatchOutput *output, const char *text, size_t length)
{
    if (length >= sizeof output->text - output->length) {
        return false;
    }
    (void)memcpy(output->text + output->length, text, length);
    output->length += length;
    return true;
}

static bool put_edit(JsonPatchOutput *output, const JsonDocument *document, const JsonEdit *edit, int at)
{
    return CHECK(edit->start >= at) && put_span(output, &document->text[at], (size_t)(edit->start - at)) &&
           put_span(output, edit->value, strlen(edit->value));
}

static bool put_rest(JsonPatchOutput *output, const JsonDocument *document, int at)
{
    return CHECK(document->length >= at) && put_span(output, &document->text[at], (size_t)(document->length - at));
}

bool json_patch_write(JsonPatch *patch, JsonPatchOutput *output)
{
    int at = 0;
    int index;
    bool fine = true;

    sort_edits(patch);
    output->length = 0U;
    for (index = 0; fine && (index < patch->count) && (index < JSON_PATCH_EDIT_CAPACITY); ++index) {
        fine = put_edit(output, patch->document, &patch->edits[index], at);
        at = patch->edits[index].end;
    }
    return fine && put_rest(output, patch->document, at);
}
