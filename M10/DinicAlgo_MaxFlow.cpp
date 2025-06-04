#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include<unordered_map>
#include<string>
#include<sstream>
#include<limits>
#include<tuple>
#include<random>

using namespace std;
#define TASK "input"
#define ll long long
#define fi first
#define se second
#define v vector
#define p pair
#define mp make_pair
#define pb push_back
#pragma GCC optimize("O2")
#pragma GCC target("avx,avx2,fma")
const int mod = 1e9 + 7;
const int MAX_CAPACITY = 1e9 + 7;
const int MAX_NODES = 1e6 + 7;

struct Edge {
    int destination;
    int capacity;
    int flow;
    int reverseEdgeIndex;
};

struct DinicMaxFlow {
    int maxCapacity;
    int numNodes;
    vector<vector<Edge>> adjacencyList;
    vector<int> distance;
    int sourceNode, sinkNode;

    DinicMaxFlow(int nodeCount, int source, int sink, int maxCap) {
        numNodes = nodeCount;
        sourceNode = source;
        sinkNode = sink;
        maxCapacity = maxCap;
        adjacencyList.clear();
        distance.clear();
        adjacencyList.resize(numNodes);
        distance.resize(numNodes);
    }

    void addDirectedEdge(int from, int to, int edgeCapacity) {
        int fromEdgeIndex = (int)adjacencyList[from].size();
        int toEdgeIndex = (int)adjacencyList[to].size();

        adjacencyList[from].push_back((Edge){
            to, edgeCapacity, 0, toEdgeIndex
        });

        adjacencyList[to].push_back((Edge){
            from, 0, 0, fromEdgeIndex
        });
    }

    bool buildLevelGraph() {
        for(int i = 0; i < numNodes; i++) distance[i] = -1;

        queue<int> nodeQueue;
        nodeQueue.push(sourceNode);
        distance[sourceNode] = 0;

        while(!nodeQueue.empty()) {
            int currentNode = nodeQueue.front();
            nodeQueue.pop();
            for(int i = 0; i < (int)adjacencyList[currentNode].size(); i++) {
                Edge neighbor = adjacencyList[currentNode][i];
                if(neighbor.flow < neighbor.capacity && distance[neighbor.destination] == -1) {
                    distance[neighbor.destination] = distance[currentNode] + 1;
                    nodeQueue.push(neighbor.destination);
                }
            }
        }

        return (distance[sinkNode] != -1);
    }

    ll augmentPath(int currentNode, int targetSink, int currentFlow, vector<int> &nextEdgeIndex) {
        if(currentNode == targetSink) return currentFlow;

        for(; nextEdgeIndex[currentNode] < (int)adjacencyList[currentNode].size(); nextEdgeIndex[currentNode]++) {
            Edge &neighbor = adjacencyList[currentNode][nextEdgeIndex[currentNode]];

            if(distance[neighbor.destination] == distance[currentNode] + 1 && neighbor.flow < neighbor.capacity) {
                int flow = augmentPath(neighbor.destination, targetSink, min(currentFlow, neighbor.capacity - neighbor.flow), nextEdgeIndex);
                if(flow > 0) {
                    neighbor.flow += flow;
                    Edge &reverseEdge = adjacencyList[neighbor.destination][neighbor.reverseEdgeIndex];
                    reverseEdge.flow -= flow;
                    return flow * 1LL;
                }
            }
        }
        return 0LL;
    }

    ll computeMaxFlow() {
        ll totalFlow = 0LL;
        while(buildLevelGraph()) {
            while(true) {
                vector<int> nextEdgeIndex(numNodes, 0);
                ll flow = augmentPath(sourceNode, sinkNode, maxCapacity, nextEdgeIndex);
                if(flow == 0) break;
                totalFlow += flow;
            }
        }
        return totalFlow;
    }
};

int numNodes, numEdges, sourceNode, sinkNode;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

//    freopen(TASK".inp", "r", stdin);

    cin >> numNodes >> numEdges;
    sourceNode = 1;
    sinkNode = numNodes;
    DinicMaxFlow dinic(numNodes + 5, sourceNode, sinkNode, MAX_CAPACITY);

    for(int i = 1; i <= numEdges; i++) {
        int fromNode, toNode, edgeCapacity;
        cin >> fromNode >> toNode >> edgeCapacity;
        dinic.addDirectedEdge(fromNode, toNode, edgeCapacity);
    }

    ll maxFlow = dinic.computeMaxFlow();

    vector<tuple<int, int, int>> usedEdges;

    for(int node = 0; node < numNodes; node++) {
        for(int i = 0; i < (int)dinic.adjacencyList[node].size(); i++) {
            Edge &edge = dinic.adjacencyList[node][i];

            if(edge.flow > 0) {
                usedEdges.push_back(make_tuple(node, edge.destination, edge.flow));
            }
        }
    }

    cout << maxFlow;

    return 0;
}
