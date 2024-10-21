#include <iostream> // Подключение библиотеки для ввода-вывода.
#include <vector>   // Подключение библиотеки для работы с динамическими массивами (векторами).
#include <limits>   // Подключение библиотеки для работы с ограничениями типа данных.
#include <locale>   // Подключение библиотеки для локализации.
#include <string>   // Подключение библиотеки для работы со строками.

using namespace std; // Использование стандартного пространства имен для упрощения записи.

enum class CallType { // Определение перечисления для типов звонков.
    Regular,       // Обычный тариф.
    Discounted     // Льготный тариф.
};

// Функция для преобразования строки в тип звонка.
CallType stringToCallType(const string& str) {
    if (str == "Regular") return CallType::Regular; // Возврат типа звонка, если строка "Regular".
    return CallType::Discounted; // Возврат льготного типа звонка по умолчанию.
}

// Структура для хранения информации о тарифе.
struct Tariff {
    string cityName; // Название города.
    double price;    // Цена за минуту разговора.

    // Конструктор для инициализации тарифов.
    Tariff(const string& name, double p) : cityName(name), price(p) {}
};

// Структура для хранения информации о звонке.
struct Call {
    string clientName; // Имя клиента.
    string cityName;   // Название города, куда был совершен звонок.
    double duration;   // Продолжительность звонка.
    double price;      // Общая стоимость звонка.

    // Конструктор для инициализации звонков.
    Call(const string& client, const string& city, double dur, double p)
        : clientName(client), cityName(city), duration(dur), price(p) {}

    // Деструктор для вывода информации при уничтожении объекта звонка.
    ~Call() {
        cout << "Деструктор для звонка: " << clientName << " -> " << cityName << endl;
    }
};

// Класс для представления АТС (автоматической телефонной станции).
class ATC {
private:
    vector<Tariff> tariffs; // Вектор тарифов.
    vector<Call> calls;     // Вектор звонков.
    double totalRevenue;    // Общая выручка.

    // Конструктор по умолчанию (приватный, чтобы предотвратить создание экземпляров).
    ATC() : totalRevenue(0) {}

    // Запрет копирования экземпляров.
    ATC(const ATC&) = delete;
    // Запрет присваивания экземпляров.
    ATC& operator=(const ATC&) = delete;

public:
    // Метод для получения единственного экземпляра класса (паттерн Singleton).
    static ATC& getInstance() {
        static ATC instance; // Создание единственного экземпляра.
        return instance; // Возврат экземпляра.
    }

    // Деструктор для вывода информации при уничтожении объекта АТС.
    ~ATC() {
        cout << "Деструктор для ATC\n";
    }

    // Метод для получения списка тарифов.
    const vector<Tariff>& getTariffs() const {
        return tariffs; // Возврат ссылки на вектор тарифов.
    }

    // Метод для добавления нового тарифа.
    void addTariff(const string& cityName, double price) {
        tariffs.emplace_back(cityName, price); // Добавление нового тарифа в вектор.
        cout << "Тариф добавлен успешно: " << cityName << " по цене " << price << " за минуту\n";
    }

    // Метод для вывода списка тарифов.
    int printTariffs() const {
        cout << "Список тарифов:\n";
        if (tariffs.empty()) { // Проверка на пустоту списка тарифов.
            cout << "Список тарифов пуст.\n";
            return 0; // Возврат 0, если тарифов нет.
        }
        else {
            // Вывод информации о каждом тарифе.
            for (size_t i = 0; i < tariffs.size(); ++i) {
                cout << i + 1 << ". " << tariffs[i].cityName << " - " << tariffs[i].price << " за минуту\n";
            }
        }
    }

    // Метод для получения цены тарифа по индексу.
    double getFarePrice(int index) const {
        if (index >= 0 && index < tariffs.size()) { // Проверка корректности индекса.
            return tariffs[index].price; // Возврат цены тарифа.
        }
        return 0.0; // Возврат 0, если индекс некорректный.
    }

    // Метод для регистрации звонка.
    void registerCall(const string& clientName, const string& cityName, double duration, double pricePerMinute) {
        double totalCost = duration * pricePerMinute; // Расчет общей стоимости звонка.
        totalRevenue += totalCost; // Обновление общей выручки.
        calls.emplace_back(clientName, cityName, duration, totalCost); // Добавление звонка в вектор.
        cout << "Звонок зарегистрирован: " << clientName << " -> " << cityName << ", стоимость: " << totalCost << endl;
    }

    // Метод для получения общей выручки.
    double getTotalRevenue() const {
        return totalRevenue; // Возврат общей выручки.
    }

    // Метод для получения стоимости звонков клиента.
    double getClientTotalCallsCost(const string& clientName) const {
        double clientTotal = 0; // Переменная для хранения общей стоимости звонков клиента.
        // Итерация по всем звонкам для подсчета стоимости.
        for (const auto& call : calls) {
            if (call.clientName == clientName) { // Проверка имени клиента.
                clientTotal += call.price; // Суммирование стоимости звонков.
            }
        }
        return clientTotal; // Возврат общей стоимости звонков клиента.
    }
};

