#include "writer_tests.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../src/json/json_writer.h"

enum { LONG_TEXT = 1000, LONG_TEXT_COUNT = (JSON_OUTPUT_CAPACITY / LONG_TEXT) + 2 };

static JsonWriter writer;

static void begin_with_a(void)
{
    json_writer_start(&writer);
    json_begin(&writer, NULL, '{');
    json_write_number(&writer, "a", 1L);
}

bool test_two_members_are_written_with_a_comma_between(void)
{
    begin_with_a();
    json_write_number(&writer, "b", 2L);
    json_end(&writer, '}');
    return text_equal(writer.data, "{\n  \"a\": 1,\n  \"b\": 2\n}");
}

bool test_the_first_member_of_an_object_inside_another_has_no_comma(void)
{
    begin_with_a();
    json_begin(&writer, "in", '{');
    json_write_number(&writer, "b", 2L);
    json_end(&writer, '}');
    json_end(&writer, '}');
    return text_equal(writer.data, "{\n  \"a\": 1,\n  \"in\": {\n    \"b\": 2\n  }\n}");
}

bool test_an_object_left_open_is_not_a_finished_text(void)
{
    begin_with_a();
    return !json_writer_succeeded(&writer);
}

bool test_a_text_too_long_for_the_writer_is_reported(void)
{
    static char long_text[LONG_TEXT + 1];
    int index;

    for (index = 0; index < LONG_TEXT; ++index) {
        long_text[index] = 'x';
    }
    long_text[LONG_TEXT] = '\0';
    begin_with_a();
    for (index = 0; index < LONG_TEXT_COUNT; ++index) {
        json_write_string(&writer, "t", long_text);
    }
    json_end(&writer, '}');
    return !json_writer_succeeded(&writer);
}
