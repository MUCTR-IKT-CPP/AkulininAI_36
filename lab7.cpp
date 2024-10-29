#include <iostream>
#include <ctime>
#include <Windows.h>

using namespace std;

template <typename T>
class MyVector {
private:
    T* data;               // ��������� �� ������ ���������
    size_t capacity;       // ����������� �������
    size_t size;           // ������� ������ �������
    size_t resize_factor;  // ������ ���������� � ����������

public:
    /**
     * �����������, ��������� ������ � �������� ��������� ������������.
     *
     * @param initial_capacity ��������� ����������� �������.
     * @param resize_factor ���������� ������� ��������� ������ ���� ��������� � �������� 
     *        � ������� ��� ���������� �������� (�� ��������� 2).
     */
    MyVector(const size_t n, size_t resize_factor = 2)
        : capacity(n), size(0), resize_factor(resize_factor) {
        data = new T[capacity];
    }
    
    /**
    * ����������� ����������� ������� � ��� ����. ������� ����� ������ � ����������� ������������,
    * �������� ������������ �������� � ����� ������ � ����������� ������ ������� �������.
    */
    void resize(size_t capacity) {
        T* new_data = new T[capacity];
        for (size_t i = 0; i < size; i++) {
            new_data[i] = data[i];
        }
        delete[] data;      // ����������� ������ ������
        data = new_data;    // ��������� �� ����� ������
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
     * ����������, ����������� ������, ������� ��������.
     */
    ~MyVector() {
        delete[] data;
    }

    /**
     * ���������, ������ �� ������.
     *
     * @return true ���� ������ ������.
     * @return false ���� ������ �� ������.
     */
    bool is_empty() const {
        return size == 0;
    }

    /**
     * �������� ������� ������ �������.
     *
     * @return ������ �������.
     */
    size_t get_size() const {
        return size;
    }

    /**
     * �������� ����������� �������.
     *
     * @return ����������� �������.
     */
    size_t get_�apacity() const {
        return capacity;
    }

    /**
     * �������� ������� � �������� ������� �� �������.
     *
     * @param index ������ ��������.
     * @return ������ �� ������� �������.
     * @throws out_of_range ���� ������ ������� �� ������� �������� ������� �������.
     */
    T& operator[](size_t index) {
        if (index >= size) {
            cout << "\nIndex out of bounds\n\n";
        }
        return data[index];
    }

    /**
     * ��������� ������� � ����� �������. ����������� ����������� ������� ��� �������������.
     *
     * @param value �������� ������������ ��������.
     */
    void pushback(const T& value) {
        if (size == capacity) {
            type_definition(1);
        }
        data[size++] = value;
    }

    /**
     * ������� ��������� ������� �������.
     *
     * @throws underflow_error ���� ������ ������.
     */
    void popback() {
        if (size > 0) {
            type_definition(2);
        }
        else {
            cout << "\nVector is empty\n\n";
        }
    }

    // �������� ��� ������ ��������� �������.
    class Iterator {
    private:
        T* ptr; // ��������� �� ������� �������

    public:
        /**
         * ����������� ���������.
         *
         * @param p ��������� �� ������� ��� ������������� ���������.
         */
        Iterator(T* p) : ptr(p) {}

        /**
         * ���������� ������ �� ������� �������, �� ������� ��������� ��������.
         *
         * @return ������ �� ������� �������.
         */
        T& operator*() { 
            return *ptr;
        }

        /**
         * ���������� �������� �� ��������� �������.
         *
         * @return ������ �� ������� �������� ��� ������� �������.
         */
        Iterator& operator++() { 
            ptr++;
            return *this;
        }

        /**
         * ���������� ���������.
         *
         * @param other ������ �������� ��� ���������.
         * @return true ���� ��������� ��������� �� ������ ��������.
         * @return false ���� ��������� ��������� �� ���� � ��� �� �������.
         */
        bool operator!=(const Iterator& other) const { 
            return ptr != other.ptr;
        }
    };

    /**
     * ���������� �������� �� ������ ������� �������.
     *
     * @return ��������, ����������� �� ������ �������.
     */
    Iterator begin() { return Iterator(data); }

    /**
     * ���������� �������� �� ������� ����� ���������� �������� �������.
     *
     * @return ��������, ����������� �� ����� �������.
     */
    Iterator end() { return Iterator(data + size); }

    /**
     * ������������� �������� ������ ��� �������� ����������� ����������� �������.
     *
     * @param os ����� ������.
     * @param vec ������ ��� �����������.
     * @return ����� ������.
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
    cout << "������� ���-�� ��������� ��� �������: ";
    cin >> n;
    MyVector<int> vector(n);
    cout << vector << endl << endl;
    do {
        cout << "�������� �������� ������� �� 1 �� 8:\n";
        cout << "1 - ��������, ������ �� ������\n";
        cout << "2 - �������� ������ �������\n";
        cout << "3 - ��������� ������������� �������� (����� ���������� operator[])\n";
        cout << "4 - ��������� ������� �������\n";
        cout << "5 - ���������� ������ �������� � ����� ������\n";
        cout << "6 - �������� ���������� �������� �� �������\n";
        cout << "7 - �������� �� ���������\n";
        cout << "0 - Exit\n";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "\n������ ������? " << (vector.is_empty() ? "��" : "���") << endl << endl;
            break;
        case 2:
            cout << "\n������ �������: " << vector.get_size() << endl;
            cout << "����������� �������: " << vector.get_�apacity() << endl << endl;
            break;
        case 3:
            n = rand() % vector.get_size();
            cout << "\n������� �� ��������� �������� " << n << " (����� ���������� operator[]): " << vector[n] << endl << endl;
            break;
        case 4:
            cout << "\n����������� ������� ��: " << vector.get_�apacity() << endl;
            if (vector.get_�apacity() - vector.get_size() >= 2) {
                vector.type_definition(3);
            }
            else {
                vector.type_definition(1);
            }
            cout << "����������� ������� �����: " << vector.get_�apacity() << endl << endl;
            break;
        case 5:
            cout << "\n������� �������, ������� ��������: ";
            cin >> n;
            vector.pushback(n);
            cout << "����� ����������: " << vector << endl << endl;
            break;
        case 6:
            vector.popback();
            cout << "\n����� ��������: " << vector << endl << endl;
            break;
        case 7:
            cout << "\n�������� �� ���������: ";
            for (auto elem : vector) {
                cout << elem << " ";
            }
            cout << endl << endl;
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "�������� �����, ���������� ��� ���.\n";
        }
    } while (choice != 0);

    return 0;
}
