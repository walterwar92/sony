#include <iostream>
#include <vector>
#include <limits>
#include <locale>
#include <string>

using namespace std;

enum class CallType {
    Regular,
    Discounted
};

CallType stringToCallType(const string& str) {
    if (str == "Regular") return CallType::Regular;
    return CallType::Discounted;
}

struct Tariff {
    string cityName;
    double price;

    Tariff(const string& name, double p) : cityName(name), price(p) {}
};

struct Call {
    string clientName;
    string cityName;
    double duration;
    double price;
    Call(const string& client, const string& city, double dur, double p)
        : clientName(client), cityName(city), duration(dur), price(p) {}

    ~Call() {
        cout << "Деструктор для звонка: " << clientName << " -> " << cityName << endl;
    }
};

class ATC {
private:
    vector<Tariff> tariffs;
    vector<Call> calls;
    double totalRevenue;
    ATC() : totalRevenue(0) {}

    ATC(const ATC&) = delete;
    ATC& operator=(const ATC&) = delete;

public:
    static ATC& getInstance() {
        static ATC instance;
        return instance;
    }

    ~ATC() {
        cout << "Деструктор для ATC\n";
    }

    const vector<Tariff>& getTariffs() const {
        return tariffs;
    }

    void addTariff(const string& cityName, double price) {
        tariffs.emplace_back(cityName, price);
        cout << "Тариф добавлен успешно: " << cityName << " по цене " << price << " за минуту\n";
    }

    int printTariffs() const {
        cout << "Список тарифов:\n";
        if (tariffs.empty()) {
            cout << "Список тарифов пуст.\n";
            return 0;
        }
        else {
            for (size_t i = 0; i < tariffs.size(); ++i) {
                cout << i + 1 << ". " << tariffs[i].cityName << " - " << tariffs[i].price << " за минуту\n";
            }
        }
    }

    double getFarePrice(int index) const {
        if (index >= 0 && index < tariffs.size()) {
            return tariffs[index].price;
        }
        return 0.0;
    }


    void registerCall(const string& clientName, const string& cityName, double duration, double pricePerMinute) {
        double totalCost = duration * pricePerMinute;
        totalRevenue += totalCost;
        calls.emplace_back(clientName, cityName, duration, totalCost);
        cout << "Звонок зарегистрирован: " << clientName << " -> " << cityName << ", стоимость: " << totalCost << endl;
    }


    double getTotalRevenue() const {
        return totalRevenue;
    }

    double getClientTotalCallsCost(const string& clientName) const {
        double clientTotal = 0;
        for (const auto& call : calls) {
            if (call.clientName == clientName) {
                clientTotal += call.price;
            }
        }
        return clientTotal;
    }
};


static void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Главное меню
static void menu() {
    ATC& atc = ATC::getInstance();
    bool OnDisplay = true;

    while (OnDisplay) {
        clearConsole();

        cout << "===== Система управления ATC =====\n";
        cout << "1. Добавить новый тариф\n";
        cout << "2. Просмотреть все тарифы\n";
        cout << "3. Зарегистрировать звонок\n";
        cout << "4. Просмотреть общую выручку за все звонки\n";
        cout << "5. Рассчитать стоимость всех звонков клиента\n";
        cout << "0. Выход\n";
        cout << "=============================================\n";

        int choice;
        cout << "Выберите опцию: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            string cityName;
            cout << "Введите название города: ";
            getline(cin, cityName);
            double price = 0;
            cout << "Введите цену за минуту разговора: ";
            cin >> price;
            if (price < 0) {
                cout << "Цена за минуту не может быть отрицательной\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            atc.addTariff(cityName, price);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        case 2:
            atc.printTariffs();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        case 3: {
            string clientName;
            if (atc.printTariffs() == 0) {
                cout << "Сначало введите хотя бы 1 тариф.\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
            }
            cout << "Введите имя клиента: ";
            getline(cin, clientName);

            atc.printTariffs();
            int tariffIndex;
            cout << "Выберите тариф (введите номер): ";
            cin >> tariffIndex;
            --tariffIndex;

            double duration;
            cout << "Введите продолжительность звонка (в минутах): ";
            cin >> duration;

            double pricePerMinute = atc.getFarePrice(tariffIndex);
            atc.registerCall(clientName, atc.getTariffs()[tariffIndex].cityName, duration, pricePerMinute);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        case 4:
            cout << "Общая выручка за все звонки: " << atc.getTotalRevenue() << endl;
            break;
        case 5: {
            string clientName;
            cout << "Введите имя клиента: ";
            getline(cin, clientName);
            double totalCost = atc.getClientTotalCallsCost(clientName);
            cout << "Общая стоимость звонков клиента " << clientName << ": " << totalCost << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        case 0:
            OnDisplay = false;
            break;
        default:
            cout << "Неверный выбор! Пожалуйста, попробуйте снова.\n";
        }

        cout << "\nНажмите Enter, чтобы продолжить...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    setlocale(LC_ALL, "");
    menu();
    return 0;
}
