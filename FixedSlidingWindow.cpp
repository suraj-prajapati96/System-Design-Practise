#include<bits/stdc++.h>
#define int long long
using namespace std;

int getCurrentTime(){
    return chrono::system_clock::now().time_since_epoch().count()/1e9;
}

class FixedWindowCounter{
    int windowSize;
    int maxRequests;
    int requestCount;
    int windowStartTime;
public:
    FixedWindowCounter(){
        windowSize=5;
        maxRequests=3;
        requestCount=0;
        windowStartTime=getCurrentTime();
    }
    void updateWindow(){
        int currentTime=getCurrentTime();
        int timeElapsed = currentTime - windowStartTime;
        if(timeElapsed < windowSize){
            return;
        }
        requestCount=0;
        windowStartTime = currentTime - (timeElapsed % windowSize);
    }

    bool processRequest(){
        updateWindow();
        if(requestCount<maxRequests){
            requestCount++;
            return true;
        }else{
            return false;
        }
    }
};

signed main(){
    FixedWindowCounter rateLimiter;
    for(int i=0; i<10; i++){
        if(rateLimiter.processRequest()){
            cout<< i+1<<" - Processed"<<endl;
        }else{
            cout<<i+1<<" - Rate Limited"<<endl;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
    return 0;
}