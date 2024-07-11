#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <utility>

using namespace std;
/*
 * Let's consider the sequence a1, a2, ..., aK of positive integers
 * and let's call median it's median (number that would be at position
 * floor((K+1)/2) in the sorted list).
 * 
 * Let's consider a number M and the sequence b1, b2, ..., bK
 * where bj = (aj >= M)?(1):(-1) and S = b1 + b2 + ... + bK
 * if (S > 0) then 
 *  	M <= median (search in bigger numbers)
 * 		else M > median (search in smaller numbers)
 * This is gonna help us implement a binary search for the maximum median
 * since we know the range in which it belongs before hand
 * 
 * let's say we create array prefix where prefix[i] = b1 + ... + bi
 * If we want to calculate the sum S for a sequence of length L which
 * has bj as it's last element then we simply do S = prefix[j] - prefix[j - L + 1]
 * 
 * Suppose that we are trying to see if any of the subsequences ending in the element
 * with index j is responsible for a positive value of S. Then we need to check for
 * S = prefix[j] - subtrahend, where subtrahend = 0, prefix[1], ..., prefix[j - K + 1].
 * Note that we only care if S happens to be positive in just one of these. If we keep
 * only the minimum value of the subtrahends then we remove from prefix[j] as little
 * an amount as possible thus keeping in as large as possible thus we only need to
 * check if prefix[j] - min{0,prefix[1],...,prefix[j - K + 1]} is positive...
 */


/*
 * this function returns a vector with the input array
 * and a vector with all the different possible values
 * sorted in O(N) time and space
 */
pair <vector<int>,vector<int>> \
read_input(istream& in, int& N, int& K)
{
	in >> N >> K;
	vector<int> c(N);
	vector<bool> exists(N + 1,false);

	for (int i = 0; i < c.size(); ++i)
	{
		in >> c[i];//0 < c[i] <= N
		exists[c[i]] = true;
	}
	vector<int> num_appear;
	for (int i = 1; i <= c.size(); ++i)
	{
		if(exists[i])
			num_appear.push_back(i);
	}
	num_appear.shrink_to_fit();

	return make_pair(c,num_appear);
}


/*
 * just create the prefix sum array...
 */
vector<int> prefixer(vector<int>& c, int M)
{
	int N = c.size();
	vector<int> prefix(N);
	for(int i = 0; i < N; ++i)
	{
		if (c[i] >= M)
			prefix[i] = 1;
		else
			prefix[i] = -1;
		if (i != 0)
			prefix[i] += prefix[i - 1];
	}
	return prefix;
}

/*
 * this function checks for the existence of at least one
 * positive S as described above...
 */

bool posS(vector<int>& prefix, int K, int M)
{
	int minuend = prefix[K - 1]; //sum of first K elements
	int subtrahend = 0; //there initially exist no elements before
						// those included in the minuend
	for(int i = K; i < prefix.size() && minuend <= 0; ++i)
	{
		subtrahend = min(subtrahend,prefix[i - K]);
		minuend = prefix[i] - subtrahend;
	}
	return minuend > 0;
}
/*
int main()
{
	int N, K;
	pair <vector<int>,vector<int>> input = read_input(cin,N,K);
	vector<int> c = input.first;
	vector<int> appearances = input.second;

	int first = 1, last = appearances.size();

	while(first != last)
	{
		int mid = (first + last + 1)/2; // get the ceiling to avoid
										// infinite loops
		int M = appearances[mid - 1];
		vector<int> prefix = prefixer(c,M);
		if(posS(prefix,K,M))
			first = mid;
		else
			last = mid - 1;
 	}
	int max_median = appearances[last - 1];
	cout << max_median << endl;
	return 0;
}
*/

int main()
{
	int inputs;
	cin >> inputs;
	for (int i = 1; i <= inputs; ++i)
	{
		ifstream file("cereal/input" + to_string(i) + ".txt");
		// N: the number of numbers to read as input
		// K: the minimum length of successive numbers to check
		int N, K;
		pair <vector<int>,vector<int>> input = read_input(file,N,K);
		vector<int> c = input.first;
		vector<int> appearances = input.second;
		file.close();

		int first = 1, last = appearances.size();
		while(first != last)
		{
			int mid = (first + last + 1)/2;
			int M = appearances[mid - 1];
			vector<int> prefix = prefixer(c,M);
			if(posS(prefix,K,M))
				first = mid;
			else
				last = mid - 1;
		}
		int max_median = appearances[first - 1];

		std::ifstream solution("cereal/output" + std::to_string(i) + ".txt");
		int ans;
		solution >> ans;
		std::cout << "Input " << i << ((max_median == ans) ? " right." : " wrong.") << std::endl;
		if (max_median != ans)
		{
			std::cout << "Solution found: " << max_median << std::endl;
			std::cout << "Actual solution: " << ans << std::endl;
		}
	}
	return 0;
}
