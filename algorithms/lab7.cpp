#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <fstream>
#include <queue>
#include <cmath>
#include <functional>

using namespace std;
using namespace std::chrono;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node* parent;
    int height;
    int priority;
    
    Node(int val) : data(val), left(nullptr), right(nullptr), parent(nullptr), height(1), priority(rand()) {}
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
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    AVL() : root(nullptr) {}
    ~AVL() {
        clear(root);
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

    int getHeight() {
        return height(root);
    }

    void export_branch_heights(ofstream& out) const {
        function<void(Node*, int)> dfs = [&](Node* node, int depth) {
            if (!node) return;
            if (!node->left && !node->right) {
                out << depth << "\n";
            }
            dfs(node->left, depth + 1);
            dfs(node->right, depth + 1);
        };
        dfs(root, 1);
    }    
};

class Treap {
private:
    Node* root;

    void split(Node* node, int data, Node*& left, Node*& right) {
        if (!node) {
            left = right = nullptr;
            return;
        }

        if (node->data <= data) {
            split(node->right, data, node->right, right);
            left = node;
        }
        else {
            split(node->left, data, left, node->left);
            right = node;
        }
    }

    Node* merge(Node* left, Node* right) {
        if (!left) return right;
        if (!right) return left;

        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            return left;
        }
        else {
            right->left = merge(left, right->left);
            return right;
        }
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    int height(Node* node) {
        return node ? node->height : 0;
    }

    void update_height(Node* node) {
        node->height = 1 + max(height(node->left), height(node->right));
    }


public:
    Treap() : root(nullptr) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(root);
    }

    void insert(int data) {
        Node* new_node = new Node(data);
        Node *left, *right;
        split(root, data, left, right);
        root = merge(merge(left, new_node), right);
        update_height(root);
    }

    bool search(int data) const {
        Node* current = root;
        while (current) {
            if (data == current->data) {
                return true;
            }
            else if (data < current->data) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return false;
    }

    void remove(int data) {
        Node *left, *mid, *right;
        split(root, data - 1, left, mid);
        split(mid, data, mid, right);
        if (mid) delete mid;
        root = merge(left, right);
        if (root) update_height(root);
    }

    int getHeight() {
        return height(root);
    }

    void export_branch_heights(ofstream& out) const {
        function<void(Node*, int)> dfs = [&](Node* node, int depth) {
            if (!node) return;
            if (!node->left && !node->right) {
                out << depth << "\n";
            }
            dfs(node->left, depth + 1);
            dfs(node->right, depth + 1);
        };
        dfs(root, 1);
    }
    
};

struct Node_rbt {
    int NodeData;
    Node_rbt* parentNode;
    Node_rbt* leftNode;
    Node_rbt* rightNode;
    int NodeColor;  // 0 - black, 1 - red
    int height;
};

typedef Node_rbt* NodePtr;

class RBT {
private:
    NodePtr root;
    NodePtr TNULL;

    void initializeNULLNode(NodePtr node, NodePtr parent) {
        node->NodeData = 0;
        node->parentNode = parent;
        node->leftNode = nullptr;
        node->rightNode = nullptr;
        node->NodeColor = 0;  // Black
        node->height = 0;
    }

    void updateHeight(NodePtr node) {
        if (node == TNULL) return;
        int leftHeight = (node->leftNode != TNULL) ? node->leftNode->height : 0;
        int rightHeight = (node->rightNode != TNULL) ? node->rightNode->height : 0;
        node->height = 1 + std::max(leftHeight, rightHeight);
    }

