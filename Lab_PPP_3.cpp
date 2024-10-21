#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <memory>
#include <iomanip>

using namespace std;

class TariffStrategy {
public:
    virtual ~TariffStrategy() = default;
    virtual double getCost() const = 0;
    virtual string getDestination() const = 0;
    virtual double getOriginalCost() const = 0;
};

class NoDiscountTariff : public TariffStrategy {
private:
    string destination;
    double cost;
public:
    NoDiscountTariff(const string& dest, double c) : destination(dest), cost(c) {}

    double getCost() const override {
        return cost;
    }

    string getDestination() const override {
        return destination;
    }

    double getOriginalCost() const override {
        return cost;
    }
};

class FixedDiscountTariff : public TariffStrategy {
private:
    string destination;
    double cost;
    double discount;
public:
    FixedDiscountTariff(const string& dest, double c, double d)
        : destination(dest), cost(c), discount(d) {}

    double getCost() const override {
        return cost - discount;
    }

    string getDestination() const override {
        return destination;
    }

    double getOriginalCost() const override {
        return cost;
    }
};

class PercentageDiscountTariff : public TariffStrategy {
private:
    string destination;
    double cost;
    double percentage;
public:
    PercentageDiscountTariff(const string& dest, double c, double p)
        : destination(dest), cost(c), percentage(p) {}

    double getCost() const override {
        return cost * (1 - percentage / 100);
    }

    string getDestination() const override {
        return destination;
    }

    double getOriginalCost() const override {
        return cost;
    }
};

class ATC {
private:
    vector<shared_ptr<TariffStrategy>> tariffs;
public:
    bool doesTariffExist(const string& destination) const {
        for (const auto& tariff : tariffs) {
            if (tariff->getDestination() == destination) {
                return true;
            }
        }
        return false;
    }

    void addTariff(shared_ptr<TariffStrategy> tariff) {
        tariffs.push_back(tariff);
    }

    double calculateAverageCost() const {
        if (tariffs.empty()) {
            return 0;
        }
        double totalCost = 0;
        for (const auto& tariff : tariffs) {
            totalCost += tariff->getCost();
        }
        return totalCost / tariffs.size();
    }

    void printAllTariffs() const {
        if (tariffs.empty()) {
            cout << "Список тарифов пуст.\n";
            return;
        }

        cout << "=== Список всех тарифов ===\n";
        for (const auto& tariff : tariffs) {
            cout << "Направление: " << tariff->getDestination()
                << " | Стоимость: " << tariff->getCost()
                << " | Исходная стоимость: " << tariff->getOriginalCost() << "\n";
        }
    }
};

static void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static double inputNumber(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: введите положительное число.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    cout << fixed;
    cout << setprecision(0);
    ATC atc;
    int choice;

    while (true) {
        clearConsole();
        cout << "=== Меню АТС ===\n";
        cout << "1. Добавить новый тариф без скидки\n";
        cout << "2. Добавить новый тариф с фиксированной скидкой\n";
        cout << "3. Добавить новый тариф с процентной скидкой\n";
        cout << "4. Показать все тарифы\n";
        cout << "5. Показать среднюю стоимость тарифов\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: введите корректный номер действия.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            clearConsole();
            string destination;
            cout << "Введите название направления: ";
            cin.ignore();
            getline(cin, destination);

            if (atc.doesTariffExist(destination)) {
                cout << "Ошибка: Тариф на данное направление уже существует. Введите другое название.\n";
                break;
            }

            double cost = inputNumber("Введите стоимость: ");
            atc.addTariff(make_shared<NoDiscountTariff>(destination, cost));
            cout << "Тариф добавлен успешно.\n";
            break;
        }
        case 2: {
            clearConsole();
            string destination;
            cout << "Введите название направления: ";
            cin.ignore();
            getline(cin, destination);

            if (atc.doesTariffExist(destination)) {
                cout << "Ошибка: Тариф на данное направление уже существует. Введите другое название.\n";
                break;
            }

            double cost = inputNumber("Введите стоимость: ");
            double discount = inputNumber("Введите размер скидки: ");
            if (cost < discount) {
                cout << "Ошибка: стоимость не может быть ниже скидки.\n";
                break;
            }
            atc.addTariff(make_shared<FixedDiscountTariff>(destination, cost, discount));
            cout << "Тариф с фиксированной скидкой добавлен успешно.\n";
            break;
        }
        case 3: {
            clearConsole();
            string destination;
            cout << "Введите название направления: ";
            cin.ignore();
            getline(cin, destination);

            if (atc.doesTariffExist(destination)) {
                cout << "Ошибка: Тариф на данное направление уже существует. Введите другое название.\n";
                break;
            }

            double cost = inputNumber("Введите стоимость: ");
            double percentage = inputNumber("Введите процент скидки: ");
            if (percentage < 0 || percentage > 100) {
                cout << "Ошибка: процент скидки должен быть от 0 до 100.\n";
                break;
            }
            atc.addTariff(make_shared<PercentageDiscountTariff>(destination, cost, percentage));
            cout << "Тариф с процентной скидкой добавлен успешно.\n";
            break;
        }
        case 4:
            atc.printAllTariffs();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        case 5: {
            double avgCost = atc.calculateAverageCost();
            cout << "Средняя стоимость всех тарифов: " << avgCost << "\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        case 0:
            return 0;
        default:
            cout << "Ошибка: неверный выбор.\n";
        }
        cout << "\nНажмите Enter для продолжения...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
