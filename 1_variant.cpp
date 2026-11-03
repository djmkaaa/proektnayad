#include <iostream>
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
        "У каждого покупателя хватит денег."
    )) {
        allPassed = false;
    }
    SessionResult third = sellTickets({200, 250, 299}, 300);
    if (!check(
        third.buyers.empty() && third.revenue == 0,
        "Если цена никому не подходит-продаж нет."
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
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    cout << "Вычисление выручки кинотеатра\n"
         << "Модель продажи билетов\n\n";
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

    return 0;
}
