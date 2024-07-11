#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

/*
 * Idea behind this algorithm:
 * 
 * Suppose Q = (q1, ..., qN) = [p1, ..., pl + x, ..., pr + x, pr+1, ..., pN]
 * is the modifiend sequence with maximum LIS.
 *
 * case 1: answer with 0 <= x <= k
 * 
 * elements of Q with index > r that belong in the LIS are larger than
 * all previous elements so adding x to them won't change the LIS.
 * In the resulting Q:
 * elements of Q with index >= l that belong in the LIS are larger than
 * all previous elements so adding (k - x) to them won't change the LIS.
 * 
 * so Q' = [p1, ..., pl + k, ..., pN + k] will give us the same LIS.
 * 
 * case 2: answer with -k <= x < 0
 * 
 * elements of Q with index < l that belong in the LIS are smaller than
 * all following elements so adding -x to them won't change the LIS.
 * In the resulting Q:
 * elements of Q with index > r that belong in the LIS are larger than
 * all previous elements so adding (k + x) to them won't change the LIS.
 * 
 * So Q' = [p1, ..., pl, ..., pr, pr+1 + k, ... pN + k] gives the same LIS.
 * 
 * We can find the answer as follows:
 * for every position find the largest IS up to that position and
 * add the length of the largest IS from that position given that
 * the last element of the first is smaller than the first element
 * of the later + k.
 * 
 */

/* read the input, this function is O(N)*/
vector<int> read_input(istream& in, int& N, int& K)
{
    in >> N >> K;
    vector<int> p(N);
    for (int i = 0; i < N; ++i)
        in >> p[i];
    return p;
}

/* this is a function for finding the LIS and also
 * finding the LIS which ends at pi in vector
 * LIS_ending_at[i]
 */
pair <int, vector<int> > LIS(vector<int>& v)
{
    vector<int> tail(v.size(), INT_MIN); // this acts as the 'm' array in
    // the lecture slides
    vector<int> LIS_ending_at(v.size());
    int length = 0; // length is always the current length for which
    // we haven't found an LIS

    vector<int>::iterator s, t, it;
    for (int i = 0; i < v.size(); ++i) {// this algorithm is very similar
    // to the one in the lecture notes for the LIS problem with the adition
    // of the LIS_ending_at vector which we explained above
    // it is heavily influenced by the Geeks for Geeks solution to this problem
        
        s = tail.begin();
        t = tail.begin() + length;
        // [s, t) are the sizes we've found
        // t is the next size that we might find
        it = lower_bound(s, t, v[i]); // binary search!!!

        if (it == tail.begin() + length)
            ++length;

        *it = v[i];

        LIS_ending_at[i] = distance(tail.begin(), it) + 1;
    }
    return make_pair(length, LIS_ending_at);
}

int main(){
    int N, K;
    vector<int> p = read_input(cin, N, K);

    pair<int, vector<int> > result = LIS(p);

    cout << result.first << endl << endl;

    for (auto& qi : result.second) cout << qi << "  ";
    cout << endl;

    return 0;
}