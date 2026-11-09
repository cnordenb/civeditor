#ifndef DOC_RULES_H
#define DOC_RULES_H

#include <stdbool.h>

bool repo_folders_hold_few_files(void);
bool repo_file_names_are_unique(void);
bool repo_holds_only_c_and_documents(void);
bool repo_placeholders_are_filled(void);
bool devlog_keeps_its_form(void);
bool tests_document_is_numbered_in_order(void);
bool tests_document_counts_every_test(void);
bool terminology_lines_are_short(void);

#endif
