#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

/*
--- Part Two ---
Now, the Elves just need help accessing as much of the paper as they can.

Once a roll of paper can be accessed by a forklift, it can be removed. Once a roll of paper is removed, the forklifts might be able to access more rolls of paper, which they might also be able to remove. How many total rolls of paper could the Elves remove if they keep repeating this process?

Starting with the same example as above, here is one way you could remove as many rolls of paper as possible, using highlighted @ to indicate that a roll of paper is about to be removed, and using x to indicate that a roll of paper was just removed:
*/

using namespace std;

using Grid = vector<string>;

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

bool RollCanBeLifted(const Grid& presents, int i, int j) {
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

    return true;
}

size_t RollsLifted(const Grid& presents) {
    size_t result = 0;
    for (int i = 0; i < presents.size(); i++) {
        for (int j = 0; j < presents[i].size(); j++) {
            if (RollCanBeLifted(presents, i, j)) {
                result++;
            }
        }
    }
    return result;
}


int main() {
    Grid presents;
    if (ParseInput("input.txt", presents)) {
        return -1;
    }
    cout << RollsLifted(presents) << "\n";
    return 0;
}