#include <iostream>
#include <chrono>
#include <unordered_map>
#include <string>
#include <mutex>
#include <thread>
#include <algorithm>

using namespace std;
using namespace std::chrono;

class TokenBucketRateLimiter
{
private:
    int maxTokens;
    double tokensPerSecond;
    unordered_map<string, double> currentTokens;
    unordered_map<string, steady_clock::time_point> lastTime;
    mutex mtx;

public:
    TokenBucketRateLimiter(int mt, double rate)
        : maxTokens(mt), tokensPerSecond(rate) {}

    bool allowRequest(const string &clientId)
    {
        auto now = steady_clock::now();
        lock_guard<mutex> lock(mtx);

        if (lastTime.find(clientId) == lastTime.end())
        {
            lastTime[clientId] = now;
            currentTokens[clientId] = maxTokens;
        }

        double secondsPassed =
            duration_cast<duration<double>>(now - lastTime[clientId]).count();

        currentTokens[clientId] = min(
            (double)maxTokens,
            currentTokens[clientId] + secondsPassed * tokensPerSecond);

        lastTime[clientId] = now;

        if (currentTokens[clientId] >= 1.0)
        {
            currentTokens[clientId] -= 1.0;
            return true;
        }

        return false;
    }
};
mutex coutMutex;
void simulateClient(TokenBucketRateLimiter &limiter, const string &clientId)
{
    for (int i = 1; i <= 5; i++)
    {
        bool allowed = limiter.allowRequest(clientId);
        {
            lock_guard<mutex>lock(coutMutex);
            cout << "Thread " << this_thread::get_id() << " | " << clientId << (allowed ? " allowed" : " rejected") << endl;
        }
        this_thread::sleep_for(milliseconds(200));
    }
}

int main()
{
    TokenBucketRateLimiter limiter(3, 1.0);

    thread t1(simulateClient, ref(limiter), "userA");
    thread t2(simulateClient, ref(limiter), "userA");
    thread t3(simulateClient, ref(limiter), "userB");

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
