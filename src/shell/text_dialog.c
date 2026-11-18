#include "shell.h"

#include "../core/check.h"

enum {
    TEMPLATE_WORDS = 1024,
    TEXT_LIMIT = 256,
    ANSWER_ID = 100,
    FONT_POINTS = 10,
    MARGIN = 8,
    WIDTH = 260,
    LABEL_HEIGHT = 24,
    FIELD_HEIGHT = 18,
    BUTTON_WIDTH = 70,
    BUTTON_HEIGHT = 26,
    ITEM_COUNT = 4,
    STATIC_CLASS = 0x0082,
    EDIT_CLASS = 0x0081,
    BUTTON_CLASS = 0x0080
};

enum {
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

typedef struct {
    const wchar_t *question;
    wchar_t *answer;
    size_t capacity;
} TextQuestion;

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

static void make_template(const wchar_t *question)
{
    static const short WHOLE[4] = { 0, 0, WIDTH, HEIGHT };
    DialogItem items[ITEM_COUNT] = {
        { SS_LEFT, { MARGIN, MARGIN, INNER_WIDTH, LABEL_HEIGHT }, 0xFFFFU, STATIC_CLASS, L"" },
        { WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL, { MARGIN, FIELD_TOP, INNER_WIDTH, FIELD_HEIGHT }, ANSWER_ID,
          EDIT_CLASS, L"" },
        { BS_DEFPUSHBUTTON | WS_TABSTOP, { WIDTH - (2 * (BUTTON_WIDTH + MARGIN)), BUTTON_TOP, BUTTON_WIDTH,
          BUTTON_HEIGHT }, IDOK, BUTTON_CLASS, L"OK" },
        { BS_PUSHBUTTON | WS_TABSTOP, { WIDTH - (BUTTON_WIDTH + MARGIN), BUTTON_TOP, BUTTON_WIDTH, BUTTON_HEIGHT },
          IDCANCEL, BUTTON_CLASS, L"Cancel" }
    };
    int item;

    items[0].text = question;
    length = 0;
    put_long(DS_MODALFRAME | DS_SETFONT | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU);
    put_long(0U);
    put_word(ITEM_COUNT);
    put_box(WHOLE);
    put_word(0U);
    put_word(0U);
    put_text(L"civbuilder");
    put_word(FONT_POINTS);
    put_text(L"Segoe UI");
    for (item = 0; item < ITEM_COUNT; ++item) {
        put_item(&items[item]);
    }
}

static INT_PTR start_answer(HWND dialog, const TextQuestion *question)
{
    HWND field = GetDlgItem(dialog, ANSWER_ID);

    (void)SetWindowTextW(field, question->answer);
    (void)SendMessageW(field, EM_LIMITTEXT, (WPARAM)(question->capacity - 1U), 0);
    (void)SendMessageW(field, EM_SETSEL, 0, -1);
    (void)SetFocus(field);
    return FALSE;
}

static INT_PTR CALLBACK answer_proc(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam)
{
    TextQuestion *question;
    LONG_PTR kept;
    WORD command = LOWORD(wparam);

    if (message == WM_INITDIALOG) {
        (void)SetWindowLongPtrW(dialog, DWLP_USER, lparam);
        return start_answer(dialog, (const TextQuestion *)lparam);
    }
    if ((message != WM_COMMAND) || ((command != IDOK) && (command != IDCANCEL))) {
        return FALSE;
    }
    kept = GetWindowLongPtrW(dialog, DWLP_USER);
    question = (TextQuestion *)kept;
    if ((command == IDOK) && (question != NULL)) {
        (void)GetDlgItemTextW(dialog, ANSWER_ID, question->answer, (int)question->capacity);
    }
    (void)EndDialog(dialog, command);
    return TRUE;
}

bool text_dialog_show(HWND owner, const wchar_t *question, wchar_t *answer, size_t capacity)
{
    TextQuestion asked;
    INT_PTR chosen;

    if (!CHECK(question != NULL) || !CHECK(answer != NULL) || !CHECK(capacity > 1U)) {
        return false;
    }
    asked.question = question;
    asked.answer = answer;
    asked.capacity = capacity;
    make_template(question);
    chosen = DialogBoxIndirectParamW(GetModuleHandleW(NULL), (LPCDLGTEMPLATEW)bytes.aligned, owner, answer_proc,
                                     (LPARAM)&asked);
    return (chosen == IDOK) && (answer[0] != L'\0');
}
