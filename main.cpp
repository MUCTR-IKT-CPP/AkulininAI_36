#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

/**
 * Инициализирует генератор случайных чисел, устанавливая начальное значение на основе текущего времени.
 */
void initializeRandom() {
    srand(static_cast<unsigned int>(time(0)));
}

/**
 * Генерирует случайную строку заданной длины.
 *
 * @param length Длина случайной строки.
 * @return Сгенерированная случайная строка.
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
 * Подсчитывает количество каждой буквы в строке.
 *
 * @param str Строка, в которой нужно подсчитать буквы.
 * @return Вектор, содержащий количество каждой буквы от 'a' до 'z'.
 */
vector<int> countLetters(const string& str) {
    vector<int> count(26, 0);

    for (char c : str) {
        count[c - 'a']++;
    }
    return count;
}

/**
 * Выводит количество каждой буквы в алфавите.
 *
 * @param count Вектор с количеством букв.
 */
void printLetterCount(const vector<int>& count) {
    cout << "The number of each letter in a line:\n";
    for (char c = 'a'; c <= 'z'; c++) {
        cout << c << ": " << count[c - 'a'] << endl;
    }
}

/**
 * Главная функция программы.
 *
 * Генерирует случайную строку, подсчитывает количество каждой буквы и выводит результат.
 */
int main() {
    const int N = 10000;

    initializeRandom();

    string random_string = generateRandomString(N);

    vector<int> count = countLetters(random_string);

    printLetterCount(count);

    return 0;
}
