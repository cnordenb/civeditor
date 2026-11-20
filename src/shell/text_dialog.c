#include "shell.h"

#include <uxtheme.h>

#include "../core/check.h"

enum {
    TEMPLATE_WORDS = 1024,
    TEXT_LIMIT = 256,
    FONT_POINTS = 10,
    MARGIN = 8,
    WIDTH = 260,
    LABEL_HEIGHT = 24,
    FIELD_HEIGHT = 18,
    LIST_HEIGHT = 120,
    BUTTON_WIDTH = 70,
    BUTTON_HEIGHT = 26,
    ITEM_LIMIT = 6,
    LIST_ITEMS = 2,
    STATIC_CLASS = 0x0082,
    EDIT_CLASS = 0x0081,
    BUTTON_CLASS = 0x0080,
    LIST_CLASS = 0x0083
};

enum {
    LIST_TOP = MARGIN + LABEL_HEIGHT,
    LIST_ROOM = LABEL_HEIGHT + LIST_HEIGHT + MARGIN,
    FIELD_TOP = MARGIN + LABEL_HEIGHT,
    BUTTON_TOP = FIELD_TOP + FIELD_HEIGHT + MARGIN,
    HEIGHT = BUTTON_TOP + BUTTON_HEIGHT + MARGIN,
    INNER_WIDTH = WIDTH - (2 * MARGIN)
};

typedef struct {
    DWORD style;
    short box[4];
    WORD id;
    WORD kind;
    const wchar_t *text;
} DialogItem;

typedef union {
    DWORD aligned[TEMPLATE_WORDS / 2];
    WORD words[TEMPLATE_WORDS];
} DialogBytes;

static DialogBytes bytes;
static int length;

static void put_word(WORD word)
{
    if (CHECK(length < TEMPLATE_WORDS)) {
        bytes.words[length] = word;
        ++length;
    }
}

static void put_long(DWORD value)
{
    put_word(LOWORD(value));
    put_word(HIWORD(value));
}

static void put_text(const wchar_t *text)
{
    int index;

    for (index = 0; (index < TEXT_LIMIT) && (text[index] != L'\0'); ++index) {
        put_word((WORD)text[index]);
    }
    put_word(0U);
}

static void put_box(const short box[4])
{
    int side;

    for (side = 0; side < 4; ++side) {
        put_word((WORD)box[side]);
    }
}

static void put_item(const DialogItem *item)
{
    if ((length % 2) != 0) {
        put_word(0U);
    }
    put_long(item->style | WS_CHILD | WS_VISIBLE);
    put_long(0U);
    put_box(item->box);
    put_word(item->id);
    put_word(0xFFFFU);
    put_word(item->kind);
    put_text(item->text);
    put_word(0U);
}

static void make_template(const AskQuestion *question)
{
    short shift = (short)((question->rows != NULL) ? LIST_ROOM : 0);
    const short whole[4] = { 0, 0, WIDTH, (short)(HEIGHT + shift) };
    DialogItem items[ITEM_LIMIT] = {
        { SS_LEFT, { MARGIN, MARGIN, INNER_WIDTH, LABEL_HEIGHT }, 0xFFFFU, STATIC_CLASS, L"" },
        { WS_BORDER | WS_TABSTOP | WS_VSCROLL | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
          { MARGIN, LIST_TOP, INNER_WIDTH, LIST_HEIGHT }, ASK_LIST_ID, LIST_CLASS, L"" },
        { SS_LEFT, { MARGIN, (short)(MARGIN + shift), INNER_WIDTH, LABEL_HEIGHT }, 0xFFFFU, STATIC_CLASS, L"" },
        { WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL, { MARGIN, (short)(FIELD_TOP + shift), INNER_WIDTH, FIELD_HEIGHT },
          ASK_ANSWER_ID, EDIT_CLASS, L"" },
        { BS_DEFPUSHBUTTON | WS_TABSTOP,
          { WIDTH - (2 * (BUTTON_WIDTH + MARGIN)), (short)(BUTTON_TOP + shift), BUTTON_WIDTH, BUTTON_HEIGHT }, IDOK,
          BUTTON_CLASS, L"OK" },
        { BS_PUSHBUTTON | WS_TABSTOP,
          { WIDTH - (BUTTON_WIDTH + MARGIN), (short)(BUTTON_TOP + shift), BUTTON_WIDTH, BUTTON_HEIGHT }, IDCANCEL,
          BUTTON_CLASS, L"Cancel" }
    };
    int first = (question->rows != NULL) ? 0 : LIST_ITEMS;
    int item;

    items[0].text = question->list_label;
    items[2].text = question->question;
    length = 0;
    put_long(DS_MODALFRAME | DS_SETFONT | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU);
    put_long(0U);
    put_word((WORD)(ITEM_LIMIT - first));
    put_box(whole);
    put_word(0U);
    put_word(0U);
    put_text(L"civeditor");
    put_word(FONT_POINTS);
    put_text(L"Segoe UI");
    for (item = first; item < ITEM_LIMIT; ++item) {
        put_item(&items[item]);
    }
}

