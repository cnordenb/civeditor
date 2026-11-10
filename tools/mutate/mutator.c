#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../tests/scan/source_scan.h"
#include "../build/source_list.h"
#include "../crap/cover_build.h"
#include "../crap/crap_cover.h"
#include "mutant_sites.h"
#include "mutation_run.h"

enum {
    WORKER_COUNT = 12,
    PREFIX_CAPACITY = 200,
    COMMAND_CAPACITY = 800,
    COMMAND_SCAN_LIMIT = 4096,
    TEXT_SPARE = 16,
    WAIT_LIMIT = 14400000,
    LINE_CAPACITY = 400,
    RESULT_LIMIT = 1000000,
    SITE_LIMIT = 20000,
    FATE_COUNT = 5,
    WORKER_MARK_LENGTH = 8
};

static const char FATE_LETTERS[FATE_COUNT + 1] = "KSNTU";
static const char WORKER_MARK[] = " worker=";

static SourceList sources;
static ScannedSource scanned;
static LineCoverage coverage;
static char mutated[SCAN_TEXT_CAPACITY + TEXT_SPARE];

static char fate_letter(MutantFate fate)
{
    switch (fate) {
    case MUTANT_KILLED:
        return 'K';
    case MUTANT_SURVIVED:
        return 'S';
    case MUTANT_TIMED_OUT:
        return 'T';
    case MUTANT_NOT_COMPILED:
    default:
        return 'N';
    }
}

static int line_of(int at)
{
    int line;

    if (!CHECK(at >= 0) || !CHECK(scanned.line_count > 0)) {
        return 0;
    }
    for (line = 0; (line + 1 < scanned.line_count) && (line < SCAN_LINE_LIMIT); ++line) {
        if (scanned.line_starts[line + 1] > at) {
            break;
        }
    }
    return line + 1;
}

static size_t make_mutant(const MutantSite *site)
{
    size_t with_length;
    size_t head;
    size_t tail;

    if (!CHECK(site != NULL) || !CHECK(site->at >= 0) || !CHECK(site->at + site->length <= scanned.length)) {
        return 0U;
    }
    with_length = text_length(site->with, MUTANT_WITH_CAPACITY);
    head = (size_t)site->at;
    tail = (size_t)(scanned.length - site->at - site->length);
    (void)memcpy(mutated, scanned.raw, head);
    (void)memcpy(&mutated[head], site->with, with_length);
    (void)memcpy(&mutated[head + with_length], &scanned.raw[site->at + site->length], tail);
    return head + with_length + tail;
}

static bool try_site(FILE *results, int worker, const char *path, const MutantSite *site, bool *prepared)
{
    char letter = 'U';
    int line;

    if (!CHECK(results != NULL) || !CHECK(path != NULL) || !CHECK(site != NULL) || !CHECK(prepared != NULL)) {
        return false;
    }
    line = line_of(site->at);
    if ((line <= SCAN_LINE_LIMIT) && coverage.ran[line]) {
        if (!*prepared) {
            *prepared = mutation_prepare(worker, path);
        }
        if (!*prepared) {
            return false;
        }
        letter = fate_letter(mutation_try(worker, path, mutated, make_mutant(site)));
    }
    return fprintf(results, "%c %s:%d %.*s -> %s\n", letter, path, line, site->length, &scanned.raw[site->at],
                   site->with) > 0;
}

static bool mutate_function(FILE *results, int worker, const char *path, int function, int *counter, bool *prepared)
{
    MutantSite site;
    int at;
    int step;

    if (!CHECK(counter != NULL) || !CHECK(function >= 0) || !CHECK(function < scanned.function_count)) {
        return false;
    }
    at = scanned.functions[function].body_start;
    for (step = 0; step < SITE_LIMIT; ++step) {
        site = mutant_next(scanned.cleaned, at, scanned.functions[function].body_end);
        if (!site.found) {
            break;
        }
        at = site.at + site.length;
        *counter += 1;
        if ((((*counter - 1) % WORKER_COUNT) == worker) && !try_site(results, worker, path, &site, prepared)) {
            return false;
        }
    }
    return true;
}

