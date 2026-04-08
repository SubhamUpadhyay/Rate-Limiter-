#include<iostream>
#include<chrono>
#include<unordered_map>
#include<deque>
#include<string>
#include<mutex>
#include<thread>

using namespace std;
using namespace std::chrono;

class SlidingWindowRateLimiter{
    private:
        int maxRequests;
        seconds windowSize;
        unordered_map<string,deque<steady_clock::time_point>>requests;
        mutex mtx;
    public:
        SlidingWindowRateLimiter(int max_request , int window_size):maxRequests(max_request),windowSize(window_size){};


        bool allowRequest(const string &clientId)
            {
                lock_guard<mutex>lock(mtx);
                auto now = steady_clock::now();
                auto& dq = requests[clientId];

                while(!dq.empty() && now-dq.front()>=windowSize)
                    {
                        dq.pop_front();
                    }

                if((int)dq.size() < maxRequests)
                    {
                        dq.push_back(now);
                        return true;
                    }
                return false;
            }

};
mutex coutmtx;

void simulateClinet(SlidingWindowRateLimiter &limiter,const string &clientId)
    {
        for(int i=1;i<=5;i++)
            {
                lock_guard<mutex>lock_guard(coutmtx);
                bool allowed = limiter.allowRequest(clientId);
                cout<<"Thread "<<this_thread::get_id()<<" | "<<clientId<<(allowed ? " Allowed":" Not Allowed")<<endl;
            }
    }

int main()
    {
        SlidingWindowRateLimiter limiter(3,5);
        thread t1(simulateClinet,ref(limiter),"userA");
        thread t2(simulateClinet,ref(limiter),"userA");
        thread t3(simulateClinet,ref(limiter),"userB");

        t1.join();
        t2.join();
        t3.join();
        return 0;
    }