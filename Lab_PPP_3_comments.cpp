#include <iostream>        // Подключаем библиотеку, которая позволяет вводить и выводить данные в консоль
#include <vector>          // Подключаем библиотеку, которая предоставляет возможность использовать динамические массивы (векторы)
#include <limits>          // Подключаем библиотеку, которая содержит информацию о пределах чисел (для проверки ввода)
#include <string>          // Подключаем библиотеку, которая позволяет работать со строками (например, текстом)
#include <memory>          // Подключаем библиотеку для работы с умными указателями, которые управляют памятью
#include <iomanip>         // Для std::setprecision   

using namespace std;       // Используем пространство имен std, чтобы не писать "std::" перед стандартными объектами, такими как cout

// Интерфейс для стратегии тарифа
// Это абстрактный класс, который задает общий набор методов для всех типов тарифов
class TariffStrategy {
public:
    virtual ~TariffStrategy() = default; // Виртуальный деструктор. Он нужен для корректного удаления объектов потомков
    virtual double getCost() const = 0;   // Чисто виртуальный метод для получения стоимости тарифа. Он должен быть реализован в дочерних классах
    virtual string getDestination() const = 0; // Чисто виртуальный метод для получения направления
    virtual double getOriginalCost() const = 0; // Чисто виртуальный метод для получения исходной стоимости (до применения скидок)
};

// Класс для тарифа без скидки
// Этот класс наследует интерфейс TariffStrategy и реализует его методы для тарифа без скидки
class NoDiscountTariff : public TariffStrategy {
private:
    string destination;  // Переменная для хранения направления звонка
    double cost;         // Переменная для хранения стоимости тарифа без скидки
public:
    // Конструктор, который инициализирует направление и стоимость
    NoDiscountTariff(const string& dest, double c) : destination(dest), cost(c) {}

    // Возвращает стоимость тарифа (в данном случае без скидки)
    double getCost() const override {
        return cost;
    }

    // Возвращает направление звонка
    string getDestination() const override {
        return destination;
    }

    // Возвращает исходную стоимость (для тарифа без скидки это фактическая стоимость)
    double getOriginalCost() const override {
        return cost;
    }
};

// Класс для тарифа с фиксированной скидкой
// Этот класс также наследует интерфейс TariffStrategy и добавляет логику для фиксированной скидки
class FixedDiscountTariff : public TariffStrategy {
private:
    string destination;  // Переменная для хранения направления звонка
    double cost;         // Переменная для хранения стоимости до скидки
    double discount;     // Переменная для хранения фиксированной суммы скидки
public:
    // Конструктор, принимающий направление, стоимость и размер скидки
    FixedDiscountTariff(const string& dest, double c, double d)
        : destination(dest), cost(c), discount(d) {}

    // Возвращает стоимость тарифа с учётом фиксированной скидки
    double getCost() const override {
        return cost - discount;  // Возвращаем стоимость после вычета скидки
    }

    // Возвращает направление звонка
    string getDestination() const override {
        return destination;
    }

    // Возвращает исходную стоимость (до скидки)
    double getOriginalCost() const override {
        return cost;
    }
};

// Класс для тарифа с процентной скидкой
// Этот класс также наследует интерфейс TariffStrategy и добавляет логику для процентной скидки
class PercentageDiscountTariff : public TariffStrategy {
private:
    string destination;  // Переменная для хранения направления звонка
    double cost;         // Переменная для хранения стоимости до скидки
    double percentage;   // Переменная для хранения процента скидки
public:
    // Конструктор, принимающий направление, стоимость и процент скидки
    PercentageDiscountTariff(const string& dest, double c, double p)
        : destination(dest), cost(c), percentage(p) {}

    // Возвращает стоимость тарифа с учётом процентной скидки
    double getCost() const override {
        return cost * (1 - percentage / 100);  // Скидка рассчитывается как процент от стоимости
    }

    // Возвращает направление звонка
    string getDestination() const override {
        return destination;
    }

    // Возвращает исходную стоимость (до скидки)
    double getOriginalCost() const override {
        return cost;
    }
};

// Класс ATC (Автоматическая Телефонная Станция), который управляет тарифами
class ATC {
private:
    vector<shared_ptr<TariffStrategy>> tariffs;  // Вектор (список) тарифов, где каждый тариф представлен умным указателем
public:
    // Проверяет, существует ли тариф для данного направления
    bool doesTariffExist(const string& destination) const {
        // Проходим по всем тарифам и проверяем совпадение по направлению
        for (const auto& tariff : tariffs) {
            if (tariff->getDestination() == destination) { // Если направление совпадает
                return true;  // Тариф с таким направлением найден
            }
        }
        return false;  // Тарифа с таким направлением нет
    }

