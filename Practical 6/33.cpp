#include <bits/stdc++.h>
using namespace std;
  
vector<int> generate(vector<int> msgBits, int m, int r)
{
    vector<int> hammingCode(r + m);
  
    for (int i = 0; i < r; ++i) {
  
        hammingCode[pow(2, i) - 1] = -1;
    }
  
    int j = 0;
  
    for (int i = 0; i < (r + m); i++) {
  
        if (hammingCode[i] != -1) {
            hammingCode[i] = msgBits[j];
            j++;
        }
    }
  
    for (int i = 0; i < (r + m); i++) {
  
        if (hammingCode[i] != -1)
            continue;
  
        int x = log2(i + 1);
        int one_count = 0;
  
        for (int j = i + 2;
             j <= (r + m); ++j) {
  
            if (j & (1 << x)) {
                if (hammingCode[j - 1] == 1) {
                    one_count++;
                }
            }
        }
  
        if (one_count % 2 == 0) {
            hammingCode[i] = 0;
        }
        else {
            hammingCode[i] = 1;
        }
    }
  
    return hammingCode;
}
  
void find(vector<int>& msgBit)
{
  
    int m = msgBit.size();
    int r = 1;
  
    while (pow(2, r) < (m + r + 1)) {
        r++;
    }
  
    vector<int> ans  = generate(msgBit, m, r);
  
    for (int i = 0; i < ans.size(); i++)
        cout << ans[i];
    cout << endl;
}
  
int main()
{
    string msg;
    vector<int> msgBit;

    cin >> msg;

    int l = msg.size();
    for(int i = 0; i < l; i++)
        msgBit.push_back(msg[i] - '0');
        
    find(msgBit);
  
    return 0;
}