// Функция для очистки консоли (поддержка Windows и Unix).
static void clearConsole() {
#ifdef _WIN32
    system("cls"); // Очистка консоли для Windows.
#else
    system("clear"); // Очистка консоли для других систем.
#endif
}

// Функция для главного меню.
static void menu() {
    ATC& atc = ATC::getInstance(); // Получение экземпляра АТС.
    bool OnDisplay = true; // Переменная для управления отображением меню.

    // Цикл отображения меню.
    while (OnDisplay) {
        clearConsole(); // Очистка консоли.

        // Вывод заголовка меню.
        cout << "===== Система управления ATC =====\n";
        cout << "1. Добавить новый тариф\n";
        cout << "2. Просмотреть все тарифы\n";
        cout << "3. Зарегистрировать звонок\n";
        cout << "4. Просмотреть общую выручку за все звонки\n";
        cout << "5. Рассчитать стоимость всех звонков клиента\n";
        cout << "0. Выход\n";
        cout << "=============================================\n";

        int choice; // Переменная для выбора опции.
        cout << "Выберите опцию: ";
        cin >> choice; // Чтение выбора пользователя.
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшихся символов в буфере ввода.

        // Обработка выбора пользователя.
        switch (choice) {
        case 1: { // Добавить новый тариф.
            string cityName; // Переменная для хранения названия города.
            cout << "Введите название города: ";
            getline(cin, cityName); // Чтение названия города.
            double price = 0; // Переменная для хранения цены.
            cout << "Введите цену за минуту разговора: ";
            cin >> price; // Чтение цены за минуту.
            if (price < 0) { // Проверка на отрицательное значение цены.
                cout << "Цена за минуту не может быть отрицательной\n"; // Сообщение об ошибке.
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшихся символов.
                break; // Прерывание выполнения текущего случая.
            }
            atc.addTariff(cityName, price); // Добавление нового тарифа.
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшихся символов.
            break; // Прерывание выполнения текущего случая.
        }
        case 2: // Просмотреть все тарифы.
            atc.printTariffs(); // Вызов функции для вывода тарифов.
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшихся символов.
            break; // Прерывание выполнения текущего случая.
        case 3: { // Зарегистрировать звонок.
            string clientName; // Переменная для хранения имени клиента.
            if (atc.printTariffs() == 0) { // Проверка наличия тарифов.
                cout << "Сначала введите хотя бы 1 тариф.\n"; // Сообщение об ошибке.
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшихся символов.
                break; // Прерывание выполнения текущего случая.
            }
            cout << "Введите имя клиента: ";
            getline(cin, clientName); // Чтение имени клиента.

            atc.printTariffs(); // Вывод списка тарифов.
            int tariffIndex; // Переменная для хранения индекса выбранного тарифа.
            cout << "Выберите тариф (введите номер): ";
            cin >> tariffIndex; // Чтение номера тарифа.
            --tariffIndex; // Корректировка индекса для работы с массивом.

            double duration; // Переменная для хранения продолжительности звонка.
            cout << "Введите продолжительность звонка (в минутах): ";
            cin >> duration; // Чтение продолжительности звонка.

            double pricePerMinute = atc.getFarePrice(tariffIndex); // Получение цены за минуту по выбранному тарифу.
            // Регистрация звонка с указанием клиента, города, продолжительности и цены.
            atc.registerCall(clientName, atc.getTariffs()[tariffIndex].cityName, duration, pricePerMinute);
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшихся символов.
            break; // Прерывание выполнения текущего случая.
        }
        case 4: // Просмотреть общую выручку за все звонки.
            cout << "Общая выручка за все звонки: " << atc.getTotalRevenue() << endl; // Вывод общей выручки.
            break; // Прерывание выполнения текущего случая.
        case 5: { // Рассчитать стоимость всех звонков клиента.
            string clientName; // Переменная для хранения имени клиента.
            cout << "Введите имя клиента: ";
            getline(cin, clientName); // Чтение имени клиента.
            double totalCost = atc.getClientTotalCallsCost(clientName); // Получение общей стоимости звонков клиента.
            cout << "Общая стоимость звонков клиента " << clientName << ": " << totalCost << endl; // Вывод общей стоимости.
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшихся символов.
            break; // Прерывание выполнения текущего случая.
        }
        case 0: // Выход из программы.
            OnDisplay = false; // Изменение состояния для выхода из цикла.
            break; // Прерывание выполнения текущего случая.
        default: // Обработка некорректного выбора.
            cout << "Неверный выбор! Пожалуйста, попробуйте снова.\n"; // Сообщение об ошибке.
        }

        cout << "\nНажмите Enter, чтобы продолжить..."; // Сообщение для продолжения.
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшихся символов.
    }
}

int main() {
    setlocale(LC_ALL, ""); // Установка локализации.
    menu(); // Вызов функции меню.
    return 0; // Возврат 0 при успешном завершении программы.
}
