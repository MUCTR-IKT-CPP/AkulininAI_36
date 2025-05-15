#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <map>
#include <utility>
#include <cmath>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace std::chrono;


class SipHash {
private:
    uint64_t v0, v1, v2, v3;
    uint64_t key[2];
    
    void sip_round() {
        v0 += v1;
        v1 = (v1 << 13) | (v1 >> 51);
        v1 ^= v0;
        v0 = (v0 << 32) | (v0 >> 32);
        
        v2 += v3;
        v3 = (v3 << 16) | (v3 >> 48);
        v3 ^= v2;
        
        v0 += v3;
        v3 = (v3 << 21) | (v3 >> 43);
        v3 ^= v0;
        
        v2 += v1;
        v1 = (v1 << 17) | (v1 >> 47);
        v1 ^= v2;
        v2 = (v2 << 32) | (v2 >> 32);
    }
    
public:
    SipHash(uint64_t k0 = 0x0706050403020100, uint64_t k1 = 0x0F0E0D0C0B0A0908) {
        key[0] = k0;
        key[1] = k1;
    }
    
    uint64_t hash(const string& message) {
        uint64_t m;
        size_t len = message.size();
        
        // Initialization
        v0 = key[0] ^ 0x736F6D6570736575;
        v1 = key[1] ^ 0x646F72616E646F6D;
        v2 = key[0] ^ 0x6C7967656E657261;
        v3 = key[1] ^ 0x7465646279746573;
        
        // Process message blocks
        size_t offset = 0;
        for(; offset + 8 <= len; offset += 8) {
            m = 0;
            for(int i = 0; i < 8; i++) {
                m |= (uint64_t)((uint8_t)message[offset + i]) << (8 * i);
            }
            
            v3 ^= m;
            for(int i = 0; i < 2; i++) sip_round();
            v0 ^= m;
        }
        
        // Process last block and length
        m = ((uint64_t)len) << 56;
        int remaining = len - offset;
        for(int i = 0; i < remaining; i++) {
            m |= (uint64_t)((uint8_t)message[offset + i]) << (8 * i);
        }
        
        v3 ^= m;
        for(int i = 0; i < 2; i++) sip_round();
        v0 ^= m;
        
        // Finalization
        v2 ^= 0xFF;
        for(int i = 0; i < 4; i++) sip_round();
        
        return v0 ^ v1 ^ v2 ^ v3;
    }
};

string generate_random_string(size_t length) {
    static const string chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    static mt19937 rng(random_device{}());
    static uniform_int_distribution<size_t> dist(0, chars.size() - 1);
    
    string result;
    result.reserve(length);
    
    for (size_t i = 0; i < length; ++i) {
        result += chars[dist(rng)];
    }
    
    return result;
}

pair<string, string> create_string_pair(const string& base, int differences) {
    string modified = base;
    mt19937 rng(random_device{}());
    uniform_int_distribution<size_t> dist(0, base.size() - 1);
    
    for (int i = 0; i < differences; ++i) {
        size_t pos = dist(rng);
        modified[pos] = (modified[pos] == 'a') ? 'b' : 'a';
    }
    
    return {base, modified};
}

size_t find_max_common_subsequence(uint64_t hash1, uint64_t hash2) {
    size_t max_len = 0;
    size_t current_len = 0;
    
    for (int i = 0; i < 64; ++i) {
        bool bit1 = (hash1 >> i) & 1;
        bool bit2 = (hash2 >> i) & 1;
        
        if (bit1 == bit2) {
            current_len++;
            max_len = max(max_len, current_len);
        } else {
            current_len = 0;
        }
    }
    
    return max_len;
}

void write_results_to_csv(const map<int, vector<size_t>>& results, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }
    
    // Заголовок CSV
    out << "differences,max_common_length\n";
    
    // Запись данных
    for (const auto& [diff, lengths] : results) {
        for (size_t len : lengths) {
            out << diff << "," << len << "\n";
        }
    }
    
    out.close();
    cout << "Data written to " << filename << endl;
}

void test1() {
    const size_t string_length = 128;
    const int num_pairs = 1000;
    const vector<int> differences = {1, 2, 4, 8, 16};
    
    SipHash hasher;
    map<int, vector<size_t>> results;
    
    for (int diff : differences) {
        for (int i = 0; i < num_pairs; ++i) {
            string base = generate_random_string(string_length);
            auto pair = create_string_pair(base, diff);
            
            uint64_t hash1 = hasher.hash(pair.first);
            uint64_t hash2 = hasher.hash(pair.second);
            
            size_t max_common = find_max_common_subsequence(hash1, hash2);
            results[diff].push_back(max_common);
        }
    }
    
    // Запись результатов в CSV
    write_results_to_csv(results, "hash_test_results.csv");
    
    // Вывод статистики
    for (const auto& [diff, lengths] : results) {
        double avg = accumulate(lengths.begin(), lengths.end(), 0.0) / lengths.size();
        cout << "Differences: " << diff 
             << ", Avg max common subsequence: " << avg 
             << ", Max observed: " << *max_element(lengths.begin(), lengths.end())
             << endl;
    }
}

void test_collisions() {
    const size_t string_length = 256;
    const vector<int> exponents = {2, 3, 4, 5, 6};
    SipHash hasher;
    ofstream out("collision_test_results.csv");
    
    out << "N,collisions_count\n";
    
    for (int exp : exponents) {
        size_t N = pow(10, exp);
        map<uint64_t, int> hash_counts;
        int collisions = 0;
        
        auto start = high_resolution_clock::now();
        
        for (size_t i = 0; i < N; ++i) {
            string s = generate_random_string(string_length);
            uint64_t h = hasher.hash(s);
            
            if (hash_counts.find(h) != hash_counts.end()) {
                collisions++;
            }
            hash_counts[h]++;
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        
        out << N << "," << collisions << "\n";
        
        cout << "N = 10^" << exp << " (" << N << "): " 
             << collisions << " collisions found. "
             << "Time: " << duration.count() << " ms\n";
    }
    
    out.close();
}

void test_hash_speed() {
    const vector<size_t> lengths = {64, 128, 256, 512, 1024, 2048, 4096, 8192};
    const int iterations = 1000;
    SipHash hasher;
    ofstream out("hash_speed_results.csv");
    
    out << "length,avg_time_ns\n";
    
    cout << "Hash Speed Test (1000 iterations each):\n";
    cout << setw(8) << "Length" << setw(15) << "Avg Time (ns)" << endl;
    cout << "---------------------------------\n";
    
    for (size_t len : lengths) {
        vector<long> durations;
        durations.reserve(iterations);
        
        for (int i = 0; i < iterations; ++i) {
            string s = generate_random_string(len);
            
            auto start = high_resolution_clock::now();
            hasher.hash(s);
            auto end = high_resolution_clock::now();
            
            auto duration = duration_cast<nanoseconds>(end - start);
            durations.push_back(duration.count());
        }
        
        long avg_time = accumulate(durations.begin(), durations.end(), 0L) / iterations;
        out << len << "," << avg_time << "\n";
        
        cout << setw(8) << len << setw(15) << avg_time << endl;
    }
    
    out.close();
}

int main() {
    test1();
    test_collisions();
    test_hash_speed();
    return 0;
}