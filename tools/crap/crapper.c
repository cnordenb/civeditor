#include <stdio.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../tests/scan/source_scan.h"
#include "../build/source_list.h"
#include "cover_build.h"
#include "crap_cover.h"
#include "crap_score.h"

enum { ROW_CAPACITY = 2400, UNTESTED_TOTAL = 1 };

typedef struct {
    int file;
    char name[SCAN_NAME_CAPACITY];
    int line;
    int complexity;
    int covered;
    int total;
    long milli;
} CrapRow;

static SourceList sources;
static ScannedSource scanned;
static LineCoverage coverage;
static CrapRow rows[ROW_CAPACITY];
static int order[ROW_CAPACITY];
static int row_count;

static bool add_function(int file, const ScannedFunction *function, bool tested)
{
    CoveredShare counted = { 0, UNTESTED_TOTAL };
    CrapRow *row;

    if (!CHECK(function != NULL) || !CHECK(row_count < ROW_CAPACITY)) {
        return false;
    }
    if (tested) {
        counted = crap_cover_share(&coverage, function->first_line + 1, function->last_line + 1);
    }
    row = &rows[row_count];
    row->file = file;
    row->line = function->first_line + 1;
    row->complexity = crap_complexity(scanned.cleaned, function->body_start, function->body_end);
    row->covered = counted.covered;
    row->total = counted.total;
    row->milli = crap_score_milli(row->complexity, row->covered, row->total);
    order[row_count] = row_count;
    ++row_count;
    return text_copy(row->name, SCAN_NAME_CAPACITY, function->name);
}

static bool score_file(int file)
{
    bool tested;
    int index;

    if (!CHECK(file >= 0) || !CHECK(file < sources.count)) {
        return false;
    }
    tested = source_in_tests(sources.files[file].path);
    if (!source_scan(&scanned, sources.files[file].path) || !source_find_functions(&scanned)) {
        return false;
    }
    if (tested && !crap_cover_read(&coverage, sources.files[file].path)) {
        return false;
    }
    for (index = 0; (index < scanned.function_count) && (index < SCAN_FUNCTION_CAPACITY); ++index) {
        if (!add_function(file, &scanned.functions[index], tested)) {
            return false;
        }
    }
    return true;
}

static void sort_rows(void)
{
    int index;
    int back;
    int held;

    if (!CHECK(row_count >= 0) || !CHECK(row_count <= ROW_CAPACITY)) {
        return;
    }
    for (index = 1; (index < row_count) && (index < ROW_CAPACITY); ++index) {
        held = order[index];
        for (back = index; (back > 0) && (back < ROW_CAPACITY); --back) {
            if (rows[order[back - 1]].milli >= rows[held].milli) {
                break;
            }
            order[back] = order[back - 1];
        }
        order[back] = held;
    }
}

static bool print_row(FILE *out, const CrapRow *row)
{
    if (!CHECK(out != NULL) || !CHECK(row != NULL)) {
        return false;
    }
    return fprintf(out, "%ld.%03ld  CC %d  covered %d of %d  %s:%d  %s\n", row->milli / CRAP_MILLI,
                   row->milli % CRAP_MILLI, row->complexity, row->covered, row->total,
                   sources.files[row->file].path, row->line, row->name) > 0;
}

static int report(void)
{
    FILE *table;
    int over = 0;
    int index;

    if (!CHECK(row_count >= 0) || !CHECK(row_count <= ROW_CAPACITY)) {
        return -1;
    }
    table = fopen("build/crap.txt", "w");
    if (table == NULL) {
        return -1;
    }
    for (index = 0; (index < row_count) && (index < ROW_CAPACITY); ++index) {
        (void)print_row(table, &rows[order[index]]);
        if (rows[order[index]].milli >= CRAP_LIMIT_MILLI) {
            ++over;
            (void)print_row(stdout, &rows[order[index]]);
        }
    }
    (void)fclose(table);
    (void)printf("%d functions, %d at CRAP 10 or above\n", row_count, over);
    return over;
}

int main(void)
{
    int index;

    if (!cover_build_run("") || !source_list_collect_all(&sources, ".c")) {
        (void)fputs("The coverage could not be measured.\n", stdout);
        return 1;
    }
    for (index = 0; (index < sources.count) && (index < SOURCE_LIST_CAPACITY); ++index) {
        if (!score_file(index)) {
            (void)fputs(sources.files[index].path, stdout);
            (void)fputs(": could not be scored.\n", stdout);
            return 1;
        }
    }
    sort_rows();
    return (report() == 0) ? 0 : 1;
}
