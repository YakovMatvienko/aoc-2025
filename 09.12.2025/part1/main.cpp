#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <tuple>

/*
--- Day 9: Movie Theater ---
You slide down the firepole in the corner of the playground and land in the North Pole base movie theater!

The movie theater has a big tile floor with an interesting pattern. Elves here are redecorating the theater by switching out some of the square tiles in the big grid they form. Some of the tiles are red; the Elves would like to find the largest rectangle that uses red tiles for two of its opposite corners. They even have a list of where the red tiles are located in the grid (your puzzle input).
*/

using namespace std;

using Coordinate = pair<uint64_t, uint64_t>;
using Coordinates = vector<Coordinate>;

int ParseInput(const string& path, Coordinates& data) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    while (getline(file, line)) {
        uint64_t x = stoi(line.substr(0, line.find(',')));
        uint64_t y = stoi(line.substr(line.find(',') + 1));
        data.push_back(make_pair(x, y));
    }
    return 0;
}

uint64_t Area(Coordinate& a, Coordinate& b) {
    uint64_t length = a.first > b.first ? a.first - b.first : b.first - a.first;
    length++;
    uint64_t width = a.second > b.second ? a.second - b.second : b.second - a.second;
    width++;
    return length * width;
}

uint64_t BiggestRectangle(Coordinates& data) {
    uint64_t result = 0;
    /*
    auto smallestX = max_element(data.begin(), data.end(), [&](Coordinate& a, Coordinate& b) { return a.first > b.first; });
    auto biggestX = max_element(data.begin(), data.end(), [&](Coordinate& a, Coordinate& b) { return a.first < b.first; });
    auto smallestY = max_element(data.begin(), data.end(), [&](Coordinate& a, Coordinate& b) { return a.second > b.second; });
    auto biggestY = max_element(data.begin(), data.end(), [&](Coordinate& a, Coordinate& b) { return a.second < b.second; });

    uint64_t areaByX = Area(*smallestX, *biggestX);
    uint64_t areaByY = Area(*biggestY, *smallestY);
    return max(areaByX, areaByY);
    */
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = i; j < data.size(); j++) {
            if (j == i) {
                continue;
            }
            uint64_t area = Area(data[i], data[j]);
            if (area > result) {
                result = area;
            }
        }
    }
    return result;
}

int main() {
    Coordinates data;
    if (ParseInput("input.txt", data)) {
        return -1;
    }
    uint64_t r = BiggestRectangle(data);
    cout << r << "\n";
    return 0;
}