    // Добавляет тариф в список
    void addTariff(shared_ptr<TariffStrategy> tariff) {
        tariffs.push_back(tariff);  // Добавляем новый тариф в список
    }

    // Вычисляет среднюю стоимость всех тарифов (с учетом скидок)
    double calculateAverageCost() const {
        if (tariffs.empty()) {  // Если список тарифов пуст
            return 0;  // Возвращаем 0, если тарифов нет
        }
        double totalCost = 0;  // Переменная для суммирования стоимости всех тарифов
        // Проходим по всем тарифам и суммируем их стоимость с учетом скидок
        for (const auto& tariff : tariffs) {
            totalCost += tariff->getCost();  // Учитываем стоимость с учетом скидок
        }
        return totalCost / tariffs.size();  // Возвращаем среднюю стоимость
    }

    // Вывод всех тарифов
    void printAllTariffs() const {
        if (tariffs.empty()) {  // Если список тарифов пуст
            cout << "Список тарифов пуст.\n";  // Выводим сообщение об отсутствии тарифов
            return;  // Завершаем выполнение функции
        }

        // Выводим заголовок списка
        cout << "=== Список всех тарифов ===\n";
        // Проходим по всем тарифам и выводим их данные
        for (const auto& tariff : tariffs) {
            cout << "Направление: " << tariff->getDestination()
                << " | Стоимость: " << tariff->getCost()  // Стоимость с учетом скидки
                << " | Исходная стоимость: " << tariff->getOriginalCost() << "\n";  // Исходная стоимость без скидок
        }
    }
};

// Функция для очистки консоли
// Эта функция зависит от операционной системы: для Windows и для других систем
static void clearConsole() {
#ifdef _WIN32
    system("cls");  // Очистка консоли для Windows
#else
    system("clear");  // Очистка консоли для других ОС (Linux, Mac)
#endif
}

// Функция для корректного ввода положительного числа
// Возвращает введенное число только если оно положительное
static double inputNumber(const string& prompt) {
    double value;  // Переменная для хранения введенного значения
    while (true) {  // Бесконечный цикл для повторного запроса ввода
        cout << prompt;  // Выводим приглашение для ввода
        cin >> value;    // Считываем значение
        // Если произошла ошибка ввода или введено отрицательное число
        if (cin.fail() || value <= 0) {
            cin.clear();  // Очищаем флаг ошибки ввода
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер ввода
            cout << "Ошибка: введите положительное число.\n";  // Выводим сообщение об ошибке
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер после успешного ввода
            return value;  // Возвращаем корректное значение
        }
    }
}

