#include<bits/stdc++.h>
using namespace std;

int dataCount;
float pixel[4]; //input layer
float firstLayer[4], secondLayer[4], thirdLayer[8], outLayer[4];

string genFileName(int n){
    string name = "D:\\Programming_new\\pattern_matching\\data\\", number;
    //add trailing 0s
    int count = 0, tmp = n;  //number of digits of n
    while (tmp > 0) {
        tmp /= 10;
        ++count;
    }
    for (int i = 0; i < 10 - count; ++i) name += "0";
    //convert number to string
    stringstream ss;
    ss << n;
    ss >> number;

    name += number;
    name += ".txt";
    return name;
}

void genSolution(){
    //first layer
    firstLayer[0] = pixel[0] + pixel[3];
    firstLayer[1] = pixel[1] + pixel[2];
    firstLayer[2] = pixel[0] - pixel[3];
    firstLayer[3] = pixel[1] - pixel[2];
    //tanh squashing function
    for (int i = 0; i < 4; ++i) firstLayer[i] = tanh(firstLayer[i]);

    //second layer
    secondLayer[0] = firstLayer[0] + firstLayer[1];
    secondLayer[1] = -firstLayer[0] + firstLayer[1];
    secondLayer[2] = firstLayer[2] - firstLayer[3];
    secondLayer[3] = firstLayer[2] + firstLayer[3];
    //tanh squashing function
    for (int i = 0; i < 4; ++i) secondLayer[i] = tanh(secondLayer[i]);

    //third layer
    for (int i = 0; i < 4; ++i){
        //rectified linear units
        thirdLayer[2*i] = (secondLayer[i] > 0 ? secondLayer[i] : 0);
        thirdLayer[2*i+1] = (-secondLayer[i] > 0 ? -secondLayer[i] : 0);
    }

    //out layer
    for (int i = 0; i < 4; ++i){
        outLayer[i] = thirdLayer[2*i] + thirdLayer[2*i+1];
    }

    //output check
    /*for (int i = 0; i < 4; ++i) cout << firstLayer[i] << " "; cout << "\n";
    for (int i = 0; i < 4; ++i) cout << secondLayer[i] << " "; cout << "\n";
    for (int i = 0; i < 8; ++i) cout << thirdLayer[i] << " "; cout << "\n";
    for (int i = 0; i < 4; ++i) cout << outLayer[i] << " "; cout << "\n============\n";*/
}

int main(){
    srand(time(NULL));
    ifstream inFile;
    inFile.open("datacount.txt");
    inFile >> dataCount;
    inFile.close();
    ofstream outFile;
    for (int i = 1; i <= dataCount; ++i){   //generate data
        outFile.open( genFileName(i) );
        //generate 4 random float between -1 and 1
        for (int i = 0; i < 4; ++i){
            pixel[i] = ((2) * ((float)rand() / RAND_MAX)) -1;
        }
        //generate solution
        genSolution();
        //output
        for (int i = 0; i < 4; ++i) outFile << pixel[i] << "\n";
        for (int i = 0; i < 4; ++i) outFile << outLayer[i] << "\n";
        outFile.close();
    }
    return 0;
}
