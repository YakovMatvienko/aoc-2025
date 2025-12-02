#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

/*
--- Part Two ---
The clerk quickly discovers that there are still invalid IDs in the ranges in your list. Maybe the young Elf was doing other silly patterns as well?

Now, an ID is invalid if it is made only of some sequence of digits repeated at least twice. So, 12341234 (1234 two times), 123123123 (123 three times), 1212121212 (12 five times), and 1111111 (1 seven times) are all invalid IDs.

From the same example as before:

11-22 still has two invalid IDs, 11 and 22.
95-115 now has two invalid IDs, 99 and 111.
998-1012 now has two invalid IDs, 999 and 1010.
1188511880-1188511890 still has one invalid ID, 1188511885.
222220-222224 still has one invalid ID, 222222.
1698522-1698528 still contains no invalid IDs.
446443-446449 still has one invalid ID, 446446.
38593856-38593862 still has one invalid ID, 38593859.
565653-565659 now has one invalid ID, 565656.
824824821-824824827 now has one invalid ID, 824824824.
2121212118-2121212124 now has one invalid ID, 2121212121.
Adding up all the invalid IDs in this example produces 4174379265.

What do you get if you add up all of the invalid IDs using these new rules?
*/

using namespace std;

struct Range {
    unsigned long long lower;
    unsigned long long upper;
};

int ParseInput(const string& path, vector<Range>& ids) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    while (getline(file, line, ',')) {
        auto upper = line.substr(line.find('-') + 1);
        auto lower = line.substr(0, line.find('-'));
        Range r{stoull(lower), stoull(upper)};
        ids.push_back(r);
    }
    return 0;
}

bool SplitIntoGroups(const string& id, size_t groups, vector<unsigned long long>& splits) {
    size_t n = id.size();
    size_t baseSize = n / groups;
    size_t remainder = n % groups;

    size_t index = 0;
    for (size_t i = 0; i < groups; ++i) {
        size_t size = baseSize + (i < remainder ? 1 : 0);
        if (size != baseSize) {
            return false;
        }
        string part = id.substr(index, size);
        index += size;

        splits.push_back(std::stoull(part));
    }
    return true;
}

inline bool IdIsValid(unsigned long long id) {
    string converted(to_string(id));
    // splitting in several equal parts, beginning from 2
    size_t splitIn = 2;
    size_t len = converted.length() / splitIn;
    while (len) {
        vector<unsigned long long> splits;
        if (SplitIntoGroups(converted, splitIn, splits)) {
            if (adjacent_find(splits.begin(), splits.end(), not_equal_to<>()) == splits.end()) {
                return false;
            }
        }
        splitIn++;
        len = converted.length() / splitIn;
    }
    return true;
}

void AccumulateRangeIds(const Range& id, unsigned long long& result) {
    for (auto i = id.lower; i < id.upper; i++) {
        if (!IdIsValid(i)) { 
            result += i;
        }
    }
}

unsigned long long AccumulateInvalidIds(const vector<Range>& ids) {
    unsigned long long result = 0;
    for (const auto& id : ids) {
        AccumulateRangeIds(id, result);
    }
    return result;
}

int main() {
    vector<Range> ids;
    if (ParseInput("input.txt", ids)) {
        return -1;
    }
    cout << AccumulateInvalidIds(ids) << "\n";
    return 0;
}