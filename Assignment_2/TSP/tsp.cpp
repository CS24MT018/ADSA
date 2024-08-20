#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>

using namespace std;

typedef pair<int, int> Edge;
typedef vector<vector<pair<int, int>>> Graph;

void addEdge(Graph& graph, int u, int v, int weight) {
    graph[u].push_back({v, weight});
    graph[v].push_back({u, weight});
}

// Prim's algorithm to generate the MST
Graph primMST(const Graph& graph, int V) {
    priority_queue<Edge, vector<Edge>, greater<Edge>> pq;
    vector<int> parent(V, -1);
    vector<int> key(V, INT_MAX);
    vector<bool> inMST(V, false);
    Graph mst(V);

    key[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (inMST[u]) continue;  // Skip if already included in MST

        inMST[u] = true;

        for (auto& [v, weight] : graph[u]) {
            if (!inMST[v] && key[v] > weight) {
                key[v] = weight;
                pq.push({key[v], v});
                parent[v] = u;
            }
        }
    }

    for (int v = 1; v < V; ++v) {
        if (parent[v] != -1) {
            addEdge(mst, v, parent[v], key[v]);
        }
    }

    return mst;
}

// Recursive function for pre-order traversal of the MST
void preOrderMST(const Graph& mst, vector<bool>& visited, int u, vector<int>& tour) {
    visited[u] = true;
    tour.push_back(u);

    for (auto& [v, _] : mst[u]) {
        if (!visited[v]) {
            preOrderMST(mst, visited, v, tour);
        }
    }
}

vector<int> tsp2Approximation(const Graph& graph, int V) {
    Graph mst = primMST(graph, V);

    vector<int> tour;
    vector<bool> visited(V, false);
    preOrderMST(mst, visited, 0, tour);

    return tour;
}

int main() {
    ifstream infile("task2-input.txt");
    string line;

    while (getline(infile, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        int V, E;
        iss >> V >> E;

        if (V <= 0 || E <= 0) {
            cerr << "Invalid input: number of vertices or edges is non-positive." << endl;
            continue;
        }

        Graph graph(V);

        for (int i = 0; i < E; ++i) {
            getline(infile, line);
            if (line.empty()) { --i; continue; }

            istringstream edgeStream(line);
            int u, v, weight;
            edgeStream >> u >> v >> weight;
            if (u < 0 || u >= V || v < 0 || v >= V || weight < 0) {
                cerr << "Invalid edge data: " << u << " " << v << " " << weight << endl;
                continue;
            }
            addEdge(graph, u, v, weight);
        }

        vector<int> tspTour = tsp2Approximation(graph, V);

        cout << "TSP Tour (2-approximation): ";
        for (int v : tspTour) {
            cout << v << " ";
        }
        cout << tspTour[0] << endl;

        cout << endl;
    }

    infile.close();
    return 0;
}