static bool mutate_file(FILE *results, int worker, const char *path, int *counter)
{
    bool prepared = false;
    int function;

    if (!CHECK(results != NULL) || !CHECK(path != NULL) || !CHECK(counter != NULL)) {
        return false;
    }
    if (!source_scan(&scanned, path) || !source_find_functions(&scanned) ||
        !crap_cover_read(&coverage, path)) {
        return false;
    }
    for (function = 0; (function < scanned.function_count) && (function < SCAN_FUNCTION_CAPACITY); ++function) {
        if (!mutate_function(results, worker, path, function, counter, &prepared)) {
            return false;
        }
    }
    return true;
}

static bool is_target(const char *path, const char *prefix)
{
    if (!CHECK(path != NULL) || !CHECK(prefix != NULL)) {
        return false;
    }
    return text_starts_with(path, prefix) && text_starts_with(path, "src/") && !text_starts_with(path, "src/win/") &&
           source_in_tests(path);
}

static int run_worker(int worker, const char *prefix)
{
    char path[MUTATION_PATH_CAPACITY];
    FILE *results;
    int counter = 0;
    int index;

    if (!CHECK(prefix != NULL) || !CHECK(worker < WORKER_COUNT) || !mutation_path(path, worker, "")) {
        return 1;
    }
    (void)CreateDirectoryA("build/mutate", NULL);
    (void)CreateDirectoryA(path, NULL);
    if (!mutation_path(path, worker, "/result.txt") || !source_list_collect_all(&sources, ".c")) {
        return 1;
    }
    results = fopen(path, "w");
    if (results == NULL) {
        return 1;
    }
    for (index = 0; (index < sources.count) && (index < SOURCE_LIST_CAPACITY); ++index) {
        if (is_target(sources.files[index].path, prefix)) {
            (void)mutate_file(results, worker, sources.files[index].path, &counter);
        }
    }
    return (fclose(results) == 0) ? 0 : 1;
}

static bool start_workers(HANDLE *handles, const char *prefix)
{
    char program[MAX_PATH];
    char command[COMMAND_CAPACITY];
    STARTUPINFOA start;
    PROCESS_INFORMATION process;
    int worker;

    if (!CHECK(handles != NULL) || !CHECK(prefix != NULL) || (GetModuleFileNameA(NULL, program, MAX_PATH) == 0U)) {
        return false;
    }
    for (worker = 0; worker < WORKER_COUNT; ++worker) {
        ZeroMemory(&start, sizeof start);
        start.cb = sizeof start;
        if (!text_copy(command, sizeof command, "\"") || !text_append(command, sizeof command, program) ||
            !text_append(command, sizeof command, "\" ") || !text_append(command, sizeof command, prefix) ||
            !text_append(command, sizeof command, WORKER_MARK) ||
            !text_append_number(command, sizeof command, worker) ||
            (CreateProcessA(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &start, &process) == 0)) {
            return false;
        }
        (void)CloseHandle(process.hThread);
        handles[worker] = process.hProcess;
    }
    return true;
}

static void merge_worker(FILE *all, int worker, int *tallies)
{
    char path[MUTATION_PATH_CAPACITY];
    char line[LINE_CAPACITY];
    FILE *results;
    int step;
    int fate;

    if (!CHECK(all != NULL) || !CHECK(tallies != NULL) || !mutation_path(path, worker, "/result.txt")) {
        return;
    }
    results = fopen(path, "r");
    if (results == NULL) {
        return;
    }
    for (step = 0; step < RESULT_LIMIT; ++step) {
        if (fgets(line, (int)sizeof line, results) == NULL) {
            break;
        }
        (void)fputs(line, all);
        for (fate = 0; fate < FATE_COUNT; ++fate) {
            tallies[fate] += (line[0] == FATE_LETTERS[fate]) ? 1 : 0;
        }
        if (line[0] == 'S') {
            (void)fputs(line, stdout);
        }
    }
    (void)fclose(results);
}

