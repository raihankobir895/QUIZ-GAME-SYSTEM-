#include <cstdlib>
#include "../Utils.h"

// Clears the console screen in a way that works on both Windows and
// Unix-like systems (Linux/macOS), so the game can be built/run anywhere.
void Utils::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
