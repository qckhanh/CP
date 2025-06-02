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
const int inf = 1e9 + 7;
const int N = 1e6 + 7 ;
struct edge{
	int to;
	int capacity;
	int flow;
	int id;
};

struct Dinic{
	int inf;
	int numbVertex;
	vector<vector<edge>> adj;
	vector<int> level;
	int source, sink;
	
	
	Dinic(int _numbVertex, int _source, int _sink, int _inf ){
		numbVertex = _numbVertex;
		source = _source;
		sink = _sink;
		inf = _inf;
		adj.clear();
		level.clear();
		adj.resize(numbVertex);
		level.resize(numbVertex);
	}
	
	void addEdge(int u, int v, int weight){
		int id_u = (int)adj[u].size();
		int id_v = (int)adj[v].size();
		
		adj[u].push_back((edge){
			v, weight, 0, id_u
		});
		
		adj[v].push_back((edge){
			u, 0, 0, id_v
		});
	}
	
	bool bfs(){
		for(int i = 0; i < numbVertex; i++) level[i] = -1;
		
		queue<int> myqueue;
		myqueue.push(source);
		
		level[source] = 0;
		
		while(!myqueue.empty()){
			int current = myqueue.front();
			cout << "Loop here" << endl;
			for(int i = 0; i < (int)adj[current].size(); i++){

				edge neighbor = adj[current][i];
				if(neighbor.flow < neighbor.capacity && level[neighbor.to] == -1){
					level[neighbor.to] = level[current] + 1;
					myqueue.push(neighbor.to);
				}
			}
		}
		
		return (level[sink] != -1);
	}
	
	int sendFlow(int u, int sink, int currentFlow, vector<int> &start){
		if(u == sink) return currentFlow;
		
		for(; start[u] < (int)adj[u].size(); start[u]++){
			edge &neighbor = adj[u][start[u]];
			
			if(level[neighbor.to] == level[u] + 1 && neighbor.flow < neighbor.capacity){
				int flow = sendFlow(neighbor.to, sink, min(currentFlow, neighbor.capacity - neighbor.flow), start);
				if(flow > 0){
					neighbor.flow += flow;
					edge &reverseEdge = adj[neighbor.to][neighbor.id];
					reverseEdge.flow -= flow;
					return flow;
				}
				
			}
		}
		return 0;
	} 
	
	int dinicMaxFlow(){
		int sum = 0LL;
		while(!bfs()){
			vector<int> start(numbVertex, 0);
			int flow = sendFlow(source, sink, inf, start);
			if(flow == 0) break;
			sum += flow;
		}
		return sum;
	}
};

int n, m, source, sink;

int main(){
	ios :: sync_with_stdio(0);
	cin.tie(0); cout.tie(0);

	freopen(TASK".inp", "r", stdin);
//	freopen(TASK".out", "w", stdout);
	
	cin>>n>>m;
	source = 1;
	sink = n;
	Dinic dinic(n + 5, source, sink, 1e9 + 7);
	
	for(int i = 1; i <= m; i++){
		int u, v, weight;	
		cin>>u>>v>>weight;
		dinic.addEdge(u, v, weight);	
	}
	
	int maxFlow = dinic.dinicMaxFlow();
	
	vector<tuple<int, int, int>> listEdges;
	
	for(int u = 0; u < n; u++){
		for(int i = 0; i < (int)dinic.adj[u].size(); i++){
			edge &e = dinic.adj[u][i];
			
			if(e.flow > 0){
				listEdges.push_back(make_tuple(u, e.to, e.flow));
			}
		}
	}
	
	cout << maxFlow;



	return 0;
}


