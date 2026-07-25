#include "sha256.h"
#include<bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

const int allOnes = (1LL << 32)-1;
int primes[64], K[64], H[8], W[64];
vector<string> messageBlocks;

int rotr(int num, int k){
    return (num >> k) | (num << (32-k));
}
int sigma0(int num){
    return rotr(num,7) ^ rotr(num,18) ^ (num >> 3);
}
int sigma1(int num){
    return rotr(num,17) ^ rotr(num,19) ^ (num >> 10);
}
int usigma0(int num){
    return rotr(num,2) ^ rotr(num,13) ^ rotr(num,22);
}
int usigma1(int num){
    return rotr(num,6) ^ rotr(num,11) ^ rotr(num,25);
}

int ch(int a, int b, int c){
    int ans=0;
    for(int i=0; i<32; i++){
        int bit = ((a >> i) & 1) == 1 ? (b >> i) & 1 : (c >> i) & 1;
        if(bit==1) ans |= 1 << i;
    }
    return ans;
}

int maj(int a, int b, int c){
    int ans=0;
    for(int i=0; i<32; i++){
        int cnt1 = ((a >> i)&1) + ((b >> i) & 1) + ((c >> i) & 1);
        if(cnt1 >= 2) ans |= 1<<i;
    }
    return ans;
}

void loadK(){
    const long long vals[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
        0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
        0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
        0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
        0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
        0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
        0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
        0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
        0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };
    for(int i=0;i<64;i++) K[i]=vals[i];
}

void init(){
    H[0]=0x6a09e667;
    H[1]=0xbb67ae85;
    H[2]=0x3c6ef372;
    H[3]=0xa54ff53a;
    H[4]=0x510e527f;
    H[5]=0x9b05688c;
    H[6]=0x1f83d9ab;
    H[7]=0x5be0cd19;
    messageBlocks.clear();
}

string stringToBinary(string input){
    string output;
    for(char &c: input){
        output+=bitset<8>(c).to_string();
    }
    return output;
}

void convertStringToMessageBlocks(string inputMessage){
    string messageBits = stringToBinary(inputMessage);
    int L = messageBits.length();
    for(int i=0; i<L; i+=512){
        int curBlockMessageLength = min((L-i),512LL);
        int zeroRequired = 512 - (curBlockMessageLength+1+64);

        string block = messageBits.substr(i,curBlockMessageLength);
        if(zeroRequired >=0){
            block+='1';
            while(zeroRequired--){
                block+='0';
            }
            block+=bitset<64>(L).to_string();
        }
        messageBlocks.push_back(block);
    }
}

void createMessageSchedule(int blockIndex){
    for(int i=0; i<64; i++){
        if(i < 16){
            W[i] = stoll(messageBlocks[blockIndex].substr(32*i,32),NULL,2);
        }else{
            W[i] = sigma1(W[i-2]) + W[i-7] + sigma0(W[i-15]) + W[i-16];
        }
        W[i] &=allOnes;
    }
}

void updateStateRegisters(){
    int initialHashes[8];
    copy(H,H+8,initialHashes);

    for(int i=0; i<64; i++){
        int T1 = usigma1(H[4]) + ch(H[4],H[5],H[6]) + H[7] + K[i] + W[i];
        int T2 = usigma0(H[0]) + maj(H[0],H[1],H[2]);

        T1 &=allOnes;
        T2 &=allOnes;

        for(int j=7; j>=0; j--){
            H[j]=H[j-1];
        }
        H[0] = (T1 + T2) & allOnes;
        H[4] = (H[4] + T1) & allOnes;
    }
    for(int i=0; i<8; i++){
        H[i] = (H[i] + initialHashes[i]) & allOnes;
    }
}

string intToHexString(int value){
    stringstream ss;
    ss << hex << value;
    string ans = ss.str();
    string zeros(8 - ans.length(),'0');
    ans = zeros + ans;
    return ans;
}

string sha256(string inputMessage){
    static bool kLoaded = false;
    if(!kLoaded){
        loadK();
        kLoaded = true;
    }
    init();
    convertStringToMessageBlocks(inputMessage);
    for(int i=0; i<(int)messageBlocks.size(); i++){
        createMessageSchedule(i);
        updateStateRegisters();
    }
    string digest;
    for(int i=0; i<8; i++){
        digest += intToHexString(H[i]);
    }
    return digest;
}
