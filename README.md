# Quiz Game (C++, File Handling)

A console-based Quiz Game built in C++ for the Software Development course.
Questions are stored in and loaded from a plain text file (no database),
following the 10-week project schedule. **This build covers Weeks 1–4.**

## Progress So Far

| Week | Topic                              | Status                                             |
|------|-------------------------------------|-----------------------------------------------------|
| 1    | Project Planning & Requirements     | Done — see `docs Folder/Project_Proposal.md`, `Requirement_Specification.md`, `Project_Workflow.md` |
| 2    | Software Design & UI                | Done — see `docs Folder/Interface_Design.md`, `Software_Design.md` |
| 3    | Main Menu Module                    | Done — `MainMenu.h` / `MainMenu.cpp` (working menu, input validation) |
| 4    | Question Management Module          | Done — `QuestionBank.h` / `QuestionBank.cpp` (create/load/validate/display via file handling) |

Weeks 5–10 (gameplay, scoring, results, full file persistence, testing,
and final delivery) are not yet implemented; the "Start Quiz" option
currently loads and previews the question bank as proof that Week 4 works.

## Folder Structure

```
Quiz game system/
├── include Folder/
│   ├── MainMenu.h / QuizScreen.h / ResultScreen.h / Utils.h / QuestionBank.h
│   ├── src Folder/
│   │   ├── main.cpp
│   │   ├── MainMenu.cpp
│   │   ├── QuizScreen.cpp
│   │   ├── QuestionBank.cpp
│   │   ├── ResultScreen.cpp
│   │   └── Utils.cpp
│   └── docs Folder/
│       ├── Project_Proposal.md
│       ├── Requirement_Specification.md
│       ├── Project_Workflow.md
│       ├── Interface_Design.md
│       ├── Software_Design.md
│       └── data Folder/
│           └── questions.txt
└── README.md
```

## Building

Requires a C++17-capable compiler (g++ recommended). Run the following
**from the project root** (the `Quiz game system` folder), since the
question file path is resolved relative to it:

```bash
g++ -std=c++17 -Wall \
  "include Folder/src Folder/main.cpp" \
  "include Folder/src Folder/MainMenu.cpp" \
  "include Folder/src Folder/QuizScreen.cpp" \
  "include Folder/src Folder/QuestionBank.cpp" \
  "include Folder/src Folder/ResultScreen.cpp" \
  "include Folder/src Folder/Utils.cpp" \
  -o quizgame
```

Then run it (also from the project root):

```bash
./quizgame        # Linux/macOS
quizgame.exe      # Windows
```

## Question Bank File Format

`include Folder/docs Folder/data Folder/questions.txt` — pipe-delimited,
one question per line:

```
Question|OptionA|OptionB|OptionC|OptionD|CorrectOption(1-4)
```

Lines starting with `#` and blank lines are ignored. Malformed lines
(wrong field count, empty text/options, or an out-of-range correct
option) are skipped with a warning instead of crashing the program.

## Notes
- No external libraries or database are used — persistence is file-based,
  per the course requirement.
- Original scaffold include paths (`../include/...`) were corrected to
  match the actual folder layout (`../*.h`) so the project compiles.
