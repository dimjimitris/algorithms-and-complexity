#include <iostream>
#include <vector>
#include <tuple>
#include <typeinfo>
#include <stack>
#include <fstream>
#include <climits>

using namespace std;

using Big = unsigned long long; // or double ??
using Num = unsigned;
using Double = tuple<Num, Big>;
using Adj = vector< vector<Double> >;

class Graph
{
public:
    Num N;
    Adj adj;
    vector<Num> P;
    vector<Num> S;

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
            addEdge(u, v, w);
        }

        for (Num i = 1; i < N; ++i)
        {
            in >> P[i] >> S[i];
        }
    }

    inline void addEdge(Num u, Num v, Big w)
    {
        adj[u].push_back(make_tuple(v,w));
        adj[v].push_back(make_tuple(u,w));
    }

};

class DAG
{
public:
    Num N;
    Adj adj;
    vector<Num> P;
    vector<Num> S;
    
    /* we create the DAG with a modified DFS, since we know the
     * graph is a connected tree thus can be turned into a DAG
     * O(N) time complexity */
    DAG(Graph& G)
    {
        N = G.N;
        adj.resize(N);
        P = G.P;
        S = G.S;

        vector<bool> visited(N, false);

        stack<Num> stack;

        stack.push(0);

        while(!stack.empty())
        {
            Num s = stack.top();
            stack.pop();

            if (!visited[s])
            {
                visited[s] = true;
            }

            for (auto x : G.adj[s])
            {
                Num node = get<0>(x);
                Big edge_weight = get<1>(x);
                if (!visited[node])
                {
                    adj[s].push_back(make_tuple(node, edge_weight));
                    stack.push(node);
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

    /* this function will add an edge from from a vertex to
     * any other vertex that is reachable from it
     * it is O(N)
     * Once again it is a modified DFS */
    void helpCompleteDAG(Num s)
    {
        stack<Double> stack; // the second Num is the carry_weight

        stack.push(make_tuple(s,0));

        while(!stack.empty())
        {
            Num next = get<0>(stack.top());
            Big carry_w = get<1>(stack.top());
            stack.pop();

            for (auto x : adj[next])
            {
                Num node = get<0>(x);
                Big edge_weight = get<1>(x);
                stack.push(make_tuple(node, carry_w + edge_weight));
                if (carry_w != 0) // we have already included this edge
                                  // (it is the direct edge from s to next)
                    adj[s].push_back(make_tuple(node, carry_w + edge_weight));
            }
        }
    }

    stack<Num> topologicalSort()
    {
        stack<Num> tS;

        vector<bool> visited(N, false);

        stack<tuple<bool, Num> > stack;

        stack.push(make_tuple(false,0));

        while(!stack.empty())
        {
            Num done = get<0>(stack.top());
            Num s = get<1>(stack.top());
            stack.pop();

            if (done)
            {
                tS.push(s);
                continue;
            }

            if (visited[s]) continue;

            visited[s] = true;
            stack.push(make_tuple(true,s));

            for (auto x : adj[s])
            {
                Num node = get<0>(x);
                if (!visited[node])
                {
                    stack.push(make_tuple(false,node));
                }
            }
        }
        return tS;
    }

    vector<Big> shortestPathOnDAG()
    {
        stack<Num> tS, tS_complete_DAG;
        tS = tS_complete_DAG = topologicalSort();
        
        while(!tS_complete_DAG.empty())
        {
            Num s = tS_complete_DAG.top();
            tS_complete_DAG.pop();
            helpCompleteDAG(s);
        }
        
        vector<Big> D(N, ULLONG_MAX);
        D[0] = 0;

        while(!tS.empty())
        {
            Num u = tS.top();
            tS.pop();

            for (auto x : adj[u])
            {
                Num v = get<0>(x);
                Big w = get<1>(x);
                if (D[v] > D[u] + P[v] + S[v] * w)
                    D[v] = D[u] + P[v] + S[v] * w;
            }
        }
        return D;
    }
};
/*
int main(){
    Graph H(cin);
    DAG G(H);

    vector<Big> D = G.shortestPathOnDAG();

    for (Num i = 1; i < G.N - 1; ++i)
        cout << D[i] << " ";
    cout << D[G.N - 1] << endl;

}*/


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