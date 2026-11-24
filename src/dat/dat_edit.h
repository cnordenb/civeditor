#ifndef DAT_EDIT_H
#define DAT_EDIT_H

#include <stdbool.h>
#include <stddef.h>

#include "dat_map.h"

enum {
    DAT_PATCH_CAPACITY = 32768,
    DAT_PATCH_WIDTH_LIMIT = 8,
    DAT_COPY_CAPACITY = 16384,
    DAT_ADDED_EFFECTS_CAPACITY = 4194304,
    DAT_ADDED_TECHS_CAPACITY = 2097152,
    DAT_NO_ID = -1
};

typedef struct {
    size_t at;
    size_t width;
    unsigned char bytes[DAT_PATCH_WIDTH_LIMIT];
} DatPatch;

typedef struct {
    size_t at;
    size_t from;
    size_t width;
} DatCopy;

typedef struct {
    size_t effect_count_at;
    int old_effect_count;
    size_t effects_end_at;
    size_t tech_count_at;
    int old_tech_count;
    size_t techs_end_at;
    DatPatch patches[DAT_PATCH_CAPACITY];
    int patch_count;
    DatCopy copies[DAT_COPY_CAPACITY];
    int copy_count;
    unsigned char effects[DAT_ADDED_EFFECTS_CAPACITY];
    size_t effects_length;
    int effect_count;
    size_t change_count_at;
    int change_count;
    unsigned char techs[DAT_ADDED_TECHS_CAPACITY];
    size_t techs_length;
    int tech_count;
} DatEdit;

typedef struct {
    unsigned char *data;
    size_t capacity;
    size_t length;
} DatOutput;

typedef struct {
    const DatBytes *dat;
    const DatMap *map;
    DatEdit *edit;
    int civ;
} DatTarget;

bool dat_edit_start(DatEdit *edit, const DatMap *map);
bool dat_edit_patch(DatEdit *edit, const DatPatch *patch);
bool dat_edit_is_patched(const DatEdit *edit, size_t at);
bool dat_edit_patch_u8(DatEdit *edit, size_t at, int value);
bool dat_edit_patch_i16(DatEdit *edit, size_t at, int value);
bool dat_edit_patch_i32(DatEdit *edit, size_t at, long value);
bool dat_edit_patch_f32(DatEdit *edit, size_t at, float value);
bool dat_edit_patch_cost(DatEdit *edit, size_t at, const DatTechCost *cost);
bool dat_edit_copy(DatEdit *edit, const DatCopy *copy);
int dat_edit_add_effect(DatEdit *edit, const char *name);
bool dat_edit_add_change(DatEdit *edit, const DatChange *change);
int dat_edit_add_tech(DatEdit *edit, const DatTech *tech);
bool dat_edit_write(const DatEdit *edit, const DatBytes *original, DatOutput *output);

#endif
