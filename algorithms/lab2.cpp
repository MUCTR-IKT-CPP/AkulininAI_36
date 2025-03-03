#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

vector<double> generateRandomArray(int size) {
    vector<double> array(size);
    mt19937 engine(static_cast<unsigned int>(time(0)));
    uniform_real_distribution<double> gen(-1.0, 1.0);
    for (auto& elem : array) {
        elem = gen(engine);
    }
    return array;
}

void makeHeap(vector<double>& array, int n, int i, int& func_calls, int& in_func_calls, int current_depth, int& max_depth) {
    func_calls++;
    if (current_depth > max_depth) {
        max_depth = current_depth;
    }

    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if ((l < n) && (array[l] > array[largest])) {
        largest = l;
    }
    if ((r < n) && (array[r] > array[largest])) {
        largest = r;
    }
    if (largest != i) {
        swap(array[i], array[largest]);
        in_func_calls++;
        makeHeap(array, n, largest, func_calls, in_func_calls, current_depth + 1, max_depth);
    }
}

void heapSort(vector<double>& array, int n, int& func_calls, int& in_func_calls, int& max_depth) {
    max_depth = 0;
    for (int i = n / 2 - 1; i >= 0; i--) {
        makeHeap(array, n, i, func_calls, in_func_calls, 1, max_depth);
    }

    for (int i = n - 1; i >= 0; i--) {
        swap(array[0], array[i]);
        makeHeap(array, i, 0, func_calls, in_func_calls, 1, max_depth);
    }
}

void saveDataToFile(const string& filename, const vector<vector<double>>& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    for (const auto& row : data) {
        for (int i = 0; i < row.size(); i++) {
            file << row[i];
            if (i != row.size() - 1) {
                file << ", ";
            }
        }
        file << endl;
    }

    file.close();
}

int main() {
    vector<vector<double>> times;
    vector<vector<double>> v_func_calls;
    vector<vector<double>> v_in_func_calls;
    vector<vector<double>> v_max_depths; // Вектор для хранения максимальных глубин рекурсии
    vector<double> sizes;

    high_resolution_clock::time_point time_all_start = high_resolution_clock::now();
    for (int i = 1000; i <= 128000; i *= 2) {
        cout << "==================\nCount elements " << i << "\n==================\n";
        high_resolution_clock::time_point time_streak_start = high_resolution_clock::now();
        
        vector<double> current_times;
        vector<double> current_func_calls;
        vector<double> current_in_func_calls;
        vector<double> current_max_depths; // Вектор для текущих максимальных глубин рекурсии

        for (int j = 0; j < 20; j++) {
            auto random_array = generateRandomArray(i);
            high_resolution_clock::time_point time_attempt_start = high_resolution_clock::now();
            
            int func_calls = 0;
            int in_func_calls = 0;
            int max_depth = 0; // Инициализируем максимальную глубину рекурсии
            
            heapSort(random_array, i, func_calls, in_func_calls, max_depth);

            high_resolution_clock::time_point time_attempt_end = high_resolution_clock::now();
            duration<double, milli> time_attempt = time_attempt_end - time_attempt_start;

            current_times.push_back(time_attempt.count());
            current_func_calls.push_back(func_calls);
            current_in_func_calls.push_back(in_func_calls);
            current_max_depths.push_back(max_depth); // Сохраняем максимальную глубину рекурсии

            cout << "Number of function calls: " << func_calls << "; Number of calls to the internal function: " << in_func_calls << "; Max recursion depth: " << max_depth << "; Time attempt: " << time_attempt.count() << " milli sec." << endl;
        }

        times.push_back(current_times);
        v_func_calls.push_back(current_func_calls);
        v_in_func_calls.push_back(current_in_func_calls);
        v_max_depths.push_back(current_max_depths); // Сохраняем максимальные глубины рекурсии для текущего размера массива
        sizes.push_back(i);

        high_resolution_clock::time_point time_streak_end = high_resolution_clock::now();
        duration<double> time_streak = time_streak_end - time_streak_start;
        cout << "-------------------------------------\nTime streak: " << time_streak.count() << " sec.\n-----------------------------------\n";
    }

    high_resolution_clock::time_point time_all_end = high_resolution_clock::now();
    duration<double> time_all = time_all_end - time_all_start;
    
    cout << "-------------------------------------\nTime all: " << time_all.count() << " sec.";

    saveDataToFile("times.csv", times);
    saveDataToFile("funcCalls.csv", v_func_calls);
    saveDataToFile("inFuncCalls.csv", v_in_func_calls);
    saveDataToFile("maxDepths.csv", v_max_depths); // Сохраняем максимальные глубины рекурсии в файл
    saveDataToFile("sizes.csv", {sizes});

    return 0;
}