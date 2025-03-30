#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <type_traits>

using namespace std;

// Класс обработки ввода
class InputControl {
public:
    // Удаление пробелов вначале и в конце (trim)
    static void trim(string& str);

    // Проверка на пустой ввод для отмены операции
    static bool is_input_empty(const string& input);

    // Проверка конвертации в целое число
    static bool is_int(const string& input, int min, int max);

    // Проверка конвертации в вещественное число
    static bool is_float(const string& input, float min, float max);

    // Вспомогательная функция для ввода целых чисел
    static bool enter_number(int& varLink, const string& label = "", int min = 0, int max = 2147483647);

    // Вспомогательная функция для ввода вещественных чисел
    static bool enter_float(float& varLink, const string& label = "", float min = -2147483647, float max = 2147483647);

    // Вспомогательная функция для ввода строк
    static bool enter_string(string& varLink, const string& label = "");
};
    
// Удаление пробелов вначале и в конце (trim)
void InputControl::trim(string& str) {
    // Удаляем пробелы с начала
    while (!str.empty() && str.front() == ' ')
        str.erase(str.begin());

    // Удаляем пробелы с конца
    while (!str.empty() && str.back() == ' ')
        str.pop_back();
}

// Проверка на пустой ввод для отмены операции
bool InputControl::is_input_empty(const string& input) {
    if (input.empty()) {
        cout << endl << "[Отмена операции]" << endl;
        return true;
    }
    return false;
}

// Проверка конвертации в целое число
bool InputControl::is_int(const string& input, int min, int max) {
    try {
        int number = stoi(input);
        if (number < min) { cout << endl << "Вы ввели \"" << number << "\" — значение должно быть не меньше \"" << min << "\"" << endl << endl; return false; }
        if (number > max) { cout << endl << "Вы ввели \"" << number << "\" — значение должно быть не больше \"" << max << "\"" << endl << endl; return false; }
    }
    catch (const invalid_argument&) {
        cerr << endl << "Введённое значение \"" << input << "\" не является числом!" << endl << endl;
        return false;
    }
    catch (const out_of_range&) {
        cerr << endl << "Число \"" << input << "\" выходит за допустимый диапазон!" << endl << endl;
        return false;
    }
    return true;
}

// Проверка конвертации в вещественное число
bool InputControl::is_float(const string& input, float min, float max) {
    try {
        float number = stof(input);
        if (number < min) { cout << endl << "Вы ввели \"" << number << "\" — значение должно быть не меньше \"" << min << "\"" << endl << endl; return false; }
        if (number > max) { cout << endl << "Вы ввели \"" << number << "\" — значение должно быть не больше \"" << max << "\"" << endl << endl; return false; }
    }
    catch (const invalid_argument&) {
        cerr << endl << "Введённое вами значение \"" << input << "\" не является числом!" << endl << endl;
        return false;
    }
    catch (const out_of_range&) {
        cerr << endl << "Число \"" << input << "\" выходит за допустимый диапазон!" << endl << endl;
        return false;
    }
    return true;
}

// Вспомогательная функция для ввода целых чисел
bool InputControl::enter_number(int& varLink, const string& label, int min, int max) {
    string raw_input;

    do {
        cout << label;
        getline(cin, raw_input);
        trim(raw_input);
        if (is_input_empty(raw_input)) return true;  // отмена операции
    } while (!is_int(raw_input, min, max));

    varLink = stoi(raw_input);
    return false;  // продолжение операции
}

// Вспомогательная функция для ввода вещественных чисел
bool InputControl::enter_float(float& varLink, const string& label, float min, float max) {
    string raw_input;

    do {
        cout << label;
        getline(cin, raw_input);
        trim(raw_input);
        if (is_input_empty(raw_input)) return true;  // отмена операции
    } while (!is_float(raw_input, min, max));

    varLink = stof(raw_input);
    return false;  // продолжение операции
}

