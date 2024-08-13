#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

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

// Function to check if a given permutation forms a Hamiltonian cycle
bool isHamiltonianCycle(const vector<int> &perm, const vector<vector<int>> &adj) {
    int n = perm.size();
    for (int i = 0; i < n; ++i) {
        if (adj[perm[i]][perm[(i + 1) % n]] == 0) {
            return false;
        }
    }
    return true;
}

// Recursive function to generate permutations and check for Hamiltonian cycle
bool findHamiltonianCycleHelper(vector<int> &perm, int l, int r, const vector<vector<int>> &adj, vector<int> &result) {
    if (l == r) {
        if (isHamiltonianCycle(perm, adj)) {
            result = perm;
            return true;
        }
        return false;
    } else {
        for (int i = l; i <= r; i++) {
            swap(perm[l], perm[i]);
            if (findHamiltonianCycleHelper(perm, l + 1, r, adj, result)) {
                return true;
            }
            swap(perm[l], perm[i]); // Backtrack
        }
    }
    return false;
}

// Function to find Hamiltonian cycle using permutations
vector<int> findHamiltonianCycle(int n, const vector<pair<int, int>> &edges) {
    vector<vector<int>> adj(n, vector<int>(n, 0));
    for (const auto &edge : edges) {
        adj[edge.first][edge.second] = 1;
        adj[edge.second][edge.first] = 1;
    }

    vector<int> perm(n);
    for (int i = 0; i < n; ++i) {
        perm[i] = i;
    }

    vector<int> result;
    if (findHamiltonianCycleHelper(perm, 1, n - 1, adj, result)) {
        return result;
    }

    return {};
}

int main() {
    string filename = "graphs.txt";
    vector<pair<int, vector<pair<int, int>>>> graphs = readGraphs(filename);

    for (const auto &graph : graphs) {
        int n = graph.first;
        const auto &edges = graph.second;
        vector<int> hamiltonian_cycle = findHamiltonianCycle(n, edges);

        cout << "Hamiltonian cycle for the graph with " << n << " vertices:" << endl;
        if (!hamiltonian_cycle.empty()) {
            for (int vertex : hamiltonian_cycle) {
                cout << vertex << " ";
            }
            cout << hamiltonian_cycle[0]; // To show the cycle is complete
        } else {
            cout << "No Hamiltonian cycle found";
        }
        cout << endl << endl;
    }

    return 0;
}

