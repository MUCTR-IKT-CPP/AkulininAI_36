#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <queue>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

class Graph {
private:
    int vertices;
    vector<vector<int>> adjacency_matrix;

public:
    Graph(int v) : vertices(v) {
        adjacency_matrix.resize(vertices, vector<int>(vertices, 0));
    }

    void addEdge(int from, int to, int weight) {
        adjacency_matrix[from][to] = weight;
        adjacency_matrix[to][from] = weight;
    }

    int prismMST() const {
        vector<int> key(vertices, INT_MAX);
        vector<bool> inMST(vertices, false);
    
        key[0] = 0;

        int mstWeight = 0;
        for (int count = 0; count < vertices; count++) {
            int u = -1;
            for (int v = 0; v < vertices; v++) {
                if (!inMST[v] && (u == -1 || key[v] < key[u])) {
                    u = v;
                }
            }

            inMST[u] = true;
            mstWeight += key[u];
    
            for (int v = 0; v < vertices; v++) {
                if (adjacency_matrix[u][v] && !inMST[v] && adjacency_matrix[u][v] < key[v]) {
                    key[v] = adjacency_matrix[u][v];
                }
            }
        }
    
        return mstWeight;
    }

    int getVertices() const {
        return vertices;
    }

    const vector<vector<int>>& getAdjacencyMatrix() {
        return adjacency_matrix;
    }

    void printAdjacencyMatrix() const {
        for (const auto& row : adjacency_matrix) {
            for (int weight : row) {
                cout << weight << " ";
            }
            cout << endl;
        }
    }
};

class GraphGenerator {
private:
    random_device rd;
    mt19937 gen;

public:
    GraphGenerator() : gen(rd()) {}

    Graph generator_graph(int vertices, int min_edges) {
        Graph graph(vertices);

        uniform_int_distribution<> weight_dist(1, 20);
        uniform_int_distribution<> vertex_dist(0, vertices - 1);

        for (int i = 1; i < vertices; i++) {
            int j = vertex_dist(gen) % i;
            int weight = weight_dist(gen);

            graph.addEdge(i, j, weight);
        }

        for (int i = 0; i < vertices; i++) {
            int count_edges = 0;
            for (int j = 0; j < vertices; j++) {
                if (graph.getAdjacencyMatrix()[i][j] > 0) {
                    count_edges++;
                }
            }
            while (count_edges < min_edges) {
                int j = vertex_dist(gen);
                if (i != j && graph.getAdjacencyMatrix()[i][j] == 0) {
                    int weight = weight_dist(gen);
                    graph.addEdge(i, j, weight);
                    count_edges++;
                }
            }
        }

        return graph;
    }
};

void test(const Graph& graph, int count_test, ofstream& outputFile) {
    double total_time = 0;
    for (int i = 0; i < count_test; i++) {
        high_resolution_clock::time_point start_time = high_resolution_clock::now();
        int mst_weight = graph.prismMST();
        high_resolution_clock::time_point end_time = high_resolution_clock::now();
        duration<double, micro> time_duration = end_time - start_time;
        
        cout << "Test " << i + 1 << ": MST Weight = " << mst_weight << ", Time = " << time_duration.count() << " microseconds\n";

        total_time += time_duration.count();
    }

    double average_time = total_time / count_test;
    outputFile << graph.getVertices() << "," << average_time << endl;
}

int main() {
    vector<int> vertices_list = {10, 20, 50, 100};
    vector<int> min_edges_list = {3, 4, 10, 20};

    GraphGenerator generator;

    ofstream outputFile("results.csv");
    outputFile << "Vertices,AverageTime(microseconds)\n";
    for (int i = 0; i < vertices_list.size(); i++) {
        cout << "Graph with " << vertices_list[i] << " vertices and " << min_edges_list[i] << " minimum edges:\n\n";
        Graph graph = generator.generator_graph(vertices_list[i], min_edges_list[i]);
        // graph.printAdjacencyMatrix();
        int count_test = 10;
        test(graph, count_test, outputFile);
        cout << "\n----------------------------------------\n";
    }

    outputFile.close();
    return 0;
}