/*
In this program the random numbers we create are also going to be called "trials".
Imagine a Graph with N vertices. As we create more and more trials this graph has
more and more edges added to it. We need to maintain the size of its components.

Each time we add a new edge we check the size of the component it belongs to and if
this size is big enough we terminate.

To maintain the components with good complexity we use a very simple implementation of
a disjoint-set data structure using Union by size in order to keep track of component
sizes too.

The implementation of this data structure is from the wikipedia page about Disjoint-set data structures
*/
#include <vector>
#include <iostream>
#include <unordered_set>

using namespace std;

#define MAX_INPUT_SIZE 63 // usefull number in this problem
using big = unsigned long long; // used in an unordered set to keep track of multiple edges

// ----------- DJS - start --------------
class subset {
public:
    int parent;
    int size;
};

// path-halving version of find function
int Find(vector<subset>& subsets, int i) {
    while (subsets[i].parent != i) {
        i = subsets[i].parent = subsets[subsets[i].parent].parent;
    }
    return i;
}

// Union by size
void Union(vector<subset>& subsets, int x, int y) {
    // replace nodes by roots
    x = Find(subsets, x);
    y = Find(subsets, y);

    if (x == y) return;

    if (subsets[x].size < subsets[y].size) {
        int z = x;
        x = y;
        y = z;
    } // we always merge the small set to the big set

    subsets[y].parent = x;
    subsets[x].size += subsets[y].size;
}
// ------------- DJS - end -------------------

// get inputs
vector<int> get_input(int& N, int& F) {
    cin >> N >> F;

    vector<int> trials(MAX_INPUT_SIZE);
    for (int i = 0; i < MAX_INPUT_SIZE; ++i) {
        cin >> trials[i];
    }
    return trials;
}

// add a new trial to our inputs vector
void set_input(vector<int>& trials, int i, int N) {
    if (i >= MAX_INPUT_SIZE) {
        if (i >= trials.size()) { // resize if needed
            trials.resize(trials.size() + N);
        }
        trials[i] = (trials[i - MAX_INPUT_SIZE] + trials[i - MAX_INPUT_SIZE/2]) % N; 
    }
}

// used in our unordered_set as a hash function. takes edge = (x, y) and maps it to a unique integer
big turn_2d_to_1d(int x, int y) {
    big a = x;
    big b = y;
    if (x > y) {
        a = y;
        b = x;
    } // a is always the small value, b is always the large value
    return ((a + b)*(a + b) + 3*a + b) / 2; // we've seen this in Discrete Maths
}

int main() {
    // get program inputs
    int N, F, target;
    vector<int> trials = get_input(N, F);
    target = N*F/100 + (N*F % 100 != 0); // we want at least N*F/100 so we include the modulo arithmetic
    // inputs have been formatted.
    // N = number of people, target = number of people which is big enough to meet the required percentage

    // Union find vector for our collection of people!
    vector<subset> subsets(N + 1);
    for (int i = 0; i < N + 1; ++i) {
        subsets[i].parent = i;
        subsets[i].size = 1;
    }

    int edges = 0; // how many edges we've added to the graph
    int loops = 0; // how many loops there are
    int multi_edges = 0; // how many multiple edges there are
    unordered_set<big> doublicates; // keeps track of edges to check for doublicates
    int trial = 0;
    while(true) { // trials[trial] and trials[trial + 1] are gonna be the new edge
        int first = trial;
        int second = trial + 1;

        set_input(trials, first, N);
        set_input(trials, second, N);

        first = trials[first]; // person that corresponds to first trial
        second = trials[second]; // person that corresponds to second trial

        big one_dim = turn_2d_to_1d(first, second);
        if (first == second) {
            ++loops; // we have looped! first, second are the same person
        }
        else if (doublicates.count(one_dim)) { // we have seen this edge already
            ++multi_edges; // it is a doublicate
        }
        else {
            ++edges; // new edge found
            doublicates.insert(one_dim); // add it to the doublicates set
            Union(subsets, first, second); // create the new component
        }

        first = Find(subsets, first);   // be careful, sizes of components are guaranteed to be stored correctly at the root of that component
        second = Find(subsets, second); // so we get the root!

        if (subsets[first].size >= target || subsets[second].size >= target) {
            break; // a component has hit the target size!
        }
        trial += 2;
    }

    int ans[] = { edges, multi_edges, loops };
    cout << ans[0] << " " << ans[1] << " " << ans[2] << "\n";
}