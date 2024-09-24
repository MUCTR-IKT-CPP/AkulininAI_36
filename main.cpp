#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

// Количество доступных валют
const int CURRENCY_COUNT = 6;
// Количество доступных номиналов банкнот
const int NOMINAL_COUNT = 7;

// Перечисление для номиналов банкнот
enum Nominal { ONE = 1, FIVE = 5, TEN = 10, TWENTY = 20, FIFTY = 50, HUNDRED = 100, THOUSAND = 1000 };
// Массив для названий валют
const string currencies[CURRENCY_COUNT] = { "dollar", "euro", "yen", "francs", "zlotys", "ruble"};
// Массив для курсов обмена валют
const float exchange_rates[CURRENCY_COUNT] = { 95.00f, 100.00f, 0.65f, 110.00f, 25.00f, 1.00f};
// Массив для значений номиналов
const int nominal_names[] = { 1, 5, 10, 20, 50, 100, 1000 };

// Структура для представления банкноты
struct Banknote {
    string currency;    ///< Валюта банкноты
    Nominal nominal;    ///< Номинал банкноты
    float exchange_rate;///< Курс обмена 

    /**
     * Печатает информацию о кошельке
     */
    void print() const {
        cout << left << setw(10) << currency
            << setw(10) << nominal_names[nominal - 1]
            << setw(15) << exchange_rate << endl;
    }
};

/**
 * Генерирует случайный номинал банкноты
 * 
 * @return Случайный номинал из перечисления Nominal
 */
Nominal get_random_nominal() {
    return static_cast<Nominal>((rand() % NOMINAL_COUNT) + 1);
}

/**
 * Подсчитывают общую сумму в рублях хранимую в кошельке.
 * 
 * @param wallet Указатель на массив банкнот
 * @param N Количество банкнот
 * @return Общая сумма в рублях
 */
float calculate_total_rub(const Banknote* wallet, int N) {
    float sum = 0.0;
    for (int i = 0; i < N; i++) {
        sum += (nominal_names[wallet[i].nominal - 1] * wallet[i].exchange_rate);
    }
    return sum;
}

/**
 * Формирует срез банкнот с номиналом больше выбранного пользователем
 * и выводит их количество разбивая их по валютам
 * 
 * @param wallet Указатель на массив банкнот
 * @param N Количество банкнот
 * @param min_nominal Минимальный номинал для фильтрации
 */
void filter_banknotes_by_nominal(const Banknote* wallet, int N, int min_nominal) {
    int currency_counts[CURRENCY_COUNT] = { 0 };
    for (int i = 0; i < N; i++) {
        if (nominal_names[wallet[i].nominal - 1] > min_nominal) {
            for (int j = 0; j < CURRENCY_COUNT; j++) {
                if (wallet[i].currency == currencies[j]) {
                    currency_counts[j]++;
                    break;
                }
            }
        }
    }

    cout << endl << "Count of banknotes with nominal greater than " << min_nominal << ":" << endl;
    for (int j = 0; j < CURRENCY_COUNT; j++) {
        if (currency_counts[j] > 0) {
            cout << currencies[j] << ": " << currency_counts[j] << endl;
        }
    }
}

/**
 * Выводит состав кошелька отсортированный по валютам (по алфавиту)
 * и номиналу с количеством каждой существующей банкноты
 * 
 * @param wallet Указатель на массив банкнот
 * @param N Количество банкнот
 */
void print_wallet_summary(const Banknote* wallet, int N) {
    map<string, map<Nominal, int>> wallet_summary;
    for (int i = 0; i < N; i++) {
        wallet_summary[wallet[i].currency][wallet[i].nominal]++;
    }

    cout << "\nWallet Summary:\n";
    for (const auto& currency_ind : wallet_summary) {
        const string& currency = currency_ind.first;
        const auto& nominal_map = currency_ind.second;
        cout << currency << ":\n";
        for (const auto& nominal_ind : nominal_map) {
            cout << "  Nominal " << nominal_names[nominal_ind.first - 1] << ": " << nominal_ind.second << endl;
        }
    }
}

/**
 * Конвертирует все банкноты в рубль через курс набирая итог банкнотами с максимальным номиналом,
 * в случае неровной суммы округляем в большую сторону
 * 
 * @param wallet Указатель на массив банкнот
 * @param N Количество банкнот
 */
void convert(const Banknote* wallet, int N) {
    float sum_rub = ceil(calculate_total_rub(wallet, N));
    vector<int> counts(NOMINAL_COUNT, 0);
    for (int i = NOMINAL_COUNT - 1; i >= 0; i--) {
        while (sum_rub >= nominal_names[i]) {
            sum_rub -= nominal_names[i];
            counts[i]++;
        }
    }

    cout << "Banknotes by denomination:" << endl;
    for (int i = NOMINAL_COUNT - 1; i >= 0; i--) {
        if (counts[i] > 0) {
            cout << nominal_names[i] << ": " << counts[i] << " RUB" << endl;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int N;
    cout << "Enter the number of banknotes: ";
    cin >> N;
    if (N <= 0) {
        cout << "The number of banknotes must be greater than 0." << endl;
        return 1;
    }

    Banknote* wallet = new Banknote[N];
    for (int i = 0; i < N; i++) {
        int currencyIndex = rand() % CURRENCY_COUNT;
    
        wallet[i].currency = currencies[currencyIndex];
        wallet[i].nominal = get_random_nominal();
        wallet[i].exchange_rate = exchange_rates[currencyIndex];
    }

    int choice, user_nominal;
    float total_rub, converted_rub;
    do {
        cout << endl << "Select an action by entering numbers from 1 to 5:" << endl;
        cout << "1 - Open wallet" << endl;
        cout << "2 - Calculate the total amount in rubles stored in the wallet" << endl;
        cout << "3 - Banknotes with a denomination greater than the one selected by the user" << endl;
        cout << "4 - Wallet sorted by currency (alphabetically) and denomination with the amount of each existing banknote" << endl;
        cout << "5 - Converts all banknotes to rubles using the rate" << endl;
        cout << "0 - Exit" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            cout << endl << left << setw(10) << "Banknote" << setw(10) << "Nominal" << setw(15) << "Exchange rate to ruble" << endl;
            cout << string(35, '-') << endl;
            for (int i = 0; i < N; i++) {
                wallet[i].print();
            }
            break;
        case 2:
            total_rub = calculate_total_rub(wallet, N);
            cout << endl << "Total amount: " << total_rub << " RUB" << endl;
            cout << string(35, '-') << endl;
            break;
        case 3:
            cout << "Enter a nominal value to filter banknotes: ";
            cin >> user_nominal;
            filter_banknotes_by_nominal(wallet, N, user_nominal);
            cout << string(35, '-') << endl;
            break;
        case 4:
            print_wallet_summary(wallet, N);
            cout << string(35, '-') << endl;
            break;
        case 5:
            convert(wallet, N);
            cout << string(35, '-') << endl;
            break;
        case 0:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice, please try again." << endl;
        }

    } while (choice != 0);

    delete[] wallet;
}
