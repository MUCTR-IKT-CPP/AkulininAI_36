#include <iostream>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <random>
#include <Windows.h>

using namespace std;

/**
* Êëàññ SortedVector, íàñëåäóþùèé îò std::vector è ïîääåðæèâàþùèé ñîðòèðîâêó ýëåìåíòîâ.
* 
* @tparam T Òèï ýëåìåíòîâ âåêòîðà.
*/
template<typename T>
class SortedVector : public vector<T> {
public:
    /**
    * Êîíñòðóêòîð, êîòîðûé èíèöèàëèçèðóåò âåêòîð ñ ïîìîùüþ çàäàííûõ èòåðàòîðîâ è ñîðòèðóåò åãî.
    * 
    * @tparam TT Èòåðàòîðíûé òèï.
    * @param a Èòåðàòîð íà íà÷àëî äèàïàçîíà.
    * @param b Èòåðàòîð íà êîíåö äèàïàçîíà.
    */
    template<typename TT>
    SortedVector(TT a, TT b) : vector<T>(a, b) {
        sort(this->begin(), this->end());
    }

    /**
    * Êîíñòðóêòîð äëÿ ñîçäàíèÿ îòñîðòèðîâàííîãî âåêòîðà. Ýòîò êîíñòðóêòîð ïðèíèìàåò ñïèñîê èíèöèàëèçàöèè
    * è ñîçäàåò âåêòîð, ñîðòèðóÿ åãî ýëåìåíòû ïî âîçðàñòàíèþ.
    *
    * @param init Ñïèñîê èíèöèàëèçàöèè, ñîäåðæàùèé ýëåìåíòû äëÿ äîáàâëåíèÿ â âåêòîð.
    * @note Ïîñëå âûçîâà êîíñòðóêòîðà âñå ýëåìåíòû âåêòîðà áóäóò îòñîðòèðîâàíû.
    */
    SortedVector(initializer_list<T> init) : vector<T>(init) {
        sort(this->begin(), this->end());
    }

    /**
    * Ïîäñ÷èòûâàåò êîëè÷åñòâî ýëåìåíòîâ, äåëÿùèõñÿ íà çàäàííîå ÷èñëî.
    * 
    * @param d Äåëèòåëü.
    * @return Êîëè÷åñòâî ýëåìåíòîâ, äåëÿùèõñÿ íà d.
    */
    int count_divider(int d) const {
        return count_if(this->begin(), this->end(), [d](T elem) {
            return elem % d == 0;
        });
    }
    
    /**
    * Âñòàâëÿåò ÷èñëî â âåêòîð.
    * 
    * @param num ×èñëî äëÿ âñòàâêè.
    */
    void insert_num(int num) {
        auto pos = lower_bound(this->begin(), this->end(), num);
        this->insert(pos, num);
    }

    /**
    * Ãåíåðèðóåò è âûâîäèò âñå ïåðåñòàíîâêè ýëåìåíòîâ âåêòîðà.
    */
    void permutations() const {
        vector<T> elements(this->begin(), this->end());
        do {
            copy(elements.begin(), elements.end(), ostream_iterator<int>(cout, " "));
            cout << endl;
        } while (next_permutation(elements.begin(), elements.end()));
    }

    /**
    * Ïåðåãðóæàåò îïåðàòîð âûâîäà, ÷òîáû âûâåñòè ñîäåðæèìîå SortedVector.
    * 
    * @param out Ïîòîê âûâîäà.
    * @param sv Îáúåêò SortedVector.
    * @return Ññûëêà íà ïîòîê ââîäà-âûâîäà.
    */
    friend ostream& operator<<(ostream& out, const SortedVector<T>& sv) {
        copy(sv.begin(), sv.end(), ostream_iterator<T>(out, " "));
        return out;
    }
};

/**
* Ãåíåðèðóåò âåêòîð ñëó÷àéíûõ ÷èñåë.
* 
* @param N Êîëè÷åñòâî ñëó÷àéíûõ ÷èñåë.
* @param min Ìèíèìàëüíîå çíà÷åíèå (âêëþ÷èòåëüíî).
* @param max Ìàêñèìàëüíîå çíà÷åíèå (âêëþ÷èòåëüíî).
* @return Ñãåíåðèðîâàííûé âåêòîð ñëó÷àéíûõ ÷èñåë.
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
    cout << "Ââåäèòå ðàçìåð âåêòîðà: ";
    cin >> N;
    cout << "Ââåäèòå íèæíèþ ãðàíèöó çàïîëíåíèÿ âåêòîðà: ";
    cin >> min;
    cout << "Ââåäèòå ââåðõíèþ ãðàíèöó çàïîëíåíèÿ âåêòîðà: ";
    cin >> max;
    if ((N <= 0) || (max == min)) {
        cout << "Îøèáêà: íåâåðíûå äàííûå ââîäà!";
        return 0;
    }

    vector<int> myvector = generate_random(N, min, max);
    SortedVector<int> sv(myvector.begin(), myvector.end());

    cout << "initializer_list" << endl;
    SortedVector<int> sv2 = { 1, 3, 2, 5, 4 };
    cout << sv2 << endl;

    do {
        cout << "\nÂûáåðèòå äåéñòâèå, ââåäÿ öèôðó îò 1 äî 5:\n";
        cout << "1 - Ïîëó÷èòü èçíà÷àëüíûé âåêòîð.\n";
        cout << "2 - Îòñîðòèðîâàòü âåêòîð.\n";
        cout << "3 - Ïðîâåðêà íà äåëèìîñòü.\n";
        cout << "4 - Âñòàâèòü ÷èñëî.\n";
        cout << "5 - Ïåðåñòàíîâêè.\n";
        cout << "0 - Âûõîä.\n";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "\nÈçíà÷àëüíûé âåêòîð: ";
            copy(myvector.begin(), myvector.end(), ostream_iterator<int>(cout, " "));
            cout << endl;
            break;
        case 2:
            cout << "\nÎòñîðòèðîâàííûé âåêòîð: " << sv << endl;
            break;
        case 3:
            cout << "\nÂâåäèòå ÷èñëî äëÿ ïðîâåðêè äåëèìîñòè: ";
            cin >> divider;
            count = sv.count_divider(divider);
            cout << "Êîëè÷åñòâî ýëåìåíòîâ, äåëÿùèõñÿ íà " << divider << ": " << count << endl;
            break;
        case 4:
            cout << "\nÂâåäèòå ÷èñëî, êîòîðîå âñòàâèòü: ";
            cin >> num;
            sv.insert_num(num);
            cout << "Âåêòîð ïîñëå âñòàâêè: " << sv << endl;
            break;
        case 5:
            cout << "\nÏåðåñòàíîâêè:" << endl;
            sv.permutations();
            break;
        case 0:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Îøèáêà: íåâåðíûé âûáîð, ïîïðîáóéòå åùå ðàç." << endl;
        }
    } while (choice != 0);

	return 0;
}
