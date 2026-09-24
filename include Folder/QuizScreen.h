#ifndef QUIZSCREEN_H
#define QUIZSCREEN_H

#include "GuiApp.h"
#include "QuestionBank.h"
#include "Score.h"

// Quiz screen: one question at a time, four option buttons, feedback,
// a Next button and a live score line.
class QuizScreen
{
public:
    QuizScreen();
    ~QuizScreen();

    void create(HWND parent);          // load questions, build controls, show Q1
    void destroy();                    // remove the controls (safe to call twice)
    void handleCommand(int controlId); // react to an option / Next click

private:
    // NOTE: member order matters - the constructor's initializer list
    // initializes members in exactly this order.
    QuestionBank bank; // has no default constructor, so it is built in the init list
    Score score;
    int currentIndex;
    bool answered;
    bool questionsLoaded;

    HWND progressLabel;
    HWND questionLabel;
    HWND optionButtons[4];
    HWND feedbackLabel;
    HWND nextButton;
    HWND scoreLabel;

    void showQuestion();
    void handleAnswer(int optionIndex);
    void handleNext();
    void updateScoreLabel();
};

#endif
