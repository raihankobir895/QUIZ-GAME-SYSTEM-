#ifndef GUIAPP_H
#define GUIAPP_H

// Trim rarely-used Win32 headers and stop windows.h from defining min()/max()
// macros that clash with the C++ standard library.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <string>

// ---------------------------------------------------------------------------
// Control IDs. Each screen owns its own range so a WM_COMMAND can never be
// mistaken for a button on another screen.
//   Menu   : 1001 - 1002
//   Quiz   : 2001 - 2005
//   Result : 3001 - 3002
// ---------------------------------------------------------------------------
namespace ControlId
{
    // Main menu
    constexpr int MenuStart = 1001;
    constexpr int MenuExit = 1002;

    // Quiz screen (options A-D must stay consecutive: index = id - QuizOptionA)
    constexpr int QuizOptionA = 2001;
    constexpr int QuizOptionB = 2002;
    constexpr int QuizOptionC = 2003;
    constexpr int QuizOptionD = 2004;
    constexpr int QuizNext = 2005;

    // Result screen
    constexpr int ResultPlayAgain = 3001;
    constexpr int ResultExit = 3002;
}

// Size of the window's client area (the part inside the title bar/borders).
namespace Layout
{
    constexpr int CLIENT_WIDTH = 800;
    constexpr int CLIENT_HEIGHT = 600;
}

// ---------------------------------------------------------------------------
// Screen switching. These do not switch instantly: they queue the switch and
// carry it out once the current button click has finished being processed, so
// a screen is never destroyed while one of its own buttons is still running.
// ---------------------------------------------------------------------------
void ShowMainMenuScreen();
void ShowQuizScreen();
void ShowResultScreen(int totalQuestions, int correct, int incorrect,
                      int points, double accuracy);
void ExitApplication();

// ---------------------------------------------------------------------------
// Helpers shared by all screens
// ---------------------------------------------------------------------------

// Gives a control a modern Segoe UI font instead of the tiny legacy Win32 one.
void ApplyDefaultFont(HWND control);
void ApplyHeadingFont(HWND control); // larger, semi-bold (questions, results)
void ApplyTitleFont(HWND control);   // largest, bold (screen titles)

// Creates a child control (STATIC label or BUTTON) inside `parent`.
// Pass id = 0 for labels that never send commands.
HWND CreateControl(HWND parent, const char *className, const std::string &text,
                   DWORD style, int x, int y, int width, int height,
                   int id = 0, bool visible = true);

// In button text a single '&' is a keyboard-shortcut marker and is NOT drawn.
// Option texts such as "&" (a C++ operator!) must be doubled to "&&" first.
std::string EscapeAmpersands(const std::string &text);

// Full path of questions.txt: looked up next to the .exe first, then relative
// to the current working directory (the project root).
std::string ResolveQuestionsPath();

// Full path of results.txt (Week 7/8 saved quiz history), resolved the same
// way as ResolveQuestionsPath() so it works both run-from-the-.exe and
// run-from-the-project-root.
std::string ResolveResultsPath();

#endif
