#include <stdio.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../tests/scan/source_scan.h"
#include "../build/source_list.h"
#include "dry_shape.h"

enum { ENTRY_CAPACITY = 1600, MIN_LINES = 4, REPORT_MILLI = 800, NOTE_MILLI = 600 };

typedef struct {
    int file;
    char name[SCAN_NAME_CAPACITY];
    int first_line;
    int last_line;
} DryEntry;

static SourceList sources;
static ScannedSource scanned;
static DryShape shapes[ENTRY_CAPACITY];
static DryEntry entries[ENTRY_CAPACITY];
static int entry_count;

static bool take_function(int file, const ScannedFunction *function)
{
    DryEntry *entry;

    if (!CHECK(function != NULL) || !CHECK(entry_count < ENTRY_CAPACITY)) {
        return false;
    }
    if ((function->last_line - function->first_line + 1) < MIN_LINES) {
        return true;
    }
    if (!dry_shape_of(&shapes[entry_count], scanned.cleaned, function->body_start, function->body_end)) {
        return false;
    }
    if (shapes[entry_count].tokens < DRY_MIN_TOKENS) {
        return true;
    }
    entry = &entries[entry_count];
    entry->file = file;
    entry->first_line = function->first_line + 1;
    entry->last_line = function->last_line + 1;
    ++entry_count;
    return text_copy(entry->name, SCAN_NAME_CAPACITY, function->name);
}

static bool take_file(int file)
{
    int index;

    if (!CHECK(file >= 0) || !CHECK(file < sources.count)) {
        return false;
    }
    if (!source_scan(&scanned, sources.files[file].path) || !source_find_functions(&scanned)) {
        return false;
    }
    for (index = 0; (index < scanned.function_count) && (index < SCAN_FUNCTION_CAPACITY); ++index) {
        if (!take_function(file, &scanned.functions[index])) {
            return false;
        }
    }
    return true;
}

static bool print_pair(FILE *out, int milli, const DryEntry *left, const DryEntry *right)
{
    if (!CHECK(out != NULL) || !CHECK(left != NULL) || !CHECK(right != NULL)) {
        return false;
    }
    return fprintf(out, "%d.%03d  %s:%d-%d %s  %s:%d-%d %s\n", milli / DRY_MILLI, milli % DRY_MILLI,
                   sources.files[left->file].path, left->first_line, left->last_line, left->name,
                   sources.files[right->file].path, right->first_line, right->last_line, right->name) > 0;
}

static int compare_with_later(FILE *notes, int left)
{
    int pairs = 0;
    int right;
    int milli;

    if (!CHECK(notes != NULL) || !CHECK(left >= 0) || !CHECK(left < entry_count)) {
        return 0;
    }
    for (right = left + 1; (right < entry_count) && (right < ENTRY_CAPACITY); ++right) {
        milli = dry_alike_milli(&shapes[left], &shapes[right]);
        if (milli >= NOTE_MILLI) {
            (void)print_pair(notes, milli, &entries[left], &entries[right]);
        }
        if (milli >= REPORT_MILLI) {
            ++pairs;
            (void)print_pair(stdout, milli, &entries[left], &entries[right]);
        }
    }
    return pairs;
}

static int report(void)
{
    FILE *notes;
    int pairs = 0;
    int left;

    if (!CHECK(entry_count >= 0) || !CHECK(entry_count <= ENTRY_CAPACITY)) {
        return -1;
    }
    notes = fopen("build/dry.txt", "w");
    if (notes == NULL) {
        return -1;
    }
    for (left = 0; (left < entry_count) && (left < ENTRY_CAPACITY); ++left) {
        pairs += compare_with_later(notes, left);
    }
    (void)fclose(notes);
    (void)printf("%d functions, %d pairs at 0.8 or above\n", entry_count, pairs);
    return pairs;
}

int main(void)
{
    int index;

    if (!source_list_collect_all(&sources, ".c")) {
        (void)fputs("The code could not be listed.\n", stdout);
        return 1;
    }
    for (index = 0; (index < sources.count) && (index < SOURCE_LIST_CAPACITY); ++index) {
        if (!take_file(index)) {
            (void)fputs(sources.files[index].path, stdout);
            (void)fputs(": could not be read.\n", stdout);
            return 1;
        }
    }
    return (report() == 0) ? 0 : 1;
}
