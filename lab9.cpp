#include <iostream>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <random>
#include <Windows.h>

using namespace std;

/**
* Класс SortedVector, наследующий от std::vector и поддерживающий сортировку элементов.
* 
* @tparam T Тип элементов вектора.
*/
template<typename T>
class SortedVector : public vector<T> {
public:
    /**
    * Конструктор, который инициализирует вектор с помощью заданных итераторов и сортирует его.
    * 
    * @tparam TT Итераторный тип.
    * @param a Итератор на начало диапазона.
    * @param b Итератор на конец диапазона.
    */
    template<typename TT>
    SortedVector(TT a, TT b) : vector<T>(a, b) {
        sort(this->begin(), this->end());
    }

    /**
    * Конструктор для создания отсортированного вектора. Этот конструктор принимает список инициализации
    * и создает вектор, сортируя его элементы по возрастанию.
    *
    * @param init Список инициализации, содержащий элементы для добавления в вектор.
    * @note После вызова конструктора все элементы вектора будут отсортированы.
    */
    SortedVector(initializer_list<T> init) : vector<T>(init) {
        sort(this->begin(), this->end());
    }

    /**
    * Подсчитывает количество элементов, делящихся на заданное число.
    * 
    * @param d Делитель.
    * @return Количество элементов, делящихся на d.
    */
    int count_divider(int d) const {
        return count_if(this->begin(), this->end(), [d](T elem) {
            return elem % d == 0;
        });
    }
    
    /**
    * Вставляет число в вектор.
    * 
    * @param num Число для вставки.
    */
    void insert_num(int num) {
        auto pos = lower_bound(this->begin(), this->end(), num);
        this->insert(pos, num);
    }

    /**
    * Генерирует и выводит все перестановки элементов вектора.
    */
    void permutations() const {
        vector<T> elements(this->begin(), this->end());
        do {
            copy(elements.begin(), elements.end(), ostream_iterator<int>(cout, " "));
            cout << endl;
        } while (next_permutation(elements.begin(), elements.end()));
    }

    /**
    * Перегружает оператор вывода, чтобы вывести содержимое SortedVector.
    * 
    * @param out Поток вывода.
    * @param sv Объект SortedVector.
    * @return Ссылка на поток ввода-вывода.
    */
    friend ostream& operator<<(ostream& out, const SortedVector<T>& sv) {
        copy(sv.begin(), sv.end(), ostream_iterator<T>(out, " "));
        return out;
    }
};

/**
* Генерирует вектор случайных чисел.
* 
* @param N Количество случайных чисел.
* @param min Минимальное значение (включительно).
* @param max Максимальное значение (включительно).
* @return Сгенерированный вектор случайных чисел.
*/
vector<int> generate_random(int N, int min, int max) {
    vector<int> random_vector(N);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);

    for (int i = 0; i < N; i++) {
        random_vector[i] = dis(gen);
    }
    return random_vector;
}

int main() {
	setlocale(LC_ALL, "");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    int N, min, max, divider, num, count, choice;
    cout << "Введите размер вектора: ";
    cin >> N;
    cout << "Введите нижнию границу заполнения вектора: ";
    cin >> min;
    cout << "Введите вверхнию границу заполнения вектора: ";
    cin >> max;
    if ((N <= 0) || (max == min)) {
        cout << "Ошибка: неверные данные ввода!";
        return 0;
    }

    vector<int> myvector = generate_random(N, min, max);
    SortedVector<int> sv(myvector.begin(), myvector.end());

    cout << "initializer_list" << endl;
    SortedVector<int> sv2 = { 1, 3, 2, 5, 4 };
    cout << sv2 << endl;

    do {
        cout << "\nВыберите действие, введя цифру от 1 до 5:\n";
        cout << "1 - Получить изначальный вектор.\n";
        cout << "2 - Отсортировать вектор.\n";
        cout << "3 - Проверка на делимость.\n";
        cout << "4 - Вставить число.\n";
        cout << "5 - Перестановки.\n";
        cout << "0 - Выход.\n";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "\nИзначальный вектор: ";
            copy(myvector.begin(), myvector.end(), ostream_iterator<int>(cout, " "));
            cout << endl;
            break;
        case 2:
            cout << "\nОтсортированный вектор: " << sv << endl;
            break;
        case 3:
            cout << "\nВведите число для проверки делимости: ";
            cin >> divider;
            count = sv.count_divider(divider);
            cout << "Количество элементов, делящихся на " << divider << ": " << count << endl;
            break;
        case 4:
            cout << "\nВведите число, которое вставить: ";
            cin >> num;
            sv.insert_num(num);
            cout << "Вектор после вставки: " << sv << endl;
            break;
        case 5:
            cout << "\nПерестановки:" << endl;
            sv.permutations();
            break;
        case 0:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Ошибка: неверный выбор, попробуйте еще раз." << endl;
        }
    } while (choice != 0);

	return 0;
}