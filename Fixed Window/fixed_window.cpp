#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

class FixedWindowRateLimiter
{
private:
    int maxRequests;
    int requestCount;
    steady_clock::time_point windowStart;
    seconds windowSize;

public:
    FixedWindowRateLimiter(int a, int windowSize) : maxRequests(a), requestCount(0), windowSize(windowSize) { windowStart = steady_clock::now();}
    bool allowRequest()
        {
            auto now = steady_clock::now();
            if(now-windowStart>=windowSize)
                {
                    windowStart=now;
                    requestCount = 0;
                }
            if(requestCount<maxRequests)
                {
                    requestCount++;
                    return true;
                }
            return false;
        }
};

int main()
{
    FixedWindowRateLimiter limiter(3,5);
    for(int i=1;i<=6;i++)
        {
            bool allowed = limiter.allowRequest();
            cout<<"Request "<<i<<(allowed?" allowed":" rejected")<<endl;
        }
    return 0;
}