#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

/*
--- Part Two ---
The escalator doesn't move. The Elf explains that it probably needs more joltage to overcome the static friction of the system and hits the big red "joltage limit safety override" button. You lose count of the number of times she needs to confirm "yes, I'm sure" and decorate the lobby a bit while you wait.

Now, you need to make the largest joltage by turning on exactly twelve batteries within each bank.

The joltage output for the bank is still the number formed by the digits of the batteries you've turned on; the only difference is that now there will be 12 digits in each bank's joltage output instead of two.

Consider again the example from before:

987654321111111
811111111111119
234234234234278
818181911112111
Now, the joltages are much larger:

In 987654321111111, the largest joltage can be found by turning on everything except some 1s at the end to produce 987654321111.
In the digit sequence 811111111111119, the largest joltage can be found by turning on everything except some 1s, producing 811111111119.
In 234234234234278, the largest joltage can be found by turning on everything except a 2 battery, a 3 battery, and another 2 battery near the start to produce 434234234278.
In 818181911112111, the joltage 888911112111 is produced by turning on everything except some 1s near the front.
The total output joltage is now much larger: 987654321111 + 811111111119 + 434234234278 + 888911112111 = 3121910778619.

What is the new total output joltage?
*/

using namespace std;

using batteries = vector<string>;

int ParseInput(const string& path, batteries& banks) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    while (getline(file, line)) {
        banks.push_back(line);
    }
    return 0;
}

string::const_iterator FindNthLargestDigit(const string& battery, string::const_iterator largestDigitIterator, size_t digitsLeft) {
    string::const_iterator largestDigitPointer = max_element(largestDigitIterator, battery.end() - digitsLeft, [](char a, char b) {
        return a < b;
        });
    return largestDigitPointer;
}

unsigned long long MaxBatteryVoltage(const string& battery) {
    vector<string::const_iterator> digits = {};
    digits.reserve(12);
    string::const_iterator pi = battery.begin();
    for (unsigned i = 0; i < 12; i++, pi++) {
        pi = FindNthLargestDigit(battery, pi, 11 - i);
        digits.push_back(pi);
    }
    string resultString;
    transform(digits.begin(), digits.end(), back_inserter(resultString), [&](string::const_iterator i) { return *i; });
    return stoull(resultString);
}

unsigned long long CalculateMaxVoltage(const batteries& banks) {
    unsigned long long result = 0;
    for (const auto& bank : banks) {
        result += MaxBatteryVoltage(bank);
    }
    return result;
}


int main() {
    batteries banks;
    if (ParseInput("input.txt", banks)) {
        return -1;
    }
    cout << CalculateMaxVoltage(banks) << "\n";
    return 0;
}