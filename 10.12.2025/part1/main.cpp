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
#include <regex>
#include <queue>

/*

*/

using namespace std;

std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}

using Lights = uint64_t;
using Wiring = vector<size_t>;
using Joltage = vector<size_t>;
using Step = size_t;
using CurrentState = pair<Step, Lights>;

struct Machine {
    Lights lights;
    vector<Wiring> buttons;
    Joltage jolts;
};
using Machines = vector<Machine>;

void ToggleState(Wiring& button, size_t& currentState) {
    for_each(button.begin(), button.end(), [&](size_t b) { currentState = currentState ^ static_cast<size_t>(1 << b); });
}

size_t MinimalPressesForMachine(Machine& machine) {
    size_t result = 0;
    queue<CurrentState> states;
    set<Lights> stateReached;
    states.push({0, 0});
    for (;;) {
        CurrentState s = states.front();
        if (s.second == machine.lights) {
            return s.first;
        }
        states.pop();
        if (find(stateReached.begin(), stateReached.end(), s.second) != stateReached.end()) {
            continue;
        }
        stateReached.insert(s.second);
        for_each(machine.buttons.begin(), machine.buttons.end(), [&](Wiring& w) {
            size_t newState = s.second;
            ToggleState(w, newState);
            states.push(make_pair(s.first + 1, newState));
            });
    }
    return result;
}

size_t MinimalPressesForAllMachines(Machines& machines) {
    size_t result = 0;
    for_each(machines.begin(), machines.end(), [&](Machine& machine) { result += MinimalPressesForMachine(machine); });
    return result;
}

void ParseTargetLights(Machine& m, string& substr) {
    size_t target = 0;
    for (size_t i = 0; i < substr.size(); i++) {
        if (substr[i] == '#') {
            target |= static_cast<size_t>(1 << i);
        }
    }
    m.lights = target;
}

void ParseJoltageSubstr(Machine& m, string& substr) {
    auto numbers = split(substr, ',');
    for (const auto& num : numbers) {
        m.jolts.push_back(stoi(num));
    }
}

void ParseButtonsSubstr(Machine& m, string& substr) {
    size_t openPos = substr.find('(');
    size_t closePos = substr.find(')');
    while (openPos != string::npos && closePos != string::npos) {
        string buttons = substr.substr(openPos + 1, closePos - openPos - 1);
        Wiring w;
        auto numbers = split(buttons, ',');
        for (const auto& num : numbers) {
            w.push_back(stoi(num));
        }
        m.buttons.push_back(w);
        openPos = substr.find('(', openPos + 1);
        closePos = substr.find(')', closePos + 1);
    }
}

int ParseInput(const string& path, Machines& machines) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    while (getline(file, line)) {
        Machine m;
        string lightsSubstr = line.substr(1, line.find(']') - 1);
        ParseTargetLights(m, lightsSubstr);
        string buttonsSubstr = line.substr(line.find('('), line.rfind(')') - line.find('(') + 1);
        ParseButtonsSubstr(m, buttonsSubstr);
        string joltageSubstr = line.substr(line.find('{') + 1, line.find('}') - line.find('{') - 1);
        ParseJoltageSubstr(m, joltageSubstr);
        machines.push_back(m);
    }

    return 0;
}

int main() {
    Machines m;
    if (ParseInput("input.txt", m)) {
        return -1;
    }
    MinimalPressesForAllMachines(m);
    return 0;
}