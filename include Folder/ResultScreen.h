#ifndef RESULTSCREEN_H
#define RESULTSCREEN_H

#include "GuiApp.h"

// Result screen: title, multi-line summary, "Play Again" and "Exit" buttons.
class ResultScreen
{
public:
    ResultScreen();
    ~ResultScreen();

    void create(HWND parent, int totalQuestions, int correct, int incorrect,
                int points, double accuracy);
    void destroy();                    // remove the controls (safe to call twice)
    void handleCommand(int controlId); // react to a button click

private:
    HWND titleLabel;
    HWND summaryLabel;
    HWND historyLabel;
    HWND playAgainButton;
    HWND exitButton;
};

#endif
