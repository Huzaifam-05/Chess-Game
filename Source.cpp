#include "chess.h"

int main()
{
    enableANSI();

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    system("chcp 65001 > nul");

    ios::sync_with_stdio(false);

    Board b;
    b.startScreen();
    b.start();

    return 0;
}