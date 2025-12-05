#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <set>

/*
--- Day 5: Cafeteria ---
As the forklifts break through the wall, the Elves are delighted to discover that there was a cafeteria on the other side after all.

You can hear a commotion coming from the kitchen. "At this rate, we won't have any time left to put the wreaths up in the dining hall!" Resolute in your quest, you investigate.

"If only we hadn't switched to the new inventory management system right before Christmas!" another Elf exclaims. You ask what's going on.

The Elves in the kitchen explain the situation: because of their complicated new inventory management system, they can't figure out which of their ingredients are fresh and which are spoiled. When you ask how it works, they give you a copy of their database (your puzzle input).

The database operates on ingredient IDs. It consists of a list of fresh ingredient ID ranges, a blank line, and a list of available ingredient IDs. For example:

The fresh ID ranges are inclusive: the range 3-5 means that ingredient IDs 3, 4, and 5 are all fresh. The ranges can also overlap; an ingredient ID is fresh if it is in any range.

The Elves are trying to determine which of the available ingredient IDs are fresh. In this example, this is done as follows:

Ingredient ID 1 is spoiled because it does not fall into any range.
Ingredient ID 5 is fresh because it falls into range 3-5.
Ingredient ID 8 is spoiled.
Ingredient ID 11 is fresh because it falls into range 10-14.
Ingredient ID 17 is fresh because it falls into range 16-20 as well as range 12-18.
Ingredient ID 32 is spoiled.
So, in this example, 3 of the available ingredient IDs are fresh.

Process the database file from the new inventory management system. How many of the available ingredient IDs are fresh?
*/

using namespace std;

struct Range {
    unsigned long long min;
    unsigned long long max;
    const bool operator < (const Range& r) const {
        return min < r.min;
    }
};

using InputRange = vector<Range>;
using Products = vector<unsigned long long>;

int ParseInput(const string& path, InputRange& ranges, Products& ids) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            break;
        }

        auto upper = line.substr(line.find('-') + 1);
        auto lower = line.substr(0, line.find('-'));
        Range r{ stoull(lower), stoull(upper) };
        ranges.push_back(r);
    }
    while (getline(file, line)) {
        ids.push_back(stoull(line));
    }
    return 0;
}

bool FallsIntoAnyRange(unsigned long long id, const InputRange& ranges) {
    return any_of(ranges.begin(), ranges.end(), [&](const Range r) {
        return id >= r.min && id <= r.max;
        });
}

size_t CalculateFreshIngredients(const InputRange& ranges, const Products& ids) {
    size_t result = 0;

    for (const auto& id : ids) {
        if (FallsIntoAnyRange(id, ranges)) {
            result++;
        }
    }

    return result;
}

int main() {
    InputRange range;
    Products ids;
    if (ParseInput("input.txt", range, ids)) {
        return -1;
    }
    cout << CalculateFreshIngredients(range, ids) << "\n";
    return 0;
}