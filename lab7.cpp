#include <iostream>
#include <ctime>
#include <Windows.h>

using namespace std;

template <typename T>
class MyVector {
private:
    T* data;               // Указатель на массив элементов
    size_t capacity;       // Вместимость вектора
    size_t size;           // Текущий размер вектора
    size_t resize_factor;  // Период расширения и сокращения

public:
    /**
     * Конструктор, создающий вектор с заданной начальной вместимостью.
     *
     * @param initial_capacity Начальная вместимость вектора.
     * @param resize_factor Определяет сколько элементов должно быть добавлено к мощности 
     *        и условие для уменьшения мощности (по умолчанию 2).
     */
    MyVector(const size_t n, size_t resize_factor = 2)
        : capacity(n), size(0), resize_factor(resize_factor) {
        data = new T[capacity];
    }
    
    /**
    * Увеличивает вместимость вектора в два раза. Создает новый массив с увеличенной вместимостью,
    * копирует существующие элементы в новый массив и освобождает память старого массива.
    */
    void resize(size_t capacity) {
        T* new_data = new T[capacity];
        for (size_t i = 0; i < size; i++) {
            new_data[i] = data[i];
        }
        delete[] data;      // Освобождаем старый массив
        data = new_data;    // Указываем на новый массив
    }

    void type_definition(int type) {
        if (type == 1) {
            capacity += resize_factor;
            resize(capacity);
        }
        else if (type == 2) {
            size--;
            if (capacity - size > resize_factor && capacity > 4) {
                capacity -= resize_factor;
                resize(capacity);
            }
        }
        else if (type == 3) {
            if (capacity - size > resize_factor && capacity > 4) {
                capacity -= resize_factor;
                resize(capacity);
            }
        }
    }

    /**
     * Деструктор, освобождает память, занятую вектором.
     */
    ~MyVector() {
        delete[] data;
    }

    /**
     * Проверяет, пустой ли вектор.
     *
     * @return true Если вектор пустой.
     * @return false Если вектор не пустой.
     */
    bool is_empty() const {
        return size == 0;
    }

    /**
     * Получает текущий размер вектора.
     *
     * @return Размер вектора.
     */
    size_t get_size() const {
        return size;
    }

    /**
     * Получает вместимости вектора.
     *
     * @return Вместимость вектора.
     */
    size_t get_сapacity() const {
        return capacity;
    }

    /**
     * Оператор доступа к элементу вектора по индексу.
     *
     * @param index Индекс элемента.
     * @return Ссылку на элемент вектора.
     * @throws out_of_range Если индекс выходит за границы текущего размера вектора.
     */
    T& operator[](size_t index) {
        if (index >= size) {
            cout << "\nIndex out of bounds\n\n";
        }
        return data[index];
    }

    /**
     * Добавляет элемент в конец вектора. Увеличивает вместимость вектора при необходимости.
     *
     * @param value Значение добавляемого элемента.
     */
    void pushback(const T& value) {
        if (size == capacity) {
            type_definition(1);
        }
        data[size++] = value;
    }

    /**
     * Удаляет последний элемент вектора.
     *
     * @throws underflow_error Если вектор пустой.
     */
    void popback() {
        if (size > 0) {
            type_definition(2);
        }
        else {
            cout << "\nVector is empty\n\n";
        }
    }

    // Итератор для обхода элементов вектора.
    class Iterator {
    private:
        T* ptr; // Указатель на текущий элемент

    public:
        /**
         * Конструктор итератора.
         *
         * @param p Указатель на элемент для инициализации итератора.
         */
        Iterator(T* p) : ptr(p) {}

        /**
         * Возвращает ссылку на текущий элемент, на который указывает итератор.
         *
         * @return Ссылку на текущий элемент.
         */
        T& operator*() { 
            return *ptr;
        }

        /**
         * Перемещает итератор на следующий элемент.
         *
         * @return Ссылку на текущий итератор для цепочки вызовов.
         */
        Iterator& operator++() { 
            ptr++;
            return *this;
        }

        /**
         * Сравнивает итераторы.
         *
         * @param other Другой итератор для сравнения.
         * @return true Если итераторы указывают на разные элементы.
         * @return false Если итераторы указывают на один и тот же элемент.
         */
        bool operator!=(const Iterator& other) const { 
            return ptr != other.ptr;
        }
    };

    /**
     * Возвращает итератор на первый элемент вектора.
     *
     * @return Итератор, указывающий на начало вектора.
     */
    Iterator begin() { return Iterator(data); }

    /**
     * Возвращает итератор на элемент после последнего элемента вектора.
     *
     * @return Итератор, указывающий на конец вектора.
     */
    Iterator end() { return Iterator(data + size); }

    /**
     * Перегруженный оператор вывода для удобного отображения содержимого вектора.
     *
     * @param os Поток вывода.
     * @param vec Вектор для отображения.
     * @return Поток вывода.
     */
    static friend ostream& operator<<(ostream& os, const MyVector<T>& vec) {
        os << "MyVector {";
        for (int i = 0; i < vec.size; i++) {
            os << vec.data[i];
            if (i < vec.size - 1) {
                os << ", ";
            }
        }
        os << "}";
        return os;
    }
};

int main() {
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    srand(static_cast<unsigned int>(time(0)));

    int choice, n;
    cout << "Введите кол-во элементов для вектора: ";
    cin >> n;
    MyVector<int> vector(n);
    cout << vector << endl << endl;
    do {
        cout << "Выберите действие цифрами от 1 до 8:\n";
        cout << "1 - Проверка, пустой ли вектор\n";
        cout << "2 - Получить размер вектора\n";
        cout << "3 - Получение произвольного элемента (через перегрузку operator[])\n";
        cout << "4 - Изменение размера вектора\n";
        cout << "5 - Добавление нового элемента в конец вектор\n";
        cout << "6 - Удаление последнего элемента из вектора\n";
        cout << "7 - Итерация по элементам\n";
        cout << "0 - Exit\n";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "\nВектор пустой? " << (vector.is_empty() ? "Да" : "Нет") << endl << endl;
            break;
        case 2:
            cout << "\nРазмер вектора: " << vector.get_size() << endl;
            cout << "Вместимость вектора: " << vector.get_сapacity() << endl << endl;
            break;
        case 3:
            n = rand() % vector.get_size();
            cout << "\nЭлемент со случайным индексом " << n << " (через перегрузку operator[]): " << vector[n] << endl << endl;
            break;
        case 4:
            cout << "\nВместимость вектора до: " << vector.get_сapacity() << endl;
            if (vector.get_сapacity() - vector.get_size() >= 2) {
                vector.type_definition(3);
            }
            else {
                vector.type_definition(1);
            }
            cout << "Вместимость вектора после: " << vector.get_сapacity() << endl << endl;
            break;
        case 5:
            cout << "\nВведите элемент, который добавить: ";
            cin >> n;
            vector.pushback(n);
            cout << "После добавления: " << vector << endl << endl;
            break;
        case 6:
            vector.popback();
            cout << "\nПосле удаления: " << vector << endl << endl;
            break;
        case 7:
            cout << "\nИтерация по элементам: ";
            for (auto elem : vector) {
                cout << elem << " ";
            }
            cout << endl << endl;
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз.\n";
        }
    } while (choice != 0);

    return 0;
}
