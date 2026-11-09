#include "json_patch.h"

#include <string.h>

#include "../core/check.h"
#include "../core/text.h"

void json_patch_start(JsonPatch *patch, const JsonDocument *document)
{
    if (!CHECK(patch != NULL) || !CHECK(document != NULL)) {
        return;
    }
    patch->document = document;
    patch->count = 0;
}

void json_patch_replace(JsonPatch *patch, int token, const char *value)
{
    const JsonDocument *document;
    JsonEdit *edit;
    int quote;

    if (!CHECK(patch != NULL) || !CHECK(value != NULL) || (token == JSON_NONE) ||
        !CHECK(patch->count < JSON_PATCH_EDIT_CAPACITY)) {
        return;
    }
    document = patch->document;
    if (!CHECK(document != NULL) || !CHECK((token >= 0) && (token < document->count))) {
        return;
    }
    edit = &patch->edits[patch->count];
    quote = json_is_kind(document, token, JSON_KIND_STRING) ? 1 : 0;
    edit->start = document->tokens[token].start - quote;
    edit->end = document->tokens[token].end + quote;
    if (text_copy(edit->value, JSON_PATCH_VALUE_CAPACITY, value)) {
        ++patch->count;
    }
}

static void sort_edits(JsonPatch *patch)
{
    JsonEdit moved;
    int index;
    int place;

    if (!CHECK(patch != NULL) || !CHECK((patch->count >= 0) && (patch->count <= JSON_PATCH_EDIT_CAPACITY))) {
        return;
    }
    for (index = 1; (index < patch->count) && (index < JSON_PATCH_EDIT_CAPACITY); ++index) {
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
    if (!CHECK(output != NULL) || !CHECK(text != NULL) || (length >= sizeof output->text - output->length)) {
        return false;
    }
    (void)memcpy(output->text + output->length, text, length);
    output->length += length;
    return true;
}

bool json_patch_write(JsonPatch *patch, JsonPatchOutput *output)
{
    const JsonDocument *document;
    const JsonEdit *edit;
    int at = 0;
    int index;
    bool fine = true;

    if (!CHECK(patch != NULL) || !CHECK(output != NULL) || !CHECK(patch->document != NULL)) {
        return false;
    }
    document = patch->document;
    sort_edits(patch);
    output->length = 0U;
    for (index = 0; fine && (index < patch->count) && (index < JSON_PATCH_EDIT_CAPACITY); ++index) {
        edit = &patch->edits[index];
        fine = CHECK(edit->start >= at) && put_span(output, &document->text[at], (size_t)(edit->start - at)) &&
               put_span(output, edit->value, strlen(edit->value));
        at = edit->end;
    }
    return fine && CHECK(document->length >= at) &&
           put_span(output, &document->text[at], (size_t)(document->length - at));
}