// Вспомогательная функция для ввода строк
bool InputControl::enter_string(string& varLink, const string& label) {
    string raw_input;

    cout << label;
    getline(cin, raw_input);
    trim(raw_input);
    if (is_input_empty(raw_input))
        return true;   // отмена операции
    else {
        varLink = raw_input;
        return false;  // продолжение операции
    }
}

// Класс панкта меню
class MenuItem {
private:
    int number;
    string label;
    function<void()> action;

public:
    MenuItem() = delete;
    MenuItem(int number, string label, function<void()> action);
    MenuItem(const MenuItem& menu_item) = default;
    ~MenuItem() = default;

    // Выполнение функции пункта
    inline void act() const;

    // Вывод пункта меню
    inline void show() const;

    // Геттер номера пункта
    inline int get_number() const;

    // Ввод и присваивание значения переменной
    static function<void()> input_var_value(float& var_link, const string& label);

    // Найти дробную часть суммы x, y, z
    static function<void()> calculate_sum_of_xyz(float& x, float& y, float& z);

    // Округлить сумму x, y, z до ближайшего целого
    static function<void()> round_sum_of_xyz(float& x, float& y, float& z);

    // Вывод меню
    template <typename Container>
    static void show_menu(const Container& menu,
        enable_if_t< is_same_v<typename Container::value_type, MenuItem>, int>* = 0);

};

MenuItem::MenuItem(int number, string label, function<void()> action) : number(number), label(label), action(action) {}

// Выполнение функции пункта
inline void MenuItem::act() const {
    action();
}

// Вывод пункта меню
inline void MenuItem::show() const {
    cout << number << ". " << label << endl;
}

// Геттер номера пункта
inline int MenuItem::get_number() const {
    return number;
}

// Ввод и присваивание значения переменной
function<void()> MenuItem::input_var_value(float& var_link, const string& label) {
    return [=, &var_link]() {
        InputControl::enter_float(var_link, label);
    };
}

// Найти дробную часть суммы x, y, z
function<void()> MenuItem::calculate_sum_of_xyz(float& x, float& y, float& z) {
    return [&]() {
        cout << x + y + z;
    };
}

// Округлить сумму x, y, z до ближайшего целого
function<void()> MenuItem::round_sum_of_xyz(float& x, float& y, float& z) {
    return [&]() {
        float sum = x + y + z;
        cout << static_cast<int>(sum + 0.5f * ((sum > 0) - (sum < 0)));
    };
}

// Вывод меню
template <typename Container>
void MenuItem::show_menu(const Container& menu,
    enable_if_t< is_same_v<typename Container::value_type, MenuItem>, int>*) {
    for (const auto& item : menu)
        item.show();
    cout << "0. Выход" << endl << endl << endl;
}

int main() {
    float x, y, z;
    vector<MenuItem> menu {
        { 1, "Ввести вещественное число x;", MenuItem::input_var_value(x, "Введите вещественное число x: ") },
        { 2, "Ввести вещественное число y;", MenuItem::input_var_value(y, "Введите вещественное число y: ") },
        { 3, "Ввести вещественное число z;", MenuItem::input_var_value(z, "Введите вещественное число z: ") },
        { 4, "Найти дробную часть суммы этих 3-х чисел;", MenuItem::calculate_sum_of_xyz(x, y, z) },
        { 5, "Округлить до ближайшего целого сумму этих 3-х чисел;", MenuItem::round_sum_of_xyz(x, y, z) },
    };

    int number;

    do {
        MenuItem::show_menu(menu);
        if (InputControl::enter_number(number, "Пункт меню: ", 0, static_cast<int>(menu.size()))) { cout << endl << endl; continue; }
        cout << endl << endl;
        try {
            auto it = find_if(menu.begin(), menu.end(), [number](MenuItem m) { return m.get_number() == number; });
            if (it != menu.end())
                (*it).act();
        }
        catch (const exception& e) {
            cout << "Ошибка: " << e.what();
        }
        cout << endl << endl << endl;
    } while (number);

    cout << "© 2025 Fedor Kondakov" << endl;

    return 0;
}