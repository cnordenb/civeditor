#include "mutation_run.h"

#include <stdio.h>
#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../tests/scan/repo_files.h"

enum { COMMAND_CAPACITY = 1024, LINE_CAPACITY = 300, RESPONSE_LIMIT = 1000, BUILD_LIMIT = 120000, TEST_LIMIT = 30000 };

static const char COMPILE[] = "gcc -std=c11 -DUNICODE -D_UNICODE -DCOBJMACROS -pipe -O0 -w -I";

bool mutation_path(char *destination, int worker, const char *name)
{
    if (!CHECK(destination != NULL) || !CHECK(name != NULL) || !CHECK(worker >= 0)) {
        return false;
    }
    return text_copy(destination, MUTATION_PATH_CAPACITY, "build/mutate/w") &&
           text_append_number(destination, MUTATION_PATH_CAPACITY, worker) &&
           text_append(destination, MUTATION_PATH_CAPACITY, name);
}

static bool run_limited(const char *command, unsigned long limit, unsigned long *exit_code)
{
    char line[COMMAND_CAPACITY];
    SECURITY_ATTRIBUTES inherit = { sizeof inherit, NULL, TRUE };
    STARTUPINFOA start;
    PROCESS_INFORMATION process;
    HANDLE silence;
    bool finished;

    if (!CHECK(command != NULL) || !CHECK(exit_code != NULL) || !text_copy(line, sizeof line, command)) {
        return false;
    }
    silence = CreateFileA("NUL", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &inherit, OPEN_EXISTING, 0, NULL);
    ZeroMemory(&start, sizeof start);
    start.cb = sizeof start;
    start.dwFlags = STARTF_USESTDHANDLES;
    start.hStdOutput = silence;
    start.hStdError = silence;
    if (CreateProcessA(NULL, line, NULL, NULL, TRUE, 0, NULL, NULL, &start, &process) == 0) {
        (void)CloseHandle(silence);
        return false;
    }
    finished = WaitForSingleObject(process.hProcess, limit) == WAIT_OBJECT_0;
    if (!finished) {
        (void)TerminateProcess(process.hProcess, 1U);
    }
    (void)GetExitCodeProcess(process.hProcess, exit_code);
    (void)CloseHandle(process.hProcess);
    (void)CloseHandle(process.hThread);
    (void)CloseHandle(silence);
    return finished;
}

static bool copy_response(FILE *in, FILE *out, const char *needle, const char *object)
{
    char line[LINE_CAPACITY];
    bool found = false;
    size_t length;
    int step;

    if (!CHECK(in != NULL) || !CHECK(out != NULL) || !CHECK(needle != NULL) || !CHECK(object != NULL)) {
        return false;
    }
    for (step = 0; step < RESPONSE_LIMIT; ++step) {
        if (fgets(line, (int)sizeof line, in) == NULL) {
            break;
        }
        length = text_length(line, sizeof line);
        if ((length > 0U) && (line[length - 1U] == '\n')) {
            line[length - 1U] = '\0';
        }
        if (repo_path_has_suffix(line, needle)) {
            found = true;
            (void)fputs(object, out);
        } else {
            (void)fputs(line, out);
        }
        (void)fputs("\n", out);
    }
    return found;
}

static bool make_response(int worker, const char *needle)
{
    char response[MUTATION_PATH_CAPACITY];
    char object[MUTATION_PATH_CAPACITY];
    FILE *in;
    FILE *out;
    bool found;

    if (!CHECK(needle != NULL) || !mutation_path(response, worker, "/tests.rsp") ||
        !mutation_path(object, worker, "/mutant.o")) {
        return false;
    }
    in = fopen("build/tests.rsp", "r");
    if (in == NULL) {
        return false;
    }
    out = fopen(response, "w");
    if (out == NULL) {
        (void)fclose(in);
        return false;
    }
    found = copy_response(in, out, needle, object);
    (void)fclose(in);
    return (fclose(out) == 0) && found;
}

bool mutation_prepare(int worker, const char *source_path)
{
    char folder[MUTATION_PATH_CAPACITY];
    char needle[MUTATION_PATH_CAPACITY] = "/";
    size_t start = (size_t)repo_path_name_start(source_path);
    size_t length = text_length(source_path, MUTATION_PATH_CAPACITY);

    if (!CHECK(source_path != NULL) || !CHECK(length > start + 2U) || !mutation_path(folder, worker, "")) {
        return false;
    }
    (void)CreateDirectoryA("build/mutate", NULL);
    (void)CreateDirectoryA(folder, NULL);
    return text_copy_span(&needle[1], sizeof needle - 1U, &source_path[start], length - start - 2U) &&
           text_append(needle, sizeof needle, ".o") && make_response(worker, needle);
}

static bool write_mutant(int worker, const char *text, size_t length)
{
    char path[MUTATION_PATH_CAPACITY];
    FILE *file;
    size_t written;

    if (!CHECK(text != NULL) || !CHECK(length > 0U) || !mutation_path(path, worker, "/mutant.c")) {
        return false;
    }
    file = fopen(path, "wb");
    if (file == NULL) {
        return false;
    }
    written = fwrite(text, 1U, length, file);
    return (fclose(file) == 0) && (written == length);
}

static bool build_mutant(int worker, const char *source_path)
{
    char command[COMMAND_CAPACITY];
    char folder[MUTATION_PATH_CAPACITY];
    unsigned long code = 1UL;

    if (!CHECK(source_path != NULL) || !mutation_path(folder, worker, "") ||
        !text_copy(command, sizeof command, COMPILE)) {
        return false;
    }
    if (!text_copy_span(&command[sizeof COMPILE - 1U], sizeof command - sizeof COMPILE, source_path,
                        (size_t)repo_path_name_start(source_path)) ||
        !text_append(command, sizeof command, " -c ") || !text_append(command, sizeof command, folder) ||
        !text_append(command, sizeof command, "/mutant.c -o ") || !text_append(command, sizeof command, folder) ||
        !text_append(command, sizeof command, "/mutant.o") || !run_limited(command, BUILD_LIMIT, &code) ||
        (code != 0UL)) {
        return false;
    }
    return text_copy(command, sizeof command, "gcc -o ") && text_append(command, sizeof command, folder) &&
           text_append(command, sizeof command, "/tests.exe @") && text_append(command, sizeof command, folder) &&
           text_append(command, sizeof command, "/tests.rsp") && run_limited(command, BUILD_LIMIT, &code) &&
           (code == 0UL);
}

MutantFate mutation_try(int worker, const char *source_path, const char *text, size_t length)
{
    char command[COMMAND_CAPACITY] = "build\\mutate\\w";
    unsigned long code = 1UL;

    if (!CHECK(source_path != NULL) || !CHECK(text != NULL) || !write_mutant(worker, text, length) ||
        !build_mutant(worker, source_path)) {
        return MUTANT_NOT_COMPILED;
    }
    if (!text_append_number(command, sizeof command, worker) ||
        !text_append(command, sizeof command, "\\tests.exe nowindow")) {
        return MUTANT_NOT_COMPILED;
    }
    if (!run_limited(command, TEST_LIMIT, &code)) {
        return MUTANT_TIMED_OUT;
    }
    return (code == 0UL) ? MUTANT_SURVIVED : MUTANT_KILLED;
}
