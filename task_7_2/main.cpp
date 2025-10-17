#include <iostream>
#include <vector>
#include <limits>

using namespace std;

class Graph {
private:
    int vertices;
    vector<vector<int>> adjacencyMatrix;

public:
    Graph(int v) : vertices(v) {
        adjacencyMatrix.resize(v, vector<int>(v, 0));
    }

    void addEdge(int u, int v, int weight) {
        adjacencyMatrix[u][v] = weight;
        adjacencyMatrix[v][u] = weight;
    }

    void inputGraph() {
        cout << "Enter the adjacency matrix of the graph (" << vertices << "x" << vertices << "):\n";
        for (int i = 0; i < vertices; i++) {
            cout << "Vertice " << i + 1 << ": ";
            for (int j = 0; j < vertices; j++) {
                cin >> adjacencyMatrix[i][j];
            }
        }
    }

    void printGraph() {
        cout << "\nAdjacency matrix of a graph\n";
        cout << "  ";
        for (int i = 0; i < vertices; i++) {
            cout << " " << i;
        }
        cout <<	'\n';

        for (int i = 0; i < vertices; i++) {
            cout << " " << i;
            for (int j = 0; j < vertices; j++) {
                cout << " " << adjacencyMatrix[i][j];
            }
            cout << '\n';
        }
    }

    void primMST() {
        vector<int> parent(vertices, -1); 
        vector<int> key(vertices, numeric_limits<int>::max());
        vector<bool> inMST(vertices, false);

        key[0] = 0;
        parent[0] = -1;

        for (int count = 0; count < vertices - 1; count++) {

            int u = -1;
            int minKey = numeric_limits<int>::max();

            for (int v = 0; v < vertices; v++) {
                if (!inMST[v] && key[v] < minKey) {
                    minKey = key[v];
                    u = v;
                }
            }

            if (u == -1) break;

            inMST[u] = true;

            for (int v = 0; v < vertices; v++) {
                if (adjacencyMatrix[u][v] > 0 && !inMST[v] && adjacencyMatrix[u][v] < key[v]) {
                    parent[v] = u;
                    key[v] = adjacencyMatrix[u][v];
                }
            }
        }

        printMST(parent);
    }

    void printMST(vector<int>& parent) {
        cout << "\nSpanning tree (Prim's algorithm)\n";
        cout << "Edge \tWeight\n";

        int totalWeight = 0;
        for (int i = 1; i < vertices; i++) {
            if (parent[i] != -1) {
                cout << parent[i] + 1 << " - " << i + 1<< " \t" << adjacencyMatrix[i][parent[i]] << '\n';
                totalWeight += adjacencyMatrix[i][parent[i]];
            }
        }
        cout << "Total weight of the spanning tree:" << totalWeight << '\n';

        cout << "\nVisual representation of a tree:\n";
        visualizeTree(parent);
    }

    void visualizeTree(vector<int>& parent) {
        vector<vector<int>> tree(vertices);

        for (int i = 1; i < vertices; i++) {
            if (parent[i] != -1) {
                tree[parent[i]].push_back(i);
            }
        }

        printTree(0, tree, 0);
    }

    void printTree(int node, vector<vector<int>>& tree, int level) {
        for (int i = 0; i < level; i++) {
            cout << "  ";
        }

        if (level > 0) {
            cout << "└── ";
        }

        cout << node + 1 << '\n';

        for (int child : tree[node]) {
            printTree(child, tree, level + 1);
        }
    }
};

int main() {
    int choice, vertices;

    cout << "Select the graph input method:\n";
    cout << "1. Use a test graph\n";
    cout << "2. Enter the graph manually\n";
    cout << "Your choice: ";
    cin >> choice;

    if (choice == 1) {

        vertices = 6;
        Graph g(vertices);

        g.addEdge(0, 1, 7);
        g.addEdge(0, 3, 2);
        g.addEdge(0, 5, 4);
        g.addEdge(1, 2, 1);
        g.addEdge(1, 3, 2);
        g.addEdge(2, 3, 2);
        g.addEdge(2, 4, 3);
	g.addEdge(3, 4, 6);
	g.addEdge(3, 5, 1);
	g.addEdge(4, 5, 8);

        cout << "\nTest graph:\n";
        g.printGraph();
        g.primMST();

    } else {
        cout << "Enter the number of vertices in the graph: ";
        cin >> vertices;

        Graph g(vertices);
        g.inputGraph();
        cout << "\nInput graph:\n";
        g.printGraph();
        g.primMST();
    }

    return 0;
}
