/*
Bredth-first search
*/

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

// shorthand for adjacency list type
typedef vector<vector<int>> AdjacencyList;

const int UNKNOWN = -1;

namespace {

	class NoPathExistsException : public runtime_error {
	public:
		NoPathExistsException() : runtime_error("No path exists between the nodes.") {}
	};

	class ShortestPaths {
	private:
		const AdjacencyList adjacencyList;
		vector<int> distances;
		vector<int> parents;

	public:
		ShortestPaths(AdjacencyList _adjacencyList)
			: adjacencyList (_adjacencyList)
			, distances(vector<int>(_adjacencyList.size(), UNKNOWN))
			, parents(vector<int>(_adjacencyList.size(), UNKNOWN)) {}

		void bfs(int start) {
			// initialization
			distances.assign(distances.size(), UNKNOWN);
			parents.assign(parents.size(), UNKNOWN);

			queue<int> unexplored;
			
			distances[start] = 0;
			parents[start] = start;
			unexplored.push(start);

			// bfs main loop
			while (!unexplored.empty()) {
				int u = unexplored.front();
				unexplored.pop();

				for (int v : adjacencyList[u]) {
					if (parents[v] == UNKNOWN) {
						// assign parent and distance
						parents[v] = u;
						distances[v] = 1 + distances[u];

						// add for exploration
						unexplored.push(v);
					}
				}
			}
		}

		vector<int> computeShortestPath(int start, int end) {
			bfs(start);
			// end was not discovered -> no path
			if (distances[end] == UNKNOWN) {
				throw NoPathExistsException();
			}

			vector<int> path;
			path.push_back(end);
			int current = end;

			// go back along parents to build the path
			while (current != start) {
				current = parents[current];
				path.push_back(current);
			}

			// path was assembled in reverse order
			std::reverse(path.begin(), path.end());
			return path;
		}

	};

	void testShortestPath() {
		int n = 6;
		AdjacencyList adjacencyList(n);
		vector<pair<int, int>> edges{ {0, 1}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {3, 4}, {4, 3}, {4, 5} };
		for (auto& edge : edges) {
			adjacencyList[edge.first].push_back(edge.second);
		}

		ShortestPaths shortestPaths(adjacencyList);
		vector<int> path04 = shortestPaths.computeShortestPath(0, 4);
		cout << "Shortest path from 0 to 4: ";
		for (int u : path04) {
			cout << u << " ";
		}
		cout << endl;
	}

	void testNoPath() {
		
		int n = 6;
		AdjacencyList adjacencyList(n);
		vector<pair<int, int>> edges{ {0, 1}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {3, 4}, {4, 3}, {4, 5} };
		for (auto& edge : edges) {
			adjacencyList[edge.first].push_back(edge.second);
		}

		ShortestPaths shortestPaths(adjacencyList);
		try {
			vector<int> path50 = shortestPaths.computeShortestPath(5, 0);
		}
		catch (NoPathExistsException exc) {
			cout << "No path from 5 to 0." << endl;
		}		
	}
}

void testShortesPaths() {
	testShortestPath();
	testNoPath();
}