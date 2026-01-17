#include <iostream>
#include <chrono>
#include <unordered_map>
#include <string>
#include <mutex>
#include <thread>

using namespace std;
using namespace std::chrono;

class FixedWindowRateLimiter
{
private:
    int maxRequest;
    seconds windowSize;
    unordered_map<string, int> requestCount;
    unordered_map<string, steady_clock::time_point> windowStart;
    mutex mtx;

public:
    FixedWindowRateLimiter(int mr, int ws) : maxRequest(mr), windowSize(ws) {};
    bool allowRequest(const string &clientId)
    {
        lock_guard<mutex> lock(mtx);
        auto now = steady_clock::now();
        if (windowStart.find(clientId) == windowStart.end())
        {
            windowStart[clientId] = now;
            requestCount[clientId] = 0;
        }
        if (now - windowStart[clientId] >= windowSize)
        {
            windowStart[clientId] = now;
            requestCount[clientId] = 0;
        }
        if (requestCount[clientId] < maxRequest)
        {
            requestCount[clientId]++;
            return true;
        }
        return false;
    }
};
mutex coutMutex;
void simulateClient(FixedWindowRateLimiter &limiter, const string &clientId)
{
    for (int i = 1; i <= 5; i++)
    {
        bool isallowed = limiter.allowRequest(clientId);
        {
            lock_guard<mutex> lock(coutMutex);
            cout << "Thread " << this_thread::get_id()<< " | " << clientId << " | "<< (isallowed ? "Allowed" : "Rejected")<< endl;
        }
        this_thread::sleep_for(milliseconds(500));
    }
}

int main()
{
    FixedWindowRateLimiter limiter(3, 5);
    thread t1(simulateClient, ref(limiter), "UserA");
    thread t2(simulateClient, ref(limiter), "UserB");

    t1.join();
    t2.join();

    return 0;
}