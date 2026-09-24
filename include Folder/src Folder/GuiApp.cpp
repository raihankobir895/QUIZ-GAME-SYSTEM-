#include "../GuiApp.h"
#include "../MainMenu.h"
#include "../QuizScreen.h"
#include "../ResultScreen.h"

#include <functional>
#include <memory>
#include <utility>

// Native Win32 GUI entry point.
// QuestionBank and Score are used exactly as they were in the console version.

namespace
{
    const char WINDOW_CLASS_NAME[] = "QuizGameMainWindow";

    // Private message: "carry out the queued screen switch now".
    const UINT WM_SWITCH_SCREEN = WM_APP + 1;

    HWND g_mainWindow = nullptr;

    HFONT g_defaultFont = nullptr;
    HFONT g_headingFont = nullptr;
    HFONT g_titleFont = nullptr;

    // Only the screen that is currently showing exists; the others are null.
    std::unique_ptr<MainMenu> g_mainMenu;
    std::unique_ptr<QuizScreen> g_quizScreen;
    std::unique_ptr<ResultScreen> g_resultScreen;

    // The screen switch waiting to be carried out (see QueueSwitch).
    std::function<void()> g_pendingSwitch;

    // ------------------------------------------------------------------ fonts

    HFONT MakeFont(int height, int weight)
    {
        return CreateFontA(
            height, 0, 0, 0, weight, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    }

    void EnsureFonts()
    {
        if (g_defaultFont)
            return;

        // Negative height = character height in pixels.
        g_defaultFont = MakeFont(-18, FW_NORMAL);
        g_headingFont = MakeFont(-24, FW_SEMIBOLD);
        g_titleFont = MakeFont(-44, FW_BOLD);
    }

    void ReleaseFonts()
    {
        if (g_defaultFont)
            DeleteObject(g_defaultFont);
        if (g_headingFont)
            DeleteObject(g_headingFont);
        if (g_titleFont)
            DeleteObject(g_titleFont);

        g_defaultFont = g_headingFont = g_titleFont = nullptr;
    }

    void SetControlFont(HWND control, HFONT font)
    {
        if (control && font)
            SendMessageA(control, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);
    }

    // --------------------------------------------------------- screen control

    void DestroyAllScreens()
    {
        // Each screen's destructor calls destroy(), which removes its controls.
        g_mainMenu.reset();
        g_quizScreen.reset();
        g_resultScreen.reset();
    }

    // Why the switch is deferred: a button's own click handler must not
    // destroy that button. Instead we store the switch here and post a
    // message to ourselves; it runs after the click has fully finished.
    void QueueSwitch(std::function<void()> action)
    {
        g_pendingSwitch = std::move(action);

        if (g_mainWindow)
            PostMessageA(g_mainWindow, WM_SWITCH_SCREEN, 0, 0);
    }

    void RouteCommand(int id)
    {
        if (id >= ControlId::MenuStart && id <= ControlId::MenuExit)
        {
            if (g_mainMenu)
                g_mainMenu->handleCommand(id);
        }
        else if (id >= ControlId::QuizOptionA && id <= ControlId::QuizNext)
        {
            if (g_quizScreen)
                g_quizScreen->handleCommand(id);
        }
        else if (id >= ControlId::ResultPlayAgain && id <= ControlId::ResultExit)
        {
            if (g_resultScreen)
                g_resultScreen->handleCommand(id);
        }
    }

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_CREATE:
            g_mainWindow = hwnd;
            ShowMainMenuScreen();
            return 0;

        case WM_SWITCH_SCREEN:
        {
            std::function<void()> action = std::move(g_pendingSwitch);
            g_pendingSwitch = nullptr;

            if (action)
                action();
            return 0;
        }

        case WM_COMMAND:
            if (HIWORD(wParam) == BN_CLICKED)
                RouteCommand(LOWORD(wParam));
            return 0;

        case WM_DESTROY:
            g_pendingSwitch = nullptr;
            DestroyAllScreens();
            ReleaseFonts();
            g_mainWindow = nullptr;
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProcA(hwnd, message, wParam, lParam);
    }
} // namespace

// ---------------------------------------------------------- public: screens

void ShowMainMenuScreen()
{
    QueueSwitch([]()
    {
        DestroyAllScreens();
        g_mainMenu.reset(new MainMenu());
        g_mainMenu->create(g_mainWindow);
    });
}

void ShowQuizScreen()
{
    QueueSwitch([]()
    {
        DestroyAllScreens();
        g_quizScreen.reset(new QuizScreen());
        g_quizScreen->create(g_mainWindow);
    });
}

