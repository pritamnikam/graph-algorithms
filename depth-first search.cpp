/*
depth-first search

*/

using namespace std;

#include <vector>
#include <iostream>


namespace {

	// shorthand for adjacency list type
	typedef vector<vector<int>> AdjacencyList;

	// define an enum for node states during DFS execution
	enum NodeState { UNVISITED, IN_PROGRESS, FINISHED };

	class CycleDetector {
	private:
		const AdjacencyList adjacencyList;
		vector<NodeState> nodeStates;
		bool cycleFound = false;

	public:
		CycleDetector(AdjacencyList _adjacencyList)
			: adjacencyList(_adjacencyList)
			// initialize all nodes as unvisited
			, nodeStates(vector<NodeState>(
				_adjacencyList.size(), 
				NodeState::UNVISITED)) {}

	private:
		void dfs(int u) {
			// mark current node as in progress
			nodeStates[u] = NodeState::IN_PROGRESS;

			for (int v : adjacencyList[u]) {
				switch (nodeStates[v]) {
					// discovery edge: recursively call dfs
					case NodeState::UNVISITED: dfs(v); break;

					// back edge: mark cycle as found
					case NodeState::IN_PROGRESS: cycleFound = true; break;

					// redundant edge: skip
					case NodeState::FINISHED: break;
				}
			}

			// mark current node as done 
			nodeStates[u] = FINISHED;
		}

		// keep track of parent vertex, default -1 (no parent)
		void dfs(int u, int parent) {
			nodeStates[u] = NodeState::IN_PROGRESS;

			for (int v : adjacencyList[u]) {
				// skip edge back to the parent
				if (parent == v) continue;

				switch (nodeStates[v])
				{
					// call dfs and pass the parent
					case NodeState::UNVISITED: dfs(v, u); break;
					case NodeState::IN_PROGRESS: cycleFound = true; break;
					case NodeState::FINISHED: break;
				}
			}

			nodeStates[u] = NodeState::FINISHED;
		}

	public:
		bool containsCycle() {
			for (int u = 0; u < adjacencyList.size(); ++u) {
				// skip nodes that were already discovered
				if (nodeStates[u] == NodeState::FINISHED) continue;
				dfs(u);
			}

			return cycleFound;
		}

		bool containsCycleUndirected() {
			for (int u = 0; u < adjacencyList.size(); ++u) {
				// skip nodes that were already discovered
				if (nodeStates[u] == NodeState::FINISHED) continue;
				dfs(u, -1);
			}

			return cycleFound;
		}
	};

	void testGraphWithCycle() {
		int n = 5;
		AdjacencyList adjacencyList(n);
		vector<pair<int, int>> edges{ {0, 1}, {0, 2}, {1, 3}, {2, 1}, {3, 0}, {4, 1} };
		for (auto& edge : edges) {
			adjacencyList[edge.first].push_back(edge.second);
		}

		CycleDetector cycleDetector(adjacencyList);
		cout << "Contains cycle: " << cycleDetector.containsCycle() << endl;
	}

	void testGraphNoCycle() {
		int n = 5;
		AdjacencyList adjacencyList(n);
		vector<pair<int, int>> edges{ {0, 1}, {0, 2}, {2, 1}, {3, 0}, {4, 1} };
		for (auto& edge : edges) {
			adjacencyList[edge.first].push_back(edge.second);
		}

		CycleDetector cycleDetector(adjacencyList);
		cout << "Contains cycle: " << cycleDetector.containsCycle() << endl;
	}
} // namespace

void testCycleDetector() {
	testGraphWithCycle();
	testGraphNoCycle();
}
