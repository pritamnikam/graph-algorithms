/*
The topological sorting problem

we’ll apply the DFS algorithm to solve a task ordering problem.

*/

using namespace std;

#include <vector>
#include <iostream>


namespace {

	// shorthand for adjacency list type
	typedef vector<vector<int>> AdjacencyList;

	enum class NodeState { UNVISITED, IN_PROGRESS, FINISHED };


	class GraphIsCyclicException : public runtime_error {
	public:
		GraphIsCyclicException() : runtime_error("Topological sort cann't be performed, as graph has cycles.") {}
	};

	class TopologicalSort {
		const AdjacencyList adjacencyList;
		vector<NodeState> nodeStates;

		// used to store the DFS finishing order
		vector<int> finishOrder;

	public:
		TopologicalSort(AdjacencyList _adjacencyList)
			: adjacencyList{ _adjacencyList }
			, nodeStates{ vector<NodeState>(adjacencyList.size(), NodeState::UNVISITED) }
		{}

		void dfs(int u) {
			nodeStates[u] = NodeState::IN_PROGRESS;

			for (int v : adjacencyList[u]) {
				switch (nodeStates[v]) {
					// discovery edge: recursively call dfs
					case NodeState::UNVISITED:
					dfs(v); break;

					// back edge: cycle found! There is no topological sort.
					case NodeState::IN_PROGRESS:
					throw GraphIsCyclicException(); break;

					// redundant edge: skip
					case NodeState::FINISHED:
					break;
				}
			}

			nodeStates[u] = NodeState::FINISHED;

			// add to finishing order
			finishOrder.push_back(u);
		}


		vector<int> computeTopologicalSort() {
			for (int u = 0; u < adjacencyList.size(); ++u) {
				if (nodeStates[u] == NodeState::FINISHED) continue;
				dfs(u);
			}

			// reverse finishing order
			reverse(finishOrder.begin(), finishOrder.end());
			return finishOrder;
		}
	};
}

void testTopologicalSort() {
	int n = 5;
	AdjacencyList adjacencyList(n);
	vector<pair<int, int>> edges{ {0, 1}, {0, 4}, {2, 4}, {2, 1}, {3, 0}, {4, 1} };
	for (auto& edge : edges) {
		adjacencyList[edge.first].push_back(edge.second);
	}

	TopologicalSort topologicalSort(adjacencyList);
	vector<int> topSort = topologicalSort.computeTopologicalSort();
	cout << "Topological sort:";
	for (int u : topSort) {
		cout << " " << u;
	}
	cout << endl;
}