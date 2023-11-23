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
const string pathToColors = R"(C:\Users\itzsk\CLionProjects\untitled1\colors.txt)"; // Путь до файла c цветами!
const string pathToHistory = R"(C:\Users\itzsk\CLionProjects\untitled1\game_history.txt)"; // Путь до файла c историей!

int game_num = 0;
int allPoints = 0;
string PlayerName;
vector<string> all_colors;

vector<string> getAllColors(const string &path); // Возвращает массив всех цветов из colors.txt
void PrintAllColors(const std::vector<std::string> &allColors); // Вывод всех возможных цветов перед игрой в консоль.
vector<string> shuffleVector(const vector<string> &allColors, bool unique_sort); // Получение перемешенного вектора
vector<string> StringToVector(const string &colors_string); // Создание вектора из строки
string vectorToString(const vector<string> &vector); // Создание строки из вектора через точку с запятой
vector<bool> colorMatchOrderMatch(vector<string> &userColors, vector<string> &pcColors); // Проверка на точное совпадение.
vector<bool> colorMatchNoOrder(vector<string> &userColors, vector<string> &pcColors); // Проверка на неточное совпадение.
void appendToFile(const std::string &line); // Запись результата игры в файл
bool getUserInput(bool &getInput, const string &message); // Обработка ввода bool-переменных

