#ifndef PACK_TESTS_H
#define PACK_TESTS_H

#include <stdbool.h>

bool test_a_dynamic_huffman_stream_inflates(void);
bool test_a_fixed_huffman_stream_inflates(void);
bool test_a_stored_stream_inflates_back(void);
bool test_packed_data_unpacks_to_the_same_bytes(void);
bool test_repeated_data_packs_small(void);

#endif
