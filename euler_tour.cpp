#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <deque>
#include <sstream>
#include <algorithm> // Include this for std::find

using namespace std;

// Function to read graphs from the file
vector<pair<int, vector<pair<int, int>>>> readGraphs(const string &filename) {
    ifstream file(filename);
    vector<pair<int, vector<pair<int, int>>>> graphs;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue; // Skip blank lines
        stringstream ss(line);
        int n, m;
        ss >> n >> m;
        vector<pair<int, int>> edges;
        for (int i = 0; i < m; ++i) {
            int u, v;
            file >> u >> v;
            edges.emplace_back(u, v);
        }
        graphs.emplace_back(n, edges);
        getline(file, line); // To handle the newline character after edges
    }

    return graphs;
}

// Function to find the Eulerian path or circuit using Hierholzer's algorithm
vector<int> findEulerianPath(int n, const vector<pair<int, int>> &edges) {
    // Adjacency list representation
    unordered_map<int, vector<int>> adj;
    unordered_map<int, int> edge_count;
    for (const auto &edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
        edge_count[edge.first]++;
        edge_count[edge.second]++;
    }

    // Find the starting point: start with a vertex with an odd degree if it exists
    int start = 0;
    for (const auto &ec : edge_count) {
        if (ec.second % 2 != 0) {
            start = ec.first;
            break;
        }
    }

    // Hierholzer's algorithm
    stack<int> current_path;
    deque<int> circuit;
    current_path.push(start);

    while (!current_path.empty()) {
        int u = current_path.top();

        if (!adj[u].empty()) {
            int v = adj[u].back();
            adj[u].pop_back();
            auto it = find(adj[v].begin(), adj[v].end(), u);
            if (it != adj[v].end()) {
                adj[v].erase(it);
            }
            current_path.push(v);
        } else {
            circuit.push_front(u);
            current_path.pop();
        }
    }

    return vector<int>(circuit.begin(), circuit.end());
}

int main() {
    string filename = "graphs.txt";
    vector<pair<int, vector<pair<int, int>>>> graphs = readGraphs(filename);

    for (const auto &graph : graphs) {
        int n = graph.first;
        const auto &edges = graph.second;
        vector<int> eulerian_path = findEulerianPath(n, edges);

        cout << "Eulerian path/circuit for the graph with " << n << " vertices:" << endl;
        for (int vertex : eulerian_path) {
            cout << vertex << " ";
        }
        cout << endl << endl;
    }

    return 0;
}
