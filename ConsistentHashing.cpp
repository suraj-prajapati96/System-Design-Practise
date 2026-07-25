#include<bits/stdc++.h>
#include "sha256.h"
#define int long long
#define endl '\n'
using namespace std;

class ConsistentHashing{
    int N = INT_MAX;  // was INT_MAX — see below
    map<int,string>ring;
    int virtualNodes;
    int gap;
public:
    ConsistentHashing(){
        ring.clear();
        virtualNodes=4;
        gap = N/virtualNodes;
    }
    int hash(string &key){
        string hashVal = sha256(key);
        if(hashVal.size() < 8){
            throw runtime_error("sha256 returned malformed digest for key: " + key);
        }
        string first32Bits = hashVal.substr(0,8);
        try{
            return stoll(first32Bits,nullptr,16) % N;
        } catch(const exception &e){
            throw runtime_error("failed to parse hash for key: " + key);
        }
    }
    void addServer(string serverIP){
        int actualServerIndex = hash(serverIP);
        for(int i=0; i<virtualNodes; i++){
            int virtualNodeIndex = (actualServerIndex + i * gap)%N;
            ring[virtualNodeIndex] = serverIP;
        }
    }
    void removeServer(string serverIP){
        int actualServerIndex = hash(serverIP);
        for(int i=0; i<virtualNodes; i++){
            int virtualNodeIndex = (actualServerIndex + i * gap)%N;
            if(virtualNodeIndex){
                ring.erase(virtualNodeIndex);
            }
        }
    }
    string lookUpServer(string key){
        int keyIndex = hash(key);
        auto it = ring.lower_bound(keyIndex);

        if(it == ring.end()){
            it = ring.begin();
        }
        return it->second;
    }

    void show(){
        for(auto &it:ring){
            cout<<it.second<<"; "<<it.first<<endl;
        }
    }
};

signed main(){
    ConsistentHashing hashRing;
    hashRing.addServer("192.168.1.1");
    hashRing.addServer("88.88.88.88");
    hashRing.addServer("123.255.78.9");

    hashRing.show();

    int numOfRequests = 100;
    map<string,int> serverLoad;
    for(int i=0; i<numOfRequests; i++){
        string key = "key-" + to_string(i+1);
        string serverIP = hashRing.lookUpServer(key);

        // cout<<key<<": "<<serverIP<<endl;
        serverLoad[serverIP]++;
    }
    // cout<<endl;

    for(auto &it:serverLoad){
        cout<<it.first << ": "<<it.second <<endl; 
    }
    cout<<endl;

    hashRing.removeServer("192.168.1.1");
    serverLoad.clear();
    for(int i=0; i<numOfRequests; i++){
        string key = "key-" + to_string(i+1);
        string serverIP = hashRing.lookUpServer(key);
        serverLoad[serverIP]++;
    }

    for(auto &it:serverLoad){
        cout<<it.first << ": "<<it.second <<endl; 
    }
    cout<<endl;


    return 0;
}