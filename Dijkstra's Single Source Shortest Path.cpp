/*
Dijkstra's Algorithm for the single source shortest path (SSSP).

Time Complexity
O(∣E∣log∣E∣)=O(∣E∣log∣V∣2)=O(∣E∣⋅2log∣V∣)=O(∣E∣log∣V∣)

Thus, the time complexity of Dijkstra can be given as O(∣E∣log∣V∣)
*/


#include <iostream>
#include <vector>
#include <queue>
using namespace std;

namespace {

	// we use the weighted adjacency list here that stores pairs of target nodes and distances
	typedef pair<int, int> NodeAndDistance;
	typedef vector<vector<NodeAndDistance>> AdjacencyList;

	class NoPathExistsException : public runtime_error {\
	public:
		NoPathExistsException() : runtime_error("No path exists between the nodes.") {}
	};


	const int UNKNOWN = -1;

	class Dijkstra {
		AdjacencyList adjacencyList;
		vector<int> distances;
		vector<int> parents;
		
	public:
		Dijkstra(AdjacencyList& _adjacencyList)
			: adjacencyList{ _adjacencyList }
			, distances { vector<int>(_adjacencyList.size(), UNKNOWN) }
			, parents { vector<int>(_adjacencyList.size(), UNKNOWN) }
		{}


		// return both the length of the shortest path and the path itself
		pair<int, vector<int>> computeShortestPath(int start, int end) {
			runDijkstra(start);

			if (distances[end] == UNKNOWN) {
				throw NoPathExistsException();
			}

			// reconstruct the path from parents
			vector<int> path;
			path.push_back(end);
			int current = end;
			while (current != start) {
				current = parents[current];
				path.push_back(current);
			}

			reverse(path.begin(), path.end());
			return { distances[end] , path };
		}
		
		
		void runDijkstra(int start) {
			auto comparator = [](const NodeAndDistance& p1, const NodeAndDistance& p2) {
				return p1.second > p2.second 
					or (p1.second == p2.second
						&& p1.first > p2.first);
			};

			priority_queue<
				NodeAndDistance,
				vector<NodeAndDistance>,
				decltype(comparator)> distanceQueue(comparator);

			vector<bool> visited(adjacencyList.size(), false);

			// handle the starting node
			distanceQueue.push({start, 0});
			parents[start];
			distances[start] = 0;

			// BFS:
			while (!distanceQueue.empty()) {
				// get the closest node from the queue
				auto& current = distanceQueue.top();
				distanceQueue.pop();

				int u = current.first;
				int distance = current.second;

				// do not visit a node more than once
				if (visited[u]) continue;

				visited[u] = true;

				for (auto& neibhor : adjacencyList[u]) {
					int v = neibhor.first, 
						weight = neibhor.second;

					// relax the edge (u, v)
					if (distances[v] == UNKNOWN 
						or distances[v] > distances[u] + weight) {

						parents[v] = u;
						distances[v] = distances[u] + weight;
						distanceQueue.push({ v, distances[v] });
					}
				}
			}
		}
	};
}

void testDijkstra() {
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

	Dijkstra dijkstra(adjacencyList);
	auto result = dijkstra.computeShortestPath(0, 2);
	int distance = result.first;
	vector<int> path02 = result.second;

	cout << "Shortest path from 0 to 2: ";
	for (int u : path02) {
		cout << u << " ";
	}
	cout << endl << "Length of the path: " << distance << endl;
}