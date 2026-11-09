#include "json_reader.h"

#include "../core/check.h"

typedef struct {
    int position;
    int open[JSON_READER_DEPTH];
    int depth;
} ParseState;

static bool is_space(char letter)
{
    return (letter == ' ') || (letter == '\n') || (letter == '\r') || (letter == '\t');
}

static bool is_number_part(char letter)
{
    return ((letter >= '0') && (letter <= '9')) || (letter == '-') || (letter == '+') || (letter == '.') ||
           (letter == 'e') || (letter == 'E');
}

static int add_token(JsonDocument *document, JsonKind kind, int start, int end)
{
    JsonToken *token;

    if (!CHECK(document != NULL) || !CHECK(document->count < JSON_TOKEN_CAPACITY)) {
        return JSON_NONE;
    }
    token = &document->tokens[document->count];
    token->kind = kind;
    token->start = start;
    token->end = end;
    token->after = document->count + 1;
    ++document->count;
    return document->count - 1;
}

static int string_end(const JsonDocument *document, int opening)
{
    int index;

    if (!CHECK(document != NULL) || !CHECK((opening >= 0) && (opening < document->length))) {
        return JSON_NONE;
    }
    for (index = opening + 1; (index < document->length) && (index < JSON_INPUT_CAPACITY); ++index) {
        if (document->text[index] == '\\') {
            ++index;
        } else if (document->text[index] == '"') {
            return index;
        }
    }
    return JSON_NONE;
}

static bool literal_at(const JsonDocument *document, int position, const char *word, int length)
{
    int index;

    if (!CHECK(document != NULL) || !CHECK(word != NULL) || (position + length > document->length)) {
        return false;
    }
    for (index = 0; (index < length) && (index < JSON_READER_DEPTH); ++index) {
        if (document->text[position + index] != word[index]) {
            return false;
        }
    }
    return true;
}

static bool open_container(JsonDocument *document, ParseState *state, JsonKind kind)
{
    int token;

    if (!CHECK(state != NULL) || !CHECK(state->depth < JSON_READER_DEPTH)) {
        return false;
    }
    token = add_token(document, kind, state->position, state->position + 1);
    if (token == JSON_NONE) {
        return false;
    }
    state->open[state->depth] = token;
    ++state->depth;
    ++state->position;
    return true;
}

static bool close_container(JsonDocument *document, ParseState *state, JsonKind kind)
{
    JsonToken *token;

    if (!CHECK(document != NULL) || !CHECK(state != NULL) || (state->depth <= 0)) {
        return false;
    }
    --state->depth;
    token = &document->tokens[state->open[state->depth]];
    if (token->kind != kind) {
        return false;
    }
    token->end = state->position + 1;
    token->after = document->count;
    ++state->position;
    return true;
}

static bool read_string(JsonDocument *document, ParseState *state)
{
    int closing;

    if (!CHECK(document != NULL) || !CHECK(state != NULL)) {
        return false;
    }
    closing = string_end(document, state->position);
    if ((closing == JSON_NONE) ||
        (add_token(document, JSON_KIND_STRING, state->position + 1, closing) == JSON_NONE)) {
        return false;
    }
    state->position = closing + 1;
    return true;
}

static bool read_number(JsonDocument *document, ParseState *state)
{
    int end;

    if (!CHECK(document != NULL) || !CHECK(state != NULL)) {
        return false;
    }
    for (end = state->position; (end < document->length) && (end < JSON_INPUT_CAPACITY); ++end) {
        if (!is_number_part(document->text[end])) {
            break;
        }
    }
    if (add_token(document, JSON_KIND_NUMBER, state->position, end) == JSON_NONE) {
        return false;
    }
    state->position = end;
    return true;
}

static bool read_literal(JsonDocument *document, ParseState *state, const char *word, JsonKind kind)
{
    int length = (kind == JSON_KIND_FALSE) ? 5 : 4;

    if (!CHECK(document != NULL) || !CHECK(state != NULL)) {
        return false;
    }
    if (!literal_at(document, state->position, word, length) ||
        (add_token(document, kind, state->position, state->position + length) == JSON_NONE)) {
        return false;
    }
    state->position += length;
    return true;
}

