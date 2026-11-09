#include "look.h"

#include "../core/check.h"

static int look_civ_of(const Catalog *catalog, int entry)
{
    if (!CHECK(catalog != NULL) || (entry == CATALOG_MISSING) || !CHECK(entry < CATALOG_APPEARANCE_CAPACITY)) {
        return LOOK_NONE;
    }
    return catalog->appearance[entry].look_civ;
}

static void put_monk(const Catalog *catalog, int entry, LookMonk *monk)
{
    int unit;

    if (!CHECK(catalog != NULL) || !CHECK(monk != NULL)) {
        return;
    }
    monk->civ = look_civ_of(catalog, entry);
    for (unit = 0; unit < CATALOG_LOOK_UNITS; ++unit) {
        monk->units[unit] = (monk->civ == LOOK_NONE) ? LOOK_NONE : catalog->appearance[entry].look_units[unit];
    }
}

bool look_sources_of(const Catalog *catalog, const CivDesign *design, LookSources *sources)
{
    int architecture;

    if (!CHECK(catalog != NULL) || !CHECK(design != NULL) || !CHECK(sources != NULL)) {
        return false;
    }
    architecture = catalog_find_look(catalog, APPEARANCE_ARCHITECTURE, design->choices[CHOICE_ARCHITECTURE]);
    sources->architecture = look_civ_of(catalog, architecture);
    sources->castle = design->choices[CHOICE_CASTLE];
    sources->wonder = design->choices[CHOICE_WONDER];
    put_monk(catalog, catalog_find_look(catalog, APPEARANCE_MONK, design->choices[CHOICE_MONK]), &sources->monk);
    return true;
}
