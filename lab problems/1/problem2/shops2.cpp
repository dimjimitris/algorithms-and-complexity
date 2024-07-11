#include <iostream>
#include <vector>

using namespace std;

int N,K;
vector<int> res; // residents
vector<int> lls; // left lengths

/*
 * this function is used just to get the input
 * and set-up variables N,K,res which we need
 * to solve the problem  
 */
inline int get_input(istream& in)
{
    in >> N >> K;
    if (K == 0) return 0; // Solution is just 0
    res.resize(N); // give this vector the proper size
    int new_residents;
    for(int i = 0; i < N; ++i)
    {
        in >> new_residents;
        if (new_residents == K)
            return 1; // if a house has K residents then
                      // we've found the solution and we
                      // can stop
        res[i] = new_residents;
    }
    return 2;
}

inline int solve()
{
    lls = vector<int>(K + 1,N + 1); // list of lengths...
    // if lls[S] = x that means that the minimum length
    // subsequence on the 'left' side of what we are checking
    // that has sum equal to S has length x.

    // this is a useful initialization, can be used when we
    // find a subsequence with sum = K in the right half...
    lls[0] = 0; // you can always get a subsequence with
                // sum equal to 0 if it's length is 0.

    int ans = N + 1, sum; // initialize ans to some value bigger
    // than anything possible, so that we can decrease it whenever we
    // find a possible solution, if at the end it has not decreased that
    // means we found no solutions...
    for(int p = 0; p < N; ++p) // p is a partitioner of
                               // the residencies into 2 parts
    { // in the end we provide a rough drawing of why the use of
      // p, f, r indexes is enough to check all possible values...
        sum = 0;
        for(int f = p - 1; f >= 0; --f) // first half
        {
            sum += res[f];
            if (sum > K) break; // the rest will be at least as big
            lls[sum] = min(lls[sum],p - f); // this is the length
        }

        // now we are gonna check for ANY possible subsequence on the
        // right side...
        sum = 0;
        for(int r = p; r < N; ++r) // right half (last half)
        {
            sum += res[r];
            if (sum > K) break; // the rest will be at least as big
            ans = min(ans, lls[K - sum] + r - p + 1);
        }
    }
    if (ans == N + 1) ans = -1;
    return ans;
}
/*-----------------------------------------------------------------
           ---------------------------->(direction of p)
 (direction of f)<------- |------->(direction of r)
                 _________ ______
                |         |      |
res     = [ _...|_ ...  _ |_ ..._|..._  ]
indexes =   0    f     p-1 p    r    N-1

*********************************************************
when we are at partioning point p then every left hand side sum
of residents we find is gonna end at index p-1 and every right
hand side sum of residents we find starts at index p. How do we
manage to check right hand side [p,...,r] part with all the left
parts? We don't but by keeping the minimum length of some sum
we found on the left side through all the iterations we've made so
far we make sure that we check all possible solutions.
-------------------------------------------------------------------
*/

int main()
{
    int ans;
    switch(get_input(cin)){
        case 0: ans = 0; break;
        case 1: ans = 1; break;
        default: ans = solve();
    }
    cout << ans << endl;
    return 0;
}
