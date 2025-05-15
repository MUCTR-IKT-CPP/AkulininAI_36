#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

struct Node {
    int data;
    Node* left;
    Node* right;
    int height;
    
    Node(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

class BST {
private:
    Node* root;

    void clear(Node* node) {
        if (!node) return;
        
        stack<Node*> nodes;
        nodes.push(node);
        
        while (!nodes.empty()) {
            Node* current = nodes.top();
            nodes.pop();
            
            if (current->left) nodes.push(current->left);
            if (current->right) nodes.push(current->right);
            
            delete current;
        }
    }

public:
    BST() : root(nullptr) {}
    ~BST() {
        clear(root);
        root = nullptr;
    }

    void insert(int value) {
        if (!root) {
            root = new Node(value);
            return;
        }

        Node* current = root;
        while (true) {
            if (value < current->data) {
                if (!current->left) {
                    current->left = new Node(value);
                    break;
                }
                current = current->left;
            }
            else if (value > current->data) {
                if (!current->right) {
                    current->right = new Node(value);
                    break;
                }
                current = current->right;
            }
            else {
                break;
            }
        }
    }

    bool search(int value) const {
        Node* current = root;
        while (current) {
            if (value == current->data) {
                return true;
            }
            else if (value < current->data) {
                current = current->left;
            } 
            else {
                current = current->right;
            }
        }
        return false;
    }

    void remove(int value) {
        Node* parent = nullptr;
        Node* current = root;

        while (current && current->data != value) {
            parent = current;
            if (value < current->data) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        if (!current) return;

        if (!current->left || !current->right) {
            Node* new_child = current->left ? current->left : current->right;
            
            if (!parent) {
                root = new_child;
            }
            else if (parent->left == current) {
                parent->left = new_child;
            }
            else {
                parent->right = new_child;
            }
            delete current;
        }
        else {
            Node* parent2 = current;
            Node* current2 = current->right;

            while (current2->left) {
                parent2 = current2;
                current2 = current2->left;
            }

            current->data = current2->data;

            if (parent2->left == current2) {
                parent2->left = current2->right;
            }
            else {
                parent2->right = current2->right;
            }

            delete current2;
        }
    }

    int find_min() const {
        if (!root) return -1;
        
        Node* current = root;
        while (current->left) {
            current = current->left;
        }
        return current->data;
    }
};

class AVL {
private:
    Node* root;

    int height(Node* node) {
        return node ? node->height : 0;
    }

    void update_height(Node* node) {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    int balance_factor(Node* node) {
        return height(node->right) - height(node->left);
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        update_height(y);
        update_height(x);
        return x;
    }
    
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        update_height(x);
        update_height(y);
        return y;
    }

    Node* balance(Node* node) {
        update_height(node);
        int bf = balance_factor(node);
        
        if (bf > 1) {
            if (balance_factor(node->right) < 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        if (bf < -1) {
            if (balance_factor(node->left) > 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        return node;
    }

    void clear(Node* node) {
        if (!node) return;
    
        stack<Node*> nodes;
        nodes.push(node);
        
        while (!nodes.empty()) {
            Node* current = nodes.top();
            nodes.pop();
            
            if (current->left) nodes.push(current->left);
            if (current->right) nodes.push(current->right);
            
            delete current;
        }
    }

public:
    AVL() : root(nullptr) {}
    ~AVL() {
        clear(root);
        root = nullptr;
    }

    void insert(int val) {
        if (!root) {
            root = new Node(val);
            return;
        }

        stack<Node*> path;
        Node* current = root;

        while (current) {
            path.push(current);
            if (val < current->data) {
                current = current->left;
            }
            else if (val > current->data) {
                current = current->right;
            }
            else {
                return;
            }
        }

        Node* parent = path.top();
        if (val < parent->data) {
            parent->left = new Node(val);
        }
        else {
            parent->right = new Node(val);
        }
    
        while (!path.empty()) {
            Node* node = path.top();
            path.pop();

            if (!path.empty()) {
                Node* parent = path.top();
                if (parent->left == node) {
                    parent->left = balance(node);
                }
                else {
                    parent->right = balance(node);
                }
            }
            else {
                root = balance(node);
            }
        }
    }

    bool search(int val) {
        Node* current = root;
        while (current) {
            if (val == current->data) {
                return true;
            }
            else if (val < current->data) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return false;
    }

    void remove(int val) {
        stack<Node*> path;
        Node* current = root;
        Node* parent = nullptr;

        while (current && current->data != val) {
            parent = current;
            path.push(parent);
            if (val < current->data) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        if (!current) return;

        if (!current->left || !current->right) {
            Node* new_child = current->left ? current->left : current->right;
            
            if (!parent) {
                root = new_child;
            }
            else if (parent->left == current) {
                parent->left = new_child;
            }
            else {
                parent->right = new_child;
            }
            delete current;
        }
        else {
            path.push(current);
            Node* successor = current->right;
            while (successor->left) {
                path.push(successor);
                successor = successor->left;
            }
            
            current->data = successor->data;
            
            if (path.top()->left == successor) {
                path.top()->left = successor->right;
            }
            else {
                path.top()->right = successor->right;
            }
            delete successor;
        }

        while (!path.empty()) {
            Node* node = path.top();
            path.pop();

            if (!path.empty()) {
                Node* parent = path.top();
                if (parent->left == node) {
                    parent->left = balance(node);
                }
                else {
                    parent->right = balance(node);
                }
            }
            else {
                root = balance(node);
            }
        }
    }

    int find_min() const {
        if (!root) return -1;
        
        Node* current = root;
        while (current->left) {
            current = current->left;
        }
        return current->data;
    }
};

void run_tests() {
    ofstream out("results.csv");
    out << "Test Series,Data Size,Data Type,BST Insert Time,AVL Insert Time,Array Search Time,BST Search Time,AVL Search Time,BST Remove Time,AVL Remove Time\n";

    random_device rd;
    mt19937 gen(rd());
    
    for (int i = 1; i <= 5; ++i) {
        int data_size = 1 << (10 + i);
        uniform_int_distribution<> dist(1, data_size * 10);
        
        // Test with random data
        for (int cycle = 1; cycle <= 20; ++cycle) {
            bool sorted = (cycle > 10);
            
            vector<int> data(data_size);
            for (int j = 0; j < data_size; ++j) {
                data[j] = sorted ? j : dist(gen);
            }
            if (sorted) {
                sort(data.begin(), data.end());
            }
            
            BST bst;
            AVL avl;
            
            // Insertion test
            auto start = high_resolution_clock::now();
            for (int val : data) {
                bst.insert(val);
            }
            auto end = high_resolution_clock::now();
            double bst_insert_time = duration_cast<microseconds>(end - start).count() / 1e6;
            
            start = high_resolution_clock::now();
            for (int val : data) {
                avl.insert(val);
            }
            end = high_resolution_clock::now();
            double avl_insert_time = duration_cast<microseconds>(end - start).count() / 1e6;
            
            // Search test (1000 operations)
            start = high_resolution_clock::now();
            for (int j = 0; j < 1000; ++j) {
                int val = dist(gen);
                auto it = find(data.begin(), data.end(), val);
            }
            end = high_resolution_clock::now();
            double array_search_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            
            start = high_resolution_clock::now();
            for (int j = 0; j < 1000; ++j) {
                int val = dist(gen);
                bst.search(val);
            }
            end = high_resolution_clock::now();
            double bst_search_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            
            start = high_resolution_clock::now();
            for (int j = 0; j < 1000; ++j) {
                int val = dist(gen);
                avl.search(val);
            }
            end = high_resolution_clock::now();
            double avl_search_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            
            // Remove test (1000 operations)
            vector<int> to_remove;
            for (int j = 0; j < 1000; ++j) {
                to_remove.push_back(data[dist(gen) % data_size]);
            }
            
            start = high_resolution_clock::now();
            for (int val : to_remove) {
                bst.remove(val);
            }
            end = high_resolution_clock::now();
            double bst_remove_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            
            start = high_resolution_clock::now();
            for (int val : to_remove) {
                avl.remove(val);
            }
            end = high_resolution_clock::now();
            double avl_remove_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            
            cout << i << "," << data_size << "," << (sorted ? "Sorted" : "Random") << ","
                << bst_insert_time << "," << avl_insert_time << ","
                << array_search_time << "," << bst_search_time << "," << avl_search_time << ","
                << bst_remove_time << "," << avl_remove_time << "\n";

            out << i << "," << data_size << "," << (sorted ? "Sorted" : "Random") << ","
                << bst_insert_time << "," << avl_insert_time << ","
                << array_search_time << "," << bst_search_time << "," << avl_search_time << ","
                << bst_remove_time << "," << avl_remove_time << "\n";
        }
        cout << "\n=====================================================\n";
    }
    
    out.close();
    cout << "Testing completed. Results saved to results.csv" << endl;
}

int main() {
    run_tests();
    return 0;
}