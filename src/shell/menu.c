#include "shell.h"

#include "../core/check.h"
#include "../core/text.h"
#include "../win/win_parts.h"

enum { FIXED_KEY_COUNT = 11, KEY_COUNT = FIXED_KEY_COUNT + STEP_COUNT, MENU_TEXT_CAPACITY = 64 };

static const ACCEL FIXED_KEYS[FIXED_KEY_COUNT] = {
    { FCONTROL | FVIRTKEY, VK_NEXT, ACTION_ID_BASE + ACTION_NEXT },
    { FCONTROL | FVIRTKEY, VK_PRIOR, ACTION_ID_BASE + ACTION_BACK },
    { FCONTROL | FVIRTKEY, 'S', ACTION_ID_BASE + ACTION_SAVE },
    { FCONTROL | FVIRTKEY, 'O', ACTION_ID_BASE + ACTION_OPEN },
    { FCONTROL | FVIRTKEY, 'Z', ACTION_ID_BASE + ACTION_UNDO },
    { FCONTROL | FVIRTKEY, 'Y', ACTION_ID_BASE + ACTION_REDO },
    { FCONTROL | FSHIFT | FVIRTKEY, 'Z', ACTION_ID_BASE + ACTION_REDO },
    { FCONTROL | FVIRTKEY, 'B', ACTION_ID_BASE + ACTION_BUILD },
    { FVIRTKEY, VK_F1, ACTION_ID_BASE + ACTION_HELP },
    { FCONTROL | FVIRTKEY, VK_UP, ACTION_ID_BASE + ACTION_SCROLL_UP },
    { FCONTROL | FVIRTKEY, VK_DOWN, ACTION_ID_BASE + ACTION_SCROLL_DOWN }
};

HACCEL menu_keys(void)
{
    ACCEL keys[KEY_COUNT];
    int index;

    if (!CHECK(KEY_COUNT > FIXED_KEY_COUNT) || !CHECK(STEP_COUNT <= 9)) {
        return NULL;
    }
    for (index = 0; index < KEY_COUNT; ++index) {
        if (index < FIXED_KEY_COUNT) {
            keys[index] = FIXED_KEYS[index];
        } else {
            keys[index].fVirt = FCONTROL | FVIRTKEY;
            keys[index].key = (WORD)('1' + (index - FIXED_KEY_COUNT));
            keys[index].cmd = (WORD)(ACTION_ID_BASE + ACTION_FIRST_STEP + (index - FIXED_KEY_COUNT));
        }
    }
    return CreateAcceleratorTableW(keys, KEY_COUNT);
}

static void add_item(HMENU menu, int action, const wchar_t *text)
{
    (void)AppendMenuW(menu, MF_STRING, (UINT_PTR)(ACTION_ID_BASE + action), text);
}

static void append_title(char *text, size_t capacity, const char *title)
{
    char letter[2] = { '\0', '\0' };
    int index;

    for (index = 0; (index < STEP_TITLE_WIDTH) && (title[index] != '\0'); ++index) {
        letter[0] = title[index];
        (void)text_append(text, capacity, (letter[0] == '&') ? "&&" : letter);
    }
}

static HMENU make_go_menu(void)
{
    HMENU go = CreatePopupMenu();
    char text[MENU_TEXT_CAPACITY];
    wchar_t wide[MENU_TEXT_CAPACITY];
    int step;

    add_item(go, ACTION_NEXT, L"&Next\tCtrl+Page Down");
    add_item(go, ACTION_BACK, L"&Back\tCtrl+Page Up");
    add_item(go, ACTION_SCROLL_UP, L"Scroll &up\tCtrl+Up");
    add_item(go, ACTION_SCROLL_DOWN, L"Scroll &down\tCtrl+Down");
    (void)AppendMenuW(go, MF_SEPARATOR, 0, NULL);
    for (step = 0; step < STEP_COUNT; ++step) {
        text[0] = '&';
        text[1] = '\0';
        (void)text_append_number(text, sizeof text, (long)step + 1L);
        (void)text_append(text, sizeof text, " ");
        append_title(text, sizeof text, wizard_page((WizardStep)step)->title);
        (void)text_append(text, sizeof text, "\tCtrl+");
        (void)text_append_number(text, sizeof text, (long)step + 1L);
        if (dialogs_widen(text, wide, MENU_TEXT_CAPACITY)) {
            add_item(go, ACTION_FIRST_STEP + step, wide);
        }
    }
    return go;
}

HMENU menu_make(void)
{
    HMENU bar = CreateMenu();
    HMENU file = CreatePopupMenu();
    HMENU edit = CreatePopupMenu();
    HMENU go = make_go_menu();
    HMENU help = CreatePopupMenu();

    add_item(file, ACTION_OPEN, L"&Open civ file...\tCtrl+O");
    add_item(file, ACTION_SAVE, L"&Save civ file...\tCtrl+S");
    add_item(file, ACTION_BUILD, L"&Build mod...\tCtrl+B");
    (void)AppendMenuW(file, MF_SEPARATOR, 0, NULL);
    add_item(file, ACTION_EXIT, L"E&xit\tAlt+F4");
    add_item(edit, ACTION_UNDO, L"&Undo\tCtrl+Z");
    add_item(edit, ACTION_REDO, L"&Redo\tCtrl+Y");
    (void)AppendMenuW(bar, MF_POPUP, (UINT_PTR)file, L"&File");
    (void)AppendMenuW(bar, MF_POPUP, (UINT_PTR)edit, L"&Edit");
    (void)AppendMenuW(bar, MF_POPUP, (UINT_PTR)go, L"&Go");
    add_item(help, ACTION_HELP, L"&Help for this control\tF1");
    (void)AppendMenuW(bar, MF_POPUP, (UINT_PTR)help, L"&Help");
    return bar;
}
