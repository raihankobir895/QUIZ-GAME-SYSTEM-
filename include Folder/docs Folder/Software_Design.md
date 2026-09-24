# Software Design — Quiz Game

## 1. Overview
The Quiz Game is a console C++ application organized into independent
modules, each with its own header/implementation file pair, communicating
through simple public interfaces. Data is persisted using plain text files
via the C++ file streams (`<fstream>`) instead of a database.

## 2. Software Structure Diagram

```
                        +-----------+
                        |  main.cpp |
                        +-----------+
                              |
                              v
                        +-----------+
                        | MainMenu  |
                        +-----------+
                          |       |
             (Start Quiz)|       |(Exit)
                          v       v
                  +-------------+   program ends
                  | QuizScreen  |
                  +-------------+
                     |        |
                     v        v
           +----------------+   +----------------+
           | QuestionBank   |   | ResultScreen    |
           +----------------+   +----------------+
                     |                    |
                     v                    v
        questions.txt (read)     results.txt (write)
```

## 3. Modules

| Module        | Files                              | Responsibility                                                        |
|---------------|-------------------------------------|-------------------------------------------------------------------------|
| MainMenu      | `MainMenu.h` / `MainMenu.cpp`       | Displays the main menu and routes user choice (Start Quiz / Exit).      |
| QuizScreen    | `QuizScreen.h` / `QuizScreen.cpp`   | Drives the quiz flow: loads the question bank and presents questions.   |
| QuestionBank  | `QuestionBank.h` / `QuestionBank.cpp` | Creates, loads (file I/O), validates, and displays quiz questions.    |
| ResultScreen  | `ResultScreen.h` / `ResultScreen.cpp` | Displays and (Week 7) saves the final quiz result.                    |
| Utils         | `Utils.h` / `Utils.cpp`             | Shared helper functions (e.g. clearing the console screen).             |

## 4. Class Design

### `Question` (struct, in `QuestionBank.h`)
- `std::string questionText`
- `std::string options[4]` — options A–D
- `int correctOption` — 1-based index of the correct option

### `QuestionBank`
- `QuestionBank(const std::string &filePath)`
- `bool loadQuestions()` — reads and validates questions from `filePath`
- `int getQuestionCount() const`
- `const Question &getQuestion(int index) const`
- `void displayQuestion(int index) const`
- `void displayAllQuestions() const`
- *(private)* `validateQuestion`, `splitLine`, `trim` — parsing/validation helpers

### `MainMenu`
- `void displayMenu()` — main input/output loop for the top-level menu

### `QuizScreen`
- `void startQuiz()` — loads the question bank and runs the quiz session

### `ResultScreen`
- `void showResult()` — displays (and later saves) the final result

### `Utils`
- `static void clearScreen()` — clears the console (cross-platform)

## 5. Data Design

**Question file** (`docs Folder/data Folder/questions.txt`), pipe-delimited,
one question per line:

```
Question|OptionA|OptionB|OptionC|OptionD|CorrectOption(1-4)
```

Lines beginning with `#` (or blank lines) are treated as comments and
ignored. Malformed lines are skipped with a warning rather than aborting
the program, so the question bank remains resilient to bad data.

**Result file** (introduced Week 7/8): will record each attempt's score,
correct/incorrect counts, and total questions in a similarly simple,
append-friendly text format.

## 6. Design Principles Applied
- **Single Responsibility** — each class owns exactly one concern
  (menu navigation, question storage, gameplay, or results).
- **Separation of interface and implementation** — every module has a
  `.h` declaring its public interface and a `.cpp` implementing it.
- **Defensive file handling** — all file reads check `is_open()` and
  validate parsed data before use.

---

## 7. Addendum (Weeks 6–10) — Final Architecture

Sections 1–6 above describe the design as planned in Week 2, for a
console interface. During Weeks 3–6 the team moved to a native Win32 GUI
instead of a text console, since it gives a much better experience for
the same underlying logic. The module responsibilities and data design
above are unchanged; only the presentation layer and the module list
below are updated to match what was actually built and shipped.

**Updated structure diagram:**

```
                         +--------------------+
                         | GuiApp.cpp/WinMain |  (window + event loop)
                         +--------------------+
                                    |
                                    v
                          +-------------------+
                          |     MainMenu      |
                          +-------------------+
                             |             |
                (Start Quiz) |             | (Exit)
                             v             v
                     +--------------+   program ends
                     | QuizScreen   |
                     +--------------+
                        |        |
                        v        v
             +----------------+   +----------------+
             | QuestionBank   |   | ResultScreen    |
             +----------------+   +--------+---------+
                        |                    |
                        v                    v
           questions.txt (read)     +----------------+
                                     |  ResultStore    |
                                     +--------+---------+
                                              |
                                     results.txt (read + append)
```

**Updated module list:**

| Module        | Files                                    | Responsibility                                                                 |
|---------------|-------------------------------------------|---------------------------------------------------------------------------------|
| GuiApp        | `GuiApp.h` / `GuiApp.cpp`                   | Win32 window, message loop, screen switching, fonts, shared control helpers.   |
| MainMenu      | `MainMenu.h` / `MainMenu.cpp`               | Title screen with "Start Quiz" / "Exit" buttons.                               |
| QuizScreen    | `QuizScreen.h` / `QuizScreen.cpp`           | One question at a time, four option buttons, live score, error handling.      |
| QuestionBank  | `QuestionBank.h` / `QuestionBank.cpp`       | Loads/validates questions from file; now also collects per-line warnings.      |
| Score         | `Score.h` / `Score.cpp`                     | Points, correct/incorrect counts, accuracy for the *current* session.         |
| ResultScreen  | `ResultScreen.h` / `ResultScreen.cpp`       | Shows the final summary; triggers saving the attempt via `ResultStore`.        |
| ResultStore   | `ResultStore.h` / `ResultStore.cpp` (Week 7/8) | Appends each attempt to `results.txt` and reads history back (best score).  |
| Utils         | `Utils.h` / `Utils.cpp`                     | Retained from the console prototype; unused by the GUI, kept for reference.   |

`main.cpp` is intentionally empty in the GUI build — `WinMain()` in
`GuiApp.cpp` is the real entry point.

**Result file** (`docs Folder/data Folder/results.txt`), same
pipe-delimited style as the question file, one attempt per line, opened
in **append** mode so history is never overwritten:

```
Timestamp|TotalQuestions|Correct|Incorrect|Points|Accuracy
```

`ResultStore` both writes new attempts and reads the file back to report
the player's best score and attempt count on the Result screen —
demonstrating full read *and* write file-handling integration (Week 8),
not just one-way loading.

**Week 9 additions:** `QuestionBank` now records validation warnings
(instead of printing to a console the GUI doesn't have) and `QuizScreen`
surfaces them in a message box; `ResultScreen` reports (without crashing)
if `results.txt` could not be written; an automated test suite
(`tests/test_core.cpp`) exercises `QuestionBank`, `Score` and
`ResultStore` directly — see `Testing_Report.md`.
