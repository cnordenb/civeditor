# Instructions for AI agents

civbuilder: designs a new civilization for Age of Empires II: Definitive Edition and builds it into a mod, in C with
the Win32 API.

- Stack: C11, the Win32 API, gcc from MinGW-w64; nothing else (`sitrep&instructions/instructions.md`)
- Build the build program once: `gcc -o build.exe tools/build/*.c src/core/*.c`
- Build and run the tests: `build.exe` (with the static analyzer) or `build.exe quick`
- Measure Rule 32 after a build: `build\crapper.exe` lists every function at CRAP 10 or above (`build\crap.txt`)
- Measure Rule 31 after a build: `build\dryer.exe` lists every two functions 80% alike or more (`build\dry.txt`)
- Measure Rule 34 after a build: `build\mutator.exe src/<folder>/` lists the mutants that survive (`build\mutants.txt`)

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

1. The tests pass, and what changed is tested, within the 100 tests Rule 5 allows.
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
