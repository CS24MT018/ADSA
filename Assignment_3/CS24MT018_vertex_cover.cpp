#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

// Function to read the graph from a file
pair<int, vector<pair<int, int>>> readGraphFromFile(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }
    
    int n, m;
    file >> n >> m; // Read number of vertices and edges
    
    vector<pair<int, int>> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        file >> u >> v;
        edges.emplace_back(u, v);
    }

    return {n, edges};
}

// Bounded Search Tree algorithm for Vertex Cover
bool boundedSearchTree(int k, vector<pair<int, int>> &edges, set<int> &cover) {
    // Base cases
    if (edges.empty()) return true;  // No edges, we have a vertex cover
    if (k == 0) return false;        // No remaining vertex slots, but still have edges

    // Choose an edge (u, v)
    auto edge = edges.back();
    edges.pop_back();
    int u = edge.first, v = edge.second;

    // Create copies of the current edge list
    vector<pair<int, int>> edges1 = edges;
    vector<pair<int, int>> edges2 = edges;

    // Remove all edges that involve 'u'
    for (auto it = edges1.begin(); it != edges1.end();) {
        if (it->first == u || it->second == u) {
            it = edges1.erase(it);
        } else {
            ++it;
        }
    }

    // Remove all edges that involve 'v'
    for (auto it = edges2.begin(); it != edges2.end();) {
        if (it->first == v || it->second == v) {
            it = edges2.erase(it);
        } else {
            ++it;
        }
    }

    // Try adding 'u' to the cover
    cover.insert(u);
    if (boundedSearchTree(k - 1, edges1, cover)) return true;
    cover.erase(u); // Backtrack

    // Try adding 'v' to the cover
    cover.insert(v);
    if (boundedSearchTree(k - 1, edges2, cover)) return true;
    cover.erase(v); // Backtrack

    return false; // No valid cover found
}

int main() {
    string filename;
    int k;

    // Input: File and parameter k
    cout << "Enter the file name: ";
    cin >> filename;
    cout << "Enter the value of k (size of vertex cover): ";
    cin >> k;

    // Read graph from the file
    auto graph = readGraphFromFile(filename);
    int n = graph.first;
    vector<pair<int, int>> edges = graph.second;

    cout << "Processing graph with " << n << " vertices and " << edges.size() << " edges." << endl;

    // Container for vertex cover
    set<int> vertexCover;

    // Run the bounded search tree algorithm
    if (boundedSearchTree(k, edges, vertexCover)) {
        cout << "Yes, vertex cover of size at most " << k << " found: " << endl;
        for (int v : vertexCover) {
            cout << v << " ";
        }
        cout << endl;
    } else {
        cout << "No, vertex cover of size " << k << " not found." << endl;
    }

    return 0;
}
