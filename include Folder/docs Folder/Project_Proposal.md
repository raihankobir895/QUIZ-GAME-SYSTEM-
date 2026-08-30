# Project Proposal — Quiz Game

## Course
Software Development (C++)

## Project Title
Quiz Game

## Objective
To design and develop a console-based Quiz Game application in C++ that lets
a user answer multiple-choice questions, tracks their score in real time,
and displays a final result summary. The project uses file handling instead
of a database, so all questions and results are stored in and read from
plain text files.

## Scope
- Single-player, console (text) based quiz application.
- Multiple-choice questions with four options (A–D) per question.
- Question bank stored in and loaded from a text file.
- Score tracking during gameplay and a final results summary.
- Persisting quiz results to a file for later reference.
- No external database or network dependency — pure file I/O.

Out of scope: multiplayer, GUI, online question banks, user accounts/login.

## Key Features
1. Interactive main menu (Start Quiz / Exit).
2. Question bank management using file handling (create, load, validate).
3. Quiz gameplay loop: display question, accept an answer, check correctness.
4. Real-time scoring.
5. Result summary screen (score, correct/incorrect counts) saved to file.

## Tools & Technologies
- Language: C++ (C++17)
- Storage: Plain text files (`.txt`) — no database
- IDE: Visual Studio Code
- Compiler: g++ / MinGW
- Version Control: Git

## Team / Roles
- Development, design, and testing carried out as a single-developer
  academic project, following the 10-week schedule below.

## Timeline
The project follows a 10-week schedule, moving from planning and design
through module development, integration, testing, and final delivery.
See `Project_Workflow.md` for the detailed week-by-week plan.

## Expected Outcome
A working, file-based Quiz Game executable, accompanied by design
documentation, that satisfies each week's deliverables and can be compiled
and run from the project root directory.
