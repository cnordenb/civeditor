#include "source_scan.h"

#include "../../src/core/check.h"
#include "../../src/core/file_io.h"
#include "../../src/core/text.h"
#include "repo_files.h"

typedef enum {
    LEXICAL_CODE,
    LEXICAL_LINE_COMMENT,
    LEXICAL_BLOCK_COMMENT,
    LEXICAL_STRING,
    LEXICAL_CHARACTER
} LexicalState;

typedef struct {
    LexicalState state;
    int line;
} Lexer;

bool source_is_word_letter(char letter)
{
    return ((letter >= 'a') && (letter <= 'z')) || ((letter >= 'A') && (letter <= 'Z')) ||
           ((letter >= '0') && (letter <= '9')) || (letter == '_');
}

static void note_comment(ScannedSource *source, Lexer *lexer, LexicalState next)
{
    if (!CHECK(source != NULL) || !CHECK(lexer != NULL)) {
        return;
    }
    if (source->comment_line == 0) {
        source->comment_line = lexer->line;
    }
    lexer->state = next;
}

static char clean_code_letter(ScannedSource *source, Lexer *lexer, int index)
{
    char letter = source->raw[index];
    char next = (index + 1 < source->length) ? source->raw[index + 1] : '\0';

    if (!CHECK(lexer != NULL) || !CHECK(index < source->length)) {
        return ' ';
    }
    if ((letter == '/') && (next == '/')) {
        note_comment(source, lexer, LEXICAL_LINE_COMMENT);
        return ' ';
    }
    if ((letter == '/') && (next == '*')) {
        note_comment(source, lexer, LEXICAL_BLOCK_COMMENT);
        return ' ';
    }
    if (letter == '"') {
        lexer->state = LEXICAL_STRING;
    } else if (letter == '\'') {
        lexer->state = LEXICAL_CHARACTER;
    }
    return letter;
}

static char clean_literal_letter(ScannedSource *source, Lexer *lexer, int *index, char closing)
{
    char letter = source->raw[*index];

    if (!CHECK(lexer != NULL) || !CHECK(*index < source->length)) {
        return ' ';
    }
    if ((letter == '\\') && (*index + 1 < source->length)) {
        source->cleaned[*index] = ' ';
        ++*index;
        return (source->raw[*index] == '\n') ? '\n' : ' ';
    }
    if (letter == closing) {
        lexer->state = LEXICAL_CODE;
        return letter;
    }
    return (letter == '\n') ? '\n' : ' ';
}

static char clean_comment_letter(ScannedSource *source, Lexer *lexer, int *index)
{
    char letter = source->raw[*index];

    if (!CHECK(lexer != NULL) || !CHECK(*index < source->length)) {
        return ' ';
    }
    if ((lexer->state == LEXICAL_LINE_COMMENT) && (letter == '\n')) {
        lexer->state = LEXICAL_CODE;
        return '\n';
    }
    if ((lexer->state == LEXICAL_BLOCK_COMMENT) && (letter == '*') && (*index + 1 < source->length) &&
        (source->raw[*index + 1] == '/')) {
        source->cleaned[*index] = ' ';
        ++*index;
        lexer->state = LEXICAL_CODE;
        return ' ';
    }
    return (letter == '\n') ? '\n' : ' ';
}

static void clean_text(ScannedSource *source)
{
    Lexer lexer = { LEXICAL_CODE, 1 };
    int index;
    char cleaned;

    if (!CHECK(source != NULL) || !CHECK(source->length < SCAN_TEXT_CAPACITY)) {
        return;
    }
    for (index = 0; (index < source->length) && (index < SCAN_TEXT_CAPACITY); ++index) {
        switch (lexer.state) {
        case LEXICAL_CODE:
            cleaned = clean_code_letter(source, &lexer, index);
            break;
        case LEXICAL_STRING:
            cleaned = clean_literal_letter(source, &lexer, &index, '"');
            break;
        case LEXICAL_CHARACTER:
            cleaned = clean_literal_letter(source, &lexer, &index, '\'');
            break;
        case LEXICAL_LINE_COMMENT:
        case LEXICAL_BLOCK_COMMENT:
        default:
            cleaned = clean_comment_letter(source, &lexer, &index);
            break;
        }
        source->cleaned[index] = cleaned;
        lexer.line += (source->raw[index] == '\n') ? 1 : 0;
    }
    source->cleaned[source->length] = '\0';
}

