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
#include <unordered_set>

/*

*/

using namespace std;

using Graph = map<string, vector<string>>;

size_t AllPossiblePasses(Graph& graph, const string& source, const string& destination) {
    size_t result = 0;
    string current = source;
    queue<string> path;
    vector<string> visited;
    path.push(source);
    while (!path.empty()) {
        current = path.front();
        path.pop();
        visited.push_back(current);
        for (const auto& next : graph[current]) {
            if (find(visited.begin(), visited.end(), next) != visited.end()) {
                continue;
            }
            if (next == destination) {
                result++;
                break;
            }
            path.push(next);
        }
    }
    return result;
}

int ParseInput(const string& path, Graph& graph) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    while (getline(file, line)) {
        string name = line.substr(0, line.find(":"));
        string nodes = line.substr(line.find(":") + 1);
        stringstream s(nodes);
        istream_iterator<string> begin(s);
        istream_iterator<string> end;
        vector<string> splitRow(begin, end);
        graph[name] = splitRow;
    }

    return 0;
}

int main() {
    Graph g;
    if (ParseInput("input.txt", g)) {
        return -1;
    }
    cout << AllPossiblePasses(g, "you", "out") << "\n";
    return 0;
}