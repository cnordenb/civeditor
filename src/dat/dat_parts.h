#ifndef DAT_PARTS_H
#define DAT_PARTS_H

#include <stdbool.h>
#include <stddef.h>

#include "dat_edit.h"
#include "dat_map.h"

enum {
    DAT_TEXT_MARKER = 0x0A60,
    DAT_COUNT_LIMIT = 32768,
    DAT_BYTE_WIDTH = 1,
    DAT_SHORT_WIDTH = 2,
    DAT_LONG_WIDTH = 4,
    DAT_POINTER_SIZE = 4,
    DAT_PLAYER_TYPE_SIZE = 1,
    DAT_RESOURCE_COUNT_SIZE = 2
};

typedef struct {
    const unsigned char *data;
    size_t size;
    size_t at;
    bool fine;
} DatCursor;

typedef struct {
    unsigned char *bytes;
    size_t capacity;
    size_t length;
    bool fine;
} DatSink;

bool dat_cursor_start(DatCursor *cursor, const DatBytes *dat, size_t at);
bool dat_cursor_skip(DatCursor *cursor, size_t count);
bool dat_cursor_skip_items(DatCursor *cursor, long count, size_t item_size);
int dat_cursor_u8(DatCursor *cursor);
int dat_cursor_i16(DatCursor *cursor);
long dat_cursor_i32(DatCursor *cursor);
float dat_cursor_f32(DatCursor *cursor);
bool dat_cursor_skip_text(DatCursor *cursor);
bool dat_cursor_text(DatCursor *cursor, char *text, size_t capacity);

DatSink dat_sink_over(unsigned char *bytes, size_t capacity, size_t length);
bool dat_edit_patch_sunk(DatEdit *edit, size_t at, DatPatch *patch, const DatSink *sink);
void dat_sink_u8(DatSink *sink, int value);
void dat_sink_i16(DatSink *sink, int value);
void dat_sink_i32(DatSink *sink, long value);
void dat_sink_f32(DatSink *sink, float value);
void dat_sink_text(DatSink *sink, const char *text);

bool dat_skip_head(DatCursor *cursor);
bool dat_skip_unit_headers(DatCursor *cursor);
int dat_skip_civ_head(DatCursor *cursor);
bool dat_read_unit(DatCursor *cursor, DatUnit *unit);
bool dat_skip_unit(DatCursor *cursor);
bool dat_skip_civ(DatCursor *cursor);

#endif
