#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <numeric>

/*
--- Part Two ---
The big cephalopods come back to check on how things are going. When they see that your grand total doesn't match the one expected by the worksheet, they realize they forgot to explain how to read cephalopod math.

Cephalopod math is written right-to-left in columns. Each number is given in its own column, with the most significant digit at the top and the least significant digit at the bottom. (Problems are still separated with a column consisting only of spaces, and the symbol at the bottom of the problem is still the operator to use.)
*/

using namespace std;

enum class Action {
    Addition,
    Multiplication
};

struct Problem {
    vector <uint64_t> element;
    Action action;
};

using Problems = vector<Problem>;

void RearrangeElements(vector<string>& rawLines, Problems& problems) {
    size_t lineLength = rawLines[0].size();
    size_t problemAssigned = 0;
    for (size_t i = 0; i < lineLength; i++) {
        uint64_t rearrangedElement = 0;
        for (size_t nextRow = 0; nextRow < rawLines.size(); nextRow++) {
            char nextChar = rawLines[nextRow][i];
            if (isdigit(nextChar)) {
                rearrangedElement *= 10;
                rearrangedElement += rawLines[nextRow][i] - '0';
                if (nextRow == rawLines.size() - 1) {
                    problems[problemAssigned].element.push_back(rearrangedElement);
                }
            }
            else if (rearrangedElement) {
                problems[problemAssigned].element.push_back(rearrangedElement);
                break;
            }
        }
        if (!rearrangedElement) {
            problemAssigned++;
        }
    }
}

int ParseInput(const string& path, Problems& problems) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    vector<string> rawLines;
    while (getline(file, line)) {
        stringstream s(line);
        istream_iterator<string> begin(s);
        istream_iterator<string> end;
        vector<string> splitRow(begin, end);
        if (!problems.size()) {
            problems.resize(splitRow.size());
        }

        bool isActionRow = false;
        for (size_t i = 0; i < splitRow.size(); i++) {
            isActionRow = splitRow[i] == "+" || splitRow[i] == "*";
            if (isActionRow) {
                problems[i].action = splitRow[i] == "+" ? Action::Addition : Action::Multiplication;
            }
        }
        if (!isActionRow) {
            rawLines.push_back(line);
        }
    }
    RearrangeElements(rawLines, problems);
    return 0;
}

uint64_t SolveIndividualProblem(const Problem& problem) {
    switch (problem.action) {
        case Action::Addition:
            return accumulate(problem.element.begin(), problem.element.end(), uint64_t(0));
            break;
        case Action::Multiplication:
            return accumulate(problem.element.begin(), problem.element.end(), uint64_t(1), [&](uint64_t a, uint64_t b) { return a * b; });
            break;
        default:
            throw exception("");
    }

}

uint64_t SolveProblems(const Problems& problems) {
    uint64_t result = 0;

    for (const auto& problem : problems) {
        result += SolveIndividualProblem(problem);
    }

    return result;
}

int main() {
    Problems problems;
    if (ParseInput("input.txt", problems)) {
        return -1;
    }
    cout << SolveProblems(problems) << "\n";
    return 0;
}