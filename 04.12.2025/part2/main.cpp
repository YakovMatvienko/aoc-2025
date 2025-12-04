#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

/*
--- Day 4: Printing Department ---
You ride the escalator down to the printing department. They're clearly getting ready for Christmas; they have lots of large rolls of paper everywhere, and there's even a massive printer in the corner (to handle the really big print jobs).

Decorating here will be easy: they can make their own decorations. What you really need is a way to get further into the North Pole base while the elevators are offline.

"Actually, maybe we can help with that," one of the Elves replies when you ask for help. "We're pretty sure there's a cafeteria on the other side of the back wall. If we could break through the wall, you'd be able to keep moving. It's too bad all of our forklifts are so busy moving those big rolls of paper around."

If you can optimize the work the forklifts are doing, maybe they would have time to spare to break through the wall.

The rolls of paper (@) are arranged on a large grid; the Elves even have a helpful diagram (your puzzle input) indicating where everything is located.
The forklifts can only access a roll of paper if there are fewer than four rolls of paper in the eight adjacent positions. If you can figure out which rolls of paper the forklifts can access, they'll spend less time looking and more time breaking down the wall to the cafeteria.
*/

using namespace std;

using Grid = vector<string>;
using RemovedRolls = vector<pair<size_t, size_t>>;

int ParseInput(const string& path, Grid& presents) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    while (getline(file, line)) {
        presents.push_back(line);
    }
    return 0;
}

bool RollCanBeLifted(const Grid& presents, int i, int j, RemovedRolls& rolls) {
    // check if roll
    if (presents[i][j] == '.') {
        return false;
    }
    size_t adjacentRolls = 0;
    for (int ii = i - 1; ii < i + 2; ii++) {
        try {
            const auto& neighbourRow = presents.at(ii);
            for (int jj = j - 1; jj < j + 2; jj++) {
                try {
                    if (ii == i && jj == j) {
                        continue;
                    }
                    char cell = neighbourRow.at(jj);
                    if (cell == '@') {
                        adjacentRolls++;
                    }
                    if (adjacentRolls == 4) {
                        return false;
                    }
                }
                catch (out_of_range const& error) {}
            }
        }
        catch (out_of_range const& error) {}
        
    }
    rolls.push_back(make_pair<size_t, size_t>(i, j));
    return true;
}

size_t RollsLifted(const Grid& presents, RemovedRolls& rolls) {
    size_t result = 0;
    for (int i = 0; i < presents.size(); i++) {
        for (int j = 0; j < presents[i].size(); j++) {
            if (RollCanBeLifted(presents, i, j, rolls)) {
                result++;
            }
        }
    }
    return result;
}

void ClearGrid(Grid& presents, RemovedRolls& rolls) {
    if (!rolls.size()) {
        return;
    }
    for (const auto& coordinate : rolls) {
        presents[coordinate.first][coordinate.second] = '.';
    }
    rolls.clear();
}

size_t LiftRollsWithRemoval(Grid& presents) {
    RemovedRolls rolls;
    size_t result = 0;
    do {
        ClearGrid(presents, rolls);
        result += RollsLifted(presents, rolls);
    } while (rolls.size());
    return result;
}


int main() {
    Grid presents;
    if (ParseInput("input.txt", presents)) {
        return -1;
    }
    cout << LiftRollsWithRemoval(presents) << "\n";
    return 0;
}