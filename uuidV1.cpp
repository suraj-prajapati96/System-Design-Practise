#include<bits/stdc++.h>
#define endl '\n'
using namespace std;

const long long TIME_BETWEEN_15_OCT_1582_TO_1_JAN_1970 = 122192928000000000;
long long prevTime =-1;
int sequenceNumber = 0;

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

string convertMacToHex(string MAC){
    string hex;
    for(auto &ch : MAC){
        if(ch=='-'){
            continue;
        }else if(ch>=0 && ch<=9){
            hex+=ch;
        }else{
            hex+=tolower(ch);
        }
    }
    return hex;
}

string uuidv1(){
    string parts[5];
    long long currentTime = chrono::system_clock::now().time_since_epoch().count();
    currentTime /= 100;
    currentTime += TIME_BETWEEN_15_OCT_1582_TO_1_JAN_1970;
    // cout<<currentTime<<endl;
    // currentTime /= 1e6;

    string first60bits = bitset<60>(currentTime).to_string();
    // cout<<first60bits<<endl;
    string hexTime = binaryToHex(first60bits);
    // cout<<hex<<endl;

    parts[0] = hexTime.substr(7,8); //time_low
    parts[1] = hexTime.substr(3,4); //time_mid

    string version = "1";

    parts[2] = version + hexTime.substr(0,3);//time_high
    // cout<<parts[0]<<" "<<parts[1]<<" "<<parts[2];

    string variant = "10";
    // cout<<prevTime<<" "<<currentTime<<" "<<sequenceNumber<<endl;

    if(currentTime == prevTime){
        sequenceNumber++;
    }else{
        sequenceNumber = 0;
    }
    prevTime = currentTime;

    string clockSequence;
    if(variant.length()==1){
        clockSequence = bitset<15>(sequenceNumber).to_string();
    }else if(variant.length()==2){
        clockSequence = bitset<14>(sequenceNumber).to_string();
    }else{
        clockSequence = bitset<13>(sequenceNumber).to_string();
    }

    parts[3] = binaryToHex(variant+clockSequence);

    string MAC = "00-B0-D0-63-C2-26";
    parts[4] = convertMacToHex(MAC);
    cout<<parts[3]<<" "<<parts[4]<<endl;

    string UUID;
    for(int i=0; i<5; i++){
        UUID+=parts[i];
        if(i<4){
            UUID+='-';
        }
    }

    return UUID;
}

int main(){

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout<<uuidv1()<<endl;

    return 0;
}