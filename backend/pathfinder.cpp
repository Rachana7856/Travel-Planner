#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <string>
#include <algorithm>
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

const int INF = numeric_limits<int>::max();

struct Edge {
    string to;
    int weight;
};

unordered_map<string, vector<Edge>> graph;

void loadGraph(const string& filename) {
    ifstream file(filename);
    string from, to;
    int weight;
    while (file >> from >> to >> weight) {
        graph[from].push_back({to, weight});
        graph[to].push_back({from, weight});
    }
}

pair<vector<string>, int> dijkstra(const string& start, const string& end) {
    unordered_map<string, int> dist;
    unordered_map<string, string> prev;
    for (auto& node : graph) dist[node.first] = INF;
    dist[start] = 0;

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [curDist, u] = pq.top(); pq.pop();
        if (u == end) break;
        for (auto& edge : graph[u]) {
            int alt = dist[u] + edge.weight;
            if (alt < dist[edge.to]) {
                dist[edge.to] = alt;
                prev[edge.to] = u;
                pq.push({alt, edge.to});
            }
        }
    }

    vector<string> path;
    for (string at = end; at != ""; at = prev[at]) path.push_back(at);
    reverse(path.begin(), path.end());

    return {path, dist[end]};
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Missing source or destination\n";
        return 1;
    }

    string start = argv[1];
    string end = argv[2];

    loadGraph("graph.txt");

    auto [path, distance] = dijkstra(start, end);

    json response;
    response["path"] = path;
    response["distance"] = distance;

    cout << response.dump() << endl;

    return 0;
}
