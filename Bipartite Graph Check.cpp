/*
Bipartite Graph

Apply graph traversal algorithms to check whether a graph is bipartite.

*/

#include <vector>
using namespace std;

typedef vector<vector<int>> AdjacencyList;
enum class NodeState { UNVISITED, RED, BLUE };

namespace {

    class BipartiteCheck {
    private:
        AdjacencyList adjacencyList;
        vector<NodeState> states;

    public:
        BipartiteCheck(AdjacencyList _adjacencyList)
            : adjacencyList{ _adjacencyList }
            , states{ vector<NodeState>(_adjacencyList.size(), NodeState::UNVISITED) }
        {}

        bool isBipartite() {
            // perform dfs
            for (int u = 0; u < adjacencyList.size(); ++i) {
                if (states[u] == NodeState::UNVISITED) {
                    dfs(u, true);
                }
            }

            // check for invalid edges between nodes of the same color
            for (int u = 0; u < this->adjacencyList.size(); ++u) {
                for (int v : this->adjacencyList[u]) {
                    if (this->states[u] == this->states[v]) {
                        return false;
                    }
                }
            }

            // no invalid edge found, graph is bipartite
            return true;
        }


        void dfs(int u, bool red) {
            // color the node
            this->states[u] = red ? NodeState::RED : NodeState::BLUE;

            // check neighbors and color them in the other color
            for (int v : this->adjacencyList[u]) {
                if (this->states[v] == NodeState::UNVISITED) {
                    dfs(v, !red);
                }
            }
        }
    };
}  // namespace