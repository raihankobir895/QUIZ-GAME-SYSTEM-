# Project Workflow — Quiz Game

## Development Approach
The project follows an incremental, module-by-module development workflow
over 10 weeks: each week builds and integrates one functional piece of the
system on top of the previous week's work, so there is always a working
(if partial) program.
# Project Workflow — Quiz Game

## Development Approach
The project follows an incremental, module-by-module development workflow
over 10 weeks: each week builds and integrates one functional piece of the
system on top of the previous week's work, so there is always a working
(if partial) program.

## High-Level Workflow

1. **Plan** — Define objectives, requirements, and features (Week 1).
2. **Design** — Design the UI screens and overall software structure (Week 2).
3. **Build core navigation** — Main menu module (Week 3).
4. **Build data layer** — Question bank with file handling and validation (Week 4).
5. **Build gameplay** — Answer selection, correctness checking, quiz flow (Week 5).
6. **Build scoring** — Live score tracking (Week 6).
7. **Build results** — Final result summary and persistence (Week 7).
8. **Integrate storage** — Confirm all file-based persistence works end-to-end (Week 8).
9. **Harden** — Testing, error handling, performance/UI polish (Week 9).
10. **Finalize** — Full system testing, documentation, and submission (Week 10).

## Module Flow (Program Execution)

```
main()
  -> MainMenu::displayMenu()
       |
       |--(1) Start Quiz --> QuizScreen::startQuiz()
       |                        -> QuestionBank::loadQuestions()   [file I/O]
       |                        -> QuestionBank::displayQuestion() (per question)
       |                        -> (Week 5) answer check -> (Week 6) scoring
       |                        -> (Week 7) ResultScreen::showResult()  [file I/O]
       |
       |--(2) Exit --> program ends
```

## Branching / Version Control Workflow
- `main` branch holds the latest stable, compiling version of the project.
- Each week's work is developed and tested locally, then committed with a
  message referencing the week/module (e.g. "Week 4: Question Management
  Module").
- Commits are kept small and scoped to a single module where possible, so
  the project history mirrors the week-by-week schedule.

## Testing Workflow
- Each module is compiled and run manually after implementation.
- File-handling modules (QuestionBank, and later result saving) are tested
  with both valid and intentionally malformed data files to confirm
  validation and error handling work as required.

## High-Level Workflow

1. **Plan** — Define objectives, requirements, and features (Week 1).
2. **Design** — Design the UI screens and overall software structure (Week 2).
3. **Build core navigation** — Main menu module (Week 3).
4. **Build data layer** — Question bank with file handling and validation (Week 4).
5. **Build gameplay** — Answer selection, correctness checking, quiz flow (Week 5).
6. **Build scoring** — Live score tracking (Week 6).
7. **Build results** — Final result summary and persistence (Week 7).
8. **Integrate storage** — Confirm all file-based persistence works end-to-end (Week 8).
9. **Harden** — Testing, error handling, performance/UI polish (Week 9).
10. **Finalize** — Full system testing, documentation, and submission (Week 10).

## Module Flow (Program Execution)

```
main()
  -> MainMenu::displayMenu()
       |
       |--(1) Start Quiz --> QuizScreen::startQuiz()
       |                        -> QuestionBank::loadQuestions()   [file I/O]
       |                        -> QuestionBank::displayQuestion() (per question)
       |                        -> (Week 5) answer check -> (Week 6) scoring
       |                        -> (Week 7) ResultScreen::showResult()  [file I/O]
       |
       |--(2) Exit --> program ends
```

## Branching / Version Control Workflow
- `main` branch holds the latest stable, compiling version of the project.
- Each week's work is developed and tested locally, then committed with a
  message referencing the week/module (e.g. "Week 4: Question Management
  Module").
- Commits are kept small and scoped to a single module where possible, so
  the project history mirrors the week-by-week schedule.

## Testing Workflow
- Each module is compiled and run manually after implementation.
- File-handling modules (QuestionBank, and later result saving) are tested
  with both valid and intentionally malformed data files to confirm
  validation and error handling work as required.