void ShowResultScreen(int totalQuestions, int correct, int incorrect,
                      int points, double accuracy)
{
    QueueSwitch([=]()
    {
        DestroyAllScreens();
        g_resultScreen.reset(new ResultScreen());
        g_resultScreen->create(g_mainWindow, totalQuestions, correct,
                               incorrect, points, accuracy);
    });
}

void ExitApplication()
{
    if (g_mainWindow)
        PostMessageA(g_mainWindow, WM_CLOSE, 0, 0);
}

// ----------------------------------------------------------- public: helpers

void ApplyDefaultFont(HWND control)
{
    EnsureFonts();
    SetControlFont(control, g_defaultFont);
}

void ApplyHeadingFont(HWND control)
{
    EnsureFonts();
    SetControlFont(control, g_headingFont);
}

void ApplyTitleFont(HWND control)
{
    EnsureFonts();
    SetControlFont(control, g_titleFont);
}

HWND CreateControl(HWND parent, const char *className, const std::string &text,
                   DWORD style, int x, int y, int width, int height,
                   int id, bool visible)
{
    DWORD fullStyle = WS_CHILD | style;
    if (visible)
        fullStyle |= WS_VISIBLE;

    HWND control = CreateWindowExA(
        0, className, text.c_str(), fullStyle,
        x, y, width, height,
        parent,
        id ? reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)) : nullptr,
        GetModuleHandleA(nullptr),
        nullptr);

    ApplyDefaultFont(control);
    return control;
}

std::string EscapeAmpersands(const std::string &text)
{
    std::string result;
    result.reserve(text.size());

    for (char ch : text)
    {
        result.push_back(ch);
        if (ch == '&')
            result.push_back('&');
    }

    return result;
}

namespace
{
    // Shared by ResolveQuestionsPath() and ResolveResultsPath(): look for
    // `relative` next to the running .exe first (so the game works no
    // matter which folder it was launched from), then fall back to the
    // current working directory (the project root).
    std::string ResolveDataPath(const std::string &relative, bool requireExisting)
    {
        char buffer[MAX_PATH] = {};
        DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);

        if (length > 0 && length < MAX_PATH)
        {
            std::string exePath(buffer, length);
            std::size_t slash = exePath.find_last_of("\\/");

            if (slash != std::string::npos)
            {
                std::string candidate = exePath.substr(0, slash) + "/" + relative;

                if (requireExisting)
                {
                    DWORD attributes = GetFileAttributesA(candidate.c_str());
                    if (attributes != INVALID_FILE_ATTRIBUTES &&
                        !(attributes & FILE_ATTRIBUTE_DIRECTORY))
                    {
                        return candidate;
                    }
                }
                else
                {
                    // results.txt may not exist yet on a fresh install, so we
                    // can't gate this path on the file already being there -
                    // just prefer the exe folder whenever we can locate it.
                    return candidate;
                }
            }
        }

        // Fall back to the current working directory (the project root).
        return relative;
    }
} // namespace

std::string ResolveQuestionsPath()
{
    // The question bank must already exist, so only accept the exe-relative
    // candidate if that file is actually there; otherwise fall back.
    return ResolveDataPath("include Folder/docs Folder/data Folder/questions.txt",
                           /*requireExisting=*/true);
}

std::string ResolveResultsPath()
{
    // The results file is created on first save, so it won't exist yet on a
    // clean checkout - don't gate the exe-relative candidate on existence.
    return ResolveDataPath("include Folder/docs Folder/data Folder/results.txt",
                           /*requireExisting=*/false);
}

// ------------------------------------------------------------------ WinMain

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSEXA wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClassExA(&wc))
    {
        MessageBoxA(nullptr, "Could not register the window class.",
                    "Quiz Game - Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Fixed-size window (no resize / maximize) whose *client area* is 800x600.
    const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    RECT frame = {0, 0, Layout::CLIENT_WIDTH, Layout::CLIENT_HEIGHT};
    AdjustWindowRect(&frame, style, FALSE);

    const int windowWidth = frame.right - frame.left;
    const int windowHeight = frame.bottom - frame.top;

    int x = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;

    HWND window = CreateWindowExA(
        0, WINDOW_CLASS_NAME, "Quiz Game", style,
        x, y, windowWidth, windowHeight,
        nullptr, nullptr, hInstance, nullptr);

    if (!window)
    {
        MessageBoxA(nullptr, "Could not create the main window.",
                    "Quiz Game - Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    ShowWindow(window, nCmdShow);
    UpdateWindow(window);

    MSG msg = {};
    while (GetMessageA(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return static_cast<int>(msg.wParam);
}
