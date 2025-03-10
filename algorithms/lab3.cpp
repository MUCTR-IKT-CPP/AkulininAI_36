#include <iostream>
#include <stdexcept>
#include <random>
#include <ctime>
#include <string>
#include <stack>
#include <chrono>

using namespace std;
using namespace std::chrono;

template <typename T>
class Queue {
    private:
        struct Node {
            T data;
            Node* next;
            Node(T data) : data(data), next(nullptr) {}
        };

        Node* head;
        Node* tail;
        int size;

    public:
        Queue() : head(nullptr), tail(nullptr), size(0) {}

        ~Queue() {
            while (head) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }

        void push(const T& value) {
            Node* newNode = new Node(value);
            if (tail) {
                tail->next = newNode;
            }
            else {
                head = newNode;
            }
            tail = newNode;
            size++;
        }

        void pop() {
            if (isEmpty()) {
                throw out_of_range("ERROR: queue is empty!");
            }
            Node* temp = head;
            head = head->next;
            delete temp;
            size--;
            if (isEmpty()) {
                tail = nullptr;
            }
        }

        T& front() {
            if (isEmpty()) {
                throw out_of_range("ERROR: queue is empty!");
            }
            return head->data;
        }

        bool isEmpty() const {
            return size == 0;
        }

        int getSize() const {
            return size;
        }

        class Iterator {
        private:
            Node* current;

        public:
            Iterator(Node* node) : current(node) {}

            Iterator& operator++() {
                if (current) {
                    current = current->next;
                }
                return *this;
            }

            bool operator!=(const Iterator& other) const {
                return current != other.current;
            }

            T& operator*() {
                return current->data;
            }
        };

        Iterator begin() {
            return Iterator(head);
        }

        Iterator end() {
            return Iterator(nullptr);
        }
};

template <typename T>
class Queue2 {
private:
    stack<T> stack_in;
    stack<T> stack_out;

    void shiftStacks() {
        if (stack_out.empty()) {
            while (!stack_in.empty()) {
                stack_out.push(stack_in.top());
                stack_in.pop();
            }
        }
    }

public:
    void push(const T& value) {
        stack_in.push(value);
    }

    void pop() {
        if (isEmpty()) {
            throw out_of_range("ERROR: queue is empty!");
        }
        shiftStacks();
        stack_out.pop();
    }

    T& front() {
        if (isEmpty()) {
            throw out_of_range("ERROR: queue is empty!");
        }
        shiftStacks();
        return stack_out.top();
    }

    bool isEmpty() const {
        return stack_in.empty() && stack_out.empty();
    }

    int getSize() const {
        return stack_in.size() + stack_out.size();
    }

    class Iterator {
    private:
        stack<T> stack_in_copy;
        stack<T> stack_out_copy;
        
    public:
        Iterator(stack<T> stack_in, stack<T> stack_out) : stack_in_copy(stack_in), stack_out_copy(stack_out) {
            if (stack_out_copy.empty()) {
                while (!stack_in_copy.empty()) {
                    stack_out_copy.push(stack_in_copy.top());
                    stack_in_copy.pop();
                }
            }
        }
        
        Iterator& operator++() {
            if (!stack_out_copy.empty()) {
                stack_out_copy.pop();
                if (stack_out_copy.empty()) {
                    while (!stack_in_copy.empty()) {
                        stack_out_copy.push(stack_in_copy.top());
                        stack_in_copy.pop();
                    }
                }
            }
            return *this;
        }
        
        bool operator!=(const Iterator& other) const {
            return !stack_out_copy.empty() || !other.stack_out_copy.empty();
        }
        
        T& operator*() {
            return stack_out_copy.top();
        }
        };

    Iterator begin() {
        return Iterator(stack_in, stack_out);
    }

    Iterator end() {
        return Iterator(stack<T>(), stack<T>());
    }
};

