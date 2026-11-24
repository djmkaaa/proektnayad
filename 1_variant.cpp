#include <iostream>
#include <array>
#include <iomanip>
#include <random>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;
constexpr int CAPACITY = 80;
struct SessionResult {
    vector<int> buyers; 
    int revenue = 0;   
};
SessionResult sellTickets(const vector<int>& budgets, int price) {
    SessionResult result;
    for (int i = 0; i < static_cast<int>(budgets.size()); ++i) {
        if (result.buyers.size() == CAPACITY) {
            break;
        }
        if (budgets[i] >= price) {
            result.buyers.push_back(i);
            result.revenue += price;
        }
    }
    return result;
}
bool check(bool condition, const char* description) {
    cout << (condition ? "[ПРОЙДЕНО] " : "[ОШИБКА] ")
         << description << '\n';
    return condition;
}
bool runTests() {
    bool allPassed = true;
    SessionResult first =
        sellTickets({199, 200, 299, 300, 301}, 300);

    if (!check(
        first.buyers == vector<int>({3, 4})
            && first.revenue == 600,
        "Покупают только зрители с достаточным бюджетом."
    )) {
        allPassed = false;
    }
    vector<int> budgets(20, 299);
    budgets.insert(budgets.end(), 100, 500);

    SessionResult second = sellTickets(budgets, 300);

    if (!check(
        second.buyers.size() == CAPACITY
            && second.revenue == 24000,
        "Продаётся не более 80 билетов."
    )) {
        allPassed = false;
    }
    bool enoughMoney = true;
    for (int index : second.buyers) {
        if (budgets[index] < 300) {
            enoughMoney = false;
        }
    }
    if (!check(
        enoughMoney,
        "У каждого покупателя достаточно денег."
    )) {
        allPassed = false;
    }
    SessionResult third = sellTickets({200, 250, 299}, 300);
    if (!check(
        third.buyers.empty() && third.revenue == 0,
        "Если цена никому не подходит, продаж нет."
    )) {
        allPassed = false;
    }
    SessionResult fourth = sellTickets({}, 300);
    if (!check(
        fourth.buyers.empty() && fourth.revenue == 0,
        "При отсутствии зрителей продаж нет."
    )) {
        allPassed = false;
    }
    return allPassed;
}
void runExperiments() {
    constexpr int experiments = 1000;
    const array<int, 7> prices = {200, 300, 400, 500, 600, 700, 800};

    // Фиксированное начальное значение помогает повторить расчёт.
    mt19937 generator(42);
    cout << fixed << setprecision(2);
    for (bool premiere : {false, true}) {
        uniform_int_distribution<int> randomBudget(
            premiere ? 400 : 200,
            premiere ? 900 : 600
        );
        for (int viewerCount : {60, 120, 180}) {
            array<int, 7> totalRevenue{};
            array<int, 7> totalTickets{};
            array<int, 7> soldOut{};

            vector<int> budgets(viewerCount);

            for (int experiment = 0; experiment < experiments; ++experiment) {
                // Создаём один набор зрителей для всех семи цен
                for (int& budget : budgets) {
                    budget = randomBudget(generator);
                }

                for (int i = 0; i < 7; ++i) {
                    SessionResult result = sellTickets(budgets, prices[i]);
                    int sold = static_cast<int>(result.buyers.size());

                    totalRevenue[i] += result.revenue;
                    totalTickets[i] += sold;

                    if (sold == CAPACITY) {
                        ++soldOut[i];
                    }
                }
            }

            cout << "\n" << (premiere ? "Премьера" : "Обычный фильм")
                 << ", потенциальных покупателей: " << viewerCount << '\n'
                 << "Цена, руб. | Средняя выручка, руб. | "
                    "Среднее число билетов | Аншлаг, %\n";

            int best = 0;
            for (int i = 0; i < 7; ++i) {
                cout << prices[i] << " | "
                     << static_cast<double>(totalRevenue[i]) / experiments << " | "
                     << static_cast<double>(totalTickets[i]) / experiments << " | "
                     << 100.0 * soldOut[i] / experiments << '\n';

                if (totalRevenue[i] > totalRevenue[best]) {
                    best = i;
                }
            }

            cout << "Лучшая из проверенных цен: "
                 << prices[best] << " руб.\n";
        }
    }
}
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    cout << "Исследование выручки кинотеатра\n"
         << "Модель продажи билетов. Добавлены эксперименты\n\n";
    cout << "Проверка программы:\n";
    if (!runTests()) {
        cout << "\nОбнаружены ошибки в работе программы.\n";
        return 1;
    }
    cout << "\nВсе проверки пройдены.\n";
    vector<int> budgets = {200, 350, 500, 250, 600, 300};
    int price = 300;
    SessionResult result = sellTickets(budgets, price);
    cout << "\nПример одного сеанса\n"
         << "Вместимость зала: " << CAPACITY << '\n'
         << "Количество потенциальных покупателей: "
         << budgets.size() << '\n'
         << "Цена билета: " << price << " руб.\n";
    cout << "Бюджеты зрителей: ";

    for (int budget : budgets) {
        cout << budget << ' ';
    }
    cout << "\nПродано билетов: " << result.buyers.size()
         << "\nВыручка: " << result.revenue << " руб.\n"
         << "Свободных мест: "
         << CAPACITY - static_cast<int>(result.buyers.size())
         << '\n';

        runExperiments();
    return 0;
}
