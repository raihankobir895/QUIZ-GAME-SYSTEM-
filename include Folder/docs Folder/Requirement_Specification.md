# Requirement Specification — Quiz Game

## 1. Purpose
This document specifies the functional and non-functional requirements for
the Quiz Game console application built in C++.

## 2. Functional Requirements

| ID   | Requirement                                                                 |
|------|------------------------------------------------------------------------------|
| FR1  | The system shall display a main menu with "Start Quiz" and "Exit" options.   |
| FR2  | The system shall reject non-numeric/invalid main menu input and re-prompt.   |
| FR3  | The system shall store quiz questions in a text file (no database).          |
| FR4  | The system shall load questions from file at runtime.                        |
| FR5  | The system shall validate each loaded question (non-empty text, four         |
|      | non-empty options, correct-option index between 1 and 4) and skip/report     |
|      | any invalid entries instead of crashing.                                     |
| FR6  | The system shall display each question with its four options (A–D).          |
| FR7  | The system shall let the user select an answer for each question.            |
| FR8  | The system shall check the selected answer against the correct answer.       |
| FR9  | The system shall automatically move to the next question after an answer.    |
| FR10 | The system shall track and update the user's score after each question.      |
| FR11 | The system shall display the current score during gameplay.                  |
| FR12 | The system shall display a final result screen: total score, correct count,  |
|      | and incorrect count.                                                          |
| FR13 | The system shall save quiz results to a file for persistence.                |

## 3. Non-Functional Requirements

| ID    | Requirement                                                                 |
|-------|------------------------------------------------------------------------------|
| NFR1  | The application shall run as a console (text-based) program.                |
| NFR2  | The application shall be written in standard C++ (C++17) and compile with   |
|       | g++ without external libraries.                                              |
| NFR3  | Data persistence shall use plain text files only — no database engine.      |
| NFR4  | The application shall handle invalid input (menu choices, malformed data    |
|       | files) gracefully, without crashing.                                         |
| NFR5  | Source code shall be organized into separate header/implementation files    |
|       | per module (MainMenu, QuizScreen, QuestionBank, ResultScreen, Utils) for     |
|       | maintainability.                                                              |
| NFR6  | The project shall be maintained under Git version control.                  |

## 4. Data Requirements
- **Question record:** question text, four answer options, correct-option index.
- **Question file format:** pipe-delimited text file, one question per line:
  `Question|OptionA|OptionB|OptionC|OptionD|CorrectOption(1-4)`
- **Result record:** date/time or attempt label, score, correct count,
  incorrect count, total questions (defined fully in Week 7/8).

## 5. Constraints
- No internet connection or external API required.
- No database (SQL/NoSQL) may be used — file handling only.
- Must run on Windows and Linux (portable console I/O).

## 6. Assumptions
- The question file is well-formed for the majority of entries; malformed
  lines are skipped with a warning rather than stopping the program.
- Single user, single session per run (no concurrent access to files).
