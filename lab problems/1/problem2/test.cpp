#include <unordered_map>
#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <string>

using namespace std;

int N,K;
vector<int> res; //residents

/*
vector<vector<vector<vector<int>>>> Memo(N + 1, vector<vector<vector<int>>>
                                        (K + 1, vector<vector<int>>
                                        (2, vector<int>
                                        (2))));
*/
vector<vector<vector<vector<int>>>> Memo;

int solv(int j, int K, int S, int C) // O(4*N*K)
{
    if (K == 0) return 0; // we've found a possible solution!
    if (K < 0 || C < 0) return N + 1; // this is definitely not gonna lead to a solution >.<
    if (j == N && K != 0) return N + 1;  // you reached the end but people are left unsatisfied...

    if(Memo[j][K][S][C] > -1)
    {
        return Memo[j][K][S][C];
    }

    int first, second, result;
    if (S) //S = 1 means skipping mode
    {
        first = solv(j+1,K,S,C);
        second = 1 + solv(j+1,K-res[j],!S,C);
        // second = solv(j,K,!S,C);
    }
    else
    {
        first = 1 + solv(j+1,K-res[j],S,C);
        second = solv(j+1,K,!S,C-1);
    }
    result = min(first,second);
    Memo[j][K][S][C] = result;
    return result;
}

int main()
{
    cout << "\nGive a number between 1 and 25\n" << endl;
    int inputs;
    cin >> inputs;
    for(int i = 1; i <= inputs; ++i)
    {
        ifstream file("shops2/input" + to_string(i) + ".txt");
        file >> N >> K;

        Memo = vector<vector<vector<vector<int>>>>
                                        (N + 1, vector<vector<vector<int>>>
                                        (K + 1, vector<vector<int>>
                                        (2, vector<int>
                                        (2,-1))));

        res.resize(N);
        for(int j = 0; j < N; ++j)
        {
            int input_num;
            file >> input_num;
            res[j] = input_num;
        }

        int result = solv(0,K,1,1);
        result = (result == N + 1)?(-1):result;
        
        ifstream solution("shops2/output" + to_string(i) + ".txt");
        int ans;
        solution >> ans;
		cout << "Input " << i << ((result == ans) ? " right." : " wrong.") << std::endl;
		if (result != ans)
		{
			std::cout << "Solution found: " << result << std::endl;
			std::cout << "Actual solution: " << ans << std::endl;
		}        
    }
    return 0;
}