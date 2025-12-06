#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <numeric>

/*
--- Day 6: Trash Compactor ---
After helping the Elves in the kitchen, you were taking a break and helping them re-enact a movie scene when you over-enthusiastically jumped into the garbage chute!

A brief fall later, you find yourself in a garbage smasher. Unfortunately, the door's been magnetically sealed.

As you try to find a way out, you are approached by a family of cephalopods! They're pretty sure they can get the door open, but it will take some time. While you wait, they're curious if you can help the youngest cephalopod with her math homework.

Cephalopod math doesn't look that different from normal math. The math worksheet (your puzzle input) consists of a list of problems; each problem has a group of numbers that need to either be either added (+) or multiplied (*) together.
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

int ParseInput(const string& path, Problems& problems) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    while (getline(file, line)) {
        stringstream s(line);
        istream_iterator<string> begin(s);
        istream_iterator<string> end;
        vector<string> splitRow(begin, end);
        if (!problems.size()) {
            problems.resize(splitRow.size());
        }
        for (size_t i = 0; i < splitRow.size(); i++) {
            bool isAction = splitRow[i] == "+" || splitRow[i] == "*";
            if (isAction) {
                problems[i].action = splitRow[i] == "+" ? Action::Addition : Action::Multiplication;
            }
            else {
                problems[i].element.push_back(stoull(splitRow[i]));
            }
        }
    }
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