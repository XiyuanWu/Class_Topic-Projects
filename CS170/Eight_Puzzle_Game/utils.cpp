#include "utils.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Define goal state
const vector<vector<int>> goalState = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

// Print the current state
void printState(const vector<vector<int>>& state) {
    for (const auto& row : state) {
        for (int cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
    cout << "----" << endl;
}

// Find the blank piece position
pair<int, int> findBlank(const vector<vector<int>>& state) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (state[i][j] == 0) {
                return {i, j};
            }
        }
    }
    // Not fcound. should never happen technically
    return {-1, -1}; 
}

// Generate child states (expand)
vector<vector<vector<int>>> expand(const vector<vector<int>>& state) {
    vector<vector<vector<int>>> children;
    pair<int, int> blank = findBlank(state);
    int x = blank.first, y = blank.second;

    // Possible moves: up, down, left, right
    vector<pair<int, int>> moves = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto& move : moves) {
        int newX = x + move.first, newY = y + move.second;
        if (newX >= 0 && newX < 3 && newY >= 0 && newY < 3) {
            vector<vector<int>> newState = state;
            swap(newState[x][y], newState[newX][newY]);
            children.push_back(newState);
        }
    }
    return children;
}

// Convert state to string for hashable comparisons
string stateToString(const vector<vector<int>>& state) {
    string result;
    for (const auto& row : state) {
        for (int cell : row) {
            result += to_string(cell) + ",";
        }
    }
    return result;
}
