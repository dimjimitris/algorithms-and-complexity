#include <iostream>
#include <vector>
#include <utility>
#include <stack>
#include <climits>
#include <fstream>

using namespace std;

using Big = unsigned long long;
using Num = unsigned;
using Neighbor = pair<Num, Big>;
using Adj = vector<vector<Neighbor> >;

/* A very simple Graph structure to hold the input */
class Graph
{
public:
    Num N;
    Adj adj;
    vector<Num> P;
    vector<Num> S;

    /* used to read input and create an appropriate
        * Graph. Node names are in [0..N-1] */
    Graph(istream& in)
    {
        in >> N;
        adj.resize(N);
        P.resize(N);
        S.resize(N);
        P[0] = S[0] = 0;

        Num u, v;
        Big w;

        for (Num i = 0; i < N - 1; ++i)
        {
            in >> u >> v >> w;
            --u; // naming scheme!
            --v; // naming scheme!
            adj[u].push_back(make_pair(v,w));
            adj[v].push_back(make_pair(u,w));
            // update adjecency lists
        }

        for (Num i = 1; i < N; ++i)
        {
            in >> P[i] >> S[i];
        }
    }
};

/* We'll need to turn our Tree into a DAG (always doable for a tree)
 * to help us with this problem */
class DAG
{
public:
    Num N;
    Adj adj;
    vector<Num> P;
    vector<Num> S;

    /* DFS to create the DAG */
    DAG(Graph& G)
    {
        N = G.N;
        adj.resize(N);
        P = G.P; // copy
        S = G.S; // copy

        vector<bool> visited(N, false);

        stack<Num> stack;
        stack.push(0);

        Num s;
        while (!stack.empty())
        {
            s = stack.top();
            stack.pop();

            if (visited[s]) continue;

            visited[s] = true;

            for (auto x : G.adj[s])
            {
                if (!visited[x.first])
                {
                    adj[s].push_back(x); // copy of x
                    stack.push(x.first);
                }
            }
        } 
    }

    /* used for checking errors and stuff */
    void printDAG()
    {
        for (Num i = 0; i < N; ++ i)
            cout << P[i] << "  ";
        cout << endl;

        for (Num i = 0; i < N; ++ i)
            cout << S[i] << "  ";
        cout << endl;

        for (Num v = 0; v < N; ++v) {
            cout << "\nAdjecency list of vertex " << v << "\nhead";

            for (auto x: adj[v])
            cout << "-> " << "(" << get<0>(x) << ", " << get<1>(x) << ")";
        }
        cout << endl;
    }

    /* Topological sort needed to find shortest paths */
    stack<Num> topologicalSort()
    {
        stack<Num> tS;
        vector<bool> visited(N,false);
        stack<pair<Num, bool> > stack;

        stack.push(make_pair(0,false));

        /* this algorithm implements the idea with the departure
         * times without actually counting them, but with a the
         * boolean added to the pairs we use in the stack */
        while(!stack.empty())
        {
            Num s = stack.top().first;
            bool done = stack.top().second;
            stack.pop();

            if (done)
            {
                tS.push(s);
                continue;
            }

            if (visited[s]) continue;

            visited[s] = true;
            stack.push(make_pair(s, true));

            for (auto x : adj[s])
            {
                if (!visited[x.first])
                {
                    stack.push(make_pair(x.first,false));
                }
            }
        }
        return tS;
    }

    /* this DAG is a tree, we do not need the visited array in
     * this DFS. What this function does is put an edge from
     * a node to all reachable nodes in the tree */
    void helpCompleteDAG(Num s)
    {
        stack<Neighbor> stack; // we are gonna be carrying weight

        stack.push(make_pair(s, 0));

        while(!stack.empty())
        {
            Num next = stack.top().first;
            Big carry_weight = stack.top().second;
            stack.pop();

            for(auto x : adj[next])
            {
                Num node = x.first;
                Big edge_weight = x.second;
                stack.push(make_pair(node, carry_weight + edge_weight));
                if (carry_weight != 0) // if the carry weight is 0, the
                //edge already exists in the DAG, no need to add it manually
                    adj[s].push_back(make_pair(node, carry_weight + edge_weight));
            }
        }
    }

    /* the classic shortest path algorithm on DAGS, but first
     * we topologically sort the nodes and then complete the DAG with
     * the above function so that we include the 'detours' */
    vector<Big> shortestPathOnDAG()
    {
        stack<Num> tS, tS_complete_DAG;
        tS = tS_complete_DAG = topologicalSort(); // copies

        while(!tS_complete_DAG.empty())
        {
            Num s = tS_complete_DAG.top();
            tS_complete_DAG.pop();
            helpCompleteDAG(s);
        }
        // now we're ready to do the shortest path algorithm
        vector<Big> D(N, ULLONG_MAX);
        D[0] = 0;

        while(!tS.empty())
        {
            Num u = tS.top();
            tS.pop();

            for (auto x : adj[u])
            {
                Num v = x.first;
                Big w = x.second;
                if (D[v] > D[u] + P[v] + S[v] * w)
                    D[v] = D[u] + P[v] + S[v] * w;
            } // slight modification to shortest path to accomodate
              // our problem
        }
        return D;
    }
};


/*
int main(){
    Graph H(cin); // O(N)
    DAG G(H); // O(N)

    vector<Big> D = G.shortestPathOnDAG();
    // O(N) + O(N^2) + O(N^2) = O(N^2)

    for (Num i = 1; i < G.N - 1; ++i) // O(N)
        cout << D[i] << " ";
    cout << D[G.N - 1] << endl;

}
// In most methods we use an implementation of DFS, which does
// a particular function on the Graph, which we've seen in lectures
*/

int main(){
    int inputs;
    cin >> inputs;

    for (int i = 1; i <= inputs; ++i) {
        ifstream file("relay/input" + to_string(i) + ".txt");

        Graph H(file);
        DAG G(H);

        vector<Big> D = G.shortestPathOnDAG();

        ifstream solution("relay/output" + to_string(i) + ".txt");
        vector<Big> ans(G.N);
        ans[0] = 0;
        for (int i = 1; i < G.N; ++i)
            solution >> ans[i];
        
        bool flag = true;
        for (Num i = 0; i < G.N; ++i)
            if (D[i] != ans[i]) flag = false;

        cout << "Input " << i << ((flag) ? " right." : " wrong.") << endl;    
    }
}