template <typename T>
void print(Queue<T>& queue, Queue2<T>& queue2) {
    cout << "QueueList: ";
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    if (!queue.isEmpty()) {
        cout << "First element: " << queue.front() << endl;
        cout << "Queue size: " << queue.getSize() << endl;
    } else {
        cout << "Queue is empty!\n";
    }
    cout << "\tQueueTwoStack: ";
    for (auto it = queue2.begin(); it != queue2.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    if (!queue2.isEmpty()) {
        cout << "First element: " << queue2.front() << endl;
        cout << "Queue size: " << queue2.getSize() << endl;
    } else {
        cout << "Queue is empty!\n";
    }
}

void test1() {
    Queue<int> queue;
    Queue2<int> queue2;
    int sum = 0;
    int min_value = 1001;
    int max_value = -1001;

    mt19937 rng(time(0));
    uniform_int_distribution<int> dist(-1000, 1000);
    for (int i = 0; i < 1000; i++) {
        int value = dist(rng);
        queue.push(value);
        queue2.push(value);

        sum += value;

        if (value < min_value) {
            min_value = value;
        }
        if (value > max_value) {
            max_value = value;
        }
    }

    cout << "-------------------------\n";
    cout << "Test 1: Integers\n";
    print(queue, queue2);
    cout << "Sum: " << sum << endl;
    cout << "Average: " << static_cast<double>(sum) / queue.getSize() << endl;
    cout << "Min: " << min_value << endl;
    cout << "Max: " << max_value << endl;
    cout << "-------------------------\n";
}

void test2() {
    Queue<string> queue;
    Queue2<string> queue2;

    string elements[] = {"apple", "banana", "cherry", "date", "elderberry", "fig", "grape", "honeydew", "kiwi", "lemon"};
    mt19937 rng(time(0));
    uniform_int_distribution<int> dist(0, 9);
    for (int i = 0; i < 10; i++) {
        queue.push(elements[dist(rng)]);
        queue2.push(elements[dist(rng)]);
    }
    cout << "------------------------\n";
    cout << "Test 2: Strings\n\t";
    print(queue, queue2);
    cout << "------------------------\n";
}

void test3() {
    struct Person {
        string surname;
        string name;
        string patronymic;
        string birth_date;
    };
    
    Queue<Person> queue;
    Queue2<Person> queue2;

    vector<string> surnames = {"Ivanov", "Petrov", "Sidorov"};
    vector<string> names = {"Ivan", "Petr", "Sidor"};
    vector<string> patronymics = {"Ivanovich", "Petrovich", "Sidorovich"};
    vector<string> birth_date;

    mt19937 rng(time(0));
    uniform_int_distribution<int> year_dist(1980, 2020);
    uniform_int_distribution<int> month_dist(1, 12);
    uniform_int_distribution<int> day_dist(1, 28);
    for (int i = 0; i < 100; i++) {
        string date = to_string(day_dist(rng)) + "." + to_string(month_dist(rng)) + "." + to_string(year_dist(rng));
        birth_date.push_back(date);
    }

    for (int i = 0; i < 100; i++) {
        Person p = {
            surnames[rng() % surnames.size()],
            names[rng() % names.size()],
            patronymics[rng() % patronymics.size()],
            birth_date[i]
        };
        queue.push(p);
        queue2.push(p);
    }

    Queue<Person> younger20;
    Queue<Person> older30;

    while (!queue.isEmpty()) {
        Person p = queue.front();
        queue.pop();

        int birth_year = stoi(p.birth_date.substr(6, 4));
        int age = 2023 - birth_year;

        if (age < 20) {
            younger20.push(p);
        } else if (age > 30) {
            older30.push(p);
        }
    }

    cout << "------------------------\n";
    cout << "Test 3: Struct\n";
    cout << "Younger than 20: " << younger20.getSize() << endl;
    cout << "Older than 30: " << older30.getSize() << endl << endl;

    Queue2<Person> younger20_2;
    Queue2<Person> older30_2;

    while (!queue2.isEmpty()) {
        Person p = queue2.front();
        queue2.pop();

        int birth_year = stoi(p.birth_date.substr(6, 4));
        int age = 2023 - birth_year;

        if (age < 20) {
            younger20_2.push(p);
        } else if (age > 30) {
            older30_2.push(p);
        }
    }

    cout << "Younger than 20: " << younger20_2.getSize() << endl;
    cout << "Older than 30: " << older30_2.getSize() << endl;
    cout << "-------------------------\n";
}

template <typename T>
void reverse_recursive(Queue<T>& queue, Queue2<T>& queue2) {
    if (queue.isEmpty() || queue2.isEmpty()) {
        return;
    }

    T element = queue.front();
    queue.pop();

    T element2 = queue2.front();
    queue2.pop();

    reverse_recursive(queue, queue2);

    queue.push(element);
    queue2.push(element2);
}

void inversion() {
    Queue<int> queue;
    Queue2<int> queue2;
    int size = 10;

    for (int i = 0; i < size; i++) {
        queue.push(i);
        queue2.push(i);
    }

    cout << "------------------------\n";
    cout << "Test 4: Inversion\n";
    cout << "+Original Queue+\n\t";

    print(queue, queue2);

    reverse_recursive(queue, queue2);

    cout << "\n+Inverted Queue+\n\t";
    print(queue, queue2);
    cout << "-------------------------\n";
}

void compare_performance() {
    const int N = 1000000;

    Queue<int> queue;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        queue.push(i);
    }
    for (int i = 0; i < N; i++) {
        queue.pop();
    }
    auto end = high_resolution_clock::now();
    auto durationLinkedList = duration_cast<milliseconds>(end - start).count();
    cout << "Linked List Queue: " << durationLinkedList << " ms\n";

    Queue2<int> queue2;
    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        queue2.push(i);
    }
    for (int i = 0; i < N; i++) {
        queue2.pop();
    }
    end = high_resolution_clock::now();
    auto durationTwoStacks = duration_cast<milliseconds>(end - start).count();
    cout << "Two Stacks Queue: " << durationTwoStacks << " ms\n";

    cout << "------------------------\n";
    cout << "Performance Comparison:\n";
    cout << "Linked List Queue is " << static_cast<double>(durationLinkedList) / durationTwoStacks << "x slower than Two Stacks Queue.\n";
    cout << "------------------------\n";
}

int main() {
    test1();
    test2();
    test3();

    inversion();

    compare_performance();

    return 0;
}