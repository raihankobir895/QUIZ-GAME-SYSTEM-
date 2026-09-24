#include "../ResultScreen.h"
#include "../ResultStore.h"

#include <iomanip>
#include <sstream>

ResultScreen::ResultScreen()
    : titleLabel(nullptr),
      summaryLabel(nullptr),
      historyLabel(nullptr),
      playAgainButton(nullptr),
      exitButton(nullptr)
{
}

ResultScreen::~ResultScreen()
{
    destroy();
}

void ResultScreen::create(HWND parent, int totalQuestions, int correct,
                          int incorrect, int points, double accuracy)
{
    titleLabel = CreateControl(parent, "STATIC", "QUIZ COMPLETE",
                               SS_CENTER | SS_CENTERIMAGE | SS_NOPREFIX,
                               40, 50, 720, 70);
    ApplyTitleFont(titleLabel);

    // "\r\n" starts a new line inside a STATIC label.
    std::ostringstream summary;
    summary << "Total Questions: " << totalQuestions << "\r\n"
            << "Correct Answers: " << correct << "\r\n"
            << "Incorrect Answers: " << incorrect << "\r\n"
            << "Final Score: " << points << " pts\r\n"
            << std::fixed << std::setprecision(1)
            << "Accuracy: " << accuracy << "%";

    summaryLabel = CreateControl(parent, "STATIC", summary.str(),
                                 SS_CENTER | SS_NOPREFIX,
                                 40, 140, 720, 170);
    ApplyHeadingFont(summaryLabel);

    // Week 7/8: persist this attempt to results.txt (plain file, no
    // database) and read the history back to show a "best score so far".
    ResultStore store(ResolveResultsPath());

    ResultRecord record;
    record.timestamp = ResultStore::currentTimestamp();
    record.totalQuestions = totalQuestions;
    record.correct = correct;
    record.incorrect = incorrect;
    record.points = points;
    record.accuracy = accuracy;

    int previousBest = store.getBestScore(); // -1 if this is the first attempt
    bool saved = store.saveResult(record);

    std::ostringstream history;
    if (!saved)
    {
        // Non-fatal: the round is still fully playable even if results.txt
        // could not be written (e.g. read-only folder). Say so quietly
        // instead of losing the result silently or crashing.
        history << "(Could not save this result to file.)";
    }
    else
    {
        int attemptCount = store.getAttemptCount();
        bool isNewBest = (previousBest < 0) || (points > previousBest);
        int bestSoFar = isNewBest ? points : previousBest;

        history << "Attempt #" << attemptCount << "  |  Best Score: "
                << bestSoFar << " pts";
        if (isNewBest && attemptCount > 1)
            history << "  -  New Best!";
    }

    historyLabel = CreateControl(parent, "STATIC", history.str(),
                                 SS_CENTER | SS_CENTERIMAGE | SS_NOPREFIX,
                                 40, 320, 720, 40);
    ApplyDefaultFont(historyLabel);

    playAgainButton = CreateControl(parent, "BUTTON", "Play Again",
                                    BS_DEFPUSHBUTTON,
                                    190, 420, 200, 56, ControlId::ResultPlayAgain);
    ApplyHeadingFont(playAgainButton);

    exitButton = CreateControl(parent, "BUTTON", "Exit",
                               BS_PUSHBUTTON,
                               410, 420, 200, 56, ControlId::ResultExit);
    ApplyHeadingFont(exitButton);
}

void ResultScreen::destroy()
{
    HWND *controls[] = {&titleLabel, &summaryLabel, &historyLabel,
                        &playAgainButton, &exitButton};

    for (HWND *control : controls)
    {
        if (*control)
        {
            DestroyWindow(*control);
            *control = nullptr;
        }
    }
}

void ResultScreen::handleCommand(int controlId)
{
    switch (controlId)
    {
    case ControlId::ResultPlayAgain:
        ShowQuizScreen();
        break;

    case ControlId::ResultExit:
        ExitApplication();
        break;
    }
}
