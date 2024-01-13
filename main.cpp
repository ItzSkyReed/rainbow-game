#include <iostream>
#include <windows.h>
#include "game.h"

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");
    playGame();
}