    void fixInsert(NodePtr k) {
        NodePtr u;
        while (k->parentNode->NodeColor == 1) {
            if (k->parentNode == k->parentNode->parentNode->rightNode) {
                u = k->parentNode->parentNode->leftNode;
                if (u->NodeColor == 1) {
                    u->NodeColor = 0;
                    k->parentNode->NodeColor = 0;
                    k->parentNode->parentNode->NodeColor = 1;
                    k = k->parentNode->parentNode;
                } else {
                    if (k == k->parentNode->leftNode) {
                        k = k->parentNode;
                        rightRotate(k);
                    }
                    k->parentNode->NodeColor = 0;
                    k->parentNode->parentNode->NodeColor = 1;
                    leftRotate(k->parentNode->parentNode);
                }
            } else {
                u = k->parentNode->parentNode->rightNode;
                if (u->NodeColor == 1) {
                    u->NodeColor = 0;
                    k->parentNode->NodeColor = 0;
                    k->parentNode->parentNode->NodeColor = 1;
                    k = k->parentNode->parentNode;
                } else {
                    if (k == k->parentNode->rightNode) {
                        k = k->parentNode;
                        leftRotate(k);
                    }
                    k->parentNode->NodeColor = 0;
                    k->parentNode->parentNode->NodeColor = 1;
                    rightRotate(k->parentNode->parentNode);
                }
            }
            if (k == root) break;
        }
        root->NodeColor = 0;
    }

    void leftRotate(NodePtr x) {
        NodePtr y = x->rightNode;
        x->rightNode = y->leftNode;
        if (y->leftNode != TNULL) {
            y->leftNode->parentNode = x;
        }
        y->parentNode = x->parentNode;
        if (x->parentNode == nullptr) {
            root = y;
        } else if (x == x->parentNode->leftNode) {
            x->parentNode->leftNode = y;
        } else {
            x->parentNode->rightNode = y;
        }
        y->leftNode = x;
        x->parentNode = y;
        
        updateHeight(x);
        updateHeight(y);
    }

    void rightRotate(NodePtr x) {
        NodePtr y = x->leftNode;
        x->leftNode = y->rightNode;
        if (y->rightNode != TNULL) {
            y->rightNode->parentNode = x;
        }
        y->parentNode = x->parentNode;
        if (x->parentNode == nullptr) {
            root = y;
        } else if (x == x->parentNode->rightNode) {
            x->parentNode->rightNode = y;
        } else {
            x->parentNode->leftNode = y;
        }
        y->rightNode = x;
        x->parentNode = y;
        
        updateHeight(x);
        updateHeight(y);
    }

    void transplant(NodePtr u, NodePtr v) {
        if (u->parentNode == nullptr) {
            root = v;
        } else if (u == u->parentNode->leftNode) {
            u->parentNode->leftNode = v;
        } else {
            u->parentNode->rightNode = v;
        }
        v->parentNode = u->parentNode;
    }

    void fixDelete(NodePtr x) {
        NodePtr s;
        while (x != root && x->NodeColor == 0) {
            if (x == x->parentNode->leftNode) {
                s = x->parentNode->rightNode;
                if (s->NodeColor == 1) {
                    s->NodeColor = 0;
                    x->parentNode->NodeColor = 1;
                    leftRotate(x->parentNode);
                    s = x->parentNode->rightNode;
                }

                if (s->leftNode->NodeColor == 0 && s->rightNode->NodeColor == 0) {
                    s->NodeColor = 1;
                    x = x->parentNode;
                } else {
                    if (s->rightNode->NodeColor == 0) {
                        s->leftNode->NodeColor = 0;
                        s->NodeColor = 1;
                        rightRotate(s);
                        s = x->parentNode->rightNode;
                    }
                    s->NodeColor = x->parentNode->NodeColor;
                    x->parentNode->NodeColor = 0;
                    s->rightNode->NodeColor = 0;
                    leftRotate(x->parentNode);
                    x = root;
                }
            } else {
                s = x->parentNode->leftNode;
                if (s->NodeColor == 1) {
                    s->NodeColor = 0;
                    x->parentNode->NodeColor = 1;
                    rightRotate(x->parentNode);
                    s = x->parentNode->leftNode;
                }

                if (s->rightNode->NodeColor == 0 && s->leftNode->NodeColor == 0) {
                    s->NodeColor = 1;
                    x = x->parentNode;
                } else {
                    if (s->leftNode->NodeColor == 0) {
                        s->rightNode->NodeColor = 0;
                        s->NodeColor = 1;
                        leftRotate(s);
                        s = x->parentNode->leftNode;
                    }
                    s->NodeColor = x->parentNode->NodeColor;
                    x->parentNode->NodeColor = 0;
                    s->leftNode->NodeColor = 0;
                    rightRotate(x->parentNode);
                    x = root;
                }
            }
        }
        x->NodeColor = 0;
    }

