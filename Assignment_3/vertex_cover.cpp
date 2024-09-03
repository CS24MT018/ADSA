#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>

using namespace std;

typedef vector<vector<int>> Graph;

// Function to check if the given cover is a valid vertex cover of size k
bool isVertexCover(Graph& graph, const vector<int>& cover) {
    set<pair<int, int>> coveredEdges;
    
    // Collect all edges in the cover
    for (int u : cover) {
        for (int v : graph[u]) {
            if (u < v) {
                coveredEdges.insert({u, v});
            } else {
                coveredEdges.insert({v, u});
            }
        }
    }

    // Check if all edges in the graph are covered
    set<pair<int, int>> allEdges;
    for (int u = 0; u < graph.size(); ++u) {
        for (int v : graph[u]) {
            if (u < v) {
                allEdges.insert({u, v});
            } else {
                allEdges.insert({v, u});
            }
        }
    }

    return coveredEdges == allEdges;
}

// Recursive function to explore vertex covers
bool boundedSearchTree(Graph& graph, vector<int>& cover, int k) {
    if (k < 0) return false;

    // Check if the cover is valid
    if (isVertexCover(graph, cover)) {
        return true;
    }

    // Find a vertex with edges
    int u = -1;
    for (int i = 0; i < graph.size(); ++i) {
        if (!graph[i].empty()) {
            u = i;
            break;
        }
    }

    if (u == -1) return cover.size() <= k;

    // Try including vertex u in the cover
    cover.push_back(u);
    Graph newGraph = graph;
    for (int v : graph[u]) {
        newGraph[v].erase(remove(newGraph[v].begin(), newGraph[v].end(), u), newGraph[v].end());
    }
    newGraph[u].clear();

    if (boundedSearchTree(newGraph, cover, k - 1)) return true;
    cover.pop_back();

    // Try including a neighbor of u in the cover
    if (!graph[u].empty()) {
        int v = graph[u][0];
        cover.push_back(v);
        newGraph = graph;
        for (int w : graph[v]) {
            newGraph[w].erase(remove(newGraph[w].begin(), newGraph[w].end(), v), newGraph[w].end());
        }
        newGraph[v].clear();

        if (boundedSearchTree(newGraph, cover, k - 1)) return true;
        cover.pop_back();
    }

    return false;
}

// Main function to determine if there is a vertex cover of size k
bool vertexCover(Graph& graph, int k, vector<int>& cover) {
    if (k > graph.size()) return false; // k cannot be larger than the number of vertices
    return boundedSearchTree(graph, cover, k);
}

int main() {
    ifstream infile("graph1.txt");  // Adjust the filename to match the input file

    if (!infile) {
        cerr << "Error opening file: graph1.txt" << endl;
        return 1;
    }

    string line;
    int n, m;

    while (getline(infile, line)) {
        if (line.empty()) continue; // Ignore blank lines
        stringstream ss(line);
        ss >> n >> m;
        break;
    }

    Graph graph(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        while (getline(infile, line)) {
            if (line.empty()) continue; // Ignore blank lines
            stringstream ss(line);
            ss >> u >> v;
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
    }

    infile.close();

    int k;
    cout << "Enter the value of k: ";
    cin >> k;

    vector<int> cover;
    if (vertexCover(graph, k, cover)) {
        cout << "Yes" << endl;
        for (int v : cover) {
            cout << v << " ";
        }
        cout << endl;
    } else {
        cout << "No" << endl;
    }

    return 0;
}