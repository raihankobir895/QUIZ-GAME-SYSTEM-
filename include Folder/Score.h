#ifndef SCORE_H
#define SCORE_H

class Score
{
public:
    Score();

    void recordAnswer(bool isCorrect);

    int getPoints() const;
    int getCorrectCount() const;
    int getIncorrectCount() const;
    int getTotalAnswered() const;
    double getAccuracy() const; // percentage, 0-100

    // Live score line, shown after each question during the quiz.
    void displayCurrentScore() const;

    // Full performance summary, shown once the quiz is complete.
    void displayFinalScore(int totalQuestions) const;

private:
    int points;
    int correctCount;
    int incorrectCount;

    static constexpr int POINTS_PER_CORRECT = 10;
};

#endif
