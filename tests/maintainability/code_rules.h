#ifndef CODE_RULES_H
#define CODE_RULES_H

#include <stdbool.h>

typedef enum {
    CODE_RULE_NO_COMMENTS,
    CODE_RULE_FUNCTION_LENGTH,
    CODE_RULE_LINE_LENGTH,
    CODE_RULE_FILE_LENGTH,
    CODE_RULE_SIMPLE_CONTROL_FLOW,
    CODE_RULE_NO_ALLOCATION,
    CODE_RULE_SIMPLE_PREPROCESSOR,
    CODE_RULE_POINTER_DEPTH,
    CODE_RULE_NO_FUNCTION_POINTERS,
    CODE_RULE_COUNT
} CodeRule;

bool code_rule_holds(CodeRule rule);
bool code_has_no_recursion(void);

#endif
