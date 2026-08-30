#include <iostream>
#include <limits>
#include <string>
#include <cctype>
#include "../QuizScreen.h"
#include "../QuestionBank.h"

using namespace std;

// Location of the question data file, relative to the project root
// ("Quiz game system"), which is where the compiled program should be run from.
static const string QUESTIONS_FILE = "include Folder/docs Folder/data Folder/questions.txt";

// Reads one line of input and keeps re-prompting until it is a single
// A/B/C/D character (case-insensitive). This is the "answer selection
// feature" required for Week 5.
char QuizScreen::readAnswer() const
{
    string input;

    while (true)
    {
        cout << "Your answer (A/B/C/D): ";
        getline(cin, input);

        if (input.size() == 1)
        {
            char ch = static_cast<char>(toupper(static_cast<unsigned char>(input[0])));
            if (ch >= 'A' && ch <= 'D')
                return ch;
        }

        cout << "Invalid input. Please enter A, B, C, or D.\n";
    }
}

void QuizScreen::startQuiz()
{
    QuestionBank bank(QUESTIONS_FILE);

    cout << "\nLoading question bank...\n";

    if (!bank.loadQuestions())
    {
        cout << "Unable to start the quiz because no valid questions could be loaded.\n";
        cout << "Please check that " << QUESTIONS_FILE << " exists and is correctly formatted.\n";
        return;
    }

    // Flush the leftover newline left in the input buffer by the main
    // menu's "cin >> choice", so it isn't mistaken for an answer below.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int totalQuestions = bank.getQuestionCount();
    int correctCount = 0;
    int incorrectCount = 0;

    // Controls the quiz flow: walks through every question in order,
    // one at a time, automatically moving on after each is answered.
    for (int i = 0; i < totalQuestions; i++)
    {
        cout << "\n-----------------------------------\n";
        bank.displayQuestion(i);

        char answer = readAnswer();
        const Question &q = bank.getQuestion(i);

        // correctOption is stored 1-based (1=A, 2=B, 3=C, 4=D).
        char correctLetter = static_cast<char>('A' + (q.correctOption - 1));

        if (answer == correctLetter)
        {
            cout << "\nCorrect!\n";
            correctCount++;
        }
        else
        {
            cout << "\nIncorrect. The correct answer was " << correctLetter
                 << ". " << q.options[q.correctOption - 1] << "\n";
            incorrectCount++;
        }
        // Loop automatically continues to the next question - no extra
        // input needed from the user to advance.
    }

    cout << "\n-----------------------------------\n";
    cout << "Quiz complete!\n";
    cout << "Correct answers   : " << correctCount << " / " << totalQuestions << "\n";
    cout << "Incorrect answers : " << incorrectCount << " / " << totalQuestions << "\n";
    cout << "(A full scoring system and dedicated results screen are added in Week 6-7.)\n";

    cout << "\nPress Enter to return to the main menu...";
    cin.get();
}