    void deleteNodeHelper(NodePtr node, int key) {
        NodePtr z = TNULL;
        NodePtr x, y;
        while (node != TNULL) {
            if (node->NodeData == key) {
                z = node;
            }
            node = (node->NodeData <= key) ? node->rightNode : node->leftNode;
        }

        if (z == TNULL) return;

        y = z;
        int y_original_color = y->NodeColor;
        if (z->leftNode == TNULL) {
            x = z->rightNode;
            transplant(z, z->rightNode);
        } else if (z->rightNode == TNULL) {
            x = z->leftNode;
            transplant(z, z->leftNode);
        } else {
            y = minimum(z->rightNode);
            y_original_color = y->NodeColor;
            x = y->rightNode;
            if (y->parentNode == z) {
                x->parentNode = y;
            } else {
                transplant(y, y->rightNode);
                y->rightNode = z->rightNode;
                y->rightNode->parentNode = y;
            }
            transplant(z, y);
            y->leftNode = z->leftNode;
            y->leftNode->parentNode = y;
            y->NodeColor = z->NodeColor;
        }
        delete z;
        
        // Update heights up the tree
        NodePtr current = (x != TNULL) ? x->parentNode : nullptr;
        while (current != nullptr && current != TNULL) {
            updateHeight(current);
            current = current->parentNode;
        }

        if (y_original_color == 0) {
            fixDelete(x);
        }
    }

public:
    RBT() {
        TNULL = new Node_rbt;
        initializeNULLNode(TNULL, nullptr);
        root = TNULL;
    }

    void insert(int key) {
        NodePtr node = new Node_rbt;
        node->parentNode = nullptr;
        node->NodeData = key;
        node->leftNode = TNULL;
        node->rightNode = TNULL;
        node->NodeColor = 1;  // Red
        node->height = 1;

        NodePtr y = nullptr;
        NodePtr x = root;

        while (x != TNULL) {
            y = x;
            if (node->NodeData < x->NodeData) {
                x = x->leftNode;
            } else {
                x = x->rightNode;
            }
        }

        node->parentNode = y;
        if (y == nullptr) {
            root = node;
        } else if (node->NodeData < y->NodeData) {
            y->leftNode = node;
        } else {
            y->rightNode = node;
        }

        // Update heights up the tree
        NodePtr current = node;
        while (current != nullptr && current != TNULL) {
            updateHeight(current);
            current = current->parentNode;
        }

        if (node->parentNode == nullptr) {
            node->NodeColor = 0;
            return;
        }

        if (node->parentNode->parentNode == nullptr) {
            return;
        }

        fixInsert(node);
    }

    void remove(int key) {
        deleteNodeHelper(root, key);
    }

    NodePtr minimum(NodePtr node) {
        while (node->leftNode != TNULL) {
            node = node->leftNode;
        }
        return node;
    }

    int getHeight() const {
        return root->height;
    }

    bool search(int key) {
        NodePtr node = root;
        while (node != TNULL) {
            if (key == node->NodeData) {
                return true;
            } else if (key < node->NodeData) {
                node = node->leftNode;
            } else {
                node = node->rightNode;
            }
        }
        return false;
    }

    void export_branch_heights(ofstream& out) const {
        function<void(NodePtr, int)> dfs = [&](NodePtr node, int depth) {
            if (!node) return;
            if (!node->leftNode && !node->rightNode) {
                out << depth << "\n";
            }
            dfs(node->leftNode, depth + 1);
            dfs(node->rightNode, depth + 1);
        };
        dfs(root, 1);
    }
};


