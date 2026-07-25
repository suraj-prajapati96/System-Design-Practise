#include<bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

const int allOnes = (1LL << 32)-1;
int primes[64], K[64], H[8], W[64];
vector<string>messageBlock;

void print(int num){
    for(int i=31; i>=0; i--){
        cout<<((num >> i)&1);
    }
    cout<<endl;
}

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
        if(bit==1){
            ans |= 1 << i;
        }
    }
    return ans;
}

int maj(int a, int b, int c){
    int ans=0;
    for(int i=0; i<32; i++){
        int cnt1 = ((a >> i)&1) + ((b >> i) & 1) + ((c >> i) & 1);
        if(cnt1 >= 2){
            ans |= 1<<i;
        }
    }
    return ans;
}

void generatePrime(){
    int n=1000;
    vector<int>sieve(n,0);
    for(int i=2; i*i<n; i++){
        if(sieve[i]==0){
            for(int j=i*i; j<n; j+=i){
                sieve[j]=1;
            }
        }
    }
    for(int i=2, k=0; i<n && k<64; i++){
        if(sieve[i]==0){
            primes[k]=i;
            k++;
            // cout<<i<< " ";
        }
    }
    // cout<<endl;
}

void generateConstants(){
    for(int i=0; i<8; i++){
        long double sq_root = sqrt(primes[i]);
        long double frac_part = sq_root - (int)sq_root;
        H[i]=frac_part * (1LL << 32);
        H[i] &=allOnes;
    }
    for(int i=0; i<64; i++){
        long double cb_root = cbrt(primes[i]);
        long double frac_part = cb_root - (int)cb_root;
        K[i]=frac_part * (1LL << 32);
        K[i] &= allOnes;
    }
    // for(int i=0; i<8; i++){
    //     cout<<(char)('a' + i)<<": ";
    //     print(H[i]);
    // }
    // cout<<endl;
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
    // cout<< L << endl;
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
        messageBlock.push_back(block);
        // cout<<block<<endl;
    }
}

void createMessageSchedule(int blockIndex){
    for(int i=0; i<64; i++){
        if(i < 16){
            W[i] = stoi(messageBlock[blockIndex].substr(32*i,32),NULL,2);
        }else{
            W[i] = sigma1(W[i-2]) + W[i-7] + sigma0(W[i-15]) + W[i-16]; 
        }
        W[i] &=allOnes;

        // cout<<i<<": ";
        // print(W[i]);
    }
}

void updateStateRegisters(){
    int initialHashes[8];
    copy(H,H+8,initialHashes);

    for(int i=0; i<64; i++){
        //T1 = usigma1(e) + ch(e,f,g) + h + Ki + Wi
        int T1 = usigma1(H[4]) + ch(H[4],H[5],H[6]) + H[7] + K[i] + W[i];
        // T2 = usigma0(a) + maj(a,b,c);
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
    // for(int i=0; i<8; i++){
    //     cout<<(char)('a' + i)<<": ";
    //     print(H[i]);
    // }
    // cout<<endl;
}

string intToHexString(int value){
    stringstream ss;
    ss << hex << value;
    string ans = ss.str();
    string zeros(8 - ans.length(),'0');
    ans = zeros + ans;
    return ans;
}

signed main(){
    // int a=0b00000000111111110000000011111111;
    // int b=0b00000000000000001111111111111111;
    // int c=0b11111111111111110000000000000000;
    //print(rotr(a,5));
    // print(ch(a,b,c));
    // print(maj(a,b,c));
    generatePrime();
    generateConstants();
    string inputMessage;
    cin>>inputMessage;

    convertStringToMessageBlocks(inputMessage);

    for(int i=0; i<messageBlock.size(); i++){
        createMessageSchedule(i);
        updateStateRegisters();
    }

    string digest;
    for(int i=0; i<8; i++){
        digest+= intToHexString(H[i]);
    }
    cout<<digest<<endl;

    return 0;
}