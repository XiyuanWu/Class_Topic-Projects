#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
using namespace std;


extern const vector<vector<int>> goalState;
void printState(const vector<vector<int>>& state);
pair<int, int> findBlank(const vector<vector<int>>& state);
vector<vector<vector<int>>> expand(const vector<vector<int>>& state);
string stateToString(const vector<vector<int>>& state);

#endif
