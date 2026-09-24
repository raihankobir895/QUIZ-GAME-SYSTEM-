#include "../MainMenu.h"

MainMenu::MainMenu()
    : titleLabel(nullptr),
      subtitleLabel(nullptr),
      startButton(nullptr),
      exitButton(nullptr)
{
}

MainMenu::~MainMenu()
{
    destroy();
}

void MainMenu::create(HWND parent)
{
    // SS_NOPREFIX: never treat '&' in label text as a shortcut marker.
    titleLabel = CreateControl(parent, "STATIC", "QUIZ GAME",
                               SS_CENTER | SS_CENTERIMAGE | SS_NOPREFIX,
                               40, 110, 720, 80);
    ApplyTitleFont(titleLabel);

    subtitleLabel = CreateControl(parent, "STATIC",
                                  "Test your knowledge with a multiple-choice quiz.",
                                  SS_CENTER | SS_CENTERIMAGE | SS_NOPREFIX,
                                  40, 200, 720, 40);
    ApplyDefaultFont(subtitleLabel);

    startButton = CreateControl(parent, "BUTTON", "Start Quiz",
                                BS_DEFPUSHBUTTON,
                                280, 300, 240, 56, ControlId::MenuStart);
    ApplyHeadingFont(startButton);

    exitButton = CreateControl(parent, "BUTTON", "Exit",
                               BS_PUSHBUTTON,
                               280, 375, 240, 56, ControlId::MenuExit);
    ApplyHeadingFont(exitButton);
}

void MainMenu::destroy()
{
    HWND *controls[] = {&titleLabel, &subtitleLabel, &startButton, &exitButton};

    for (HWND *control : controls)
    {
        if (*control)
        {
            DestroyWindow(*control);
            *control = nullptr;
        }
    }
}

void MainMenu::handleCommand(int controlId)
{
    switch (controlId)
    {
    case ControlId::MenuStart:
        ShowQuizScreen();
        break;

    case ControlId::MenuExit:
        ExitApplication();
        break;
    }
}
