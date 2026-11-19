# Instructions for AI agents

civbuilder: designs a new civilization for Age of Empires II: Definitive Edition and builds it into a mod, in C with
the Win32 API.

- Stack: C11, the Win32 API, gcc from MinGW-w64; nothing else (`sitrep&instructions/instructions.md`)
- Build the build program once: `gcc -o build.exe tools/build/*.c src/core/*.c`
- Build and run the tests: `build.exe` (with the static analyzer) or `build.exe quick`
- Requirements 10, 11 and 12 are measured by ub-mode's crapper, dryer and mutator only, which lie beside the project
  in `..\crapper`, `..\dryer` and `..\mutator`. Run them from the project's folder. They pass by every folder named
  `tests` or `build`; name the files of `tools/build/` to measure them.
- Measure Requirement 10: every `.c` file touched (`find src tests tools -name "*.c" -exec touch {} +`), then
  `.\build.exe coverage redundancy`, then
  `..\crapper\.venv\Scripts\python.exe -m gcovr --root . --lcov target/coverage/c/lcov.info build/obj_coverage`,
  then `..\crapper\crapper.cmd --use-existing-coverage` scores every function (`.metrics\crap.edn`)
- Measure Requirement 11: `..\dryer\dryer.cmd --threshold 0.8` lists every two functions 80% alike or more
- Requirement 12: `..\mutator\mutator.cmd --use-existing-coverage --test-command ".\build.exe quick nowindow norules
  redundancy" src/<folder>` after every change lists the mutants that survive (`.metrics\mutate\`); leave `nowindow` out for the
  folders of the window
- Ask before a run that opens the window: the mutator without `nowindow` (a worker per core, each opening the
  window, leaves the computer unusable for the whole run) and any build without `nowindow` (a few minutes).

## Read before changing anything

1. `documentation/RULES.md`: how the project must be made. The rules come before any habit or default of yours.
2. `documentation/REQUIREMENTS.md`: what the project must do.
3. `documentation/TERMINOLOGY.md`: the project's words. Use them, in the documents and in the code.
4. The newest part of `documentation/DEVLOG/`: what was done last, what was left undone, and the open questions.
5. `documentation/DECISIONS.md`: what has been decided. Do not reopen a decision without a new reason.
6. `documentation/MAINTAINER.md`, when the change touches code: how the code works.
7. `sitrep&instructions/instructions.md`: P10, Head First Design Patterns and Clean Code are followed, and only C and Win32 are used.

## Never

- Commit, push, merge, rebase, cherry-pick, or open or merge a pull request (Rule 0). Leave every change in the working
  tree for a human to review and commit.
- Change `documentation/REQUIREMENTS.md` (Rule 2). If a requirement looks wrong, unclear or in conflict with a rule, ask.
- Change `documentation/RULES.md` unless the chief developer asks for that change.
- Name a person or an agent in the development log (Rule 17).
- Rewrite an earlier section of the log. A correction is a new section that says what it corrects.
- Delete a file or a folder. List it in `documentation/DELETION_REQUESTS.md` for a human instead.
- Add code in a language other than C, or a dependency other than the Win32 API.

## A change is done when

1. The tests pass, and a test of a requirement or of a bug found before covers what changed (Rules 5, 6).
2. `TESTS.md` matches the tests: their numbers, files, lines and purposes (Rule 6).
3. `MAINTAINER.md` still explains the code and how to maintain it, its first chapter still first (Rules 7 and 8).
4. `TERMINOLOGY.md` defines every new term (Rule 9).
5. The development log has a section on the change, and `DECISIONS.md` lists every decision in it (Rules 16 to 23).
6. `build.exe` passes: no warning, no finding of the analyzer, every test passed.

Then report what was done, how it was checked, and what was not done. "Not done" is never left out to look complete.

## When something is unclear

Do not guess silently, and do not stop at the first doubt. Take the reading that breaks the fewest rules and
requirements, carry it out, and write it under "Questions" in the log's section, numbered, with what the chief developer
needs to decide and a proposal. Ask the same questions in your reply. When they are answered, the next section records
the answers and `DECISIONS.md` the decisions.

## Procedures

Each procedure is a file in `.claude/skills/`. Claude Code loads them by itself; any other agent reads the matching
`SKILL.md` before that task.

| Task | Procedure |
| --- | --- |
| Write in the development log, record decisions and answers, start a new part | `devlog` |
| Add, remove, renumber or describe a test in `TESTS.md` | `tests-doc` |
| Keep `MAINTAINER.md` true after a change | `maintainer-guide` |
| Add or change a term in `TERMINOLOGY.md` | `terminology` |
| Check the project against `RULES.md` and `REQUIREMENTS.md` | `rules-check` |
