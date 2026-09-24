#include <iostream>
#include <limits>
#include "../MainMenu.h"
#include "../QuizScreen.h"
#include "../Utils.h"

using namespace std;

void MainMenu::displayMenu()
{
    int choice = 0;

    do
    {
        Utils::clearScreen();

        cout << "=====================\n";
        cout << "      QUIZ GAME\n";
        cout << "=====================\n";
        cout << "1. Start Quiz\n";
        cout << "2. Exit\n";
        cout << "Enter Choice: ";
        
        while (!(cin >> choice))
        {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice)
        {
            case 1:
            {
                QuizScreen quiz;
                quiz.startQuiz();
                break;
            }

            case 2:
                cout << "\nThank You For Playing!\n";
                break;

            default:
                cout << "\nInvalid Choice! Please select 1 or 2.\n";
        }

    } while (choice != 2);
}
