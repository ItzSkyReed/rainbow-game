#include "game.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::to_string;


const string pathToColors = "colors.txt"; // Путь до файла c цветами!
const string pathToHistory = "game_history.txt"; // Путь до файла с историей игр!
int games = 0; // Кол-во игр пользователя
int allPoints = 0; // Кол-во цветов пользователя

void playGame() {
    // Настройка файла с цветами для игры, создание файла и запись цветов если его не существует.
    configureColorsFile(pathToColors);
    // Настройка файла с историей игр, создание файла если его не существует.
    configureHistoryFile(pathToHistory);

    vector<string> allColors = getAllColors(pathToColors); // Тут хранятся цвета
    string PlayerName; // Имя пользователя

    bool getRules;
    getUserInput(getRules, "Введите '1' для просмотра правил, введите '0' для продолжения: ");
    if (getRules) {
        cout << "\nИгра \"Радуга\". Задача пользователя - угадать порядок из четырех цветов и сами цвета в последовательности.\n"
             << "За каждую попытку снимаются баллы: 10, если угадано с первого раза, 9 – со второго, 8 – с третьего и т.п.\n\n";
    }

    bool play_again = true;
    while (play_again) {
        if (games == 0) {
            while (PlayerName.empty()) {
                cout << "Введите имя пользователя: ";
                cin.ignore();
                getline(cin, PlayerName);
                cout << endl;
                if (PlayerName.empty()) cout << "Имя не может быть пустым." << endl;
            }
            appendToFile("\n\n\n------------ Играет пользователь: " + PlayerName + " ------------\n");
        }

        game(allColors, PlayerName); // Запуск игры

        if (games > 0) {
            getUserInput(play_again, "Введите '1' для новой игры, '0' для выхода: ");
            if (!play_again) {
                cout << "За " << games << " игр вы набрали " << allPoints << " баллов" << endl;
                appendToFile("За " + to_string(games) + " игр пользователь " + PlayerName +
                                  " набрал " + to_string(allPoints) + " баллов");
            }
        }
    }
    system("pause");
}


