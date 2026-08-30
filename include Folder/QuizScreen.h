#ifndef QUIZSCREEN_H
#define QUIZSCREEN_H

class QuizScreen
{
public:
    void startQuiz();

private:
    // Prompts the user for an answer and re-prompts until a valid A-D
    // choice is entered. Returns the answer as an uppercase letter.
    char readAnswer() const;
};

#endif
