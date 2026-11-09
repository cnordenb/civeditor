#ifndef DEFLATE_STORE_H
#define DEFLATE_STORE_H

#include "inflate.h"

size_t deflate_store_size(size_t plain_size);
bool deflate_store(ByteSpan plain, ByteBuffer *packed);

#endif
