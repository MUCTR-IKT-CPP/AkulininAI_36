#include <iostream>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <random>
#include <Windows.h>

using namespace std;

/**
* ����� SortedVector, ����������� �� std::vector � �������������� ���������� ���������.
* 
* @tparam T ��� ��������� �������.
*/
template<typename T>
class SortedVector : public vector<T> {
public:
    /**
    * �����������, ������� �������������� ������ � ������� �������� ���������� � ��������� ���.
    * 
    * @tparam TT ����������� ���.
    * @param a �������� �� ������ ���������.
    * @param b �������� �� ����� ���������.
    */
    template<typename TT>
    SortedVector(TT a, TT b) : vector<T>(a, b) {
        sort(this->begin(), this->end());
    }

    /**
    * ����������� ��� �������� ���������������� �������. ���� ����������� ��������� ������ �������������
    * � ������� ������, �������� ��� �������� �� �����������.
    *
    * @param init ������ �������������, ���������� �������� ��� ���������� � ������.
    * @note ����� ������ ������������ ��� �������� ������� ����� �������������.
    */
    SortedVector(initializer_list<T> init) : vector<T>(init) {
        sort(this->begin(), this->end());
    }

    /**
    * ������������ ���������� ���������, ��������� �� �������� �����.
    * 
    * @param d ��������.
    * @return ���������� ���������, ��������� �� d.
    */
    int count_divider(int d) const {
        return count_if(this->begin(), this->end(), [d](T elem) {
            return elem % d == 0;
        });
    }
    
    /**
    * ��������� ����� � ������.
    * 
    * @param num ����� ��� �������.
    */
    void insert_num(int num) {
        auto pos = lower_bound(this->begin(), this->end(), num);
        this->insert(pos, num);
    }

    /**
    * ���������� � ������� ��� ������������ ��������� �������.
    */
    void permutations() const {
        vector<T> elements(this->begin(), this->end());
        do {
            copy(elements.begin(), elements.end(), ostream_iterator<int>(cout, " "));
            cout << endl;
        } while (next_permutation(elements.begin(), elements.end()));
    }

    /**
    * ����������� �������� ������, ����� ������� ���������� SortedVector.
    * 
    * @param out ����� ������.
    * @param sv ������ SortedVector.
    * @return ������ �� ����� �����-������.
    */
    friend ostream& operator<<(ostream& out, const SortedVector<T>& sv) {
        copy(sv.begin(), sv.end(), ostream_iterator<T>(out, " "));
        return out;
    }
};

/**
* ���������� ������ ��������� �����.
* 
* @param N ���������� ��������� �����.
* @param min ����������� �������� (������������).
* @param max ������������ �������� (������������).
* @return ��������������� ������ ��������� �����.
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
    cout << "������� ������ �������: ";
    cin >> N;
    cout << "������� ������ ������� ���������� �������: ";
    cin >> min;
    cout << "������� �������� ������� ���������� �������: ";
    cin >> max;
    if ((N <= 0) || (max == min)) {
        cout << "������: �������� ������ �����!";
        return 0;
    }

    vector<int> myvector = generate_random(N, min, max);
    SortedVector<int> sv(myvector.begin(), myvector.end());

    cout << "initializer_list" << endl;
    SortedVector<int> sv2 = { 1, 3, 2, 5, 4 };
    cout << sv2 << endl;

    do {
        cout << "\n�������� ��������, ����� ����� �� 1 �� 5:\n";
        cout << "1 - �������� ����������� ������.\n";
        cout << "2 - ������������� ������.\n";
        cout << "3 - �������� �� ���������.\n";
        cout << "4 - �������� �����.\n";
        cout << "5 - ������������.\n";
        cout << "0 - �����.\n";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "\n����������� ������: ";
            copy(myvector.begin(), myvector.end(), ostream_iterator<int>(cout, " "));
            cout << endl;
            break;
        case 2:
            cout << "\n��������������� ������: " << sv << endl;
            break;
        case 3:
            cout << "\n������� ����� ��� �������� ���������: ";
            cin >> divider;
            count = sv.count_divider(divider);
            cout << "���������� ���������, ��������� �� " << divider << ": " << count << endl;
            break;
        case 4:
            cout << "\n������� �����, ������� ��������: ";
            cin >> num;
            sv.insert_num(num);
            cout << "������ ����� �������: " << sv << endl;
            break;
        case 5:
            cout << "\n������������:" << endl;
            sv.permutations();
            break;
        case 0:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "������: �������� �����, ���������� ��� ���." << endl;
        }
    } while (choice != 0);

	return 0;
}