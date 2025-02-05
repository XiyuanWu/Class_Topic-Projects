#include <iostream>
#include "ucs.h"
#include "heu.h"
#include "mar.h"
#include "utils.h"

int main() {
    int option;
    int numberTestCases = 1;

    // Example initial states
    vector<vector<int>> initialState = {
        {1, 2, 3},
        {4, 5, 6},
        {0, 7, 8}
    };

    vector<vector<int>> goalState = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    // Test cases provided by documentation
    vector<vector<vector<int>>> testCases = {
        // Depth 0, 123456780
        {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}},
         // Depth 1, 123456078
        {{1, 2, 3}, {4, 5, 6}, {0, 7, 8}},
         // Depth 4, 123506478
        {{1, 2, 3}, {5, 0, 6}, {4, 7, 8}},
        // Depth 8, 136502478
        {{1, 3, 6}, {5, 0, 2}, {4, 7, 8}},
        // Depth 12, 136507482
        {{1, 3, 6}, {5, 0, 7}, {4, 8, 2}},
        // Depth 16, 167503482
        {{1, 6, 7}, {5, 0, 3}, {4, 8, 2}},
        // Depth 20, 712485630
        {{7, 1, 2}, {4, 8, 5}, {6, 3, 0}},
        // Depth 24, 072461358
        {{0, 7, 2}, {4, 6, 1}, {3, 5, 8}},
    };

    cout << "-----8 Puzzle Game-----" << endl;
    cout << "Use sample(1) or all test cases(2)?" << endl;
    cin >> option;

    if (option == 1) {

        /*---Sample output using sample initial state---*/
        cout << "Initial State:" << endl;
        printState(initialState);
        cout << endl;

        // Call UCS
        cout << "Uniform Cost Search:" << endl;
        uniformCostSearch(initialState);
        cout << endl;

        // Call A* with Misplaced Tile heuristic
        cout << "A* with Misplaced Tile heuristic:" << endl;
        aStarMisplaced(initialState);
        cout << endl;

        // Call A* with Manhattan Distance heuristic
        cout << "A* with Manhattan Distance heuristic:" << endl;
        aStarManhattan(initialState);
        cout << endl;
    } 
    
    else if (option == 2) {

        // output result for test cases
        for (const auto& testCase : testCases) {
            cout << "-----This is test case " << numberTestCases << "-----" << endl;
            printState(testCase);
            cout << endl;

            // Call UCS
            cout << "Uniform Cost Search:" << endl;
            uniformCostSearch(testCase);
            cout << endl;

            // Call A* with Misplaced Tile heuristic
            cout << "A* with Misplaced Tile heuristic:" << endl;
            aStarMisplaced(testCase);
            cout << endl;

            // Call A* with Manhattan Distance heuristic
            cout << "A* with Manhattan Distance heuristic:" << endl;
            aStarManhattan(testCase);
            cout << endl;

            cout << endl;
            numberTestCases++;
        }
    }
    else {
        cout << "Invalid option. Please enter 1 or 2." << endl;
    }





    return 0;
}

/*
To run the code:
g++ -o puzzle_solver main.cpp utils.cpp ucs.cpp heu.cpp mar.cpp
./puzzle_solver
*/
