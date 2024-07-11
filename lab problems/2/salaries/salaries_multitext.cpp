#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <climits> // just for the INT_MIN, INT_MAX

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
 * of the later + K.
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
  
  
/* this function is a modified Longest Increasing Subsequence 
 * algorithm based on the lecture slides and examples of our subject  
 * This is the usual implementation of the LIS O(NlogN) but with some 
 * extra features */  
pair <int, vector<int> > LIS(vector<int>& v, int K,  
                            const vector<int>& LIS_start_at = vector<int>(1,INT_MIN))  
{  
    bool flag = false;  
    if (LIS_start_at.size() == 1 && LIS_start_at[0] == INT_MIN) flag = true;  
    /* This flag is used to inform us if the function has been supplied with a 
     * vector that contains the LIS which starts at each possible element of v */  
  
    vector<int> tail(v.size()); // tail is the equivalent of the 'm' array in the lecture slides.  
  
    vector<int> LIS_ending_at(v.size()); // this array will store the following information:  
    // if L = LIS_ending_at[i] then the LIS which ends at pi has length L in the standard LIS version  
      
    int max_length = 0; // this is the result in the modified version, otherwise, doesn't do anything useful  
    int length = 0; // the current length of an LIS which we have not found...  
  
    vector<int>::iterator s, t, it;  
    for (int i = 0; i < v.size(); ++i) {  
          
        s = tail.begin();  
        t = tail.begin() + length;  
  
        it = lower_bound(s, t, v[i] + K); // the binary search in the 'm' array done with a standard function  
        // iterator is the location of the first element >= to v[i] + K  
          
        if (!flag) // if we have been given the LIS_start_at array we should be updating the max_length  
            max_length = max(max_length,  
                            ((int) distance(s, it)) + LIS_start_at[i]);  
        // this would be length1 + length2 - 1, however, the -1 is emitted because the distance() function  
        // returns (length1 - 1)  
          
        it = lower_bound(s, t, v[i]); // if we are running the algorithm with a K > 0, we want the next  
        // iterations to use the 'm' array with v[i] included, since we are looking for the element which  
        // is the first element of the array we'll increase by K.  
        // if we are running this with K = 0 then the result will be the exact same as in the previous  
        // lower_bound execution  
  
        if (it == tail.begin() + length) // we have unlocked a new length!  
            ++length;  
  
        *it = v[i]; // update the length to the new minimum value of the final element for that length LIS  
  
        LIS_ending_at[i] = distance(tail.begin(), it) + 1;  
    } //important to note: iterators of vectors are random access iterators, that means that distance() and   
      // lower_bound() are optimized. They run in O(1) and O(log(last - first)) = O(logN) respectively.  
    return make_pair(max_length, LIS_ending_at);  
    // when we use it as standard LIS we will be taking the second return value  
    // and using that we will run the modified LIS to get the first return value which will be our answer.  
}  
  
// how do we use the standard LIS (K = 0 and no LIS_start_at argument in the above function) to calculate  
// the LIS_start_at array?  
// Reverse the input --> then we need to find the LDS (Decreasing) which ends at each index of the input  
// Negate all elements in the reversed input --> then you can just run a classic LIS on this.  
// Reverse the LIS_ending_at vector that you find  

int main() {
    int inputs;
    cin >> inputs;

    for (int i = 1; i <= inputs; ++i) {
        ifstream file("salaries/input" + to_string(i) + ".txt");

        int N, K;  
        vector<int> p, r, helper;  
        r = p = read_input(file, N, K); // get input  
    
        reverse(r.begin(), r.end()); // reverse input  
        for (int& ri: r) ri = -ri; // negate the input  
    
        vector<int> LIS_start_at = LIS(r,0).second; // standard LIS on the  
        // reversed/negated input to find the LIS_start_at vector  
        reverse(LIS_start_at.begin(),LIS_start_at.end()); // reverse the output  
    
        int max_length = LIS(p, K, LIS_start_at).first; // modified LIS to  
        // find the max_length by supplying it with a K and the LIS_start_at vector  
    
        ifstream solution("salaries/output" + to_string(i) + ".txt");
        int ans;
        solution >> ans;
        cout << "Input " << i << ((max_length == ans) ? " right." : " wrong.") << endl;
        if (max_length != ans)
        {
            cout << "Solution found: " << max_length << endl;
            cout << "Actual solution: " << ans << endl;
        }     
    }

    return 0;
}