static bool read_item(JsonDocument *document, ParseState *state, char letter)
{
    if (!CHECK(document != NULL) || !CHECK(state != NULL)) {
        return false;
    }
    switch (letter) {
    case '{':
        return open_container(document, state, JSON_KIND_OBJECT);
    case '[':
        return open_container(document, state, JSON_KIND_ARRAY);
    case '}':
        return close_container(document, state, JSON_KIND_OBJECT);
    case ']':
        return close_container(document, state, JSON_KIND_ARRAY);
    case '"':
        return read_string(document, state);
    case ':':
    case ',':
        ++state->position;
        return state->depth > 0;
    case 't':
        return read_literal(document, state, "true", JSON_KIND_TRUE);
    case 'f':
        return read_literal(document, state, "false", JSON_KIND_FALSE);
    case 'n':
        return read_literal(document, state, "null", JSON_KIND_NULL);
    default:
        return is_number_part(letter) && read_number(document, state);
    }
}

bool json_parse(JsonDocument *document)
{
    ParseState state = { 0, { 0 }, 0 };
    int step;

    if (!CHECK(document != NULL) || !CHECK((document->length >= 0) && (document->length < JSON_INPUT_CAPACITY))) {
        return false;
    }
    document->count = 0;
    for (step = 0; step < JSON_INPUT_CAPACITY; ++step) {
        if (state.position >= document->length) {
            break;
        }
        if (is_space(document->text[state.position])) {
            ++state.position;
        } else if (!read_item(document, &state, document->text[state.position])) {
            return false;
        }
    }
    return (state.depth == 0) && (document->count > 0) && json_is_kind(document, 0, JSON_KIND_OBJECT);
}

bool json_is_kind(const JsonDocument *document, int token, JsonKind kind)
{
    if (!CHECK(document != NULL) || !CHECK(token >= JSON_NONE) || !CHECK(document->count <= JSON_TOKEN_CAPACITY)) {
        return false;
    }
    if ((token < 0) || (token >= document->count)) {
        return false;
    }
    return document->tokens[token].kind == kind;
}

int json_first_child(const JsonDocument *document, int container)
{
    if (!CHECK(document != NULL) || !CHECK(container >= JSON_NONE) || (container < 0) ||
        (container >= document->count)) {
        return JSON_NONE;
    }
    if (!json_is_kind(document, container, JSON_KIND_OBJECT) && !json_is_kind(document, container, JSON_KIND_ARRAY)) {
        return JSON_NONE;
    }
    return (container + 1 < document->tokens[container].after) ? container + 1 : JSON_NONE;
}

int json_next_sibling(const JsonDocument *document, int container, int child)
{
    int next;

    if (!CHECK(document != NULL) || !CHECK(child >= JSON_NONE) || (container < 0) || (child <= container) ||
        (child >= document->count)) {
        return JSON_NONE;
    }
    next = document->tokens[child].after;
    return (next < document->tokens[container].after) ? next : JSON_NONE;
}

static bool key_matches(const JsonDocument *document, int token, const char *key)
{
    int index;
    int length;

    if (!CHECK(document != NULL) || !CHECK(key != NULL) || !json_is_kind(document, token, JSON_KIND_STRING)) {
        return false;
    }
    length = document->tokens[token].end - document->tokens[token].start;
    for (index = 0; (index < length) && (index < JSON_INPUT_CAPACITY); ++index) {
        if (document->text[document->tokens[token].start + index] != key[index]) {
            return false;
        }
    }
    return key[length] == '\0';
}

int json_member(const JsonDocument *document, int object, const char *key)
{
    int name = json_first_child(document, object);
    int value;
    int step;

    if (!CHECK(key != NULL) || !CHECK(object >= JSON_NONE) || !json_is_kind(document, object, JSON_KIND_OBJECT)) {
        return JSON_NONE;
    }
    for (step = 0; (step < JSON_TOKEN_CAPACITY) && (name != JSON_NONE); ++step) {
        value = json_next_sibling(document, object, name);
        if (value == JSON_NONE) {
            return JSON_NONE;
        }
        if (key_matches(document, name, key)) {
            return value;
        }
        name = json_next_sibling(document, object, value);
    }
    return JSON_NONE;
}

TextNumber json_number(const JsonDocument *document, int token)
{
    TextNumber missing = { false, 0L };
    int length;
    int index;

    if (!CHECK(document != NULL) || !CHECK(token >= JSON_NONE) || !json_is_kind(document, token, JSON_KIND_NUMBER)) {
        return missing;
    }
    length = document->tokens[token].end - document->tokens[token].start;
    for (index = 0; (index < length) && (index < TEXT_NUMBER_CAPACITY); ++index) {
        if (document->text[document->tokens[token].start + index] == '.') {
            length = index;
        }
    }
    return text_to_number(&document->text[document->tokens[token].start], (size_t)length);
}
