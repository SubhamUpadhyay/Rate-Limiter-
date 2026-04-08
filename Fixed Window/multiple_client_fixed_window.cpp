#include<bits/stdc++.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

class FixedWindowRateLimiter{
    private:
        int maxRequest;
        seconds windowSize;
        unordered_map<string,int>requestCount;
        unordered_map<string,steady_clock::time_point>windowStart;
    public:
        FixedWindowRateLimiter(int maxReq , int windowSize):maxRequest(maxReq),windowSize(windowSize){};
        bool allowRequest(const string &clientId)
            {
                auto now = steady_clock::now();
                if(windowStart.find(clientId)==windowStart.end())
                    {
                        windowStart[clientId] = now;
                        requestCount[clientId] = 0;
                    }
                if(now-windowStart[clientId]>=windowSize)
                    {
                        windowStart[clientId] = now;
                        requestCount[clientId] = 0;
                    }
                if(requestCount[clientId]<maxRequest)
                    {
                        requestCount[clientId]++;
                        return true;
                    }
                return false;
            }        
};

int main()
{
    FixedWindowRateLimiter limiter(2,5);
    string clientA = "userA";
    string clientB = "userB";

    cout<<"....................................................................................\n";
    cout<<clientA<<endl;
    cout<<"....................................................................................\n";

    for(int i=1;i<=5;i++)
        {
            cout<<"Request "<<i<<" from "<<clientA<<(limiter.allowRequest(clientA)?" allowed":" rejected")<<endl;
        }
    cout<<"\n....................................................................................\n";
    cout<<clientB<<endl;
    cout<<"....................................................................................\n";
    for(int i=1;i<=5;i++)
        {
            cout<<"Request "<<i<<" from "<<clientB<<(limiter.allowRequest(clientB)?" allowed":" rejected")<<endl;
        }
    return 0;
}
