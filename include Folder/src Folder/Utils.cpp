#include <cstdlib>
#include "../Utils.h"
void Utils::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
