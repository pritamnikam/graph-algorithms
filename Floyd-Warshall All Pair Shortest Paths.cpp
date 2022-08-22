/*
Floyd-Warshall All Pair Shortest Paths (APSP)

*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace {

    typedef vector<vector<int>> AdjacencyMatrix;

    class NoPathExistsException : public runtime_error {
    public:
        NoPathExistsException() : runtime_error("No path exists between the nodes.") {}
    };

    class FloydWarshall {
    private:
        AdjacencyMatrix distanceMatrix;
        AdjacencyMatrix predecessorMatrix;
        static constexpr int UNKNOWN = -1;
        // static constexpr int INFINITY = 1'000'000'000;

        void initialize(AdjacencyMatrix const& adjacencyMatrix) {
            int n = adjacencyMatrix.size();
            this->distanceMatrix = AdjacencyMatrix(n, vector<int>(n, INFINITY));
            this->predecessorMatrix = AdjacencyMatrix(n, vector<int>(n, UNKNOWN));
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (i == j) {
                        this->distanceMatrix[i][j] = 0;
                        this->predecessorMatrix[i][j] = i;
                    }
                    else if (adjacencyMatrix[i][j] > 0) {
                        this->distanceMatrix[i][j] = adjacencyMatrix[i][j];
                        this->predecessorMatrix[i][j] = i;
                    }
                }
            }
        }

        void compute() {
            int n = this->distanceMatrix.size();
            for (int k = 0; k < n; ++k) {
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < n; ++j) {
                        if (this->distanceMatrix[i][k] + this->distanceMatrix[k][j] < this->distanceMatrix[i][j]) {
                            this->distanceMatrix[i][j] = this->distanceMatrix[i][k] + this->distanceMatrix[k][j];
                            this->predecessorMatrix[i][j] = this->predecessorMatrix[k][j];
                        }
                    }
                }
            }
        }
    public:
        FloydWarshall(AdjacencyMatrix const& adjacencyMatrix) {
            this->initialize(adjacencyMatrix);
            this->compute();
        }

        pair<int, vector<int>> getShortestPath(int start, int end) {
            int distance = this->distanceMatrix[start][end];
            if (distance == INFINITY) {
                throw NoPathExistsException();
            }

            vector<int> path{ end };
            int current = end;
            while (current != UNKNOWN and current != start) {
                current = this->predecessorMatrix[start][current];
                path.push_back(current);
            }
            std::reverse(path.begin(), path.end());
            return make_pair(distance, path);
        }
    };
}

void testFloydWarshall() {
	int n = 4;
	AdjacencyMatrix adjacencyMatrix(n, vector<int>(n, 0));
	vector<vector<int>> edges{
		{0, 1, 6},
		{0, 2, 3},
		{0, 3, 12},
		{1, 2, 9},
		{1, 3, 5},
		{2, 3, 2},
		{3, 1, 4},
		{3, 2, 1}
	};

	for (auto& edge : edges) {
		adjacencyMatrix[edge[1]][edge[0]] = edge[2];
	}

	FloydWarshall floydWarshall(adjacencyMatrix);
	// auto [distance, path] = floydWarshall.getShortestPath(0, 3);
	auto result = floydWarshall.getShortestPath(0, 3);
	int distance = result.first;
	vector<int> path = result.second;

	cout << "Shortest path from 0 to 3:";
	for (int u : path) {
		cout << " " << u;
	}
	cout << endl << "Length of the path: " << distance << endl;

	try {
		floydWarshall.getShortestPath(2, 0);
	}
	catch (NoPathExistsException exc) {
		cout << "No path from 2 to 0." << endl;
	}
}