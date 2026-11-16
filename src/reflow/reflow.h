#ifndef REFLOW_H
#define REFLOW_H

#include <stdbool.h>

#include "../view/layout.h"

enum { REFLOW_MIN_WIDTH = 320, REFLOW_MIN_HEIGHT = 256, REFLOW_LINE = 44 };

bool reflow_is_needed(int width, int height);
Box reflow_box(ControlId control, WizardStep step, int width);
Box reflow_label_box(ControlId control, WizardStep step, int width);
int reflow_height(WizardStep step);
int reflow_scroll_limit(int content, int view);
int reflow_scroll_within(int scroll, int content, int view);
int reflow_scroll_to_show(Box box, int scroll, int view);

#endif
