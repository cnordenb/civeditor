#include "core_tests.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"

bool test_failed_check_names_its_condition(void)
{
    int width = 3;

    (void)CHECK(width > 5);
    (void)CHECK(width > 4);
    return text_equal(check_last_failure(), __FILE__ ":11: check failed: width > 4");
}
