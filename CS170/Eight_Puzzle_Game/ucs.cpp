#include "ucs.h"
#include "utils.h"
#include <iostream>
#include <queue>
#include <unordered_set>
#include <chrono>
using namespace std;

// Perform UCS
void uniformCostSearch(const vector<vector<int>>& initialState) {
    // Define nevessary quenes: one for the nodes and one for the visited states
    queue<pair<vector<vector<int>>, int>> nodes; // Queue of nodes
    unordered_set<string> visited; // Track visited states
    int maxQueueSize = 0; 
    int nodesExpanded = 0; 

    // Add the initial state to the queue
    nodes.push({initialState, 0});
    visited.insert(stateToString(initialState));

    // Track the time
    auto start = chrono::high_resolution_clock::now();

    // Loop to process states
    while (!nodes.empty()) {
        maxQueueSize = max(maxQueueSize, (int)nodes.size());

        auto current = nodes.front();
        nodes.pop();
        vector<vector<int>> currentState = current.first;
        int currentCost = current.second;

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
                nodes.push({child, currentCost + 1});
                visited.insert(childStr);
            }
        }

        // Otherwise, no solution found
    }
}