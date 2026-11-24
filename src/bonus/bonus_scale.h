#ifndef BONUS_SCALE_H
#define BONUS_SCALE_H

#include "../dat/dat_edit.h"
#include "../model/civ_design.h"

typedef struct {
    DatChange changes[CIV_MULTIPLIER_LIMIT];
    int count;
} ScaledChanges;

double bonus_raised(double base, double power);
ScaledChanges bonus_scale(const DatChange *change, double times);
bool bonus_add_scaled(DatEdit *edit, const DatChange *change, double times);

#endif
