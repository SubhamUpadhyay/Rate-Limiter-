#include<iostream>
#include<chrono>

using namespace std;
using namespace std::chrono;

class TokenBucket{
   private:
    int maxTokens;
    double currentTokens;
    double tokensperSecond;
    steady_clock::time_point lastTime;
    public:
        TokenBucket(int limit , double rate):maxTokens(limit),currentTokens(limit),tokensperSecond(rate){
            lastTime = steady_clock::now();
        }

    bool allowRequest()
    {
        auto now = steady_clock::now();
        double secondPassed = duration_cast<duration<double>>(now-lastTime).count();
        
    //Refill token based on time passed
    currentTokens = min((double)maxTokens,currentTokens+secondPassed*tokensperSecond);

    lastTime = now;
    if(currentTokens>=1.0)
        {
            currentTokens-=1.0;
            return true;
        }
    return false;

    }

};


int main()
{
    TokenBucket limiter(3,1.0);  //3 token max in bucket , 1 token refill every second
    // string accepted = "";
    for(int i=1;i<=150000;i++)
        {
            bool allowed = limiter.allowRequest();
            cout<<"Request "<<i<<((allowed)?" Allowed":" Rejected")<<endl;
            // if(allowed)
                // {
                    // accepted += to_string(i)+"   ";
                // }
        }
    // cout<<"\n\n\n\n\n"<<accepted;
    return 0;
}