#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

/*
--- Part Two ---
The Elves start bringing their spoiled inventory to the trash chute at the back of the kitchen.

So that they can stop bugging you when they get new inventory, the Elves would like to know all of the IDs that the fresh ingredient ID ranges consider to be fresh. An ingredient ID is still considered fresh if it is in any range.

Now, the second section of the database (the available ingredient IDs) is irrelevant. Here are the fresh ingredient ID ranges from the above example:

3-5
10-14
16-20
12-18
The ingredient IDs that these ranges consider to be fresh are 3, 4, 5, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, and 20. So, in this example, the fresh ingredient ID ranges consider a total of 14 ingredient IDs to be fresh.

Process the database file again. How many ingredient IDs are considered to be fresh according to the fresh ingredient ID ranges?
*/

using namespace std;

struct Range {
    unsigned long long min;
    unsigned long long max;
    const bool operator < (const Range& r) const {
        return min < r.min;
    }
    const bool operator == (const Range& r) const {
        return min == r.min && max == r.max;
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

void MergeOverlappingRanges(InputRange& ranges) {
    bool mergingContinues = false;
    do {
        mergingContinues = false;
        for (size_t i = 0; i < ranges.size(); i++) {
            Range r = ranges[i];
            auto rangeToMerge = find_if(ranges.begin() + i + 1, ranges.end(), [&](Range f) {
                return max(r.min, f.min) <= min(r.max, f.max);
                });
            if (rangeToMerge != ranges.end()) {
                Range mergedRange = { min(r.min, rangeToMerge->min), max(r.max, rangeToMerge->max) };
                ranges.erase(rangeToMerge);
                ranges.erase(remove(ranges.begin(), ranges.end(), ranges[i]), ranges.end());
                ranges.push_back(mergedRange);
                mergingContinues = true;
                break;
            }
        }
    } while (mergingContinues);
}

unsigned long long CalculateAllFreshIngredients(InputRange& ranges) {
    unsigned long long result = 0;
    InputRange mergedRanges;
    MergeOverlappingRanges(ranges);
    for (const auto& r : ranges) {
        result += r.max - r.min + 1;
    }
    return result;
}

int main() {
    InputRange range;
    Products ids;
    if (ParseInput("input.txt", range, ids)) {
        return -1;
    }
    cout << CalculateAllFreshIngredients(range) << "\n";
    return 0;
}