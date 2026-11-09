#include "../maintainability/doc_rules.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../src/data/tsv.h"
#include "../scan/repo_files.h"
#include "../test_outcome.h"

enum {
    FOLDER_FILE_LIMIT = 7,
    ROOT_FILE_LIMIT = 8,
    ALLOWED_SUFFIX_COUNT = 8,
    SUFFIX_WIDTH = 16,
    NAME_CAPACITY = 128
};

static const char ALLOWED_SUFFIXES[ALLOWED_SUFFIX_COUNT][SUFFIX_WIDTH] = {
    ".c", ".h", ".md", ".txt", ".json", "LICENSE", ".gitignore", ".gitattributes"
};

static RepoFiles files;
static TsvFile document;
static TsvRecord line;

static bool same_folder(const char *left, const char *right)
{
    int left_start = repo_path_name_start(left);
    int right_start = repo_path_name_start(right);
    int index;

    if (!CHECK(left != NULL) || !CHECK(right != NULL) || (left_start != right_start)) {
        return false;
    }
    for (index = 0; (index < left_start) && (index < REPO_PATH_CAPACITY); ++index) {
        if (left[index] != right[index]) {
            return false;
        }
    }
    return true;
}

static int folder_size(int file)
{
    int count = 0;
    int index;

    if (!CHECK(file >= 0) || !CHECK(file < files.count)) {
        return 0;
    }
    for (index = 0; (index < files.count) && (index < REPO_FILE_CAPACITY); ++index) {
        count += same_folder(files.paths[file], files.paths[index]) ? 1 : 0;
    }
    return count;
}

bool repo_folders_hold_few_files(void)
{
    int index;
    int limit;

    if (!repo_files_list(&files) || !CHECK(files.count > 0)) {
        return false;
    }
    for (index = 0; (index < files.count) && (index < REPO_FILE_CAPACITY); ++index) {
        if (text_starts_with(files.paths[index], "documentation/DEVLOG/")) {
            continue;
        }
        limit = (repo_path_name_start(files.paths[index]) == 0) ? ROOT_FILE_LIMIT : FOLDER_FILE_LIMIT;
        if (folder_size(index) > limit) {
            test_note(files.paths[index]);
            return false;
        }
    }
    return true;
}

static void lower_name(const char *path, char *name)
{
    int start = repo_path_name_start(path);
    int index;

    if (!CHECK(path != NULL) || !CHECK(name != NULL)) {
        return;
    }
    for (index = 0; (index < NAME_CAPACITY - 1) && (path[start + index] != '\0'); ++index) {
        char letter = path[start + index];
        name[index] = ((letter >= 'A') && (letter <= 'Z')) ? (char)(letter - 'A' + 'a') : letter;
    }
    name[index] = '\0';
}

static bool name_repeats(int file)
{
    char name[NAME_CAPACITY];
    char other[NAME_CAPACITY];
    int index;

    if (!CHECK(file >= 0) || !CHECK(file < files.count)) {
        return false;
    }
    lower_name(files.paths[file], name);
    if (text_equal(name, "skill.md")) {
        return false;
    }
    for (index = file + 1; (index < files.count) && (index < REPO_FILE_CAPACITY); ++index) {
        lower_name(files.paths[index], other);
        if (text_equal(name, other)) {
            return true;
        }
    }
    return false;
}

bool repo_file_names_are_unique(void)
{
    int index;

    if (!repo_files_list(&files) || !CHECK(files.count > 0)) {
        return false;
    }
    for (index = 0; (index < files.count) && (index < REPO_FILE_CAPACITY); ++index) {
        if (name_repeats(index)) {
            test_note(files.paths[index]);
            return false;
        }
    }
    return true;
}

static bool has_allowed_suffix(const char *path)
{
    int index;

    if (!CHECK(path != NULL) || !CHECK(path[0] != '\0')) {
        return false;
    }
    for (index = 0; index < ALLOWED_SUFFIX_COUNT; ++index) {
        if (repo_path_has_suffix(path, ALLOWED_SUFFIXES[index])) {
            return true;
        }
    }
    return false;
}

bool repo_holds_only_c_and_documents(void)
{
    int index;

    if (!repo_files_list(&files) || !CHECK(files.count > 0)) {
        return false;
    }
    for (index = 0; (index < files.count) && (index < REPO_FILE_CAPACITY); ++index) {
        if (!has_allowed_suffix(files.paths[index])) {
            test_note(files.paths[index]);
            return false;
        }
    }
    return true;
}

static bool line_has_placeholder(const char *text)
{
    size_t index;

    if (!CHECK(text != NULL) || !CHECK(TSV_LINE_CAPACITY > 2)) {
        return false;
    }
    for (index = 0U; (index + 2U < (size_t)TSV_LINE_CAPACITY) && (text[index] != '\0'); ++index) {
        if ((text[index] == '{') && (text[index + 1U] == '{') && (text[index + 2U] >= 'A') &&
            (text[index + 2U] <= 'Z')) {
            return true;
        }
    }
    return text_contains_folded(text, "template:start") || text_contains_folded(text, "template:end");
}

static bool document_has_placeholder(const char *path)
{
    wchar_t wide[REPO_PATH_CAPACITY];
    size_t index;
    int step;

    if (!CHECK(path != NULL) || !CHECK(text_length(path, REPO_PATH_CAPACITY) < REPO_PATH_CAPACITY)) {
        return false;
    }
    for (index = 0U; (index < REPO_PATH_CAPACITY) && (index <= text_length(path, REPO_PATH_CAPACITY)); ++index) {
        wide[index] = (wchar_t)(unsigned char)path[index];
    }
    if (!tsv_open(&document, wide)) {
        return false;
    }
    for (step = 0; (step < TSV_FILE_CAPACITY) && tsv_next(&document, &line); ++step) {
        if (line_has_placeholder(line.text)) {
            return true;
        }
    }
    return false;
}

bool repo_placeholders_are_filled(void)
{
    int index;

    if (!repo_files_list(&files) || !CHECK(files.count > 0)) {
        return false;
    }
    for (index = 0; (index < files.count) && (index < REPO_FILE_CAPACITY); ++index) {
        if (!repo_path_has_suffix(files.paths[index], ".md") || text_equal(files.paths[index], "instructions.md")) {
            continue;
        }
        if (document_has_placeholder(files.paths[index])) {
            test_note(files.paths[index]);
            return false;
        }
    }
    return true;
}
