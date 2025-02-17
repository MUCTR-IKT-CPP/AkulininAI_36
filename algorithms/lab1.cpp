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

pair<long long, int> bubble_sort(vector<double>& array) {
    long long count_swap = 0;
    int repeat_passes = 0;
    bool swapped;
    do {
        swapped = false;
        for (int i = 0; i < array.size() - 1; i++) {
            if (array[i] > array[i + 1]) {
                swap(array[i], array[i + 1]);
                swapped = true;
                count_swap++;
            }
        }
        repeat_passes++;
    } while (swapped);

    return {count_swap, repeat_passes};
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
    vector<vector<double>> exchanges;
    vector<vector<double>> repeats;
    vector<double> sizes;

    high_resolution_clock::time_point time_all_start = high_resolution_clock::now();
    for (int i = 1000; i <= 128000; i *= 2) {
        cout << "==================\nCount elements " << i << "\n==================\n";
        high_resolution_clock::time_point time_streak_start = high_resolution_clock::now();
        
        vector<double> current_times;
        vector<double> current_exchanges;
        vector<double> current_repeats;

        for (int j = 0; j < 20; j++) {
            auto random_array = generateRandomArray(i);
            high_resolution_clock::time_point time_attempt_start = high_resolution_clock::now();
            
            pair<long long, int> result = bubble_sort(random_array);
            long long count_swap = result.first;
            int repeat_passes = result.second;

            high_resolution_clock::time_point time_attempt_end = high_resolution_clock::now();
            duration<double> time_attempt = time_attempt_end - time_attempt_start;

            current_times.push_back(time_attempt.count());
            current_exchanges.push_back(count_swap);
            current_repeats.push_back(repeat_passes);

            cout << "Repeat passes: " << repeat_passes << "; Exchange operations: " << count_swap << "; Time attempt: " << time_attempt.count() << " sec." << endl;
        }

        times.push_back(current_times);
        exchanges.push_back(current_exchanges);
        repeats.push_back(current_repeats);
        sizes.push_back(i);

        high_resolution_clock::time_point time_streak_end = high_resolution_clock::now();
        duration<double> time_streak = time_streak_end - time_streak_start;
        cout << "-------------------------------------\nTime streak: " << time_streak.count() << " sec.\n-----------------------------------\n";
    }

    high_resolution_clock::time_point time_all_end = high_resolution_clock::now();
    duration<double> time_all = time_all_end - time_all_start;
    
    cout << "-------------------------------------\nTime all: " << time_all.count() << " sec.";

    saveDataToFile("times.csv", times);
    saveDataToFile("exchanges.csv", exchanges);
    saveDataToFile("repeats.csv", repeats);
    saveDataToFile("sizes.csv", {sizes});

    return 0;
}