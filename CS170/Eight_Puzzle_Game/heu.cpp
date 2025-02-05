#include "heu.h"
#include "utils.h"
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

// Node structure for the A* algorithm without pointer
struct Node {
    vector<vector<int>> state; 
    int g_cost; 
    int h_cost; 

    // Constructor
    Node(vector<vector<int>> s, int g, int h)
        : state(move(s)), g_cost(g), h_cost(h) {}

    // Total cost f(n) = g(n) + h(n)
    int f_cost() const { return g_cost + h_cost; }

    // Comparator for priority queue
    bool operator<(const Node& other) const {
        return f_cost() > other.f_cost(); // Min-heap based on f(n)
    }
};

// Find misplace tile
int calculateMisplacedTiles(const vector<vector<int>>& state, const vector<vector<int>>& goalState) {
    int h_cost = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (state[i][j] != 0 && state[i][j] != goalState[i][j]) {
                ++h_cost;
            }
        }
    }
    return h_cost;
}

// Perform A* with Misplaced Tile heuristic
void aStarMisplaced(const vector<vector<int>>& initialState) {
    priority_queue<Node> frontier;  // Here is 'Node' type, priority queue will automatically manage the nodes like f_cost
    unordered_set<string> visited; 
    int nodesExpanded = 0; 
    int maxQueueSize = 0; 

    // Add the initial state to the queue
    frontier.push(Node(initialState, 0, calculateMisplacedTiles(initialState, goalState)));
    visited.insert(stateToString(initialState));

    // Track the time
    auto start = chrono::high_resolution_clock::now();

    // Loop to process states
    while (!frontier.empty()) {
        maxQueueSize = max(maxQueueSize, (int)frontier.size());
        
        // Only get smallest f_cost node
        Node current = frontier.top(); 
        frontier.pop();

        vector<vector<int>> currentState = current.state;
        int currentCost = current.g_cost;

        // Check if the current state is the goal state
        if (currentState == goalState) {
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = end - start;

            cout << "Solution depth: " << currentCost << endl;
            cout << "Nodes expanded: " << nodesExpanded << endl;
            cout << "Max queue size: " << maxQueueSize << endl;
            cout << "Runtime: " << elapsed.count() << " seconds" << endl;
            return;
        }

        // Else, expand the current state and add to visited
        vector<vector<vector<int>>> children = expand(currentState);
        nodesExpanded++;
        for (const auto& child : children) {
            string childStr = stateToString(child);
            if (visited.find(childStr) == visited.end()) {
                frontier.push(Node(child, currentCost + 1, calculateMisplacedTiles(child, goalState)));
                visited.insert(childStr);
            }
        }

        // Otherwise, no solution found
    }
}