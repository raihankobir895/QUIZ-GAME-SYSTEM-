# Interface Design — Quiz Game

This document defines the console (text-based) interface for each screen
in the Quiz Game, as required for Week 2.

## 1. Main Menu Screen

```
=====================
      QUIZ GAME
=====================
1. Start Quiz
2. Exit
Enter Choice:
```

- Accepts numeric input only; invalid input re-prompts the user.
- Option 1 navigates to the Quiz Question Screen (via the question bank).
- Option 2 exits the program.

## 2. Quiz Question Screen

```
Q1. What is the capital of France?
   A. Berlin
   B. Madrid
   C. Paris
   D. Rome

Your answer (A-D):
```

- Displays one question at a time with its four options.
- After Week 5 integration, the user's selected option is read here,
  checked for correctness, and the score is updated (Week 6) before the
  next question is shown automatically.
- Currently (Week 4), this screen previews the full loaded question bank
  to confirm questions are stored, loaded, and displayed correctly.

## 3. Result Display Screen

```
=====================
      RESULTS
=====================
Total Questions : 10
Correct Answers : 7
Incorrect Answers: 3
Final Score      : 70%

Results saved to results.txt
Press Enter to return to the main menu...
```

- Shown after the last question is answered.
- Summarizes correct/incorrect counts and final score.
- Persists the result to a text file (Week 7/8).

## 4. Navigation Flow

```
Main Menu -> Start Quiz -> Quiz Question Screen (loop per question)
                              -> Result Display Screen -> back to Main Menu
Main Menu -> Exit -> program ends
```

## 5. Design Notes
- All screens are plain console text — no external UI libraries.
- The screen is cleared between menu redraws (`Utils::clearScreen()`) to
  keep the interface uncluttered.
- Option letters (A–D) are used consistently across the question display
  and result summary for clarity.

---

## 6. Addendum (Weeks 6–10) — Final GUI Interface

The screens above were the Week 2 console mockups. The shipped game is a
native Win32 GUI instead (same navigation flow, same information on each
screen), built with plain buttons and labels — no external UI toolkit:

- **Main Menu** — window titled "Quiz Game", with the title label,
  a subtitle, and two buttons: **Start Quiz** and **Exit**.
- **Quiz Screen** — a progress label ("Question N of M"), the question
  text, four answer buttons arranged in a 2x2 grid, a feedback line
  (correct/incorrect, with the right answer shown on a miss), a live
  score line, and a **Next Question ->** button that only appears after
  answering (so each question can only be answered once).
- **Result Screen** — title "QUIZ COMPLETE", the full summary (total /
  correct / incorrect / points / accuracy), a history line showing the
  attempt number and best score saved across all past runs (Week 7/8),
  and **Play Again** / **Exit** buttons.
- **Error dialogs** — a message box if `questions.txt` can't be found or
  parsed at all, and a separate warning message box listing any
  individual skipped lines (Week 9), so bad data is never silently lost.

The navigation flow itself is unchanged from section 4 above.
