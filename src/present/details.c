#include "details.h"

#include "../core/check.h"
#include "../core/text.h"

static void add_number(DetailText *detail, long value)
{
    (void)text_append_number(detail->text, sizeof detail->text, value);
}

DetailText details_of(const CivDesign *design, ControlId control)
{
    DetailText detail = { "" };

    switch ((int)control) {
    case CONTROL_TREE_INFO:
        summary_add_tree(&detail, design, " buildings and ", " techs are in the tree.");
        break;
    case CONTROL_HERO_NAME:
        summary_add(&detail, design->texts[TEXT_FIELD_HERO_NAME]);
        break;
    default:
        break;
    }
    return detail;
}

DetailText details_title(WizardStep step)
{
    DetailText detail = { "" };

    if (!CHECK((int)step >= 0) || !CHECK(step < STEP_COUNT)) {
        return detail;
    }
    summary_add(&detail, "Step ");
    add_number(&detail, (long)step + 1L);
    summary_add(&detail, " of ");
    add_number(&detail, (long)STEP_COUNT);
    summary_add(&detail, ": ");
    summary_add(&detail, wizard_page(step)->title);
    return detail;
}

DetailText details_window_title(const CivDesign *design, WizardStep step)
{
    DetailText title = details_title(step);
    DetailText detail = { "" };
    const char *name = civ_design_text(design, TEXT_FIELD_NAME);

    summary_add(&detail, (name[0] != '\0') ? name : "A new civ");
    summary_add(&detail, " - ");
    summary_add(&detail, title.text);
    summary_add(&detail, " - civeditor");
    return detail;
}

DetailText details_changed(int count)
{
    DetailText detail = { .text = "" };

    add_number(&detail, (long)count);
    summary_add(&detail, (count == 1) ? " civ changed" : " civs changed");
    return detail;
}

DetailText details_civ_row(const char *name, bool changed)
{
    DetailText detail = { .text = "" };

    summary_add(&detail, name);
    summary_add(&detail, changed ? "*" : "");
    return detail;
}
