#include "core_tests.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"

bool test_failed_check_names_its_condition(void)
{
    int width = 3;

    (void)CHECK(width > 4);
    return text_contains_folded(check_last_failure(), "core_tests.c:10: check failed: width > 4");
}