void game(const vector<string> &allColors) {
    game_num += 1; // Увеличиваем счётчик игр
    // Спрашиваем пользователя, хочет ли он чтобы цвета были уникальными
    bool uniqueColorsMode;
    getUserInput(uniqueColorsMode, "Введите '1' если хотите чтобы цвета не повторялись, иначе введите '0': "); // Обработка ошибок.
    cout << endl;

    // Создаем вектор цветов в зависимости от ответа
    vector<string> colorsToGuess = shuffleVector(allColors, uniqueColorsMode);
    if (uniqueColorsMode) cout << "Выбран режим без повторения цветов." << endl;
    else cout << "Выбран режим с повторением цветов." << endl;

    PrintAllColors(allColors); // Сообщаем пользователю о всех возможных цветах (из файла colors.txt)

    PrintAllColors(colorsToGuess); // Сообщаем пользователю о всех возможных цветах (из файла colors.txt)

    // Запись в файл
    string gameType = (uniqueColorsMode) ? "без повторений цветов" : "с повторением цветов";
    string correctColorsString = vectorToString(colorsToGuess);
    appendToFile("Игра " + to_string(game_num) + "; Режим: " + gameType + "; Правильная последовательность цветов: " + correctColorsString);

    // Процесс игры
    cout << "Игра началась." << endl << endl;
    cin.ignore();
    unsigned short pointsAndAttempts = 10;

    while (pointsAndAttempts > 0) {
        std::map<string, short> colorsCountMap; // Создаем словарь, чтобы хранить, сколько раз каждый цвет встречается для режима с повторением цветов

        // Проходим по вектору и увеличиваем счетчик для каждой строки в map
        for (const auto &str: colorsToGuess) {
            colorsCountMap[str]++;
        }

        cout << "Попыток осталось: " << pointsAndAttempts << endl;

        // Создание вектора из строки цветов пользователя
        vector<string> userColorsVector;
        string colorsFromUser;
        do {
            cout << "Введите четыре цвета через ', ': ";
            getline(cin, colorsFromUser);
            userColorsVector = StringToVector(colorsFromUser);
            if (userColorsVector.size() != 4) cout << "Вы ввели не 4 цвета. Введите по новому." << endl;
        } while (userColorsVector.size() != 4);

        // Запись в файл
        appendToFile("Попытка " + to_string(10 - pointsAndAttempts) + "; Цвета пользователя: " + colorsFromUser);

        // Вывод цветов с точным совпадением
        vector<bool> matchedColors = colorMatchOrderMatch(userColorsVector, colorsToGuess);
        for (int i = 0; i < 4; i++) {
            string color;
            if (matchedColors[i]) {
                color = userColorsVector[i];
                colorsCountMap[color]--;
                color[0] = std::toupper(color[0]);
            } else color = "Не угадан";
            cout << "Цвет " << i + 1 << ": " << color << endl;
        }

        // Проверка на победу!
        if (std::equal(colorsToGuess.begin(), colorsToGuess.end(), userColorsVector.begin())) {
            cout << PlayerName << ", вы победили, набрав " << pointsAndAttempts << " баллов!" << endl;
            allPoints += pointsAndAttempts;
            appendToFile("Пользователь ввёл правильную последовательность, получив " + to_string(pointsAndAttempts) + " баллов\n");
            return;
        }

        // Вывод цветов с неточным совпадением
        vector<bool> matchedColorsWithoutOrder = colorMatchNoOrder(userColorsVector, colorsToGuess);
        cout << "Цвета на неправильном месте, но присутствующие в списке: ";
        bool anyColor = false;
        for (int i = 0; i < 4; i++) {
            if (matchedColorsWithoutOrder[i] and (colorsCountMap[userColorsVector[i]] != 0)) {
                colorsCountMap[userColorsVector[i]] = 0;
                if (anyColor) cout << ", ";
                cout << userColorsVector[i];
                if (!anyColor) anyColor = true;
            }
        }
        if (!anyColor) cout << "Отсутствуют";
        pointsAndAttempts--;
        cout << endl;
    }
    cout << "Вы исчерпали все 10 попыток." << endl;
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    // Получаем список цветов и выводим пользователю
    all_colors = getAllColors(pathToColors);

    // Спрашиваем пользователя хочет ли он узнать правила
    bool getRules;
    getUserInput(getRules, "Введите '1' если хотите увидеть правила, иначе введите '0': "); // Обработка ошибок.
    if (getRules) {
        cout << endl << "Игра \"Радуга\". Задача пользователя угадать порядок цветов и сами цвета в сгенерированной последовательности." << endl \
 << "За каждую попытку снимаются баллы: 10, если последовательность угадана с первого раза, 9 – со второго, 8 – с третьего и т.п." << endl << endl;
    }

    // Начинаем процесс игры
    bool play_again = true;
    while (play_again) {
        if (game_num == 0) {
            while (PlayerName.empty()) {
                cout << "Введите имя пользователя: ";
                cin.ignore();
                getline(std::cin, PlayerName);
                cout << endl;
                if (PlayerName.empty()) cout << "Имя не может быть пустым." << endl;
            }
            appendToFile("------------ Играет пользователь: " + PlayerName + " ------------\n");
            game(all_colors);
        } else {
            getUserInput(play_again, "Введите '1' если хотите сыграть еще раз, '0' если хотите выйти: "); // Обработка ошибок.
            if (!play_again) {
                cout << "За " << game_num << " игр вы набрали " << allPoints << " баллов" << endl;
                appendToFile("За " + to_string(game_num) + " игр пользователь " + PlayerName + " набрал " + to_string(allPoints) + " баллов" + "\n\n");
            } else game(all_colors);
        }
    }
    system("pause");
}

// Возвращает массив всех цветов из colors.txt
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

// Вывод всех возможных цветов перед игрой в консоль
void PrintAllColors(const std::vector<std::string> &allColors) {
    std::cout << "Возможные доступные цвета: ";
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
    if (vector.empty()) return ""; // Возвращаем пустую строку для пустого вектора
    string result = vector[0]; // Инициализируем результат первым элементом
    for (size_t i = 1; i < vector.size(); ++i) result += ", " + vector[i];
    return result;
}

vector<string> StringToVector(const string &colors_string) {
    std::vector<std::string> colors_vector;
    std::istringstream iss(colors_string);

    // Разделяем строку на слова по запятой и добавляем их в вектор
    std::string color;
    while (std::getline(iss >> std::ws, color, ',')) {
        // Удаляем начальные и конечные пробелы у каждого слова
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
            std::cout << "Ошибка ввода. Пожалуйста, введите '1' или '0'." << std::endl;
        } else if (getInput != 0 && getInput != 1) std::cout << "Ошибка ввода. Пожалуйста, введите '1' или '0'." << std::endl;
        else isValidInput = true;
    }
    return getInput;
}
