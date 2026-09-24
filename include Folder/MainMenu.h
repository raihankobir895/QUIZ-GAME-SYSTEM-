#ifndef MAINMENU_H
#define MAINMENU_H

#include "GuiApp.h"

// Main menu screen: title, "Start Quiz" and "Exit" buttons.
class MainMenu
{
public:
    MainMenu();
    ~MainMenu();

    void create(HWND parent);        // build the controls inside `parent`
    void destroy();                  // remove the controls (safe to call twice)
    void handleCommand(int controlId); // react to a button click

private:
    HWND titleLabel;
    HWND subtitleLabel;
    HWND startButton;
    HWND exitButton;
};

#endif
