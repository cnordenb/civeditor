#include "tool_tests.h"

#include <stdio.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../tools/build/build_plan.h"
#include "../../tools/build/source_list.h"

enum { LINE_CAPACITY = 256, OLDER = 5, NEWER = 9 };

static const char BROKEN[] = "tests/fixtures/broken_rules.txt";
static const char OBJECTS[] = "build/test_tool_objects.txt";

static SourceList list;

static bool lists(const char *path)
{
    int index;

    if (!CHECK(path != NULL) || !CHECK(list.count <= SOURCE_LIST_CAPACITY)) {
        return false;
    }
    for (index = 0; (index < list.count) && (index < SOURCE_LIST_CAPACITY); ++index) {
        if (text_equal(list.files[index].path, path)) {
            return true;
        }
    }
    return false;
}

static bool is_fresh_against(long long source_time, long long header_time)
{
    SourceFile source = { "", 0LL };

    if (!CHECK(source_time >= 0LL) || !CHECK(text_copy(source.path, SOURCE_PATH_CAPACITY, BROKEN))) {
        return false;
    }
    source.modified = source_time;
    return source_is_fresh(&source, BROKEN, header_time);
}

static bool write_objects(SourceProgram program)
{
    FILE *out = fopen(OBJECTS, "w");
    bool written;

    if (out == NULL) {
        return false;
    }
    written = source_write_objects(out, &list, program, "obj");
    return (fclose(out) == 0) && written;
}

static size_t read_objects(char *written, size_t capacity)
{
    FILE *in = fopen(OBJECTS, "r");
    size_t length;

    if (in == NULL) {
        return 0U;
    }
    length = fread(written, 1U, capacity - 1U, in);
    (void)fclose(in);
    written[length] = '\0';
    return length;
}

static bool objects_are(SourceProgram program, const char *expected)
{
    char written[LINE_CAPACITY] = "";

    if (!CHECK(expected != NULL) || !CHECK(program < SOURCE_PROGRAM_COUNT)) {
        return false;
    }
    list.count = 2;
    (void)CHECK(text_copy(list.files[0].path, SOURCE_PATH_CAPACITY, "src/core/text.c"));
    (void)CHECK(text_copy(list.files[1].path, SOURCE_PATH_CAPACITY, "tools/build/build.c"));
    return write_objects(program) && (read_objects(written, sizeof written) > 0U) && text_equal(written, expected);
}

bool test_the_sources_of_a_folder_include_those_of_its_folders(void)
{
    return source_list_collect(&list, "tests", ".txt") && lists(BROKEN);
}

static SourceList fresh;

bool test_a_folder_lists_only_its_own_sources(void)
{
    int index;

    if (!source_list_collect(&fresh, "tests", ".txt")) {
        return false;
    }
    for (index = 0; index < fresh.count; ++index) {
        if (!text_starts_with(fresh.files[index].path, "tests/")) {
            return false;
        }
    }
    return fresh.count > 0;
}

static SourceList part;

static int sources_in(const char *root)
{
    return source_list_collect(&part, root, ".h") ? part.count : -1;
}

bool test_the_sources_of_the_project_are_those_of_src_tests_and_tools(void)
{
    int expected = sources_in("src") + sources_in("tests") + sources_in("tools");

    return source_list_collect_all(&list, ".h") && (list.count == expected);
}

bool test_the_newest_source_is_the_one_changed_last(void)
{
    list.count = 2;
    list.files[0].modified = NEWER;
    list.files[1].modified = OLDER;
    return source_list_newest(&list) == NEWER;
}

bool test_the_object_of_a_source_is_named_after_it_in_the_folder_given(void)
{
    char object[SOURCE_PATH_CAPACITY] = "";

    return source_built_path(object, "build/obj_quick", "src/core/text.c", ".o") &&
           text_equal(object, "build/obj_quick/text.o");
}

bool test_a_source_older_than_its_object_is_not_built_again(void)
{
    return is_fresh_against(0LL, 0LL);
}

bool test_a_source_with_a_newer_header_is_built_again(void)
{
    return !is_fresh_against(0LL, source_file_modified(BROKEN) + 1LL);
}

bool test_a_source_changed_when_its_object_was_made_is_built_again(void)
{
    return !is_fresh_against(source_file_modified(BROKEN), 0LL);
}

bool test_the_objects_of_the_build_tool_are_listed_without_libraries(void)
{
    return objects_are(SOURCE_PROGRAM_BUILD_TOOL, "obj/text.o\nobj/build.o\n");
}

bool test_a_coverage_build_is_asked_for_by_coverage(void)
{
    return build_mode_of("build.exe coverage") == BUILD_MODE_COVERAGE;
}

bool test_the_tests_are_run_with_the_arguments_given_to_the_build(void)
{
    return text_equal(build_test_arguments("build.exe quick nowindow norules"), " nowindow norules");
}

bool test_a_coverage_build_links_the_tests_alone(void)
{
    BuildPrograms programs = build_programs_of(BUILD_MODE_COVERAGE);

    return (programs.first == SOURCE_PROGRAM_TESTS) && (programs.end == SOURCE_PROGRAM_BUILD_TOOL);
}

bool test_a_coverage_build_links_the_tests_with_coverage(void)
{
    return text_contains_folded(build_link_command(SOURCE_PROGRAM_TESTS, BUILD_MODE_COVERAGE),
                                "gcc -g --coverage -o build/tests.exe");
}

bool test_a_coverage_build_makes_the_folder_of_its_report(void)
{
    return text_equal(build_folder(BUILD_MODE_COVERAGE, build_folder_count(BUILD_MODE_COVERAGE) - 1),
                      "target/coverage/c");
}
