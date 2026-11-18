#include "patch_tests.h"

#include "../../../src/core/check.h"
#include "../../../src/dat/dat_edit.h"
#include "../../dat/small_dat.h"

enum { ONE_CIV = 1, FIRST_BYTE = 0, PATCHED_VALUE = 1 };

bool test_an_edit_knows_the_first_place_it_patched(void)
{
    return CHECK(small_dat_start(ONE_CIV)) && CHECK(dat_edit_patch_i16(&small.edit, FIRST_BYTE, PATCHED_VALUE)) &&
           dat_edit_is_patched(&small.edit, FIRST_BYTE);
}
