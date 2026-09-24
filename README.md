# Quiz Game (C++, File Handling)

An interactive console quiz game where users answer multiple-choice
questions and see their score. Built in C++ for the Software Development
course. Questions are stored in and loaded from a plain text file
(no database), following the 10-week project schedule.
**This build covers Weeks 1–6.**

## Progress So Far

| Week | Topic                              | Status                                             |
|------|-------------------------------------|-----------------------------------------------------|
| 1    | Project Planning & Requirements     | Done — see `docs Folder/Project_Proposal.md`, `Requirement_Specification.md`, `Project_Workflow.md` |
| 2    | Software Design & UI                | Done — see `docs Folder/Interface_Design.md`, `Software_Design.md` |
| 3    | Main Menu Module                    | Done — `MainMenu.h` / `MainMenu.cpp` (working menu, input validation) |
| 4    | Question Management Module          | Done — `QuestionBank.h` / `QuestionBank.cpp` (create/load/validate/display via file handling) |
| 5    | Quiz Gameplay Module                | Done — `QuizScreen.h` / `QuizScreen.cpp` (answer selection, correctness checking, automatic flow control) |
| 6    | Scoring System Development          | Done — `Score.h` / `Score.cpp` (points, live score display, correct/incorrect tracking, accuracy) |

Weeks 7–10 (dedicated results screen and persistence, full file-based
persistence review, testing/polish, and final delivery) are not yet
implemented.

## What Works Right Now

Picking **"1. Start Quiz"** from the main menu will:
1. Load and validate all questions from `questions.txt`.
2. Show each question one at a time with options A–D.
3. Accept your answer (re-prompting on anything other than A/B/C/D).
4. Tell you immediately whether you were right, and show the correct
   answer if you weren't.
5. Award 10 points per correct answer and display the running score
   (points, correct count, incorrect count) after every question.
6. Automatically move on to the next question — no extra keypress needed.
7. After the last question, show a full performance summary: total
   questions, correct/incorrect counts, final score, and accuracy
   percentage, before returning to the main menu.

A dedicated, persisted results screen (saving each attempt to a file)
is scoped for Week 7.

## Folder Structure

```
Quiz game system/
├── include Folder/
│   ├── MainMenu.h / QuizScreen.h / ResultScreen.h / Utils.h / QuestionBank.h / Score.h
│   ├── src Folder/
│   │   ├── main.cpp
│   │   ├── MainMenu.cpp
│   │   ├── QuizScreen.cpp
│   │   ├── QuestionBank.cpp
│   │   ├── Score.cpp
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
  "include Folder/src Folder/Score.cpp" \
  "include Folder/src Folder/ResultScreen.cpp" \
  "include Folder/src Folder/Utils.cpp" \
  -o quizgame
```

Then run it (also from the project root):

```bash
./quizgame        # Linux/macOS
quizgame.exe      # Windows
```

## Playing a Round

```
1. Start Quiz
2. Exit
Enter Choice: 1

Loading question bank...
10 question(s) loaded successfully.

-----------------------------------

Q1. What is the capital of France?
   A. Berlin
   B. Madrid
   C. Paris
   D. Rome
Your answer (A/B/C/D): C

Correct!

-----------------------------------
...
-----------------------------------
Quiz complete!
Correct answers   : 8 / 10
Incorrect answers : 2 / 10
(A full scoring system and dedicated results screen are added in Week 6-7.)

Press Enter to return to the main menu...
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