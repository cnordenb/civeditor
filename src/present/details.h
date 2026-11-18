#ifndef DETAILS_H
#define DETAILS_H

#include "../data/catalog.h"
#include "../model/civ_design.h"
#include "../view/layout.h"
#include "summary.h"

typedef SummaryText DetailText;

DetailText details_of(const CivDesign *design, const Catalog *catalog, ControlId control);
DetailText details_title(WizardStep step);
DetailText details_window_title(const CivDesign *design, WizardStep step);
DetailText details_changed(int count);

#endif