static INT_PTR start_answer(HWND dialog, const AskQuestion *question)
{
    HWND field = GetDlgItem(dialog, ASK_ANSWER_ID);
    HWND list = GetDlgItem(dialog, ASK_LIST_ID);
    int row;

    for (row = 0; row < question->row_count; ++row) {
        (void)SendMessageW(list, LB_ADDSTRING, 0, (LPARAM)question->rows[row]);
    }
    (void)SendMessageW(list, LB_SETCURSEL, (WPARAM)question->row, 0);
    (void)SetWindowTheme(list, L"", L"");
    (void)SetWindowTextW(field, question->answer);
    (void)SendMessageW(field, EM_LIMITTEXT, (WPARAM)(question->capacity - 1U), 0);
    (void)SendMessageW(field, EM_SETSEL, 0, -1);
    (void)SetFocus((list != NULL) ? list : field);
    return FALSE;
}

static void take_row(HWND dialog)
{
    wchar_t row[ASK_ROW_WIDTH];
    LRESULT picked = SendDlgItemMessageW(dialog, ASK_LIST_ID, LB_GETCURSEL, 0, 0);

    if ((picked < 0) || (SendDlgItemMessageW(dialog, ASK_LIST_ID, LB_GETTEXTLEN, (WPARAM)picked, 0) >= ASK_ROW_WIDTH)) {
        return;
    }
    (void)SendDlgItemMessageW(dialog, ASK_LIST_ID, LB_GETTEXT, (WPARAM)picked, (LPARAM)row);
    (void)SetDlgItemTextW(dialog, ASK_ANSWER_ID, row);
}

static void end_answer(HWND dialog, WORD command)
{
    LONG_PTR kept = GetWindowLongPtrW(dialog, DWLP_USER);
    AskQuestion *question = (AskQuestion *)kept;

    if ((command == IDOK) && (question != NULL)) {
        (void)GetDlgItemTextW(dialog, ASK_ANSWER_ID, question->answer, (int)question->capacity);
        question->row = (int)SendDlgItemMessageW(dialog, ASK_LIST_ID, LB_GETCURSEL, 0, 0);
    }
    (void)EndDialog(dialog, command);
}

static INT_PTR answer_command(HWND dialog, WPARAM wparam)
{
    WORD command = LOWORD(wparam);

    if ((command == ASK_LIST_ID) && (HIWORD(wparam) == LBN_SELCHANGE)) {
        take_row(dialog);
        return TRUE;
    }
    if ((command != IDOK) && (command != IDCANCEL)) {
        return FALSE;
    }
    end_answer(dialog, command);
    return TRUE;
}

static INT_PTR CALLBACK answer_proc(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_INITDIALOG) {
        (void)SetWindowLongPtrW(dialog, DWLP_USER, lparam);
        return start_answer(dialog, (const AskQuestion *)lparam);
    }
    return (message == WM_COMMAND) ? answer_command(dialog, wparam) : FALSE;
}

bool text_dialog_show(HWND owner, AskQuestion *question)
{
    INT_PTR chosen;

    if (!CHECK(question->capacity > 1U)) {
        return false;
    }
    make_template(question);
    chosen = DialogBoxIndirectParamW(GetModuleHandleW(NULL), (LPCDLGTEMPLATEW)bytes.aligned, owner, answer_proc,
                                     (LPARAM)question);
    return (chosen == IDOK) && (question->answer[0] != L'\0');
}
