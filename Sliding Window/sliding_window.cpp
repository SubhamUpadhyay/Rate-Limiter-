#include<iostream>
#include <chrono>
#include <deque>
#include<thread>

using namespace std;
using namespace std::chrono;

class SlidingWindowRateLimiter{
    private:
        int maxRequests;
        seconds windowSize;
        deque<steady_clock::time_point>requestTimes;
    public:
        SlidingWindowRateLimiter(int limit , int seconds):maxRequests(limit),windowSize(seconds){};

        bool allowRequest()
            {
                auto now = steady_clock::now();

                //remove timestamps outside the window frame

                while(!requestTimes.empty() && now-requestTimes.front()>=windowSize)
                    {
                        requestTimes.pop_front();
                    }

                if((int)requestTimes.size()<maxRequests)
                    {
                        requestTimes.push_back(now);
                        return true;
                    }
                return false;
            }
};
int main()
{
    SlidingWindowRateLimiter limiter(3,5); //3 request per 5 second time

    // int count = 0;

    for(int  i=1 ; i<7;i++)
        {
            cout<<"Request "<<i<<(limiter.allowRequest()?" Allowed":" Rejected")<<endl;
            this_thread::sleep_for(seconds(1));
            
        }

    // cout<<"Count : "<<count;

    return 0;
}