void run_tests() {
    ofstream out_random("results_random.csv");
    ofstream out_sorted("results_sorted.csv");
    ofstream out_heights("tree_heights.csv");
    ofstream out_hist("height_distribution_2^18.csv");
    
    out_random << "Test Series,Data Size,Tree Type,Insert Time (us),Search Time (us),Remove Time (us)\n";
    out_sorted << "Test Series,Data Size,Tree Type,Insert Time (us),Search Time (us),Remove Time (us)\n";
    out_heights << "Data Size,Tree Type,Max Height,Log2N\n";
    out_hist << "Series,AVL Random,Treap Random,RBT Random,AVL Sorted,Treap Sorted,RBT Sorted\n";
    
    random_device rd;
    mt19937 gen(rd());

    vector<int> heights_avl_random;
    vector<int> heights_avl_sorted;
    vector<int> heights_treap_random;
    vector<int> heights_treap_sorted;
    vector<int> heights_rbt_random;
    vector<int> heights_rbt_sorted;
    
    for (int i = 10; i <= 18; ++i) {
        int data_size = 1 << i;
        uniform_int_distribution<> dist(1, data_size * 10);

        int max_avl_random = 0;
        int max_treap_random = 0;
        int max_rbt_random = 0;
        int max_avl_sorted = 0;
        int max_treap_sorted = 0;
        int max_rbt_sorted = 0;
        
        // Тесты со случайными данными
        {
            vector<double> avl_insert_times;
            vector<double> treap_insert_times;
            vector<double> rbt_insert_times;
            vector<double> avl_search_times;
            vector<double> treap_search_times;
            vector<double> rbt_search_times;
            vector<double> avl_remove_times;
            vector<double> treap_remove_times;
            vector<double> rbt_remove_times;
            
                
            for (int test = 0; test < 50; ++test) {
                vector<int> data(data_size);
                for (int j = 0; j < data_size; ++j) {
                    data[j] = dist(gen);
                }
                    
                // AVL tree
                AVL avl;
                auto start = high_resolution_clock::now();
                for (int val : data) {
                    avl.insert(val);
                }
                auto end = high_resolution_clock::now();
                avl_insert_times.push_back(duration_cast<microseconds>(end - start).count());
                
                int h = avl.getHeight();
                if (i == 18) {
                    heights_avl_random.push_back(h);
                }
                max_avl_random = max(max_avl_random, h);
                    
                // Treap
                Treap treap;
                start = high_resolution_clock::now();
                for (int val : data) {
                    treap.insert(val);
                }
                end = high_resolution_clock::now();
                treap_insert_times.push_back(duration_cast<microseconds>(end - start).count());

                h = treap.getHeight();
                if (i == 18) {
                    heights_treap_random.push_back(h);
                }
                max_treap_random = max(max_treap_random, h);

                // Red-Black Tree
                RBT rbt;
                start = high_resolution_clock::now();
                for (int val : data) {
                    rbt.insert(val);
                }
                end = high_resolution_clock::now();
                rbt_insert_times.push_back(duration_cast<microseconds>(end - start).count());

                h = rbt.getHeight();
                if (i == 18) {
                    heights_rbt_random.push_back(h);
                }
                max_rbt_random = max(max_rbt_random, h);

                if (i == 18) {
                    ofstream out_avl("avl_branch_heights.csv");
                    avl.export_branch_heights(out_avl);
                    out_avl.close();
                
                    ofstream out_treap("treap_branch_heights.csv");
                    treap.export_branch_heights(out_treap);
                    out_treap.close();
                
                    ofstream out_rbt("rbt_branch_heights.csv");
                    rbt.export_branch_heights(out_rbt);
                    out_rbt.close();
                }
                    
                // Search test (1000 operations)
                start = high_resolution_clock::now();
                for (int j = 0; j < 1000; ++j) {
                    int val = dist(gen);
                    avl.search(val);
                }
                end = high_resolution_clock::now();
                avl_search_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
                    
                start = high_resolution_clock::now();
                for (int j = 0; j < 1000; ++j) {
                    int val = dist(gen);
                    treap.search(val);
                }
                end = high_resolution_clock::now();
                treap_search_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
                    
                start = high_resolution_clock::now();
                for (int j = 0; j < 1000; ++j) {
                    int val = dist(gen);
                    rbt.search(val);
                }
                end = high_resolution_clock::now();
                rbt_search_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);

                // Remove test (1000 operations)
                vector<int> to_remove;
                for (int j = 0; j < 1000; ++j) {
                    to_remove.push_back(data[dist(gen) % data_size]);
                }
                    
                // AVL remove
                start = high_resolution_clock::now();
                for (int val : to_remove) {
                    avl.remove(val);
                }
                end = high_resolution_clock::now();
                avl_remove_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
                    
                // Treap remove
                start = high_resolution_clock::now();
                for (int val : to_remove) {
                    treap.remove(val);
                }
                end = high_resolution_clock::now();
                treap_remove_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
                    
                // RBT remove
                start = high_resolution_clock::now();
                for (int val : to_remove) {
                    rbt.remove(val);
                }
                end = high_resolution_clock::now();
                rbt_remove_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
            }
            
            auto write_results = [&](ofstream& out, const vector<double>& insert, const vector<double>& search, 
                                    const vector<double>& remove, const string& type) {
                    double avg_insert = accumulate(insert.begin(), insert.end(), 0.0) / insert.size();
                    double avg_search = accumulate(search.begin(), search.end(), 0.0) / search.size();
                    double avg_remove = accumulate(remove.begin(), remove.end(), 0.0) / remove.size();
                    
                    out << i << "," << data_size << "," << type << "," 
                        << avg_insert << "," << avg_search << "," << avg_remove << "\n";
            };
                
            write_results(out_random, avl_insert_times, avl_search_times, avl_remove_times, "AVL");
            write_results(out_random, treap_insert_times, treap_search_times, treap_remove_times, "Treap");
            write_results(out_random, rbt_insert_times, rbt_search_times, rbt_remove_times, "RBT");
        }
        
        // Тесты с отсортированными данными
        {
            vector<double> avl_insert_times;
            vector<double> treap_insert_times;
            vector<double> rbt_insert_times;
            vector<double> avl_search_times;
            vector<double> treap_search_times;
            vector<double> rbt_search_times;
            vector<double> avl_remove_times;
            vector<double> treap_remove_times;
            vector<double> rbt_remove_times;
            
            for (int test = 0; test < 50; ++test) {
                vector<int> data(data_size);
                for (int j = 0; j < data_size; ++j) {
                    data[j] = j + 1;
                }
                
                // AVL tree
                AVL avl;
                auto start = high_resolution_clock::now();
                for (int val : data) {
                    avl.insert(val);
                }
                auto end = high_resolution_clock::now();
                avl_insert_times.push_back(duration_cast<microseconds>(end - start).count());

                int h = avl.getHeight();
                if (i == 18) {
                    heights_avl_sorted.push_back(h);
                }
                max_avl_sorted = max(max_avl_sorted, h);
                
                // Treap
                Treap treap;
                start = high_resolution_clock::now();
                for (int val : data) {
                    treap.insert(val);
                }
                end = high_resolution_clock::now();
                treap_insert_times.push_back(duration_cast<microseconds>(end - start).count());

                h = treap.getHeight();
                if (i == 18) {
                    heights_treap_sorted.push_back(h);
                }
                max_treap_sorted = max(max_treap_sorted, h);
                
                // Red-Black Tree
                RBT rbt;
                start = high_resolution_clock::now();
                for (int val : data) {
                    rbt.insert(val);
                }
                end = high_resolution_clock::now();
                rbt_insert_times.push_back(duration_cast<microseconds>(end - start).count());

                h = rbt.getHeight();
                if (i == 18) {
                    heights_rbt_sorted.push_back(h);
                }
                max_rbt_sorted = max(max_rbt_sorted, h);

                if (i == 18) {
                    ofstream out_avl("avl_branch_heights_sort.csv");
                    avl.export_branch_heights(out_avl);
                    out_avl.close();    
                
                    ofstream out_treap("treap_branch_heights_sort.csv");
                    treap.export_branch_heights(out_treap);
                    out_treap.close();
                
                    ofstream out_rbt("rbt_branch_heights_sort.csv");
                    rbt.export_branch_heights(out_rbt);
                    out_rbt.close();
                }
                
                // Search test (1000 operations)
                uniform_int_distribution<> search_dist(1, data_size);
                start = high_resolution_clock::now();
                for (int j = 0; j < 1000; ++j) {
                    int val = search_dist(gen);
                    avl.search(val);
                }
                end = high_resolution_clock::now();
                avl_search_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
                
                start = high_resolution_clock::now();
                for (int j = 0; j < 1000; ++j) {
                    int val = search_dist(gen);
                    treap.search(val);
                }
                end = high_resolution_clock::now();
                treap_search_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
                
                start = high_resolution_clock::now();
                for (int j = 0; j < 1000; ++j) {
                    int val = search_dist(gen);
                    rbt.search(val);
                }
                end = high_resolution_clock::now();
                rbt_search_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);

                // Remove test (1000 operations)
                vector<int> to_remove;
                uniform_int_distribution<> remove_dist(1, data_size);
                for (int j = 0; j < 1000; ++j) {
                    to_remove.push_back(remove_dist(gen));
                }
                
                // AVL remove
                start = high_resolution_clock::now();
                for (int val : to_remove) {
                    avl.remove(val);
                }
                end = high_resolution_clock::now();
                avl_remove_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
                
                // Treap remove
                start = high_resolution_clock::now();
                for (int val : to_remove) {
                    treap.remove(val);
                }
                end = high_resolution_clock::now();
                treap_remove_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
                
                // RBT remove
                start = high_resolution_clock::now();
                for (int val : to_remove) {
                    rbt.remove(val);
                }
                end = high_resolution_clock::now();
                rbt_remove_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
            }
            
            auto write_results = [&](ofstream& out, const vector<double>& insert, const vector<double>& search, 
                                   const vector<double>& remove, const string& type) {
                double avg_insert = accumulate(insert.begin(), insert.end(), 0.0) / insert.size();
                double avg_search = accumulate(search.begin(), search.end(), 0.0) / search.size();
                double avg_remove = accumulate(remove.begin(), remove.end(), 0.0) / remove.size();
                
                out << i << "," << data_size << "," << type << ","
                    << avg_insert << "," << avg_search << "," << avg_remove << "\n";
            };
            
            write_results(out_sorted, avl_insert_times, avl_search_times, avl_remove_times, "AVL");
            write_results(out_sorted, treap_insert_times, treap_search_times, treap_remove_times, "Treap");
            write_results(out_sorted, rbt_insert_times, rbt_search_times, rbt_remove_times, "RBT");

            out_heights << data_size << ",AVL Random," << max_avl_random << "," << log2(data_size) << "\n";
            out_heights << data_size << ",Treap Random," << max_treap_random << "," << log2(data_size) << "\n";
            out_heights << data_size << ",RBT Random," << max_rbt_random << "," << log2(data_size) << "\n";
            out_heights << data_size << ",AVL Sorted," << max_avl_sorted << "," << log2(data_size) << "\n";
            out_heights << data_size << ",Treap Sorted," << max_treap_sorted << "," << log2(data_size) << "\n";
            out_heights << data_size << ",RBT Sorted," << max_rbt_sorted << "," << log2(data_size) << "\n";
        }
        cout << "Completed tests for 2^" << i << " = " << data_size << " elements\n";
    }
    
    for (int i = 0; i < heights_avl_random.size(); ++i) {
        out_hist << i + 1 << "," << heights_avl_random[i] << "," << heights_treap_random[i] << "," << heights_rbt_random[i] 
                 << "," << heights_avl_sorted[i] << "," << heights_treap_sorted[i] << "," << heights_rbt_sorted[i] << "\n";
    }

    out_random.close();
    out_sorted.close();
    out_heights.close();
    out_hist.close();
    cout << "Testing completed. Results saved to CSV files." << endl;
}

int main() {
    run_tests();
    return 0;
}