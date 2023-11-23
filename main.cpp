#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <windows.h>
#include <sstream>
#include <cctype>
#include <map>

#ifdef max
#undef max
#endif

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::to_string;
const string pathToColors = R"(C:\Users\itzsk\CLionProjects\untitled1\colors.txt)"; // ���� �� ����� c �������!
const string pathToHistory = R"(C:\Users\itzsk\CLionProjects\untitled1\game_history.txt)"; // ���� �� ����� c �������!

int game_num = 0;
int allPoints = 0;
string PlayerName;
vector<string> all_colors;

vector<string> getAllColors(const string &path); // ���������� ������ ���� ������ �� colors.txt
void PrintAllColors(const std::vector<std::string> &allColors); // ����� ���� ��������� ������ ����� ����� � �������.
vector<string> shuffleVector(const vector<string> &allColors, bool unique_sort); // ��������� ������������� �������
vector<string> StringToVector(const string &colors_string); // �������� ������� �� ������
string vectorToString(const vector<string> &vector); // �������� ������ �� ������� ����� ����� � �������
vector<bool> colorMatchOrderMatch(vector<string> &userColors, vector<string> &pcColors); // �������� �� ������ ����������.
vector<bool> colorMatchNoOrder(vector<string> &userColors, vector<string> &pcColors); // �������� �� �������� ����������.
void appendToFile(const std::string &line); // ������ ���������� ���� � ����
bool getUserInput(bool &getInput, const string &message); // ��������� ����� bool-����������

void game(const vector<string> &allColors) {
    game_num += 1; // ����������� ������� ���
    // ���������� ������������, ����� �� �� ����� ����� ���� �����������
    bool uniqueColorsMode;
    getUserInput(uniqueColorsMode, "������� '1' ���� ������ ����� ����� �� �����������, ����� ������� '0': "); // ��������� ������.
    cout << endl;

    // ������� ������ ������ � ����������� �� ������
    vector<string> colorsToGuess = shuffleVector(allColors, uniqueColorsMode);
    if (uniqueColorsMode) cout << "������ ����� ��� ���������� ������." << endl;
    else cout << "������ ����� � ����������� ������." << endl;

    PrintAllColors(allColors); // �������� ������������ � ���� ��������� ������ (�� ����� colors.txt)

    PrintAllColors(colorsToGuess); // �������� ������������ � ���� ��������� ������ (�� ����� colors.txt)

    // ������ � ����
    string gameType = (uniqueColorsMode) ? "��� ���������� ������" : "� ����������� ������";
    string correctColorsString = vectorToString(colorsToGuess);
    appendToFile("���� " + to_string(game_num) + "; �����: " + gameType + "; ���������� ������������������ ������: " + correctColorsString);

    // ������� ����
    cout << "���� ��������." << endl << endl;
    cin.ignore();
    unsigned short pointsAndAttempts = 10;

    while (pointsAndAttempts > 0) {
        std::map<string, short> colorsCountMap; // ������� �������, ����� �������, ������� ��� ������ ���� ����������� ��� ������ � ����������� ������

        // �������� �� ������� � ����������� ������� ��� ������ ������ � map
        for (const auto &str: colorsToGuess) {
            colorsCountMap[str]++;
        }

        cout << "������� ��������: " << pointsAndAttempts << endl;

        // �������� ������� �� ������ ������ ������������
        vector<string> userColorsVector;
        string colorsFromUser;
        do {
            cout << "������� ������ ����� ����� ', ': ";
            getline(cin, colorsFromUser);
            userColorsVector = StringToVector(colorsFromUser);
            if (userColorsVector.size() != 4) cout << "�� ����� �� 4 �����. ������� �� ������." << endl;
        } while (userColorsVector.size() != 4);

        // ������ � ����
        appendToFile("������� " + to_string(10 - pointsAndAttempts) + "; ����� ������������: " + colorsFromUser);

        // ����� ������ � ������ �����������
        vector<bool> matchedColors = colorMatchOrderMatch(userColorsVector, colorsToGuess);
        for (int i = 0; i < 4; i++) {
            string color;
            if (matchedColors[i]) {
                color = userColorsVector[i];
                colorsCountMap[color]--;
                color[0] = std::toupper(color[0]);
            } else color = "�� ������";
            cout << "���� " << i + 1 << ": " << color << endl;
        }

        // �������� �� ������!
        if (std::equal(colorsToGuess.begin(), colorsToGuess.end(), userColorsVector.begin())) {
            cout << PlayerName << ", �� ��������, ������ " << pointsAndAttempts << " ������!" << endl;
            allPoints += pointsAndAttempts;
            appendToFile("������������ ��� ���������� ������������������, ������� " + to_string(pointsAndAttempts) + " ������\n");
            return;
        }

        // ����� ������ � �������� �����������
        vector<bool> matchedColorsWithoutOrder = colorMatchNoOrder(userColorsVector, colorsToGuess);
        cout << "����� �� ������������ �����, �� �������������� � ������: ";
        bool anyColor = false;
        for (int i = 0; i < 4; i++) {
            if (matchedColorsWithoutOrder[i] and (colorsCountMap[userColorsVector[i]] != 0)) {
                colorsCountMap[userColorsVector[i]] = 0;
                if (anyColor) cout << ", ";
                cout << userColorsVector[i];
                if (!anyColor) anyColor = true;
            }
        }
        if (!anyColor) cout << "�����������";
        pointsAndAttempts--;
        cout << endl;
    }
    cout << "�� ��������� ��� 10 �������." << endl;
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    // �������� ������ ������ � ������� ������������
    all_colors = getAllColors(pathToColors);

    // ���������� ������������ ����� �� �� ������ �������
    bool getRules;
    getUserInput(getRules, "������� '1' ���� ������ ������� �������, ����� ������� '0': "); // ��������� ������.
    if (getRules) {
        cout << endl << "���� \"������\". ������ ������������ ������� ������� ������ � ���� ����� � ��������������� ������������������." << endl \
 << "�� ������ ������� ��������� �����: 10, ���� ������������������ ������� � ������� ����, 9 � �� �������, 8 � � �������� � �.�." << endl << endl;
    }

    // �������� ������� ����
    bool play_again = true;
    while (play_again) {
        if (game_num == 0) {
            while (PlayerName.empty()) {
                cout << "������� ��� ������������: ";
                cin.ignore();
                getline(std::cin, PlayerName);
                cout << endl;
                if (PlayerName.empty()) cout << "��� �� ����� ���� ������." << endl;
            }
            appendToFile("------------ ������ ������������: " + PlayerName + " ------------\n");
            game(all_colors);
        } else {
            getUserInput(play_again, "������� '1' ���� ������ ������� ��� ���, '0' ���� ������ �����: "); // ��������� ������.
            if (!play_again) {
                cout << "�� " << game_num << " ��� �� ������� " << allPoints << " ������" << endl;
                appendToFile("�� " + to_string(game_num) + " ��� ������������ " + PlayerName + " ������ " + to_string(allPoints) + " ������" + "\n\n");
            } else game(all_colors);
        }
    }
    system("pause");
}

