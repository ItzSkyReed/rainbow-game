#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <sstream>
#include <cctype>
#include <map>
#include <filesystem>


#ifndef UNTITLED_GAME_H
#define UNTITLED_GAME_H

#ifdef max
#undef max
#endif

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::to_string;


void playGame(); // Функция для старта, перезапуска, выхода из программы

void game(const vector<string> &allColors, const string &PlayerName); // Основная функция игры

vector<string> getAllColors(const string &path); // Возвращает массив всех цветов из colors.txt

void PrintAllColors(const std::vector<std::string> &allColors); // Вывод всех возможных цветов перед игрой в консоль

vector<string> shuffleVector(const vector<string> &allColors, bool unique_sort); // Получение перемешенного вектора

vector<string> StringToVector(const string &colors_string); // Создание вектора из строки

string vectorToString(const vector<string> &vector); // Создание строки из вектора через точку с запятой

vector<bool> colorMatchOrderMatch(const vector<string> &userColors, const vector<string> &pcColors); // Проверка на точное совпадение

vector<bool> colorMatchNoOrder(const vector<string> &userColors, const vector<string> &pcColors); // Проверка на неточное совпадение

bool getUserInput(bool &getInput, const string &message); // Обработка ввода bool-переменных

void appendToFile(const std::string &line); // Запись результата игры в файл

bool fileExistance(const string &filename); // Проверка на существование файла

void configureHistoryFile(const string &filename); // Конфигурация файла истории игры

void configureColorsFile(const string &filename); // Конфигурация файла цветов для игры


#endif //UNTITLED_GAME_H
