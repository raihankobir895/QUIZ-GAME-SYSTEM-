#include "../QuizScreen.h"

#include <string>

QuizScreen::QuizScreen()
    : bank(ResolveQuestionsPath()),
      score(),
      currentIndex(0),
      answered(false),
      questionsLoaded(false),
      progressLabel(nullptr),
      questionLabel(nullptr),
      optionButtons{nullptr, nullptr, nullptr, nullptr},
      feedbackLabel(nullptr),
      nextButton(nullptr),
      scoreLabel(nullptr)
{
}

QuizScreen::~QuizScreen()
{
    destroy();
}

void QuizScreen::create(HWND parent)
{
    // Load first: if the file is missing/empty there is nothing to show,
    // so go back to the menu without building any controls.
    if (!bank.loadQuestions())
    {
        std::string message =
            "Unable to load the question bank.\n\n"
            "Please check that this file exists and is correctly formatted:\n" +
            ResolveQuestionsPath();

        MessageBoxA(parent, message.c_str(), "Quiz Game - Error",
                    MB_OK | MB_ICONERROR);

        ShowMainMenuScreen();
        return;
    }

    questionsLoaded = true;

    // Week 9: surface skipped/malformed lines instead of losing them
    // silently (they used to only go to a console the GUI build has none
    // of). Non-fatal - the quiz still starts with whatever loaded cleanly.
    if (!bank.getWarnings().empty())
    {
        std::string message = "Loaded " + std::to_string(bank.getQuestionCount()) +
                              " question(s). The following line(s) in the "
                              "question file were skipped:\n\n";

        for (const std::string &warning : bank.getWarnings())
            message += "- " + warning + "\n";

        MessageBoxA(parent, message.c_str(), "Quiz Game - Question File Warnings",
                    MB_OK | MB_ICONWARNING);
    }

    progressLabel = CreateControl(parent, "STATIC", "",
                                  SS_CENTER | SS_CENTERIMAGE | SS_NOPREFIX,
                                  40, 20, 720, 30);
    ApplyDefaultFont(progressLabel);

    // No SS_CENTERIMAGE here: it would stop long questions from wrapping.
    questionLabel = CreateControl(parent, "STATIC", "",
                                  SS_CENTER | SS_NOPREFIX,
                                  40, 60, 720, 100);
    ApplyHeadingFont(questionLabel);

    // 2 x 2 grid of option buttons. BS_MULTILINE lets long options wrap.
    const int optionX[4] = {40, 410, 40, 410};
    const int optionY[4] = {180, 180, 265, 265};

    for (int i = 0; i < 4; i++)
    {
        optionButtons[i] = CreateControl(parent, "BUTTON", "",
                                         BS_PUSHBUTTON | BS_MULTILINE,
                                         optionX[i], optionY[i], 350, 70,
                                         ControlId::QuizOptionA + i);
    }

    feedbackLabel = CreateControl(parent, "STATIC", "",
                                  SS_CENTER | SS_NOPREFIX,
                                  40, 365, 720, 70);
    ApplyHeadingFont(feedbackLabel);

    // Hidden until the player answers.
    nextButton = CreateControl(parent, "BUTTON", "Next Question ->",
                               BS_DEFPUSHBUTTON,
                               280, 450, 240, 52, ControlId::QuizNext,
                               false);
    ApplyHeadingFont(nextButton);

    scoreLabel = CreateControl(parent, "STATIC", "",
                               SS_CENTER | SS_CENTERIMAGE | SS_NOPREFIX,
                               40, 530, 720, 40);
    ApplyDefaultFont(scoreLabel);

    currentIndex = 0;
    showQuestion();
}

void QuizScreen::destroy()
{
    HWND *controls[] = {&progressLabel, &questionLabel,
                        &optionButtons[0], &optionButtons[1],
                        &optionButtons[2], &optionButtons[3],
                        &feedbackLabel, &nextButton, &scoreLabel};

    for (HWND *control : controls)
    {
        if (*control)
        {
            DestroyWindow(*control);
            *control = nullptr;
        }
    }
}

void QuizScreen::showQuestion()
{
    answered = false;

    const Question &q = bank.getQuestion(currentIndex);

    SetWindowTextA(progressLabel,
                   ("Question " + std::to_string(currentIndex + 1) + " of " +
                    std::to_string(bank.getQuestionCount()))
                       .c_str());

    SetWindowTextA(questionLabel, q.questionText.c_str());

    for (int i = 0; i < 4; i++)
    {
        std::string text;
        text.push_back(static_cast<char>('A' + i));
        text += ". ";
        text += q.options[i];

        SetWindowTextA(optionButtons[i], EscapeAmpersands(text).c_str());
        EnableWindow(optionButtons[i], TRUE);
    }

    SetWindowTextA(feedbackLabel, "Choose an answer.");
    ShowWindow(nextButton, SW_HIDE);

    updateScoreLabel();
}

void QuizScreen::handleAnswer(int optionIndex)
{
    // Ignore clicks if the quiz isn't running or this question is already answered.
    if (!questionsLoaded || answered || optionIndex < 0 || optionIndex > 3)
        return;

    answered = true;

    const Question &q = bank.getQuestion(currentIndex);
    const int correctIndex = q.correctOption - 1;
    const bool isCorrect = (optionIndex == correctIndex);

    score.recordAnswer(isCorrect);

    if (isCorrect)
    {
        SetWindowTextA(feedbackLabel, "Correct!");
    }
    else
    {
        std::string message = "Incorrect. The correct answer was ";
        message.push_back(static_cast<char>('A' + correctIndex));
        message += ". ";
        message += q.options[correctIndex];
        SetWindowTextA(feedbackLabel, message.c_str());
    }

    // One answer per question: lock all four options, then reveal Next.
    for (int i = 0; i < 4; i++)
        EnableWindow(optionButtons[i], FALSE);

    const bool isLastQuestion = (currentIndex + 1 >= bank.getQuestionCount());
    SetWindowTextA(nextButton,
                   isLastQuestion ? "See Results ->" : "Next Question ->");
    ShowWindow(nextButton, SW_SHOW);
    SetFocus(nextButton);

    updateScoreLabel();
}

void QuizScreen::handleNext()
{
    if (!questionsLoaded || !answered)
        return;

    // Once we move on, a second click on Next must do nothing.
    answered = false;

    currentIndex++;

    if (currentIndex >= bank.getQuestionCount())
    {
        ShowResultScreen(bank.getQuestionCount(),
                         score.getCorrectCount(),
                         score.getIncorrectCount(),
                         score.getPoints(),
                         score.getAccuracy());
        return;
    }

    showQuestion();
}

void QuizScreen::updateScoreLabel()
{
    std::string text = "Score: " + std::to_string(score.getPoints()) + " pts   |   " +
                       "Correct: " + std::to_string(score.getCorrectCount()) + "   |   " +
                       "Incorrect: " + std::to_string(score.getIncorrectCount());

    SetWindowTextA(scoreLabel, text.c_str());
}

void QuizScreen::handleCommand(int controlId)
{
    switch (controlId)
    {
    case ControlId::QuizOptionA:
    case ControlId::QuizOptionB:
    case ControlId::QuizOptionC:
    case ControlId::QuizOptionD:
        handleAnswer(controlId - ControlId::QuizOptionA);
        break;

    case ControlId::QuizNext:
        handleNext();
        break;
    }
}
