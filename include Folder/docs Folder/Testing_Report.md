# Testing Report — Quiz Game (Week 9)

## 1. Automated Module Tests

`tests/test_core.cpp` exercises the three platform-independent modules —
`QuestionBank`, `Score`, and `ResultStore` — directly, without needing
Windows or the GUI. It builds with a plain `g++` (no MinGW required) and
returns a non-zero exit code if any check fails, so it can be run in any
CI pipeline.

**Build and run:**

```bash
g++ -std=c++17 -Wall -Wextra "tests/test_core.cpp" \
  "include Folder/src Folder/QuestionBank.cpp" \
  "include Folder/src Folder/Score.cpp" \
  "include Folder/src Folder/ResultStore.cpp" \
  -o tests/test_core
./tests/test_core
```

**Result of the last run: 30/30 checks passed.**

| Area                         | Cases covered                                                                 |
|-------------------------------|--------------------------------------------------------------------------------|
| `QuestionBank` — valid file    | Correct question count; question text, option, and correct-answer parsing.    |
| `QuestionBank` — malformed file| Wrong field count, empty question text, out-of-range correct option, and a non-numeric correct option are each skipped and recorded as a warning, while the one valid line still loads. |
| `QuestionBank` — missing file  | `loadQuestions()` returns `false` (no crash) and explains why in a warning.    |
| `Score`                        | Zero-state accuracy (avoids divide-by-zero), point totals, correct/incorrect counts, and accuracy percentage after a mix of right/wrong answers. |
| `ResultStore` — round trip     | Save → load returns the same records; attempt count and best-score tracking across two saved attempts. |
| `ResultStore` — corrupted file | A results file with a garbage line and a non-numeric field still loads the one well-formed record, instead of failing outright. |

## 2. Manual GUI Test Scenarios

The GUI itself (Win32 windows, buttons, message boxes) isn't practical to
drive from an automated test, so it was checked manually against these
scenarios:

| # | Scenario                                              | Expected result                                              | Result |
|---|--------------------------------------------------------|----------------------------------------------------------------|--------|
| 1 | Launch the game                                         | Main menu appears, centered, with Start Quiz / Exit.           | Pass   |
| 2 | Click Start Quiz                                        | First question loads with 4 options, no answer selected.       | Pass   |
| 3 | Answer correctly                                        | "Correct!" feedback, score increases by 10, options lock.      | Pass   |
| 4 | Answer incorrectly                                      | Shows the correct answer, score unchanged, options lock.       | Pass   |
| 5 | Click Next Question repeatedly                          | Advances one question at a time; button reads "See Results ->" on the last question. | Pass |
| 6 | Finish the last question                                | Result screen shows totals, score, accuracy, attempt #, best score. | Pass |
| 7 | Play a second round in the same session                 | Attempt # increments; "New Best!" shown only if the new score is higher. | Pass |
| 8 | Rename/hide `questions.txt` before launching             | A clear error message box appears; app returns to the main menu instead of crashing. | Pass |
| 9 | Add a malformed line to `questions.txt` (e.g. missing a field) | A warning message box lists the skipped line(s); the quiz still starts with the remaining valid questions. | Pass |
| 10| Make the results folder read-only, then finish a quiz    | Result screen still displays correctly and shows "(Could not save this result to file.)" instead of crashing. | Pass |
| 11| Click Exit from the main menu and from the result screen | Application closes cleanly both times.                         | Pass   |

## 3. Known Limitations

- The GUI is Win32-specific (Windows only). `QuestionBank`, `Score`, and
  `ResultStore` have no Windows dependency and are covered by the
  automated tests above; `MainMenu`, `QuizScreen`, `ResultScreen`, and
  `GuiApp` require Windows (or a cross-compiler, for building only) since
  they call the Win32 API directly.
- `Utils::clearScreen()` is unused by the GUI; it's kept only as a
  leftover from the original console version described in Week 2–3 docs.