// Основная функция программы
int main() {
    setlocale(LC_ALL, "Russian");  // Устанавливаем локаль для корректного отображения текста на русском
    // Устанавливаем фиксированный формат вывода
    cout << fixed; // Включаем фиксированный формат

    // Устанавливаем необходимую точность
    cout << setprecision(0); // Убираем десятичные знаки
    ATC atc;  // Создаем объект класса ATC, который будет управлять тарифами
    int choice;  // Переменная для хранения выбора пользователя

    // Основной цикл программы, который будет выполняться, пока пользователь не выберет "Выход"
    while (true) {
        clearConsole();  // Очищаем консоль перед каждым выводом меню
        cout << "=== Меню АТС ===\n";  // Выводим заголовок меню
        cout << "1. Добавить новый тариф без скидки\n";  // Выводим опцию для добавления тарифа без скидки
        cout << "2. Добавить новый тариф с фиксированной скидкой\n";  // Опция для добавления тарифа с фиксированной скидкой
        cout << "3. Добавить новый тариф с процентной скидкой\n";  // Опция для добавления тарифа с процентной скидкой
        cout << "4. Показать все тарифы\n";  // Опция для вывода всех тарифов
        cout << "5. Показать среднюю стоимость тарифов\n";  // Опция для расчета средней стоимости тарифов
        cout << "0. Выход\n";  // Опция для выхода из программы
        cout << "Выберите действие: ";  // Просим пользователя выбрать действие
        cin >> choice;  // Считываем выбор пользователя

        // Проверяем корректность ввода
        if (cin.fail()) {
            cin.clear();  // Очищаем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер
            cout << "Ошибка: введите корректный номер действия.\n";  // Сообщаем об ошибке
            continue;  // Переходим к следующей итерации цикла
        }

        // Обрабатываем выбор пользователя
        switch (choice) {
        case 1: {  // Добавление нового тарифа без скидки
            clearConsole();  // Очищаем консоль перед вводом
            string destination;  // Переменная для хранения введенного направления
            cout << "Введите название направления: ";  // Запрашиваем название направления
            cin.ignore();  // Очищаем буфер, чтобы избежать проблем с getline
            getline(cin, destination);  // Считываем направление

            // Проверяем, существует ли уже тариф на это направление
            if (atc.doesTariffExist(destination)) {
                cout << "Ошибка: Тариф на данное направление уже существует. Введите другое название.\n";  // Сообщаем об ошибке
                break;  // Выходим из текущего случая
            }

            double cost = inputNumber("Введите стоимость: ");  // Запрашиваем стоимость тарифа
            atc.addTariff(make_shared<NoDiscountTariff>(destination, cost));  // Добавляем тариф без скидки
            cout << "Тариф добавлен успешно.\n";  // Сообщаем о успешном добавлении
            break;  // Выходим из текущего случая
        }
        case 2: {  // Добавление нового тарифа с фиксированной скидкой
            clearConsole();  // Очищаем консоль перед вводом
            string destination;  // Переменная для хранения направления
            cout << "Введите название направления: ";  // Запрашиваем название направления
            cin.ignore();  // Очищаем буфер
            getline(cin, destination);  // Считываем направление

            // Проверяем, существует ли уже тариф на это направление
            if (atc.doesTariffExist(destination)) {
                cout << "Ошибка: Тариф на данное направление уже существует. Введите другое название.\n";  // Сообщаем об ошибке
                break;  // Выходим из текущего случая
            }

            double cost = inputNumber("Введите стоимость: ");  // Запрашиваем стоимость тарифа
            double discount = inputNumber("Введите размер скидки: ");  // Запрашиваем размер скидки
            if (cost < discount) {  // Проверяем, что скидка не превышает стоимость
                cout << "Ошибка: стоимость не может быть ниже скидки.\n";  // Сообщаем об ошибке
                break;  // Выходим из текущего случая
            }
            atc.addTariff(make_shared<FixedDiscountTariff>(destination, cost, discount));  // Добавляем тариф с фиксированной скидкой
            cout << "Тариф с фиксированной скидкой добавлен успешно.\n";  // Сообщаем о успешном добавлении
            break;  // Выходим из текущего случая
        }
        case 3: {  // Добавление нового тарифа с процентной скидкой
            clearConsole();  // Очищаем консоль перед вводом
            string destination;  // Переменная для хранения направления
            cout << "Введите название направления: ";  // Запрашиваем название направления
            cin.ignore();  // Очищаем буфер
            getline(cin, destination);  // Считываем направление

            // Проверяем, существует ли уже тариф на это направление
            if (atc.doesTariffExist(destination)) {
                cout << "Ошибка: Тариф на данное направление уже существует. Введите другое название.\n";  // Сообщаем об ошибке
                break;  // Выходим из текущего случая
            }

            double cost = inputNumber("Введите стоимость: ");  // Запрашиваем стоимость тарифа
            double percentage = inputNumber("Введите процент скидки: ");  // Запрашиваем процент скидки
            if (percentage < 0 || percentage > 100) {  // Проверяем корректность значения скидки
                cout << "Ошибка: процент скидки должен быть от 0 до 100.\n";  // Сообщаем об ошибке
                break;  // Выходим из текущего случая
            }
            atc.addTariff(make_shared<PercentageDiscountTariff>(destination, cost, percentage));  // Добавляем тариф с процентной скидкой
            cout << "Тариф с процентной скидкой добавлен успешно.\n";  // Сообщаем о успешном добавлении
            break;  // Выходим из текущего случая
        }
        case 4:
            atc.printAllTariffs();  // Вывод всех тарифов
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ожидание нажатия Enter для продолжения
            break;  // Выходим из текущего случая
        case 5: {  // Вывод средней стоимости тарифов
            double avgCost = atc.calculateAverageCost();  // Вычисление средней стоимости тарифов
            cout << "Средняя стоимость всех тарифов: " << avgCost << "\n";  // Вывод средней стоимости
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ожидание нажатия Enter для продолжения
            break;  // Выходим из текущего случая
        }
        case 0:  // Выход из программы
            return 0;  // Завершение программы
        default:
            cout << "Ошибка: неверный выбор.\n";  // Обработка некорректного выбора
        }
        cout << "\nНажмите Enter для продолжения...";  // Просим пользователя нажать Enter для продолжения
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ожидание нажатия Enter для продолжения
    }
}
