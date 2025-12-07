#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <set>

/*
* --- Day 7: Laboratories ---
You thank the cephalopods for the help and exit the trash compactor, finding yourself in the familiar halls of a North Pole research wing.

Based on the large sign that says "teleporter hub", they seem to be researching teleportation; you can't help but try it for yourself and step onto the large yellow teleporter pad.

Suddenly, you find yourself in an unfamiliar room! The room has no doors; the only way out is the teleporter. Unfortunately, the teleporter seems to be leaking magic smoke.

Since this is a teleporter lab, there are lots of spare parts, manuals, and diagnostic equipment lying around. After connecting one of the diagnostic tools, it helpfully displays error code 0H-N0, which apparently means that there's an issue with one of the tachyon manifolds.

You quickly locate a diagram of the tachyon manifold (your puzzle input). A tachyon beam enters the manifold at the location marked S; tachyon beams always move downward. Tachyon beams pass freely through empty space (.). However, if a tachyon beam encounters a splitter (^), the beam is stopped; instead, a new tachyon beam continues from the immediate left and from the immediate right of the splitter.
*/

using namespace std;

using Diagram = vector<string>;

int ParseInput(const string& path, Diagram& diagram) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    vector<string> rawLines;
    while (getline(file, line)) {
        diagram.push_back(line);
    }
    return 0;
}

size_t CalculateSplits(Diagram& diagram) {
    set<size_t> ongoingBeams;
    size_t source = diagram[0].find('S');
    if (source == string::npos) {
        return 0;
    }
    ongoingBeams.insert(source);
    size_t result = 0;

    for (size_t i = 1; i < diagram.size(); i++) {
        set<size_t> cloneBeams(ongoingBeams);
        set<size_t> newBeams;
        set<size_t> beamToRemove;
        for (size_t beam: cloneBeams) {
            if (diagram[i][beam] == '^') {
                result++;
                beamToRemove.insert(beam);
                newBeams.insert(beam - 1);
                newBeams.insert(beam + 1);
                diagram[i][beam - 1] = '|';
                diagram[i][beam + 1] = '|';
            }
            else if (diagram[i][beam] == '.') {
                diagram[i][beam] = '|';
            }
        }
        for (const auto& removeBeam : beamToRemove) {
            cloneBeams.erase(removeBeam);
        }
        if (!newBeams.empty()) {
            cloneBeams.insert(newBeams.begin(), newBeams.end());
        }
        ongoingBeams = cloneBeams;
    }

    return result;
}


void Output(Diagram& d) {
    ofstream f("output.txt");
    for (const auto& line : d) {
        f << line << '\n';
    }
    f.close();
}

int main() {
    Diagram diagram;
    if (ParseInput("input.txt", diagram)) {
        return -1;
    }
    cout << CalculateSplits(diagram) << "\n";
    Output(diagram);
    return 0;
}