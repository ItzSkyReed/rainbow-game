#include "game.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::to_string;


const string pathToColors = "colors.txt"; // ���� �� ����� c �������!
const string pathToHistory = "game_history.txt"; // ���� �� ����� � �������� ���!
int games = 0; // ���-�� ��� ������������
int allPoints = 0; // ���-�� ������ ������������

void playGame() {
    // ��������� ����� � ������� ��� ����, �������� ����� � ������ ������ ���� ��� �� ����������.
    configureColorsFile(pathToColors);
    // ��������� ����� � �������� ���, �������� ����� ���� ��� �� ����������.
    configureHistoryFile(pathToHistory);

    vector<string> allColors = getAllColors(pathToColors); // ��� �������� �����
    string PlayerName; // ��� ������������

    bool getRules;
    getUserInput(getRules, "������� '1' ��� ��������� ������, ������� '0' ��� �����������: ");
    if (getRules) {
        cout << "\n���� \"������\". ������ ������������ - ������� ������� �� ������� ������ � ���� ����� � ������������������.\n"
             << "�� ������ ������� ��������� �����: 10, ���� ������� � ������� ����, 9 � �� �������, 8 � � �������� � �.�.\n\n";
    }

    bool play_again = true;
    while (play_again) {
        if (games == 0) {
            while (PlayerName.empty()) {
                cout << "������� ��� ������������: ";
                cin.ignore();
                getline(cin, PlayerName);
                cout << endl;
                if (PlayerName.empty()) cout << "��� �� ����� ���� ������." << endl;
            }
            appendToFile("\n\n\n------------ ������ ������������: " + PlayerName + " ------------\n");
        }

        game(allColors, PlayerName); // ������ ����

        if (games > 0) {
            getUserInput(play_again, "������� '1' ��� ����� ����, '0' ��� ������: ");
            if (!play_again) {
                cout << "�� " << games << " ��� �� ������� " << allPoints << " ������" << endl;
                appendToFile("�� " + to_string(games) + " ��� ������������ " + PlayerName +
                                  " ������ " + to_string(allPoints) + " ������");
            }
        }
    }
    system("pause");
}


