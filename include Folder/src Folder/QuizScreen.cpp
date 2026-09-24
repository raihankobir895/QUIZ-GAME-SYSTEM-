#include <iostream>
#include <limits>
#include <string>
#include <cctype>
#include "../QuizScreen.h"
#include "../QuestionBank.h"
#include "../Score.h"

using namespace std;

static const string QUESTIONS_FILE = "include Folder/docs Folder/data Folder/questions.txt";

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

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int totalQuestions = bank.getQuestionCount();
    Score score;

    for (int i = 0; i < totalQuestions; i++)
    {
        cout << "\n-----------------------------------\n";
        bank.displayQuestion(i);

        char answer = readAnswer();
        const Question &q = bank.getQuestion(i);

        char correctLetter = static_cast<char>('A' + (q.correctOption - 1));
        bool isCorrect = (answer == correctLetter);

        score.recordAnswer(isCorrect);

        if (isCorrect)
        {
            cout << "\nCorrect!\n";
        }
        else
        {
            cout << "\nIncorrect. The correct answer was " << correctLetter
                 << ". " << q.options[q.correctOption - 1] << "\n";
        }

        score.displayCurrentScore();
    }

    cout << "\n-----------------------------------\n";
    cout << "Quiz complete!\n";
    score.displayFinalScore(totalQuestions);
    cout << "(A dedicated, persisted results screen is added in Week 7.)\n";

    cout << "\nPress Enter to return to the main menu...";
    cin.get();
}