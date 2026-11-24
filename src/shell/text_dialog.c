#include "shell.h"

#include <uxtheme.h>
#include <wchar.h>

#include "../core/check.h"

enum {
    TEMPLATE_WORDS = 1024,
    TEXT_LIMIT = 256,
    FONT_POINTS = 10,
    MARGIN = 8,
    WIDTH = 260,
    TICKS_WIDTH = 450,
    LABEL_HEIGHT = 24,
    FIELD_HEIGHT = 18,
    LIST_HEIGHT = 120,
    BUTTON_WIDTH = 70,
    BUTTON_HEIGHT = 26,
    ITEM_LIMIT = 9,
    LIST_ITEMS = 4,
    ANSWER_END = 6,
    CHECK_ITEM = 6,
    STATIC_CLASS = 0x0082,
    EDIT_CLASS = 0x0081,
    BUTTON_CLASS = 0x0080,
    LIST_CLASS = 0x0083,
    MESSAGE_LIMIT = 2147483647,
    STILL_OPEN = 0
};

enum {
    FIELD_TOP = MARGIN + LABEL_HEIGHT,
    FIELD_ROOM = LABEL_HEIGHT + FIELD_HEIGHT + MARGIN,
    LIST_TOP = FIELD_ROOM + MARGIN + LABEL_HEIGHT,
    LIST_ROOM = FIELD_ROOM + LABEL_HEIGHT + LIST_HEIGHT + MARGIN,
    BUTTON_TOP = FIELD_TOP + FIELD_HEIGHT + MARGIN,
    HEIGHT = BUTTON_TOP + BUTTON_HEIGHT + MARGIN,
    CHECK_ROOM = FIELD_HEIGHT + MARGIN
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
static AskQuestion *asked;
static WORD answered;

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

static bool is_wanted(const AskQuestion *question, int item)
{
    if (item < LIST_ITEMS) {
        return question->rows != NULL;
    }
    if (item == CHECK_ITEM) {
        return question->check != NULL;
    }
    return (question->ticks == NULL) || (item >= ANSWER_END);
}

static short shift_of(const AskQuestion *question)
{
    if (question->rows == NULL) {
        return 0;
    }
    return (short)((question->ticks != NULL) ? LIST_ROOM - FIELD_ROOM : LIST_ROOM);
}

static short drop_of(const AskQuestion *question)
{
    return (short)(shift_of(question) + ((question->check != NULL) ? CHECK_ROOM : 0));
}

static void put_head(const AskQuestion *question, short width)
{
    const short whole[4] = { 0, 0, width, (short)(HEIGHT + drop_of(question)) };
    WORD count = 0U;
    int item;

    for (item = 0; item < ITEM_LIMIT; ++item) {
        count = (WORD)(count + (is_wanted(question, item) ? 1U : 0U));
    }
    length = 0;
    put_long(DS_MODALFRAME | DS_SETFONT | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU);
    put_long(0U);
    put_word(count);
    put_box(whole);
    put_word(0U);
    put_word(0U);
    put_text(L"civeditor");
    put_word(FONT_POINTS);
    put_text(L"Segoe UI");
}

static void make_template(const AskQuestion *question)
{
    short shift = shift_of(question);
    short drop = drop_of(question);
    short width = (short)((question->ticks != NULL) ? TICKS_WIDTH : WIDTH);
    short inner = (short)(width - (2 * MARGIN));
    DWORD ticking = (question->ticks != NULL) ? (DWORD)LBS_MULTIPLESEL : 0U;
    DialogItem items[ITEM_LIMIT] = {
        { SS_LEFT, { MARGIN, MARGIN, inner, LABEL_HEIGHT }, 0xFFFFU, STATIC_CLASS, L"Filter" },
        { WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL, { MARGIN, FIELD_TOP, inner, FIELD_HEIGHT }, ASK_FILTER_ID,
          EDIT_CLASS, L"" },
        { SS_LEFT, { MARGIN, FIELD_ROOM + MARGIN, inner, LABEL_HEIGHT }, 0xFFFFU, STATIC_CLASS, question->list_label },
        { WS_BORDER | WS_TABSTOP | WS_VSCROLL | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | ticking,
          { MARGIN, LIST_TOP, inner, LIST_HEIGHT }, ASK_LIST_ID, LIST_CLASS, L"" },
        { SS_LEFT, { MARGIN, (short)(MARGIN + shift), inner, LABEL_HEIGHT }, 0xFFFFU, STATIC_CLASS,
          question->question },
        { WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL, { MARGIN, (short)(FIELD_TOP + shift), inner, FIELD_HEIGHT },
          ASK_ANSWER_ID, EDIT_CLASS, L"" },
        { BS_AUTOCHECKBOX | WS_TABSTOP, { MARGIN, (short)(BUTTON_TOP + shift), inner, FIELD_HEIGHT }, ASK_CHECK_ID,
          BUTTON_CLASS, question->check_label },
        { BS_DEFPUSHBUTTON | WS_TABSTOP,
          { (short)(width - (2 * (BUTTON_WIDTH + MARGIN))), (short)(BUTTON_TOP + drop), BUTTON_WIDTH, BUTTON_HEIGHT },
          IDOK, BUTTON_CLASS, L"OK" },
        { BS_PUSHBUTTON | WS_TABSTOP,
          { (short)(width - (BUTTON_WIDTH + MARGIN)), (short)(BUTTON_TOP + drop), BUTTON_WIDTH, BUTTON_HEIGHT },
          IDCANCEL, BUTTON_CLASS, L"Cancel" }
    };
    int item;

    put_head(question, width);
    for (item = 0; item < ITEM_LIMIT; ++item) {
        if (is_wanted(question, item)) {
            put_item(&items[item]);
        }
    }
}

static bool row_holds(const wchar_t *row, const wchar_t *filter)
{
    wchar_t folded[ASK_ROW_WIDTH];

    (void)lstrcpynW(folded, row, ASK_ROW_WIDTH);
    (void)CharLowerW(folded);
    return wcsstr(folded, filter) != NULL;
}

static void fill_list(HWND dialog)
{
    HWND list = GetDlgItem(dialog, ASK_LIST_ID);
    wchar_t filter[ASK_ROW_WIDTH] = L"";
    LRESULT placed;
    int row;

    (void)GetDlgItemTextW(dialog, ASK_FILTER_ID, filter, ASK_ROW_WIDTH);
    (void)CharLowerW(filter);
    (void)SendMessageW(list, LB_RESETCONTENT, 0, 0);
    for (row = 0; row < asked->row_count; ++row) {
        if (row_holds(asked->rows[row], filter)) {
            placed = SendMessageW(list, LB_ADDSTRING, 0, (LPARAM)asked->rows[row]);
            (void)SendMessageW(list, LB_SETITEMDATA, (WPARAM)placed, (LPARAM)row);
            if ((asked->ticks != NULL) && asked->ticks[row]) {
                (void)SendMessageW(list, LB_SETSEL, TRUE, placed);
            } else if (row == asked->row) {
                (void)SendMessageW(list, LB_SETCURSEL, (WPARAM)placed, 0);
            }
        }
    }
}

static INT_PTR start_answer(HWND dialog)
{
    HWND field = GetDlgItem(dialog, ASK_ANSWER_ID);
    HWND list = GetDlgItem(dialog, ASK_LIST_ID);

    fill_list(dialog);
    (void)SetWindowTheme(list, L"", L"");
    (void)SetWindowTextW(field, asked->answer);
    (void)SendMessageW(field, EM_LIMITTEXT, (WPARAM)(asked->capacity - 1U), 0);
    (void)SendMessageW(field, EM_SETSEL, 0, -1);
    if (asked->check != NULL) {
        (void)CheckDlgButton(dialog, ASK_CHECK_ID, *asked->check ? BST_CHECKED : BST_UNCHECKED);
    }
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
    asked->row = (int)SendDlgItemMessageW(dialog, ASK_LIST_ID, LB_GETITEMDATA, (WPARAM)picked, 0);
    (void)SendDlgItemMessageW(dialog, ASK_LIST_ID, LB_GETTEXT, (WPARAM)picked, (LPARAM)row);
    (void)SetDlgItemTextW(dialog, ASK_ANSWER_ID, row);
}

static void take_ticks(HWND dialog)
{
    HWND list = GetDlgItem(dialog, ASK_LIST_ID);
    LRESULT count = SendMessageW(list, LB_GETCOUNT, 0, 0);
    LRESULT placed;
    LRESULT row;

    for (placed = 0; placed < count; ++placed) {
        row = SendMessageW(list, LB_GETITEMDATA, (WPARAM)placed, 0);
        asked->ticks[row] = SendMessageW(list, LB_GETSEL, (WPARAM)placed, 0) > 0;
    }
}

static void take_choice(HWND dialog)
{
    if (asked->ticks != NULL) {
        take_ticks(dialog);
    } else {
        take_row(dialog);
    }
}

static void end_answer(HWND dialog, WORD command)
{
    if (command == IDOK) {
        (void)GetDlgItemTextW(dialog, ASK_ANSWER_ID, asked->answer, (int)asked->capacity);
    }
    if ((command == IDOK) && (asked->check != NULL)) {
        *asked->check = IsDlgButtonChecked(dialog, ASK_CHECK_ID) == BST_CHECKED;
    }
    answered = command;
}

static INT_PTR answer_command(HWND dialog, WPARAM wparam)
{
    WORD command = LOWORD(wparam);

    if ((command == ASK_LIST_ID) && (HIWORD(wparam) == LBN_SELCHANGE)) {
        take_choice(dialog);
        return TRUE;
    }
    if ((command == ASK_FILTER_ID) && (HIWORD(wparam) == EN_CHANGE)) {
        fill_list(dialog);
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
    (void)lparam;
    if (message == WM_INITDIALOG) {
        return start_answer(dialog);
    }
    return (message == WM_COMMAND) ? answer_command(dialog, wparam) : FALSE;
}

static void run_dialog(HWND owner, HWND dialog)
{
    MSG message;
    int step;

    (void)EnableWindow(owner, FALSE);
    (void)ShowWindow(dialog, SW_SHOW);
    for (step = 0; (step < MESSAGE_LIMIT) && (answered == STILL_OPEN); ++step) {
        if (GetMessageW(&message, NULL, 0, 0) <= 0) {
            PostQuitMessage(0);
            answered = IDCANCEL;
        } else if (!list_scroll_takes(&message) && (IsDialogMessageW(dialog, &message) == 0)) {
            (void)TranslateMessage(&message);
            (void)DispatchMessageW(&message);
        }
    }
    (void)EnableWindow(owner, TRUE);
}

static bool takes_no_text(const AskQuestion *question)
{
    return (question->ticks != NULL) || (question->check != NULL);
}

bool text_dialog_show(HWND owner, AskQuestion *question)
{
    HWND dialog;

    if (!CHECK(question->capacity > 1U)) {
        return false;
    }
    make_template(question);
    asked = question;
    answered = STILL_OPEN;
    dialog = CreateDialogIndirectParamW(GetModuleHandleW(NULL), (LPCDLGTEMPLATEW)bytes.aligned, owner, answer_proc, 0);
    if (dialog == NULL) {
        return false;
    }
    if (answered == STILL_OPEN) {
        run_dialog(owner, dialog);
    }
    (void)DestroyWindow(dialog);
    return (answered == IDOK) && (takes_no_text(question) || (question->answer[0] != L'\0'));
}
