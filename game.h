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


void playGame(); // ������� ��� ������, �����������, ������ �� ���������

void game(const vector<string> &allColors, const string &PlayerName); // �������� ������� ����

vector<string> getAllColors(const string &path); // ���������� ������ ���� ������ �� colors.txt

void PrintAllColors(const std::vector<std::string> &allColors); // ����� ���� ��������� ������ ����� ����� � �������

vector<string> shuffleVector(const vector<string> &allColors, bool unique_sort); // ��������� ������������� �������

vector<string> StringToVector(const string &colors_string); // �������� ������� �� ������

string vectorToString(const vector<string> &vector); // �������� ������ �� ������� ����� ����� � �������

vector<bool> colorMatchOrderMatch(const vector<string> &userColors, const vector<string> &pcColors); // �������� �� ������ ����������

vector<bool> colorMatchNoOrder(const vector<string> &userColors, const vector<string> &pcColors); // �������� �� �������� ����������

bool getUserInput(bool &getInput, const string &message); // ��������� ����� bool-����������

void appendToFile(const std::string &line); // ������ ���������� ���� � ����

bool fileExistance(const string &filename); // �������� �� ������������� �����

void configureHistoryFile(const string &filename); // ������������ ����� ������� ����

void configureColorsFile(const string &filename); // ������������ ����� ������ ��� ����


#endif //UNTITLED_GAME_H
