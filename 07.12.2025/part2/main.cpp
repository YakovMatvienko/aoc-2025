#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>

/*
--- Part Two ---
With your analysis of the manifold complete, you begin fixing the teleporter. However, as you open the side of the teleporter to replace the broken manifold, you are surprised to discover that it isn't a classical tachyon manifold - it's a quantum tachyon manifold.

With a quantum tachyon manifold, only a single tachyon particle is sent through the manifold. A tachyon particle takes both the left and right path of each splitter encountered.

Since this is impossible, the manual recommends the many-worlds interpretation of quantum tachyon splitting: each time a particle reaches a splitter, it's actually time itself which splits. In one timeline, the particle went left, and in the other timeline, the particle went right.

To fix the manifold, what you really need to know is the number of timelines active after a single particle completes all of its possible journeys through the manifold.
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


using Coordinate = pair<size_t, size_t>;

size_t CalculateAllPaths(Diagram& diagram) {
    set<size_t> ongoingBeams;
    size_t source = diagram[0].find('S');
    if (source == string::npos) {
        return 0;
    }
    ongoingBeams.insert(source);
    size_t result = 0;
    map<Coordinate, size_t> heatMap;
    heatMap[make_pair(0, source)] = 1;

    for (size_t i = 1; i < diagram.size(); i++) {
        set<size_t> cloneBeams(ongoingBeams);
        set<size_t> newBeams;
        set<size_t> beamToRemove;
        for (size_t beam: cloneBeams) {
            Coordinate rootingCoordinate = make_pair(i - 1, beam);
            if (diagram[i][beam] == '^') {
                beamToRemove.insert(beam);
                newBeams.insert(beam - 1);
                newBeams.insert(beam + 1);
                diagram[i][beam - 1] = '|';
                diagram[i][beam + 1] = '|';
                Coordinate left = make_pair(i, beam - 1);
                if (heatMap[left]) {
                    heatMap[left] += heatMap[rootingCoordinate];
                }
                else {
                    heatMap[left] = heatMap[rootingCoordinate];
                }
                Coordinate right = make_pair(i, beam + 1);
                if (heatMap[right]) {
                    heatMap[right] += heatMap[rootingCoordinate];
                }
                else {
                    heatMap[right] = heatMap[rootingCoordinate];
                }
            }
            else if (diagram[i][beam] == '.') {
                diagram[i][beam] = '|';
                heatMap[make_pair(i, beam)] = heatMap[rootingCoordinate];
            }
            else if (diagram[i][beam] == '|') {
                heatMap[make_pair(i, beam)] += heatMap[rootingCoordinate];
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

    for (const auto& k : heatMap) {
        if (k.first.first != diagram.size() - 1) {
            continue;
        }
        result += k.second;
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
    cout << CalculateAllPaths(diagram) << "\n";
    Output(diagram);
    return 0;
}