void game(const vector<string> &allColors, const string &PlayerName) {
    games += 1; // Увеличиваем счётчик игр

    // Спрашиваем пользователя, хочет ли он чтобы цвета были уникальными
    bool uniqueColorsMode;
    // Обработка ошибок.
    getUserInput(uniqueColorsMode, "Введите '1' если хотите чтобы цвета не повторялись, иначе введите '0': ");
    cout << endl;
    vector<string> colorsToGuess = shuffleVector(allColors, uniqueColorsMode); // Создаем вектор цветов в зависимости от ответа
    if (uniqueColorsMode) cout << "Выбран режим без повторения цветов." << endl;
    else cout << "Выбран режим с повторением цветов." << endl;

    PrintAllColors(allColors); // Сообщаем пользователю о всех возможных цветах (из файла colors.txt)

    string gameType = (uniqueColorsMode) ? "без повторений цветов" : "с повторением цветов"; // Запись в файл строк с информацией об игре.
    string correctColorsString = vectorToString(colorsToGuess);
    appendToFile("Игра " + to_string(games) + "; Режим: " + gameType +
                      "; Правильная последовательность цветов: " + correctColorsString + ".");

    // Процесс игры
    cout << "Игра началась." << endl << endl;
    cin.ignore();
    int pointsAndAttempts = 10;

    while (pointsAndAttempts > 0) {
        // Создаем словарь, чтобы хранить, сколько раз каждый цвет встречается (полезно для режима с повторением цветов)
        std::map<string, short> colorsCountMap;
        for (const string &str: colorsToGuess) { // Проходим по вектору и увеличиваем счетчик для каждой строки в map
            colorsCountMap[str]++;
        }

        cout << "Попыток осталось: " << pointsAndAttempts << endl; // Сообщаем пользователю количество оставшихся попыток.

        // Создание вектора из строки цветов пользователя
        vector<string> userColorsVector;
        string colorsFromUser;
        do { // Проверка на правильность введенной строки
            cout << "Введите четыре цвета через запятую с пробелом: ";
            getline(cin, colorsFromUser);
            userColorsVector = StringToVector(colorsFromUser);
            if (userColorsVector.size() != 4) {
                cout << "Вы ввели не 4 цвета. Введите строку снова." << endl;
            }

        } while (userColorsVector.size() != 4);

        // Запись в файл актуального количества попыток пользователя и цвета, которые он ввёл
        appendToFile("Попытка " + to_string(10 - pointsAndAttempts) + "; Цвета пользователя: " + colorsFromUser);

        // Вывод цветов с совпадением места
        vector<bool> matchedColors = colorMatchOrderMatch(userColorsVector, colorsToGuess);
        for (int i = 0; i < 4; i++) {
            string color;
            if (matchedColors[i]) {
                color = userColorsVector[i];
                colorsCountMap[color]--; // Уменьшаем оставшееся кол-во определенного цвета в словаре
                color[0] = (char)(std::toupper((unsigned char)(color[0]))); // Меняем первую букву на заглавную для красоты
            } else color = "Не угадан";
            cout << "Цвет " << i + 1 << ": " << color << endl;
        }

        // Проверка на победу
        if (std::equal(colorsToGuess.begin(), colorsToGuess.end(), userColorsVector.begin())) {
            cout << PlayerName << ", вы победили, набрав " << pointsAndAttempts << " баллов!" << endl;
            allPoints += pointsAndAttempts;
            appendToFile("\nПользователь ввёл правильную последовательность, получив " +
            to_string(pointsAndAttempts) + " баллов\n\n");
            return;
        }

        // Вывод цветов с присутствием в списке
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
    cout << "Вы не смогли угадать цвета за 10 попыток." << endl;
    cout << "Список цветов: " << correctColorsString << "." << endl;
    appendToFile("\n" + PlayerName + " не смог отгадать комбинацию цветов.\n\n");
}


// Возвращает массив всех цветов из colors.txt
vector<string> getAllColors(const string &path) {
    vector<string> colors;
    string temp_string;
    std::ifstream file(path);
    while (getline(file, temp_string)) {
        if (!temp_string.empty()) { // Проверяем, что строка не пустая, прежде чем добавить её в вектор
            colors.push_back(temp_string);
        }
    }
    file.close();
    return colors;
}

// Вывод всех возможных цветов в консоль
void PrintAllColors(const vector<string> &allColors) {
    cout << "Возможные доступные цвета: ";
    for (size_t i = 0; i < allColors.size(); ++i) {
        cout << allColors[i];
        if (i != allColors.size() - 1) cout << ", ";
        else cout << '.';
        if ((i + 1) % 9 == 0 && i != 0) cout << endl;
    }
    cout << endl;
}

// Перемешивание вектора двумя способами - с сохранением уникальных и без сохранения
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

// Конвертация вектора в строку через запятую
string vectorToString(const vector <string> &vector) {
    if (vector.empty()) return ""; // Возвращаем пустую строку для пустого вектора
    string result = vector[0]; // Инициализируем результат первым элементом
    for (size_t i = 1; i < vector.size(); ++i) result += ", " + vector[i];
    return result;
}

// Конвертация строки в вектор, разделитель - запятая.
vector<string> StringToVector(const string &colors_string) {
    vector<string> colors_vector;
    std::istringstream iss(colors_string);
    string color;
    while (getline(iss >> std::ws, color, ',')) { // Разделяем строку на слова по запятой и добавляем их в вектор
        // Удаление начальных и конечных пробелов у каждого слова
        color.erase(std::remove_if(color.begin(), color.end(), ::isspace), color.end());
        colors_vector.push_back(color);
    }
    return colors_vector;
}

// Возвращение вектора, содержащего true (совпадение цвета) или false (несовпадение цвета) по индексам цветов пользователя
vector<bool> colorMatchOrderMatch(const vector<string> &userColors, const vector<string> &pcColors) {
    vector<bool> sortedColors;
    for (size_t i = 0; i < userColors.size(); ++i) sortedColors.push_back(userColors[i] == pcColors[i]);
    return sortedColors;
}

// Возвращение вектора, содержащего true (наличие цвета в списке правильных) или false (отсутствие цвета в списке правильных)
vector<bool> colorMatchNoOrder(const vector<string> &userColors, const vector<string> &pcColors) {
    vector<bool> sortedColors;
    for (const string &userColor: userColors) {
        auto iterator = std::find(pcColors.begin(), pcColors.end(), userColor);
        sortedColors.push_back(iterator != pcColors.end());
    }
    return sortedColors;
}

// Дописывание в файл строк (используется для записи истории игры)
void appendToFile(const std::string &line) {
    std::ofstream file(pathToHistory, std::ios::app);
    if (file.is_open()) {
        file << line << endl;
    }
}

// Безопасное чтение bool переменных пользователя
bool getUserInput(bool &getInput, const std::string &message) {
    while (true) {
        cout << message;
        cin >> getInput;
        if (std::cin.fail() || (getInput != 0 && getInput != 1)) {
            std::cout << "Ошибка ввода. Пожалуйста, введите '1' или '0'." << endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else break;
    }
    return getInput;
}

// Проверка на существование файла
bool fileExistance(const string &filename) {
    bool exist = std::filesystem::exists(filename);
    return exist;
}

// Создание файла colors.txt и запись стандартных цветов при отсутствии
void configureColorsFile(const string &filename) {
    if (fileExistance(filename)) {
        cout << "Файл с цветами успешно найден." << endl;
    } else {
        cout << "Файл с историей игр не был найден. Создание нового файла и запись стандартных цветов." << endl;
        std::ofstream file;
        file.open("colors.txt");
        if (file.is_open()) {
            file << "красный\nсиний\nзелёный\nжёлтый\nбелый\nчёрный\nголубой\nрозовый\nфиолетовый\nсерый\nкоричневый\nоранжевый";
        }
        file.close();
    }
}

// Создание файла game_history.txt при его отсутствие
void configureHistoryFile(const string &filename) {
    if (fileExistance(filename)) {
        cout << "Файл с историей игр успешно найден." << endl << endl;
    } else {
        cout << "Файл с историей игр не был найден. Создание нового файла." << endl << endl;
        std::ofstream file;
        // Открываем файл для записи и обрезаем его до нулевой длины
        file.open(filename, std::ios::out | std::ios::trunc);
        file.close();
    }
}