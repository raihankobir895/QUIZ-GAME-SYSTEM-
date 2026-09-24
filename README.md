# Quiz Game (C++, Win32 GUI, File Handling)

An interactive multiple-choice quiz game with a native Windows GUI. Built
in C++ for the Software Development course, following a 10-week project
schedule. Questions are stored in and loaded from a plain text file, and
every completed attempt is appended to a second text file — **no
database is used anywhere**, per the course requirement.

**Status: complete — Weeks 1 through 10 are all done.** (Reviewed and one
small consistency fix applied since the last build: saved accuracy in
`results.txt` now matches the 1-decimal display on the Result screen —
see "Building" below, rebuild before running.)

## Progress Summary

| Week | Topic                              | Status                                                                 |
|------|-------------------------------------|--------------------------------------------------------------------------|
| 1    | Project Planning & Requirements     | Done — `docs Folder/Project_Proposal.md`, `Requirement_Specification.md`, `Project_Workflow.md` |
| 2    | Software Design & UI                | Done — `docs Folder/Interface_Design.md`, `Software_Design.md`          |
| 3    | Main Menu Module                    | Done — `MainMenu.h` / `MainMenu.cpp`                                     |
| 4    | Question Management Module          | Done — `QuestionBank.h` / `QuestionBank.cpp` (file-based load/validate)  |
| 5    | Quiz Gameplay Module                | Done — `QuizScreen.h` / `QuizScreen.cpp` (answer selection, auto-advance)|
| 6    | Scoring System                      | Done — `Score.h` / `Score.cpp` (points, live score, accuracy)            |
| 7    | Result Management Module            | Done — `ResultScreen.h` / `ResultScreen.cpp` (final summary + save)      |
| 8    | File Handling Integration           | Done — `ResultStore.h` / `ResultStore.cpp` (read + append results history)|
| 9    | Testing & Enhancement               | Done — `tests/test_core.cpp` (30 automated checks), warning/error handling|
| 10   | Final Testing, Docs, Deployment     | Done — this README, `Testing_Report.md`, verified clean release build   |

> **Note on the interface:** the Week 2 design docs describe a console
> UI. During Weeks 3–6 the project moved to a native Win32 GUI instead
> (same navigation flow, same underlying modules) for a better user
> experience. `Interface_Design.md` and `Software_Design.md` each have a
> "Weeks 6–10 Addendum" section documenting the GUI as actually built.

## What the Game Does

1. **Main Menu** — "Quiz Game" title with **Start Quiz** and **Exit**.
2. **Quiz Screen** — one question at a time with four answer buttons.
   Picking an answer instantly shows Correct/Incorrect feedback (with the
   right answer if you missed it), locks the buttons, updates a live
   score line, and reveals **Next Question ->** (or **See Results ->** on
   the last question).
3. **Result Screen** — total questions, correct/incorrect counts, final
   score, accuracy percentage, plus your **attempt number and best score
   across every past run** (read back from `results.txt`), with a
   "New Best!" callout when you beat it. **Play Again** or **Exit**.
4. **Error handling** — a clear message box if the question file is
   missing or empty (instead of crashing), and a separate warning message
   box listing any individual malformed lines it had to skip.

## Folder Structure

```
QUIZ GAME SYSTEM/
├── README.md
├── LICENSE
├── Working schedule .pdf
├── include Folder/
│   ├── GuiApp.h / MainMenu.h / QuizScreen.h / ResultScreen.h
│   ├── ResultStore.h / QuestionBank.h / Score.h / Utils.h
│   ├── src Folder/
│   │   ├── main.cpp            (intentionally empty — see below)
│   │   ├── GuiApp.cpp          (WinMain, window, screen switching)
│   │   ├── MainMenu.cpp
│   │   ├── QuizScreen.cpp
│   │   ├── QuestionBank.cpp
│   │   ├── Score.cpp
│   │   ├── ResultScreen.cpp
│   │   ├── ResultStore.cpp     (Week 7/8 — results file persistence)
│   │   ├── Utils.cpp
│   │   └── _deprecated/
│   │       └── Win32Main.cpp.reference  (superseded prototype, not built)
│   └── docs Folder/
│       ├── Project_Proposal.md
│       ├── Requirement_Specification.md
│       ├── Project_Workflow.md
│       ├── Interface_Design.md
│       ├── Software_Design.md
│       ├── Testing_Report.md   (Week 9)
│       └── data Folder/
│           ├── questions.txt   (question bank — read at startup)
│           └── results.txt     (created on first save — quiz history)
└── tests/
    └── test_core.cpp           (Week 9 — automated module tests)
```

