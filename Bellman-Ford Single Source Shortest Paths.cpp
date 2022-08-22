/*
Bellman-Ford - Single Source Shortest Paths (SSSP)
*/

#include <iostream>
#include <vector>
using namespace std;

namespace {
    typedef vector<vector<pair<int, int>>> AdjacencyList;
    constexpr int INF = 1'000'000'000;
    class BellmannFord {
    private:
        AdjacencyList adjacencyList;
         
    public:
        BellmannFord(AdjacencyList _adjacencyList)
            : adjacencyList{ _adjacencyList }
        {}

        vector<int> computeDistances(int start) {
            int n = this->adjacencyList.size();
            vector<int> distances(n, INFINITY);
            distances[start] = 0;
            
            // keep track of changes in distances
            bool changed = true;
            
            // count executions of step (2), run at most (|V| - 1)
            int executions = 0;
            do {
                changed = false;
                executions++;

                // try to relax all edges
                for (int u = 0; u < n; ++u) {
                    for (pair<int, int> entry : this->adjacencyList[u]) {
                        int v = entry.first, 
                            w = entry.second;
                        if (distances[u] + w < distances[v]) {
                            distances[v] = distances[u] + w;
                            // keep track of the change
                            changed = true;
                        }
                    }
                }
            } while (changed && executions < n - 1);

            return distances;
        }
    };

} // namespace

void testBellmannFord() {
    int n = 5;
    AdjacencyList adjacencyList(n);
    vector<vector<int>> edges{
        {0, 1, 6},
        {0, 2, 10},
        {0, 3, 4},
        {1, 0, 2},
        {1, 2, 3},
        {1, 4, 5},
        {3, 1, 1},
        {3, 4, 2},
        {4, 2, 1}
    };
    for (auto& edge : edges) {
        adjacencyList[edge[0]].push_back(make_pair(edge[1], edge[2]));
    }

    BellmannFord bellmannFord(adjacencyList);
    vector<int> path02 = bellmannFord.computeDistances(0);
    
    cout << "Shortest path from 0 to 2: ";
    for (int u : path02) {
        cout << u << " ";
    }
    cout << endl;
}