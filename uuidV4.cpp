#include<bits/stdc++.h>
#define endl '\n'
using namespace std;

string binaryToHex(string input){
    string output;
    for(int i=0; i<input.length(); i+=4){
        string nibble = input.substr(i,4);
        int digit = stoi(nibble,NULL,2);

        char hexDigit;
        if(digit<=9){
            hexDigit = '0' + digit;
        }else{
            hexDigit = 'a' + (digit-10);
        }
        output+=hexDigit;
    }
    return output;
}

string uuidv4(){
    string binary = bitset<128>(0).to_string();
    for(int i=0; i<128; i++){
        binary[i] = (rand()%2) + '0';
    }
    // cout<<binary<<endl;
    //8-4-4-4-12
    binary[48]= '0';
    binary[49]= '1';
    binary[50]= '0';
    binary[51]= '0';

    binary[64] = '1';
    binary[65] = '0';

    string hex = binaryToHex(binary);
    return hex.substr(0,8) + '-'+ hex.substr(8,4)+'-'+hex.substr(12,4)+'-'+hex.substr(16,4)+"-"+hex.substr(20);
}

int main(){
    srand(0);
    cout<<uuidv4()<<endl;
}