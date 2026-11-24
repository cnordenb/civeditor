#include "source_list.h"

#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"

enum { FOLDER_ENTRY_LIMIT = 4096, FILE_TIME_HIGH_SHIFT = 32, EXTENSION_CAPACITY = 16, ROOT_COUNT = 3, ROOT_WIDTH = 8 };

static const char SOURCE_ROOTS[ROOT_COUNT][ROOT_WIDTH] = { "src", "tests", "tools" };

static const char PROGRAM_PREFIXES[SOURCE_PROGRAM_COUNT][SOURCE_PREFIX_LIMIT][SOURCE_PREFIX_WIDTH] = {
    { "src/" },
    { "src/", "tests/", "tools/build/source_", "tools/build/build_plan" },
    { "tools/build/", "src/core/" }
};
static const char LIBRARIES[] =
    "-lgdi32\n-lcomctl32\n-lcomdlg32\n-lshell32\n-ladvapi32\n-loleacc\n-lole32\n-loleaut32\n-luxtheme\n";

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
    return text_copy(destination, SOURCE_PATH_CAPACITY, folder) &&
           text_append(destination, SOURCE_PATH_CAPACITY, "/") &&
           text_append(destination, SOURCE_PATH_CAPACITY, name);
}

static bool is_navigation_entry(const char *name)
{
    if (!CHECK(name[0] != '\0')) {
        return true;
    }
    return text_equal(name, ".") || text_equal(name, "..");
}

static bool add_file(SourceList *list, const char *path, FILETIME stamp)
{
    if (!CHECK(list->count < SOURCE_LIST_CAPACITY)) {
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
    if (!CHECK(walk->pending_count < SOURCE_FOLDER_CAPACITY)) {
        return false;
    }
    ++walk->pending_count;
    return text_copy(walk->pending[walk->pending_count - 1], SOURCE_PATH_CAPACITY, path);
}

static bool visit_entry(SourceList *list, FolderWalk *walk, const WIN32_FIND_DATAA *entry)
{
    char path[SOURCE_PATH_CAPACITY];

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

    if (!join_path(pattern, walk->current, "*")) {
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

static bool start_walk(FolderWalk *walk, const char *root, const char *extension)
{
    walk->pending_count = 1;
    return text_copy(walk->pending[0], SOURCE_PATH_CAPACITY, root) &&
           text_copy(walk->extension, EXTENSION_CAPACITY, extension);
}

bool source_list_collect(SourceList *list, const char *root, const char *extension)
{
    static FolderWalk walk;
    int next;

    if (!start_walk(&walk, root, extension)) {
        return false;
    }
    list->count = 0;
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

    list->count = 0;
    for (root = 0; root < ROOT_COUNT; ++root) {
        if (!source_list_collect(&part, SOURCE_ROOTS[root], extension)) {
            return false;
        }
        for (index = 0; index < part.count; ++index) {
            if (!CHECK(list->count < SOURCE_LIST_CAPACITY)) {
                return false;
            }
            list->files[list->count] = part.files[index];
            ++list->count;
        }
    }
    return true;
}

bool source_in_program(SourceProgram program, const char *path)
{
    int index;

    for (index = 0; (index < SOURCE_PREFIX_LIMIT) && (PROGRAM_PREFIXES[program][index][0] != '\0'); ++index) {
        if (text_starts_with(path, PROGRAM_PREFIXES[program][index])) {
            return true;
        }
    }
    return false;
}

static size_t name_start(const char *path, size_t length)
{
    size_t start = 0U;
    size_t index;

    if (!CHECK(length < SOURCE_PATH_CAPACITY)) {
        return 0U;
    }
    for (index = 0U; index < length; ++index) {
        start = (path[index] == '/') ? index + 1U : start;
    }
    return start;
}

bool source_built_path(char *destination, const char *folder, const char *source, const char *suffix)
{
    size_t length = text_length(source, SOURCE_PATH_CAPACITY);
    size_t start = name_start(source, length);
    size_t used;

    if (!CHECK(length > start + 2U)) {
        return false;
    }
    if (!text_join(destination, SOURCE_PATH_CAPACITY, folder, "/")) {
        return false;
    }
    used = text_length(destination, SOURCE_PATH_CAPACITY);
    return text_copy_span(&destination[used], SOURCE_PATH_CAPACITY - used, &source[start], length - start - 2U) &&
           text_append(destination, SOURCE_PATH_CAPACITY, suffix);
}

bool source_is_fresh(const SourceFile *source, const char *built_path, long long newest_header)
{
    long long built = source_file_modified(built_path);

    if (!CHECK(newest_header >= 0LL)) {
        return false;
    }
    return (built > source->modified) && (built > newest_header);
}

static bool write_object(FILE *out, const char *folder, const char *path)
{
    char object[SOURCE_PATH_CAPACITY];

    return source_built_path(object, folder, path, ".o") && (fputs(object, out) != EOF) && (fputc('\n', out) != EOF);
}

bool source_write_objects(FILE *out, const SourceList *list, SourceProgram program, const char *folder)
{
    const char *path;
    int index;

    for (index = 0; index < list->count; ++index) {
        path = list->files[index].path;
        if (source_in_program(program, path) && !write_object(out, folder, path)) {
            return false;
        }
    }
    return (program > SOURCE_PROGRAM_TESTS) || (fputs(LIBRARIES, out) >= 0);
}

long long source_list_newest(const SourceList *list)
{
    long long newest = 0LL;
    int index;

    for (index = 0; index < list->count; ++index) {
        if (list->files[index].modified > newest) {
            newest = list->files[index].modified;
        }
    }
    return newest;
}

long long source_file_modified(const char *path)
{
    WIN32_FILE_ATTRIBUTE_DATA attributes;

    if (GetFileAttributesExA(path, GetFileExInfoStandard, &attributes) == 0) {
        return 0LL;
    }
    return file_time_value(attributes.ftLastWriteTime);
}
