#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>


using namespace std;


class Graph {
private:
    int vertexes;
    int max_edge;
    bool is_directed;
    vector<vector<int>> adjacency_matrix;
    vector<vector<int>> incidence_matrix;
    vector<set<int>> adjacency_list;
    vector<pair<int, int>> edges_list;

public:
    Graph (int vertexes, int max_edge = INT_MAX, bool is_directed = false) : vertexes(vertexes), is_directed(is_directed),
    max_edge(max_edge) {
        adjacency_matrix.resize(vertexes, vector<int>(vertexes, 0));
        adjacency_list.resize(vertexes);
    }

    void add_edge(int v1, int v2) {
        if (adjacency_list[v1].size() >= max_edge) {
            cerr << "Error: Vertex " << v1 << " has reached the maximum number of edges (" << max_edge << ").\n";
            return;
        }
        if (!is_directed && adjacency_list[v2].size() >= max_edge) {
            cerr << "Error: Vertex " << v2 << " has reached the maximum number of edges (" << max_edge << ").\n";
            return;
        }
        adjacency_matrix[v1][v2] = 1;
        adjacency_list[v1].insert(v2);
        edges_list.push_back({v1, v2});

        if (!is_directed) {
            adjacency_matrix[v2][v1] = 1;
            adjacency_list[v2].insert(v1);
        }
    }

    void fillIncidenceMatrix() {
        incidence_matrix.resize(vertexes, vector<int>(edges_list.size(), 0));

        for (int i = 0; i < edges_list.size(); i++) {
            int u = edges_list[i].first;
            int v = edges_list[i].second;

            if (is_directed) {
                incidence_matrix[u][i] = -1;
                incidence_matrix[v][i] = 1;
            } else {
                incidence_matrix[u][i] = 1;
                incidence_matrix[v][i] = 1;
            }
        }
    }

    vector<vector<int>> get_adjacencyMatrix() const {
        return adjacency_matrix;
    }

    vector<vector<int>> get_incidenceMatrix() const {
        return incidence_matrix;
    }

    vector<set<int>> get_adjacencyList() const {
        return adjacency_list;
    }

    vector<pair<int, int>> get_edgesList() const {
        return edges_list;
    }

