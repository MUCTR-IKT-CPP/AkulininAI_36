#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <numeric>

using namespace std;
using namespace std::chrono;

// ===================== Бинарная куча =====================
class BinaryHeap {
private:
    vector<int> heap;

    void siftUp(int index) {
        while (index > 0 && heap[(index - 1) / 2] > heap[index]) {
            swap(heap[(index - 1) / 2], heap[index]);
            index = (index - 1) / 2;
        }
    }

    void siftDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < heap.size() && heap[left] < heap[smallest])
                smallest = left;
            if (right < heap.size() && heap[right] < heap[smallest])
                smallest = right;
            if (smallest == index) break;

            swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }

public:
    void insert(int value) {
        heap.push_back(value);
        siftUp(heap.size() - 1);
    }

    int getMin() {
        if (heap.empty()) throw runtime_error("Heap is empty");
        return heap[0];
    }

    int extractMin() {
        if (heap.empty()) throw runtime_error("Heap is empty");
        
        int min = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) siftDown(0);
        return min;
    }

    bool isEmpty() const { return heap.empty(); }
    int size() const { return heap.size(); }
};

// ===================== Биномиальная куча =====================
class BinomialHeap {
private:
    struct Node {
        int value;
        vector<Node*> children;
        Node(int v) : value(v) {}
    };

    vector<Node*> trees;

    Node* mergeTrees(Node* a, Node* b) {
        if (a->value > b->value) swap(a, b);
        a->children.push_back(b);
        return a;
    }

    void mergeHeaps(vector<Node*>& h1, vector<Node*>& h2) {
        vector<Node*> result;
        int i = 0, j = 0;
        
        while (i < h1.size() && j < h2.size()) {
            if (h1[i]->children.size() < h2[j]->children.size())
                result.push_back(h1[i++]);
            else
                result.push_back(h2[j++]);
        }
        while (i < h1.size()) result.push_back(h1[i++]);
        while (j < h2.size()) result.push_back(h2[j++]);

        for (int k = 0; k + 1 < result.size(); ) {
            if (result[k]->children.size() == result[k+1]->children.size()) {
                result[k] = mergeTrees(result[k], result[k+1]);
                result.erase(result.begin() + k + 1);
            } else {
                k++;
            }
        }
        trees = result;
    }

public:
    void insert(int value) {
        vector<Node*> temp = {new Node(value)};
        mergeHeaps(trees, temp);
    }

    int getMin() {
        if (trees.empty()) throw runtime_error("Heap is empty");
        
        auto min_it = min_element(trees.begin(), trees.end(),
            [](Node* a, Node* b) { return a->value < b->value; });
        
        return (*min_it)->value;
    }

    int extractMin() {
        if (trees.empty()) throw runtime_error("Heap is empty");
        
        auto min_it = min_element(trees.begin(), trees.end(),
            [](Node* a, Node* b) { return a->value < b->value; });
        
        Node* minTree = *min_it;
        int minValue = minTree->value;
        trees.erase(min_it);

        vector<Node*> childHeap = minTree->children;
        delete minTree;
        mergeHeaps(trees, childHeap);
        
        return minValue;
    }

    bool isEmpty() const { return trees.empty(); }
};

// ===================== Тестирование =====================
void test(const string& filename) {
    ofstream out(filename);
    out << "HeapType,Size,FillTime(us),FindAvg(us),FindMax(us),"
        << "ExtractAvg(us),ExtractMax(us),InsertAvg(us),InsertMax(us)\n";

    auto test_operations = [](auto& heap, int ops_count) {
        // Тест поиска минимума
        vector<double> find_times;
        for (int i = 0; i < ops_count; ++i) {
            auto start = high_resolution_clock::now();
            heap.getMin();
            auto duration = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count();
            find_times.push_back(duration);
        }
        double find_avg = accumulate(find_times.begin(), find_times.end(), 0.0) / ops_count / 1000.0;
        double find_max = *max_element(find_times.begin(), find_times.end()) / 1000.0;

        // Тест извлечения минимума
        vector<double> extract_times;
        for (int i = 0; i < ops_count; ++i) {
            auto start = high_resolution_clock::now();
            int val = heap.extractMin();
            heap.insert(val); // Возвращаем элемент обратно
            auto duration = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count();
            extract_times.push_back(duration);
        }
        double extract_avg = accumulate(extract_times.begin(), extract_times.end(), 0.0) / ops_count / 1000.0;
        double extract_max = *max_element(extract_times.begin(), extract_times.end()) / 1000.0;

        // Тест вставки (вставляем последовательные числа)
        vector<double> insert_times;
        int next_val = heap.size(); // Продолжаем последовательность
        for (int i = 0; i < ops_count; ++i) {
            auto start = high_resolution_clock::now();
            heap.insert(next_val++);
            auto duration = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count();
            insert_times.push_back(duration);
        }
        double insert_avg = accumulate(insert_times.begin(), insert_times.end(), 0.0) / ops_count / 1000.0;
        double insert_max = *max_element(insert_times.begin(), insert_times.end()) / 1000.0;

        return make_tuple(find_avg, find_max, extract_avg, extract_max, insert_avg, insert_max);
    };

    for (int size : {1000, 10000, 100000, 1000000, 10000000}) {
        // Тестирование BinaryHeap
        {
            BinaryHeap heap;
            auto fill_start = high_resolution_clock::now();
            // Заполняем отсортированными значениями 0..size-1
            for (int i = 0; i < size; ++i) {
                heap.insert(i);
            }
            auto fill_time = duration_cast<microseconds>(high_resolution_clock::now() - fill_start).count();

            auto [find_avg, find_max, extract_avg, extract_max, insert_avg, insert_max] = test_operations(heap, 1000);

            out << "BinaryHeap," << size << "," << fill_time << ","
                << find_avg << "," << find_max << ","
                << extract_avg << "," << extract_max << ","
                << insert_avg << "," << insert_max << "\n";
        }

        // Тестирование BinomialHeap (только для размеров ≤ 1M)
        if (size <= 1000000) {
            BinomialHeap heap;
            auto fill_start = high_resolution_clock::now();
            // Заполняем отсортированными значениями 0..size-1
            for (int i = 0; i < size; ++i) {
                heap.insert(i);
            }
            auto fill_time = duration_cast<microseconds>(high_resolution_clock::now() - fill_start).count();

            auto [find_avg, find_max, extract_avg, extract_max, insert_avg, insert_max] = test_operations(heap, 1000);

            out << "BinomialHeap," << size << "," << fill_time << ","
                << find_avg << "," << find_max << ","
                << extract_avg << "," << extract_max << ","
                << insert_avg << "," << insert_max << "\n";
        }

        cout << "Completed tests for size: " << size << endl;
    }
}

int main() {
    test("heap_performance_sorted.csv");
    cout << "Results saved to heap_performance_sorted.csv" << endl;
    return 0;
}