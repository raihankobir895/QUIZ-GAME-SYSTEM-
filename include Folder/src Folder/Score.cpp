#include <iostream>
#include <iomanip>
#include "../Score.h"

using namespace std;

Score::Score() : points(0), correctCount(0), incorrectCount(0) {}

void Score::recordAnswer(bool isCorrect)
{
    if (isCorrect)
    {
        correctCount++;
        points += POINTS_PER_CORRECT;
    }
    else
    {
        incorrectCount++;
    }
}

int Score::getPoints() const
{
    return points;
}

int Score::getCorrectCount() const
{
    return correctCount;
}

int Score::getIncorrectCount() const
{
    return incorrectCount;
}

int Score::getTotalAnswered() const
{
    return correctCount + incorrectCount;
}

// Tracks quiz performance as an accuracy percentage.
double Score::getAccuracy() const
{
    int total = getTotalAnswered();
    if (total == 0)
        return 0.0;

    return (static_cast<double>(correctCount) / total) * 100.0;
}

void Score::displayCurrentScore() const
{
    cout << "Current Score: " << points << " pts  ("
         << correctCount << " correct, " << incorrectCount << " incorrect)\n";
}

void Score::displayFinalScore(int totalQuestions) const
{
    cout << "\n=====================\n";
    cout << "   QUIZ PERFORMANCE\n";
    cout << "=====================\n";
    cout << "Total Questions   : " << totalQuestions << "\n";
    cout << "Correct Answers   : " << correctCount << "\n";
    cout << "Incorrect Answers : " << incorrectCount << "\n";
    cout << "Final Score       : " << points << " pts\n";
    cout << fixed << setprecision(1);
    cout << "Accuracy          : " << getAccuracy() << "%\n";
}