static int merge_results(void)
{
    int tallies[FATE_COUNT] = { 0, 0, 0, 0, 0 };
    FILE *all;
    int worker;

    if (!CHECK(FATE_COUNT == 5) || !CHECK(WORKER_COUNT > 0)) {
        return -1;
    }
    all = fopen("build/mutants.txt", "w");
    if (all == NULL) {
        return -1;
    }
    for (worker = 0; worker < WORKER_COUNT; ++worker) {
        merge_worker(all, worker, tallies);
    }
    (void)fclose(all);
    (void)printf("%d mutants: %d killed, %d survived, %d ended by the time limit, %d not compiled; "
                 "%d more on lines no test runs\n",
                 tallies[0] + tallies[1] + tallies[2] + tallies[3], tallies[0], tallies[1], tallies[3], tallies[2],
                 tallies[4]);
    return tallies[1];
}

static int run_parent(const char *prefix)
{
    HANDLE handles[WORKER_COUNT];
    int worker;

    if (!CHECK(prefix != NULL) || (GetFileAttributesA("build/tests.rsp") == INVALID_FILE_ATTRIBUTES)) {
        (void)fputs("Run build.exe first: the mutator links the test program it made.\n", stdout);
        return 1;
    }
    if (!cover_build_run(" nowindow") || !cover_build_tests_passed()) {
        (void)fputs("The tests must pass before the code is mutated.\n", stdout);
        return 1;
    }
    if (!start_workers(handles, prefix)) {
        (void)fputs("The workers could not be started.\n", stdout);
        return 1;
    }
    (void)WaitForMultipleObjects(WORKER_COUNT, handles, TRUE, WAIT_LIMIT);
    for (worker = 0; worker < WORKER_COUNT; ++worker) {
        (void)CloseHandle(handles[worker]);
    }
    return (merge_results() == 0) ? 0 : 1;
}

static int find_text(const char *text, const char *needle)
{
    size_t length = text_length(text, COMMAND_SCAN_LIMIT);
    size_t index;

    if (!CHECK(text != NULL) || !CHECK(needle != NULL)) {
        return -1;
    }
    for (index = 0U; (index < length) && (index < COMMAND_SCAN_LIMIT); ++index) {
        if (text_starts_with(&text[index], needle)) {
            return (int)index;
        }
    }
    return -1;
}

static bool take_prefix(char *prefix, const char *command_line)
{
    int at = find_text(command_line, " src/");
    size_t length = 0U;

    if (!CHECK(prefix != NULL) || !CHECK(command_line != NULL)) {
        return false;
    }
    if (at < 0) {
        return text_copy(prefix, PREFIX_CAPACITY, "src/");
    }
    for (length = 0U; length < PREFIX_CAPACITY; ++length) {
        if ((command_line[(size_t)at + 1U + length] == ' ') || (command_line[(size_t)at + 1U + length] == '\0')) {
            break;
        }
    }
    return text_copy_span(prefix, PREFIX_CAPACITY, &command_line[at + 1], length);
}

int main(void)
{
    const char *command_line = GetCommandLineA();
    char prefix[PREFIX_CAPACITY];
    TextNumber worker;
    int at;

    if (!CHECK(command_line != NULL) || !take_prefix(prefix, command_line)) {
        return 1;
    }
    at = find_text(command_line, WORKER_MARK);
    if (at < 0) {
        return run_parent(prefix);
    }
    worker = text_to_number(&command_line[at + WORKER_MARK_LENGTH],
                            text_length(&command_line[at + WORKER_MARK_LENGTH], PREFIX_CAPACITY));
    return (worker.valid && (worker.value >= 0L)) ? run_worker((int)worker.value, prefix) : 1;
}
