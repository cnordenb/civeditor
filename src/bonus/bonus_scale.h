#ifndef BONUS_SCALE_H
#define BONUS_SCALE_H

#include "../dat/dat_edit.h"
#include "../model/civ_design.h"

typedef struct {
    DatChange changes[CIV_MULTIPLIER_LIMIT];
    int count;
} ScaledChanges;

ScaledChanges bonus_scale(const DatChange *change, int multiplier);
bool bonus_add_scaled(DatEdit *edit, const DatChange *change, int multiplier);

#endif
