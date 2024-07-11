/*
Fun fact: if p is an s-t path in a graph G(V,E), |V| = N then |p| <= N - 1,
otherwise there would be a circle and we don't want circles in the paths in this exercise.

Observation: So in this problem no matter what happens, we will skip 0, 1, ..., N - 1 edges in the s-t path
we find.

In the second series of written problems there was a problem about finding the minimum s-t path
when we can skip k edges and in that problem I used a "layered" approach. We are going to adapt
that algorithm to this problem.

Imagine there is an edge e = (u,v) in a path, then we have two options:
1. include the edge in the path and pay its weight (weight is travel time in this problem)
2. fast-travel across this edge and do not pay its weight

Our model works as follows:
vert(u,i) means the replica of vertex u found in the i-th copy of the original graph
In order to model edge e:
For 1: we include in the final graph the edge = (vert(u,i), vert(v,i)) 
       with weight = w(e) = w(u,v) for all i (aka for all "layers")
And for 2: we include in the final graph the edge = (vert(u,i), vert(v,i + 1)) with
           weight = 0 for all i except the maximum value (aka for all "layers" except the top layer)
If in our path we cross an edge of type 2 that means we decided to skip or fast-travel this edge
in the original problem.

How many layers do we need though? From our observation we conclude that we should use N layers. In other
words i = 0, 1, ..., N - 1.

In the final graph: Let's take a look at p(s,t,i) = path from vert(s,0) to vert(t,i), i = 0, 1, ..., N - 1.

If we reach vert(t, i) that means this path includes exactly i skipped or fast-travelled edges. In other words
the "layer" number i shows how many layers "up" we went or how many edges we skipped.

By finding p(s,t,i) for all i using a Shortest Path algorithm we make sure that the comparison we do with B 
(B is the parameter given in the input) is indeed valid. All that remains is to look for the p(s,t,i) value that is
no more than B and has the minimum i. That i is the return value.

This "layered" graph we created has O(N^2) vertices and O(N*M) edges.
We do not need to run a Dijkstra on the whole graph though. We can run Dijkstra on a layer, check to see if we've
managed our goal and if not go to the next layer.

Dijkstra on 1 layer is O(M*logM)
And we'll run it N times at most so O(N*M*logM) = O(N*M*logN) which should meet the time requirements for this problem.
*/

#include <queue> // priority_queue
#include <vector> // all the vectors we use
#include <forward_list> // used in the neighbor list for our graph
#include <iostream> // cout, cin etc.
#include <climits> // ULLONG_MAX

using namespace std;

using num = unsigned long long; // used for big values, mainly weights
using vertex = pair<int, int>; // (vertex name, layer) pairs (name, layer) pairs uniquely identify vertices in the layered graph
using neighbor = pair<int, int>; // (neighbor name, weight) pairs for our adjecency lists
using adjecency_list = forward_list<neighbor>; // our adjecency list...
using queue_element = pair<vertex, num>; // to what vertex we are connected and with what path-weight. Used in Dijkstra

// this function gets our inputs and creates the adjecency matrix
vector<adjecency_list> get_input(
    int& N, // number of vertices
    int& M, // number of edges
    int& s, // starting vertex
    int& t, // last vertex
    num& B  // maximum allowed path weight
    ) {

    cin >> N >> M >> s >> t >> B;

    --s; // format vertex name
    --t; // format vertex name

    // there are gonna be M edges
    vector<adjecency_list> adj(N);
    int u, v, w;
    for (int i = 0; i < M; ++i) {
        cin >> u >> v >> w;
        --u; // format vertex name
        --v; // format vertex name
        adj[u].push_front({v,w}); // add edge to adjecency list
    }
    return adj;
}

// This class exists solely to help us implement our priority queue...
// couldn't find another way to do this!
class my_comp
{
public:
    bool operator() (const queue_element& a, const queue_element& b) {
        return a.second > b.second; // second is the weight of the path
    }
};

// basically the C++ way of defining a min priority queue or, at least, I couldn't do better
using pr_q = priority_queue<queue_element, vector<queue_element>, my_comp>;

// Modified Dijkstra with our "layered" approach and using min priority queues
int shortest_path(int s, int t, num B, const vector<adjecency_list>& adj) {
    int N = adj.size(); // get number of vertices in the original graph
                        // a usefull constant
    vector<vector<num>> dist(N);
    // dist[u][layer] means distance of vertex (u,layer) from vertex (s,0) in the layered graph
    // set up Dijkstra distance vector or array
    for (vector<num>& vec: dist) {
        vec.assign(N, ULLONG_MAX);
    }
    dist[s][0] = 0; // our starting point

    pr_q main_queue; // Dijkstra on current layer
    pr_q next_layer_queue; // Dijkstra on the next layer
    next_layer_queue.push({{s, 0}, dist[s][0]}); // ((name, layer), distance)

    int result = -1;
    while (!next_layer_queue.empty()) {
        main_queue = next_layer_queue; // we have basically arrived at this layer now, so check these elements
        next_layer_queue = pr_q(); // new empty queue for elements on the next layer

        int u, layer; // (u,layer) is the vertex we will pop from main_queue
        while(!main_queue.empty()) {
            vertex popped = main_queue.top().first;
            main_queue.pop();

            u = popped.first; // name
            layer = popped.second; // layer

            num distance;
            for (const neighbor& n: adj[u]) {
                int v = n.first; // name
                int w = n.second; // weight

                distance = dist[u][layer] + w;
                if (dist[v][layer] > distance) {
                    dist[v][layer] = distance;
                    main_queue.push({{v, layer}, distance});
                } // Dijkstra for neighbors on the same layer as us

                if (layer == N - 1) continue; // the final layer has no next layer!

                distance = dist[u][layer]; // we skip this edge since we'll change layer
                if (dist[v][layer + 1] > distance) {
                    dist[v][layer + 1] = distance;
                    next_layer_queue.push({{v, layer + 1}, distance});
                } // Dijkstra for neighbors on the upper layer, we decided to skip (u,v) edge
            }
        }
        // after running Dijkstra on a layer we should check whether we're done or go to the next layer
        if (dist[t][layer] <= B) {
            result = layer;
            break;
        }
    }
    return result; // if we return -1, that means that there is no s-t path in the Graph
} // this algorithm behaves weirdly in results on upper layers if there are cycles in the original graph,
  // this however does not influence the correctness of the result! (if it finds a cheap path with a lot
  // of skipped edges that includes a cycle, then it would have found a better "true" path without a cycle earlier)

int main() {
    int N, M, s, t;
    num B;
    vector<adjecency_list> adj = get_input(N, M, s, t, B);
    cout << shortest_path(s, t, B, adj) << "\n";
}