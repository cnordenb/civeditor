#include "source_list.h"

#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"

enum { FOLDER_ENTRY_LIMIT = 4096, FILE_TIME_HIGH_SHIFT = 32, EXTENSION_CAPACITY = 16, ROOT_COUNT = 3, ROOT_WIDTH = 8 };

static const char SOURCE_ROOTS[ROOT_COUNT][ROOT_WIDTH] = { "src", "tests", "tools" };

typedef struct {
    char pending[SOURCE_FOLDER_CAPACITY][SOURCE_PATH_CAPACITY];
    int pending_count;
    char current[SOURCE_PATH_CAPACITY];
    char extension[EXTENSION_CAPACITY];
} FolderWalk;

static long long file_time_value(FILETIME stamp)
{
    unsigned long long high = (unsigned long long)stamp.dwHighDateTime;
    unsigned long long value = (high << FILE_TIME_HIGH_SHIFT) | (unsigned long long)stamp.dwLowDateTime;

    if (!CHECK(value < 0x7FFFFFFFFFFFFFFFULL)) {
        return 0LL;
    }
    return (long long)value;
}

static bool ends_with(const char *name, const char *extension)
{
    size_t name_length = text_length(name, SOURCE_PATH_CAPACITY);
    size_t extension_length = text_length(extension, EXTENSION_CAPACITY);

    if (!CHECK(name_length < SOURCE_PATH_CAPACITY) || !CHECK(extension_length > 0U)) {
        return false;
    }
    if (name_length < extension_length) {
        return false;
    }
    return text_equal(&name[name_length - extension_length], extension);
}

static bool join_path(char *destination, const char *folder, const char *name)
{
    if (!CHECK(destination != NULL) || !CHECK(folder != NULL)) {
        return false;
    }
    return text_copy(destination, SOURCE_PATH_CAPACITY, folder) &&
           text_append(destination, SOURCE_PATH_CAPACITY, "/") &&
           text_append(destination, SOURCE_PATH_CAPACITY, name);
}

static bool is_navigation_entry(const char *name)
{
    if (!CHECK(name != NULL) || !CHECK(name[0] != '\0')) {
        return true;
    }
    return text_equal(name, ".") || text_equal(name, "..");
}

static bool add_file(SourceList *list, const char *path, FILETIME stamp)
{
    if (!CHECK(list != NULL) || !CHECK(list->count < SOURCE_LIST_CAPACITY)) {
        return false;
    }
    if (!text_copy(list->files[list->count].path, SOURCE_PATH_CAPACITY, path)) {
        return false;
    }
    list->files[list->count].modified = file_time_value(stamp);
    ++list->count;
    return true;
}

static bool queue_folder(FolderWalk *walk, const char *path)
{
    if (!CHECK(walk != NULL) || !CHECK(walk->pending_count < SOURCE_FOLDER_CAPACITY)) {
        return false;
    }
    ++walk->pending_count;
    return text_copy(walk->pending[walk->pending_count - 1], SOURCE_PATH_CAPACITY, path);
}

static bool visit_entry(SourceList *list, FolderWalk *walk, const WIN32_FIND_DATAA *entry)
{
    char path[SOURCE_PATH_CAPACITY];

    if (!CHECK(walk != NULL) || !CHECK(entry != NULL)) {
        return false;
    }
    if (is_navigation_entry(entry->cFileName)) {
        return true;
    }
    if (!join_path(path, walk->current, entry->cFileName)) {
        return false;
    }
    if ((entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0U) {
        return queue_folder(walk, path);
    }
    if (!ends_with(entry->cFileName, walk->extension)) {
        return true;
    }
    return add_file(list, path, entry->ftLastWriteTime);
}

static bool scan_folder(SourceList *list, FolderWalk *walk)
{
    char pattern[SOURCE_PATH_CAPACITY];
    WIN32_FIND_DATAA entry;
    HANDLE search;
    bool fine = true;
    int step;

    if (!CHECK(walk != NULL) || !join_path(pattern, walk->current, "*")) {
        return false;
    }
    search = FindFirstFileA(pattern, &entry);
    if (search == INVALID_HANDLE_VALUE) {
        return true;
    }
    for (step = 0; (step < FOLDER_ENTRY_LIMIT) && fine; ++step) {
        fine = visit_entry(list, walk, &entry);
        if (FindNextFileA(search, &entry) == 0) {
            break;
        }
    }
    (void)FindClose(search);
    return CHECK(fine);
}

bool source_list_collect(SourceList *list, const char *root, const char *extension)
{
    static FolderWalk walk;
    int next;

    if (!CHECK(list != NULL) || !CHECK(root != NULL) || !CHECK(extension != NULL)) {
        return false;
    }
    list->count = 0;
    walk.pending_count = 1;
    if (!text_copy(walk.pending[0], SOURCE_PATH_CAPACITY, root) ||
        !text_copy(walk.extension, EXTENSION_CAPACITY, extension)) {
        return false;
    }
    for (next = 0; (next < walk.pending_count) && (next < SOURCE_FOLDER_CAPACITY); ++next) {
        if (!text_copy(walk.current, SOURCE_PATH_CAPACITY, walk.pending[next]) || !scan_folder(list, &walk)) {
            return false;
        }
    }
    return true;
}

bool source_list_collect_all(SourceList *list, const char *extension)
{
    static SourceList part;
    int root;
    int index;

    if (!CHECK(list != NULL) || !CHECK(extension != NULL)) {
        return false;
    }
    list->count = 0;
    for (root = 0; root < ROOT_COUNT; ++root) {
        if (!source_list_collect(&part, SOURCE_ROOTS[root], extension)) {
            return false;
        }
        for (index = 0; (index < part.count) && (index < SOURCE_LIST_CAPACITY); ++index) {
            if (!CHECK(list->count < SOURCE_LIST_CAPACITY)) {
                return false;
            }
            list->files[list->count] = part.files[index];
            ++list->count;
        }
    }
    return true;
}

bool source_in_tests(const char *path)
{
    if (!CHECK(path != NULL) || !CHECK(path[0] != '\0')) {
        return false;
    }
    return text_starts_with(path, "src/") || text_starts_with(path, "tests/") ||
           text_starts_with(path, "tools/crap/crap_") || text_starts_with(path, "tools/dry/dry_") ||
           text_starts_with(path, "tools/mutate/mutant_");
}

const char *source_test_libraries(void)
{
    static const char LIBRARIES[] = "-lgdi32\n-lcomdlg32\n-lshell32\n-ladvapi32\n"
                                    "-loleacc\n-lole32\n-loleaut32\n";

    if (!CHECK(sizeof LIBRARIES > 1U) || !CHECK(LIBRARIES[0] == '-')) {
        return "";
    }
    return LIBRARIES;
}

long long source_list_newest(const SourceList *list)
{
    long long newest = 0LL;
    int index;

    if (!CHECK(list != NULL) || !CHECK(list->count <= SOURCE_LIST_CAPACITY)) {
        return 0LL;
    }
    for (index = 0; (index < list->count) && (index < SOURCE_LIST_CAPACITY); ++index) {
        if (list->files[index].modified > newest) {
            newest = list->files[index].modified;
        }
    }
    return newest;
}

long long source_file_modified(const char *path)
{
    WIN32_FILE_ATTRIBUTE_DATA attributes;

    if (!CHECK(path != NULL) || !CHECK(path[0] != '\0')) {
        return 0LL;
    }
    if (GetFileAttributesExA(path, GetFileExInfoStandard, &attributes) == 0) {
        return 0LL;
    }
    return file_time_value(attributes.ftLastWriteTime);
}