void game(const vector<string> &allColors, const string &PlayerName) {
    games += 1; // ����������� ������� ���

    // ���������� ������������, ����� �� �� ����� ����� ���� �����������
    bool uniqueColorsMode;
    // ��������� ������.
    getUserInput(uniqueColorsMode, "������� '1' ���� ������ ����� ����� �� �����������, ����� ������� '0': ");
    cout << endl;
    vector<string> colorsToGuess = shuffleVector(allColors, uniqueColorsMode); // ������� ������ ������ � ����������� �� ������
    if (uniqueColorsMode) cout << "������ ����� ��� ���������� ������." << endl;
    else cout << "������ ����� � ����������� ������." << endl;

    PrintAllColors(allColors); // �������� ������������ � ���� ��������� ������ (�� ����� colors.txt)

    string gameType = (uniqueColorsMode) ? "��� ���������� ������" : "� ����������� ������"; // ������ � ���� ����� � ����������� �� ����.
    string correctColorsString = vectorToString(colorsToGuess);
    appendToFile("���� " + to_string(games) + "; �����: " + gameType +
                      "; ���������� ������������������ ������: " + correctColorsString + ".");

    // ������� ����
    cout << "���� ��������." << endl << endl;
    cin.ignore();
    int pointsAndAttempts = 10;

    while (pointsAndAttempts > 0) {
        // ������� �������, ����� �������, ������� ��� ������ ���� ����������� (������� ��� ������ � ����������� ������)
        std::map<string, short> colorsCountMap;
        for (const string &str: colorsToGuess) { // �������� �� ������� � ����������� ������� ��� ������ ������ � map
            colorsCountMap[str]++;
        }

        cout << "������� ��������: " << pointsAndAttempts << endl; // �������� ������������ ���������� ���������� �������.

        // �������� ������� �� ������ ������ ������������
        vector<string> userColorsVector;
        string colorsFromUser;
        do { // �������� �� ������������ ��������� ������
            cout << "������� ������ ����� ����� ������� � ��������: ";
            getline(cin, colorsFromUser);
            userColorsVector = StringToVector(colorsFromUser);
            if (userColorsVector.size() != 4) {
                cout << "�� ����� �� 4 �����. ������� ������ �����." << endl;
            }

        } while (userColorsVector.size() != 4);

        // ������ � ���� ����������� ���������� ������� ������������ � �����, ������� �� ���
        appendToFile("������� " + to_string(10 - pointsAndAttempts) + "; ����� ������������: " + colorsFromUser);

        // ����� ������ � ����������� �����
        vector<bool> matchedColors = colorMatchOrderMatch(userColorsVector, colorsToGuess);
        for (int i = 0; i < 4; i++) {
            string color;
            if (matchedColors[i]) {
                color = userColorsVector[i];
                colorsCountMap[color]--; // ��������� ���������� ���-�� ������������� ����� � �������
                color[0] = (char)(std::toupper((unsigned char)(color[0]))); // ������ ������ ����� �� ��������� ��� �������
            } else color = "�� ������";
            cout << "���� " << i + 1 << ": " << color << endl;
        }

        // �������� �� ������
        if (std::equal(colorsToGuess.begin(), colorsToGuess.end(), userColorsVector.begin())) {
            cout << PlayerName << ", �� ��������, ������ " << pointsAndAttempts << " ������!" << endl;
            allPoints += pointsAndAttempts;
            appendToFile("\n������������ ��� ���������� ������������������, ������� " +
            to_string(pointsAndAttempts) + " ������\n\n");
            return;
        }

        // ����� ������ � ������������ � ������
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
    cout << "�� �� ������ ������� ����� �� 10 �������." << endl;
    cout << "������ ������: " << correctColorsString << "." << endl;
    appendToFile("\n" + PlayerName + " �� ���� �������� ���������� ������.\n\n");
}


// ���������� ������ ���� ������ �� colors.txt
vector<string> getAllColors(const string &path) {
    vector<string> colors;
    string temp_string;
    std::ifstream file(path);
    while (getline(file, temp_string)) {
        if (!temp_string.empty()) { // ���������, ��� ������ �� ������, ������ ��� �������� � � ������
            colors.push_back(temp_string);
        }
    }
    file.close();
    return colors;
}

// ����� ���� ��������� ������ � �������
void PrintAllColors(const vector<string> &allColors) {
    cout << "��������� ��������� �����: ";
    for (size_t i = 0; i < allColors.size(); ++i) {
        cout << allColors[i];
        if (i != allColors.size() - 1) cout << ", ";
        else cout << '.';
        if ((i + 1) % 9 == 0 && i != 0) cout << endl;
    }
    cout << endl;
}

// ������������� ������� ����� ��������� - � ����������� ���������� � ��� ����������
vector<string> shuffleVector(const vector<string> &allColors, const bool unique_sort) {
    vector<string> shuffledColors;
    if (unique_sort) {
        shuffledColors = allColors;
        std::shuffle(shuffledColors.begin(), shuffledColors.end(), std::mt19937(std::random_device()()));
        shuffledColors.resize(4);
    } else {
        std::random_device random;
        std::mt19937 rng(random());
        std::uniform_int_distribution<std::mt19937::result_type> RandomNumber(0, allColors.size() - 1);
        for (int i = 0; i < 4; i++) shuffledColors.push_back(allColors[RandomNumber(rng)]);
    }
    return shuffledColors;
}

// ����������� ������� � ������ ����� �������
string vectorToString(const vector <string> &vector) {
    if (vector.empty()) return ""; // ���������� ������ ������ ��� ������� �������
    string result = vector[0]; // �������������� ��������� ������ ���������
    for (size_t i = 1; i < vector.size(); ++i) result += ", " + vector[i];
    return result;
}

// ����������� ������ � ������, ����������� - �������.
vector<string> StringToVector(const string &colors_string) {
    vector<string> colors_vector;
    std::istringstream iss(colors_string);
    string color;
    while (getline(iss >> std::ws, color, ',')) { // ��������� ������ �� ����� �� ������� � ��������� �� � ������
        // �������� ��������� � �������� �������� � ������� �����
        color.erase(std::remove_if(color.begin(), color.end(), ::isspace), color.end());
        colors_vector.push_back(color);
    }
    return colors_vector;
}

// ����������� �������, ����������� true (���������� �����) ��� false (������������ �����) �� �������� ������ ������������
vector<bool> colorMatchOrderMatch(const vector<string> &userColors, const vector<string> &pcColors) {
    vector<bool> sortedColors;
    for (size_t i = 0; i < userColors.size(); ++i) sortedColors.push_back(userColors[i] == pcColors[i]);
    return sortedColors;
}

// ����������� �������, ����������� true (������� ����� � ������ ����������) ��� false (���������� ����� � ������ ����������)
vector<bool> colorMatchNoOrder(const vector<string> &userColors, const vector<string> &pcColors) {
    vector<bool> sortedColors;
    for (const string &userColor: userColors) {
        auto iterator = std::find(pcColors.begin(), pcColors.end(), userColor);
        sortedColors.push_back(iterator != pcColors.end());
    }
    return sortedColors;
}

// ����������� � ���� ����� (������������ ��� ������ ������� ����)
void appendToFile(const std::string &line) {
    std::ofstream file(pathToHistory, std::ios::app);
    if (file.is_open()) {
        file << line << endl;
    }
}

// ���������� ������ bool ���������� ������������
bool getUserInput(bool &getInput, const std::string &message) {
    while (true) {
        cout << message;
        cin >> getInput;
        if (std::cin.fail() || (getInput != 0 && getInput != 1)) {
            std::cout << "������ �����. ����������, ������� '1' ��� '0'." << endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else break;
    }
    return getInput;
}

// �������� �� ������������� �����
bool fileExistance(const string &filename) {
    bool exist = std::filesystem::exists(filename);
    return exist;
}

// �������� ����� colors.txt � ������ ����������� ������ ��� ����������
void configureColorsFile(const string &filename) {
    if (fileExistance(filename)) {
        cout << "���� � ������� ������� ������." << endl;
    } else {
        cout << "���� � �������� ��� �� ��� ������. �������� ������ ����� � ������ ����������� ������." << endl;
        std::ofstream file;
        file.open("colors.txt");
        if (file.is_open()) {
            file << "�������\n�����\n������\n�����\n�����\n������\n�������\n�������\n����������\n�����\n����������\n���������";
        }
        file.close();
    }
}

// �������� ����� game_history.txt ��� ��� ����������
void configureHistoryFile(const string &filename) {
    if (fileExistance(filename)) {
        cout << "���� � �������� ��� ������� ������." << endl << endl;
    } else {
        cout << "���� � �������� ��� �� ��� ������. �������� ������ �����." << endl << endl;
        std::ofstream file;
        // ��������� ���� ��� ������ � �������� ��� �� ������� �����
        file.open(filename, std::ios::out | std::ios::trunc);
        file.close();
    }
}