// ���������� ������ ���� ������ �� colors.txt
vector<string> getAllColors(const string &path) {
    std::vector<string> colors;
    string temp_string;
    std::ifstream file(path);
    while (!file.eof()) {
        getline(file, temp_string);
        colors.push_back(temp_string);
    }
    file.close();
    return colors;
}

// ����� ���� ��������� ������ ����� ����� � �������
void PrintAllColors(const std::vector<std::string> &allColors) {
    std::cout << "��������� ��������� �����: ";
    for (size_t i = 0; i < allColors.size(); ++i) {
        std::cout << allColors[i];
        if (i != allColors.size() - 1) std::cout << ", ";
        else std::cout << '.';
        if ((i + 1) % 9 == 0 && i != 0) std::cout << std::endl;
    }
    std::cout << std::endl;
}

vector<string> shuffleVector(const vector<string> &allColors, const bool unique_sort) {
    if (unique_sort) {
        vector<string> shuffledColors = allColors;
        std::shuffle(shuffledColors.begin(), shuffledColors.end(), std::mt19937(std::random_device()()));
        return shuffledColors;
    } else {
        std::srand(std::time(nullptr));
        vector<string> shuffledColors;
        for (int i = 0; i < allColors.size(); i++) {
            shuffledColors.push_back(allColors[rand() % allColors.size()]);
        }
        return shuffledColors;
    }
}

string vectorToString(const vector <string> &vector) {
    if (vector.empty()) return ""; // ���������� ������ ������ ��� ������� �������
    string result = vector[0]; // �������������� ��������� ������ ���������
    for (size_t i = 1; i < vector.size(); ++i) result += ", " + vector[i];
    return result;
}

vector<string> StringToVector(const string &colors_string) {
    std::vector<std::string> colors_vector;
    std::istringstream iss(colors_string);

    // ��������� ������ �� ����� �� ������� � ��������� �� � ������
    std::string color;
    while (std::getline(iss >> std::ws, color, ',')) {
        // ������� ��������� � �������� ������� � ������� �����
        color.erase(std::remove_if(color.begin(), color.end(), ::isspace), color.end());
        colors_vector.push_back(color);
    }
    return colors_vector;
}


vector<bool> colorMatchOrderMatch(vector<string> &userColors, vector<string> &pcColors) {
    vector<bool> sortedColors;
    for (int i = 0; i < pcColors.size(); i++) sortedColors.push_back(userColors[i] == pcColors[i]);
    return sortedColors;
}

vector<bool> colorMatchNoOrder(vector<string> &userColors, vector<string> &pcColors) {
    vector<bool> sortedColors;
    for (int i = 0; i < pcColors.size(); i++) {
        if (std::find(pcColors.begin(), pcColors.end(), userColors[i]) != pcColors.end())
            sortedColors.push_back(true);
        else sortedColors.push_back(false);
    }
    return sortedColors;
}

void appendToFile(const std::string &line) {
    std::ofstream file(pathToHistory, std::ios::app);
    if (file.is_open()) {
        file << line << endl;
        file.close();
    }
}

bool getUserInput(bool &getInput, const string &message) {
    bool isValidInput = false;
    while (!isValidInput) {
        std::cout << message;
        std::cin >> getInput;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "������ �����. ����������, ������� '1' ��� '0'." << std::endl;
        } else if (getInput != 0 && getInput != 1) std::cout << "������ �����. ����������, ������� '1' ��� '0'." << std::endl;
        else isValidInput = true;
    }
    return getInput;
}