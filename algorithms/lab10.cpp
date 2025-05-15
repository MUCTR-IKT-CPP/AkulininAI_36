#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <iomanip>
#include <ctime>
#include <chrono>

using namespace std;

vector<string> generateAllCombinations() {
    vector<string> combs;
    for (int i = 0; i < 8; ++i) {
        string s;
        for (int j = 2; j >= 0; --j) {
            s += ((i >> j) & 1) ? '1' : '0';
        }
        combs.push_back(s);
    }
    return combs;
    
}

string simulateGame(const string& A, const string& B, mt19937& rng) {
    uniform_int_distribution<int> dist(0, 1);
    string history;

    while (true) {
        history += (dist(rng) == 0) ? '0' : '1';

        if (history.size() < 3) {
            continue;
        }
        string last3 = history.substr(history.size() - 3, 3);

        if (last3 == A) return A;
        if (last3 == B) return B;
    }
}


double calculateWinProbability(const string& A, const string& B, int simulations, mt19937& base_rng) {
    int winsA = 0;
    for (int i = 0; i < simulations; ++i) {
        mt19937 rng(base_rng());
        string winner = simulateGame(A, B, rng);
        if (winner == A) winsA++;
    }
    return static_cast<double>(winsA) / simulations;
}

int main() {
    vector<string> combs = generateAllCombinations();
    const int SIMULATIONS = 10000;

    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 base_rng(static_cast<unsigned int>(seed));


    unordered_map<string, double> totalWinsA;
    unordered_map<string, double> totalWinsB;

    cout << "A\\B";
    for (const string& a : combs) cout << "\t" << a;
    cout << endl;

    for (const string& a : combs) {
        cout << a;
        for (const string& b : combs) {
            if (a == b) {
                cout << "\t" << fixed << setprecision(2) << 0;
            }
            else { 
                double probA = calculateWinProbability(a, b, SIMULATIONS, base_rng);
                double probB = 1.0 - probA;
                cout << "\t" << fixed << setprecision(2) << probA;
                totalWinsA[a] += probA;
                totalWinsB[b] += probB;
            }
        }
        cout << endl;
    }

    double avgA = 0, avgB = 0;
    for (const auto& pair : totalWinsA) avgA += pair.second;
    for (const auto& pair : totalWinsB) avgB += pair.second;

    int totalPairs = avgA + avgB;
    avgA /= totalPairs;
    avgB /= totalPairs;

    cout << "\nAverage win chance:\n";
    cout << "Player A: " << fixed << setprecision(2) << avgA * 100 << "%\n";
    cout << "Player B: " << fixed << setprecision(2) << avgB * 100 << "%\n";

    return 0;
}