    void print_adjacencyMatrix() const {
        cout << "Adjacency Matrix:\n";
        for (const auto& row : adjacency_matrix) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << endl;
        }
    }

    void print_incidenceMatrix() const {
        cout << "Incidence Matrix:\n";
        for (const auto& row : incidence_matrix) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << endl;
        }
    }

    void print_adjacencyList() const {
        cout << "Adjacency List:\n";
        for (int i = 0; i < adjacency_list.size(); i++) {
            cout << i << ": ";
            for (int elem : adjacency_list[i]) {
                cout << elem << " ";
            }
            cout << endl;
        }
    }

    void print_edgesList() const {
        cout << "Edge List:\n";
        for (const auto& elem : edges_list) {
            cout << "(" << elem.first << ", " << elem.second << ") ";
        }
        cout << endl;
    }

    bool bfs(int start, int end, vector<int>& v_bfs) {
        vector<bool> visited(vertexes, false);
        queue<int> q;
        vector<int> parent(vertexes, -1);

        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (u == end) {
                for (int v = end; v != -1; v = parent[v]) {
                    v_bfs.push_back(v);
                }
                reverse(v_bfs.begin(), v_bfs.end());
                return true;
            }

            for (int v : adjacency_list[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        return false;
    }

    bool dfs(int start, int end, vector<int>& v_dfs) {
        vector<bool> visited(vertexes, false);
        stack<int> s;
        vector<int> parent(vertexes, -1);

        s.push(start);
        visited[start] = true;

        while (!s.empty()) {
            int u = s.top();
            s.pop();
            
            if (u == end) {
                for (int v = end; v != -1; v = parent[v]) {
                    v_dfs.push_back(v);
                }
                reverse(v_dfs.begin(), v_dfs.end());
                return true;
            }

            for (int v : adjacency_list[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    s.push(v);
                }
            }
        }

        return false;
    }
};


class RandomGeneratorGraph {
private:
    int vertexMIN;
    int vertexMAX;
    int edgeMIN;
    int edgeMAX;
    int connected_edgesMAX;
    bool is_direct;
    int incoming_edgesMAX;
    int outgoing_edgesMAX;

public:
    RandomGeneratorGraph(int vertexMIN, int vertexMAX, int edgeMIN, int edgeMAX, int connected_edgesMAX,
        bool is_direct, int incoming_edgesMAX, int outgoing_edgesMAX) : vertexMIN(vertexMIN), vertexMAX(vertexMAX),
        edgeMIN(edgeMIN), edgeMAX(edgeMAX), connected_edgesMAX(connected_edgesMAX), is_direct(is_direct),
        incoming_edgesMAX(incoming_edgesMAX), outgoing_edgesMAX(outgoing_edgesMAX) {}

    Graph generate(int vertexes, int edges) {
        Graph graph(vertexes, is_direct, connected_edgesMAX);
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> vertex_dist(0, vertexes - 1);

        vector<int> out_degree(vertexes, 0);
        vector<int> in_degree(vertexes, 0);

        for (int i = 0; i < edges; i++) {
            int v1 = vertex_dist(gen);
            int v2 = vertex_dist(gen);

            if (v1 != v2 && graph.get_adjacencyMatrix()[v1][v2] == 0) {
                if ((out_degree[v1] <= outgoing_edgesMAX) && (in_degree[v2] <= incoming_edgesMAX)) {
                    graph.add_edge(v1, v2);
                    out_degree[v1]++;
                    in_degree[v2]++;
                    if (!is_direct) {
                        graph.add_edge(v2, v1);
                        out_degree[v2]++;
                        in_degree[v1]++;
                    }
                }
            }
        }

        return graph;
    }
};

int main() {
    int minV = 5;
    int maxV = 20;
    int minE = 10;
    int maxE = 50;

    RandomGeneratorGraph generator(5, 20, 10, 50, 5, false, 5, 5);

    ofstream outputFile("results.csv");
    outputFile << "Vertices,Edges,BFS Time (micro s),DFS Time (micro s)\n";

    for (int i = 0; i < 10; i++) {
        int vertex = minV + i * ((maxV - minV) / 9);
        int edges = minE + i * ((maxE - minE) / 9);

        Graph graph = generator.generate(vertex, edges);

        graph.fillIncidenceMatrix();

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> vertexDist(0, vertex - 1);
        int start = vertexDist(gen);
        int end = vertexDist(gen);

        vector<int> v_bfs, v_dfs;

        chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();
        bool bfs_answer = graph.bfs(start, end, v_bfs);
        chrono::high_resolution_clock::time_point end_time = chrono::high_resolution_clock::now();
        chrono::duration<double, micro> bfsDuration = end_time - start_time;

        start_time = chrono::high_resolution_clock::now();
        bool dfs_answer = graph.dfs(start, end, v_dfs);
        end_time = chrono::high_resolution_clock::now();
        chrono::duration<double, micro> dfsDuration = end_time - start_time;

        outputFile << vertex << "," << edges << "," << bfsDuration.count() << "," << dfsDuration.count() << endl;

        cout << "Graph " << i + 1 << ":\n";
        cout << "Vertices: " << vertex << ", Edges: " << edges << endl;
        graph.print_adjacencyMatrix();
        graph.print_incidenceMatrix();
        graph.print_adjacencyList();
        graph.print_edgesList();

        cout << "\nBFS Path from " << start << " to " << end << ": ";
        if (bfs_answer) {
            for (int v : v_bfs) {
                cout << v << " ";
            }
        } 
        else {
            cout << "No path found";
        }
        cout << "\nDFS Path from " << start << " to " << end << ": ";
        if (dfs_answer) {
            for (int v : v_dfs) {
                cout << v << " ";
            }
        }
        else {
            cout << "No path found";
        }
        cout << "\nBFS Time: " << bfsDuration.count() << " micro s\n";
        cout << "DFS Time: " << dfsDuration.count() << " micro s\n-------------------------\n";
    }

    outputFile.close();
    return 0;
}