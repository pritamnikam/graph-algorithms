/*
Strongly Connected Components (SCC)

Kosaraju's algorithm applies 2-pass DFS on the whole graph to compute SCCs.
The first pass is ordinary forward DFS. while the second pass is backward DFS, which follows the edges in reverse direction.

*/

using namespace std;

#include <iostream>
#include <vector>


namespace {

	// shorthand for adjustancy list
	typedef vector<vector<int>> AdjacencyList;

	enum class NodeState { UNVISITED, IN_PROGRESS, FINISHED };

	class StronglyConnectedComponents {
		AdjacencyList adjacencyList;
		vector<NodeState> nodeStates;
		vector<int> finishOrder;

	public:
		StronglyConnectedComponents(AdjacencyList _adjustancyList)
			: adjacencyList(_adjustancyList)
			, nodeStates { vector<NodeState>(_adjustancyList.size(), NodeState::UNVISITED) } {}

	private:
		// DFS with topological order
		void dfs(int u) {
			nodeStates[u] = NodeState::IN_PROGRESS;

			for (int v : adjacencyList[u]) {
				switch (nodeStates[v]) {
					case  NodeState::UNVISITED: dfs(v); break;
					// we ignore back edges and redundant edges here
					case NodeState::IN_PROGRESS:
					case NodeState::FINISHED: 
						break;
				}
			}

			nodeStates[u] = NodeState::FINISHED;
			finishOrder.push_back(u);
		}

		// reverse all edges i.e. {u, v} -> {v, u}
		void transpose() {
			// create new transposed adjacency list
			AdjacencyList transposeList = AdjacencyList(adjacencyList.size());

			for (int u = 0; u < adjacencyList.size(); ++u) {
				for (int v : adjacencyList[u]) {
					// add reverse edge (v, u)
					transposeList[v].push_back(u);
				}
			}

			// replace the adjacency list
			transposeList = transposeList;
		}

	public:
		vector<vector<int>> computeScc() {
			// forward DFS:
			for (int u = 0; u < adjacencyList.size(); ++u) {
				if (nodeStates[u] == NodeState::FINISHED)
					continue;

				dfs(u);
			}

			// store the finish order
			vector<int> traverseOrder(finishOrder.rbegin(), finishOrder.rend());

			// Backward DFS:
			// a. transpose of graph
			transpose();

			// b. reset:
			fill(nodeStates.begin(), nodeStates.end(), NodeState::UNVISITED);
			finishOrder.clear();

			vector<vector<int>> ssc;
			for (int u : traverseOrder) {
				if (nodeStates[u] == NodeState::FINISHED)
					continue;

				dfs(u);
				ssc.push_back(finishOrder);
				finishOrder.clear();
			}


			// Restore the graph
			transpose();
			fill(nodeStates.begin(), nodeStates.end(), NodeState::UNVISITED);
			finishOrder.clear();

			return ssc;
		}
	};

}


void testSSC() {
	int n = 7;
	AdjacencyList adjacencyList(n);
	vector<pair<int, int>> edges{ {0, 1}, {0, 6}, {1, 4}, {2, 3}, {2, 5}, {3, 2}, {3, 5}, {4, 0}, {4, 5}, {6, 4} };
	for (auto& edge : edges) {
		adjacencyList[edge.first].push_back(edge.second);
	}

	StronglyConnectedComponents components(adjacencyList);
	auto scc = components.computeScc();
	cout << "Strongly connected components:" << endl;
	for (auto component : scc) {
		for (int u : component) {
			cout << u << " ";
		}
		cout << endl;
	}
	cout << endl;
}