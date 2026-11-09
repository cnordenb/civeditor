#ifndef DEFLATE_H
#define DEFLATE_H

#include "../pack/inflate.h"

bool deflate_pack(ByteSpan plain, ByteBuffer *packed);

#endif