`main.cpp` is kept empty on purpose: the real entry point is `WinMain()`
in `GuiApp.cpp`. It's left in the project (rather than deleted) only so
the original folder layout stays intact.

## Building

### Option A — Windows, with MinGW-w64 (recommended)

From the project root (the `QUIZ GAME SYSTEM` folder — the questions and
results files are resolved relative to the `.exe`'s own folder first,
then this folder):

```bash
g++ -std=c++17 -Wall -Wextra -mwindows -O2 \
  "include Folder/src Folder/GuiApp.cpp" \
  "include Folder/src Folder/MainMenu.cpp" \
  "include Folder/src Folder/QuizScreen.cpp" \
  "include Folder/src Folder/QuestionBank.cpp" \
  "include Folder/src Folder/Score.cpp" \
  "include Folder/src Folder/ResultScreen.cpp" \
  "include Folder/src Folder/ResultStore.cpp" \
  "include Folder/src Folder/Utils.cpp" \
  -o quizgame_gui.exe
```

Then run `quizgame_gui.exe` (double-click, or from a terminal).

### Option B — Cross-compiling from Linux/macOS (for building/testing only)

Install a MinGW-w64 cross-compiler (e.g. `sudo apt install
g++-mingw-w64-x86-64` on Ubuntu/Debian) and swap `g++` for
`x86_64-w64-mingw32-g++` in the command above. The resulting `.exe`
still needs to be copied to a Windows machine (or run under Wine) to
actually play, since it uses the native Win32 GUI API.

### Option C — Prebuilt executable

Not shipped in this copy: the source was just updated (accuracy is now
saved to `results.txt` at 1 decimal place, matching what the Result
screen shows on the game itself, instead of a raw unrounded value), so
any previously built `.exe` would be one step out of date. Build with
Option A above to get a `quizgame_gui.exe` that matches this source
exactly.

## Running the Automated Tests (Week 9)

`QuestionBank`, `Score`, and `ResultStore` have no Windows dependency, so
they're covered by a plain-`g++` test suite that runs on any platform:

```bash
g++ -std=c++17 -Wall -Wextra "tests/test_core.cpp" \
  "include Folder/src Folder/QuestionBank.cpp" \
  "include Folder/src Folder/Score.cpp" \
  "include Folder/src Folder/ResultStore.cpp" \
  -o tests/test_core
./tests/test_core
```

Expected output ends with `30/30 checks passed.` and exit code `0`. See
`include Folder/docs Folder/Testing_Report.md` for what each check
covers, plus the manual GUI test scenarios that were run by hand.

## File Formats

**Question bank** (`include Folder/docs Folder/data Folder/questions.txt`),
pipe-delimited, one question per line:

```
Question|OptionA|OptionB|OptionC|OptionD|CorrectOption(1=A,2=B,3=C,4=D)
```

Lines starting with `#` and blank lines are ignored. Malformed lines
(wrong field count, empty text/option, or an out-of-range correct
option) are skipped — with a warning shown in the app — instead of
crashing the program.

**Results history** (`include Folder/docs Folder/data Folder/results.txt`,
created automatically on the first completed quiz), same pipe-delimited
style, one attempt per line, always **appended** (never overwritten):

```
Timestamp|TotalQuestions|Correct|Incorrect|Points|Accuracy
```

This file is how the Result screen knows your best score and attempt
count across every session — it's plain text, so you can open it
yourself at any time to see your full history.

## Deployment Notes

- The game is a single self-contained `.exe` plus two small text data
  files — no installer, database, or extra runtime is required.
- To deploy: copy the whole `QUIZ GAME SYSTEM` folder (or at minimum
  `quizgame_gui.exe` together with `include Folder/docs Folder/data
  Folder/questions.txt`) to the target Windows machine, keeping the same
  relative layout.
- `results.txt` is created next to the question file the first time
  someone finishes a quiz, so a fresh deployment doesn't need to include
  one.

## Notes
- No external libraries or database are used anywhere — persistence is
  entirely file-based (`<fstream>`), per the course requirement.
- Original scaffold include paths (`../include/...`) were corrected to
  match the actual folder layout (`../*.h`) so the project compiles.
- An early single-file GUI prototype (`Win32Main.cpp`) was superseded by
  the modular `GuiApp`/`MainMenu`/`QuizScreen`/`ResultScreen` design
  actually used in the build; it's kept for reference under
  `include Folder/src Folder/_deprecated/`, excluded from every build
  command.
