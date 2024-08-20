#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// Function to add an edge to the graph
void addEdge(vector<vector<int>>& graph, int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

// Function to implement the 2-approximation algorithm for Vertex Cover
vector<int> vertexCover2Approximation(vector<vector<int>>& graph, int V) {
    vector<bool> visited(V, false); // To keep track of visited vertices
    vector<int> cover; // To store the vertex cover

    // Iterate over all edges
    for (int u = 0; u < V; ++u) {
        if (!visited[u]) { // If vertex u is not visited
            for (int v : graph[u]) {
                if (!visited[v]) { // If vertex v is not visited
                    // Add u and v to the cover
                    cover.push_back(u);
                    cover.push_back(v);

                    // Mark both vertices as visited
                    visited[u] = true;
                    visited[v] = true;

                    // Break as we have covered this edge
                    break;
                }
            }
        }
    }
    return cover;
}

int main() {
    ifstream infile("task1-input.txt"); // Open the input file
    string line;

    while (getline(infile, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        // Read the number of vertices and edges
        istringstream iss(line);
        int V, E;
        iss >> V >> E;

        // Initialize the graph with V vertices
        vector<vector<int>> graph(V);

        // Read the edges
        for (int i = 0; i < E; ++i) {
            getline(infile, line); // Get the next line containing an edge
            if (line.empty()) { // Just in case there's an empty line
                --i; // Ensure we process exactly E edges
                continue;
            }
            istringstream edgeStream(line);
            int u, v;
            edgeStream >> u >> v;
            addEdge(graph, u, v);
        }

        // Get the vertex cover using the 2-approximation algorithm
        vector<int> cover = vertexCover2Approximation(graph, V);

        // Output the vertex cover
        cout << "Vertex Cover (2-approximation): ";
        for (int v : cover) {
            cout << v << " ";
        }
        cout << endl;
    }

    infile.close(); // Close the input file
    return 0;
}