static void index_lines(ScannedSource *source)
{
    int index;

    if (!CHECK(source != NULL) || !CHECK(source->length >= 0)) {
        return;
    }
    source->line_count = 1;
    source->line_starts[0] = 0;
    for (index = 0; (index < source->length) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if ((source->raw[index] == '\n') && (source->line_count < SCAN_LINE_LIMIT) && (index + 1 < source->length)) {
            source->line_starts[source->line_count] = index + 1;
            ++source->line_count;
        }
    }
}

int source_line_length(const ScannedSource *source, int line)
{
    int start;
    int index;

    if (!CHECK(source != NULL) || !CHECK((line >= 0) && (line < source->line_count))) {
        return 0;
    }
    start = source->line_starts[line];
    for (index = start; (index < source->length) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if ((source->raw[index] == '\n') || (source->raw[index] == '\r')) {
            break;
        }
    }
    return index - start;
}

bool source_line_is_blank(const ScannedSource *source, int line)
{
    int start;
    int index;
    int length = source_line_length(source, line);

    if (!CHECK(source != NULL) || !CHECK(length >= 0)) {
        return true;
    }
    start = source->line_starts[line];
    for (index = start; (index < start + length) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if ((source->raw[index] != ' ') && (source->raw[index] != '\t')) {
            return false;
        }
    }
    return true;
}

int source_count_word(const ScannedSource *source, int start, int end, const char *word)
{
    int count = 0;
    int index;
    size_t length = text_length(word, SCAN_NAME_CAPACITY);

    if (!CHECK(source != NULL) || !CHECK(length > 0U)) {
        return 0;
    }
    for (index = start; (index < end) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if ((index > 0) && source_is_word_letter(source->cleaned[index - 1])) {
            continue;
        }
        if (text_starts_with(&source->cleaned[index], word) &&
            !source_is_word_letter(source->cleaned[index + (int)length])) {
            ++count;
        }
    }
    return count;
}

static void measure_lines(ScannedSource *source)
{
    int line;
    int length;

    if (!CHECK(source != NULL) || !CHECK(source->line_count <= SCAN_LINE_LIMIT)) {
        return;
    }
    source->longest_line = 0;
    source->lines_of_code = 0;
    for (line = 0; (line < source->line_count) && (line < SCAN_LINE_LIMIT); ++line) {
        length = source_line_length(source, line);
        source->longest_line = (length > source->longest_line) ? length : source->longest_line;
        source->lines_of_code += source_line_is_blank(source, line) ? 0 : 1;
    }
}


bool source_scan(ScannedSource *source, const char *path)
{
    wchar_t wide[REPO_PATH_CAPACITY];
    FileRead read;
    size_t index;

    if (!CHECK(source != NULL) || !CHECK(path != NULL)) {
        return false;
    }
    for (index = 0U; (index < REPO_PATH_CAPACITY) && (index <= text_length(path, REPO_PATH_CAPACITY)); ++index) {
        wide[index] = (wchar_t)(unsigned char)path[index];
    }
    read = file_read_all(wide, source->raw, sizeof source->raw);
    if (!read.succeeded) {
        return false;
    }
    source->length = (int)read.length;
    source->comment_line = 0;
    clean_text(source);
    index_lines(source);
    measure_lines(source);
    return source_find_functions(source);
}

bool source_span_is(const char *text, int start, int stop, const char *word)
{
    int index;

    if (!CHECK(text != NULL) || !CHECK(word != NULL) || (stop - start >= SCAN_NAME_CAPACITY) || (stop < start)) {
        return false;
    }
    for (index = 0; (index < SCAN_NAME_CAPACITY) && (index < stop - start); ++index) {
        if ((word[index] == '\0') || (text[start + index] != word[index])) {
            return false;
        }
    }
    return word[stop - start] == '\0';
}