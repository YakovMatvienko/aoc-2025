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
#include <array>
#include <ppl.h>
#include <windows.h>

/*
--- Part Two ---
The Elves just remembered: they can only switch out tiles that are red or green. So, your rectangle can only include red or green tiles.

In your list, every red tile is connected to the red tile before and after it by a straight line of green tiles. The list wraps, so the first red tile is also connected to the last red tile. Tiles that are adjacent in your list will always be on either the same row or the same column.
*/

using namespace std;

using Coordinate = pair<uint64_t, uint64_t>;
using Coordinates = vector<Coordinate>;
using Line = pair<Coordinate, Coordinate>;
using Bounds = vector<Line>;

int ParseInput(const string& path, Coordinates& data, Bounds& border) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    while (getline(file, line)) {
        uint64_t x = stoi(line.substr(0, line.find(',')));
        uint64_t y = stoi(line.substr(line.find(',') + 1));
        Coordinate newCoordinate = make_pair(x, y);
        data.push_back(newCoordinate);
        if (data.size() > 1) {
            Coordinate oldCoordinate = data[data.size() - 2];
            border.push_back(make_pair(oldCoordinate, newCoordinate));
        }
    }
    border.push_back(make_pair(data[data.size() - 1], data[0]));
    return 0;
}

bool LinesIntersect(Line& l1, Line& l2) {
    auto [minx1, maxx1] = minmax(l1.first.first, l1.second.first);
    auto [miny1, maxy1] = minmax(l1.first.second, l1.second.second);

    auto [minx2, maxx2] = minmax(l2.first.first, l2.second.first);
    auto [miny2, maxy2] = minmax(l2.first.second, l2.second.second);

    return maxx2 > minx1 && minx2 < maxx1 && maxy2 > miny1 && miny2 < maxy1;
}

bool LinesIntersect(Line& line, Bounds& borders) {
    return any_of(borders.begin(), borders.end(), [&](Line& l) { return LinesIntersect(l, line); });
}

bool RectangleOutOfBorder(Coordinate& a, Coordinate& b, Bounds& border) {
    Coordinate neighbour1{ min(a.first, b.first) + 1, min(a.second, b.second) + 1 };
    Coordinate neighbour2{ max(a.first, b.first) - 1, max(a.second, b.second) - 1 };
    Coordinate neighbour3{ neighbour1.first, neighbour2.second };
    Coordinate neighbour4{ neighbour2.first, neighbour1.second };
    Line ab{ neighbour1, neighbour2 };
    Line bc{ neighbour2, neighbour3 };
    Line cd{ neighbour3, neighbour4 };
    Line da{ neighbour4, neighbour1 };
    array<Line, 4> lines = { ab, bc, cd, da };

    return any_of(lines.begin(), lines.end(), [&](Line& l) { return LinesIntersect(l, border); });
}

uint64_t Area(Coordinate& a, Coordinate& b, Bounds& border) {
    if (RectangleOutOfBorder(a, b, border)) {
        return 0;
    }
    uint64_t length = a.first > b.first ? a.first - b.first : b.first - a.first;
    length++;
    uint64_t width = a.second > b.second ? a.second - b.second : b.second - a.second;
    width++;
    return length * width;
}

uint64_t BiggestRectangle(Coordinates& data, Bounds& borders) {
    uint64_t result = 0;
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = i; j < data.size(); j++) {
            if (j == i) {
                continue;
            }
            uint64_t area = Area(data[i], data[j], borders);
            if (area > result) {
                result = area;
            }
        }
    }
    return result;
}

int main() {
    Coordinates data;
    Bounds borders; 
    if (ParseInput("input.txt", data, borders)) {
        return -1;
    }
    uint64_t r = BiggestRectangle(data, borders);
    cout << r << "\n";
    return 0;
}