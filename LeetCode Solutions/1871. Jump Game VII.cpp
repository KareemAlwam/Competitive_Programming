class Solution {
public:
    bool canReach(string s, int minJump, int maxJump) {
        s='#'+s;
        int n=s.size();
        vector<int>dp(n);
        dp[1]=1;
        for(int i=2;i<s.size();i++)
        {
            if(s[i]=='0')
            {
                int l=max(0,i-minJump),r=max(0,i-maxJump-1);
                int cnt=dp[l]-dp[r];
                dp[i]=cnt>0;
            }
            dp[i]+=dp[i-1];
        }
        return dp[n-1]-dp[n-2];
    }
};
