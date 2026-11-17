#include "look.h"

static int look_civ_of(const Catalog *catalog, int entry)
{
    if (entry == CATALOG_MISSING) {
        return LOOK_NONE;
    }
    return catalog->appearance[entry].look_civ;
}

static void put_monk(const Catalog *catalog, int entry, LookMonk *monk)
{
    int unit;

    monk->civ = look_civ_of(catalog, entry);
    for (unit = 0; unit < CATALOG_LOOK_UNITS; ++unit) {
        monk->units[unit] = (monk->civ == LOOK_NONE) ? LOOK_NONE : catalog->appearance[entry].look_units[unit];
    }
}

bool look_sources_of(const Catalog *catalog, const CivDesign *design, LookSources *sources)
{
    int architecture;

    architecture = catalog_find_look(catalog, APPEARANCE_ARCHITECTURE, design->choices[CHOICE_ARCHITECTURE]);
    sources->architecture = look_civ_of(catalog, architecture);
    sources->castle = design->choices[CHOICE_CASTLE];
    sources->wonder = design->choices[CHOICE_WONDER];
    put_monk(catalog, catalog_find_look(catalog, APPEARANCE_MONK, design->choices[CHOICE_MONK]), &sources->monk);
    return true;
}

bool look_dress_units(const DatTarget *target, const Catalog *catalog, const CivDesign *design)
{
    const UnitLook *look;
    int index;
    bool fine = true;

    for (index = 0; fine && (index < catalog->unit_look_count) && (index < CATALOG_UNIT_LOOK_CAPACITY); ++index) {
        look = &catalog->unit_looks[index];
        if (civ_design_has_bonus(design, look->bonus_id)) {
            fine = look_copy_unit(target, look);
        }
    }
    return fine;
}
