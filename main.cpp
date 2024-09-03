#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

/**
 * �������������� ��������� ��������� �����, ������������ ��������� �������� �� ������ �������� �������.
 */
void initializeRandom() {
    srand(static_cast<unsigned int>(time(0)));
}

/**
 * ���������� ��������� ������ �������� �����.
 *
 * @param length ����� ��������� ������.
 * @return ��������������� ��������� ������.
 */
string generateRandomString(int length) {
    string random_string;
    for (int i = 0; i < length; i++) {
        char random_char = 'a' + rand() % 26;
        random_string += random_char;
    }
    return random_string;
}

/**
 * ������������ ���������� ������ ����� � ������.
 *
 * @param str ������, � ������� ����� ���������� �����.
 * @return ������, ���������� ���������� ������ ����� �� 'a' �� 'z'.
 */
vector<int> countLetters(const string& str) {
    vector<int> count(26, 0);

    for (char c : str) {
        count[c - 'a']++;
    }
    return count;
}

/**
 * ������� ���������� ������ ����� � ��������.
 *
 * @param count ������ � ����������� ����.
 */
void printLetterCount(const vector<int>& count) {
    cout << "The number of each letter in a line:\n";
    for (char c = 'a'; c <= 'z'; c++) {
        cout << c << ": " << count[c - 'a'] << endl;
    }
}

/**
 * ������� ������� ���������.
 *
 * ���������� ��������� ������, ������������ ���������� ������ ����� � ������� ���������.
 */
int main() {
    const int N = 10000;

    initializeRandom();

    string random_string = generateRandomString(N);

    vector<int> count = countLetters(random_string);

    printLetterCount(count);

    return 0;
}
