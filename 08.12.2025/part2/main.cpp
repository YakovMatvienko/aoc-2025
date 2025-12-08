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

/*
The Elves were right; they definitely don't have enough extension cables. You'll need to keep connecting junction boxes together until they're all in one large circuit.

Continuing the above example, the first connection which causes all of the junction boxes to form a single circuit is between the junction boxes at 216,146,977 and 117,168,530. The Elves need to know how far those junction boxes are from the wall so they can pick the right extension cable; multiplying the X coordinates of those two junction boxes (216 and 117) produces 25272.

Continue connecting the closest unconnected pairs of junction boxes together until they're all in the same circuit. What do you get if you multiply together the X coordinates of the last two junction boxes you need to connect?
*/

using namespace std;

using Coordinate = tuple<int64_t, int64_t, int64_t>;
using Data = vector<Coordinate>;
using Circuit = set<Coordinate>;
using Line = pair<Coordinate, Coordinate>;
vector<Circuit> circuits;
map<pair<Coordinate, Coordinate>, int64_t> distanceMap;
vector<pair<int64_t, Line>> distancesToCoords;

int64_t CalculateDistance(Coordinate& a, Coordinate& b) {
    int64_t xDist = powl((get<0>(b) - get<0>(a)), 2);
    int64_t yDist = powl((get<1>(b) - get<1>(a)), 2);
    int64_t zDist = powl((get<2>(b) - get<2>(a)), 2);
    return sqrt(xDist + yDist + zDist);
}

int ParseInput(const string& path, Data& data) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    while (getline(file, line)) {
        size_t firstComma = line.find(',');
        size_t secondComma = line.find(',', firstComma + 1);
        Coordinate jbox;
        int64_t x = stoll(string(line.begin(), line.begin() + firstComma));
        int64_t y = stoll(string(line.begin() + firstComma + 1, line.begin() + secondComma));
        int64_t z = stoll(string(line.begin() + secondComma + 1, line.end()));
        jbox = make_tuple(x, y, z);
        data.push_back(jbox);
    }
    return 0;
}

void ConnectClosestBoxes(Data& data) {
    int64_t closestDistance = INT64_MAX;
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data.size(); j++) {
            if (i == j) {
                continue;
            }
            Line line = make_pair(data[i], data[j]);
            Line reverse = make_pair(data[j], data[i]);
            int64_t dist = distanceMap[line];
            if (!dist) {
                dist = CalculateDistance(data[i], data[j]);
                distanceMap[line] = dist;
                distanceMap[reverse] = dist;
                distancesToCoords.push_back(make_pair(dist, line));
            }
        }
    }
    sort(distancesToCoords.begin(), distancesToCoords.end(), [](pair<int64_t, Line>& a, pair<int64_t, Line>& b) { return a.first < b.first; });
}

void CreateConnections(Data& data, size_t amountOfConnections, Line* lastMerged = nullptr) {
    ConnectClosestBoxes(data);

    size_t maxConnections = min(amountOfConnections, distancesToCoords.size());
    for (size_t i = 0; i < maxConnections; i++) {
        auto& c1 = distancesToCoords[i].second.first;
        auto& c2 = distancesToCoords[i].second.second;
        auto circuitA = find_if(circuits.begin(), circuits.end(), [&](Circuit& c) { return find(c.begin(), c.end(), c1) != c.end(); });
        auto circuitB = find_if(circuits.begin(), circuits.end(), [&](Circuit& c) { return find(c.begin(), c.end(), c2) != c.end(); });
        if (circuitA == circuitB && circuitA == circuits.end()) {
            Circuit newCircuit;
            newCircuit.insert(c1);
            newCircuit.insert(c2);
            circuits.push_back(newCircuit);
            continue;
        }
        else if (circuitA == circuitB) {
            continue;
        }
        else if (circuitA != circuits.end() && circuitB != circuits.end()) {
            circuitA->insert(circuitB->begin(), circuitB->end());
            circuits.erase(circuitB);
            if (lastMerged) {
                *lastMerged = make_pair(c1, c2);
            }
            continue;
        }
        if (circuitA != circuits.end()) {
            circuitA->insert(c2);
            if (lastMerged) {
                *lastMerged = make_pair(c1, c2);
            }
        }
        if (circuitB != circuits.end()) {
            circuitB->insert(c1);
            if (lastMerged) {
                *lastMerged = make_pair(c1, c2);
            }
        }
    }
}

uint64_t CalculateLargestCircuits(Data& data) {
    uint64_t result = 0;
    CreateConnections(data, 10);
    vector<size_t> sizes;
    for (const auto& circuit : circuits) {
        sizes.push_back(circuit.size());
    }
    sort(sizes.begin(), sizes.end(), [&](size_t a, size_t b) { return a > b; });
    result = sizes[0] * sizes[1] * sizes[2];
    cout << "result " << result << "\n";
    return result;
}

uint64_t HowFarAreLastCoords(Data& data) {
    uint64_t result = 0;
    Line lastMerge;
    CreateConnections(data, UINT32_MAX, &lastMerge);
    result = get<0>(lastMerge.first) * get<0>(lastMerge.second);
    return result;
}

int main() {
    Data jbox;
    if (ParseInput("input.txt", jbox)) {
        return -1;
    }
    HowFarAreLastCoords(jbox);
    return 0;
}