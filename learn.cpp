#include<bits/stdc++.h>
using namespace std;
const float adjustMagnitude = 0.000001;

int dataCount;
float firstLayerWeight[4][4], secondLayerWeight[4][4];
float inLayer[4], firstLayer[4], secondLayer[4], outLayer[4];
float actualOutput[4];
float firstLayerSlope[4], secondLayerSlope[4], outLayerSlope[4], secondWeightSlope[4][4], firstWeightSlope[4][4];
float preFirstLayer[4], preSecondLayer[4];    //before sigmoid

void inputWeight(){
    ifstream inFile;

    inFile.open("firstLayerWeight.txt");
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            inFile >> firstLayerWeight[i][j];
        }
    }
    inFile.close();

    inFile.open("secondLayerWeight.txt");
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            inFile >> secondLayerWeight[i][j];
        }
    }
    inFile.close();
}

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

void inputData(int n){
    ifstream inFile;
    inFile.open( genFileName(n) );
    for (int i = 0; i < 4; ++i){
        inFile >> inLayer[i];
    }
    for (int i = 0; i < 4; ++i){
        inFile >> actualOutput[i];
    }
}

void outputWeight(){
    ofstream outFile;

    outFile.open("firstLayerWeight.txt");
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            outFile << firstLayerWeight[i][j] << "\n";
        }
    }
    outFile.close();

    outFile.open("secondLayerWeight.txt");
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            outFile << secondLayerWeight[i][j] << "\n";
        }
    }
    outFile.close();
}

void calculateResult(){
    //first layer
    for (int i = 0; i < 4; ++i){
        preFirstLayer[i] = 0;
        for (int j = 0; j < 4; ++j){
            preFirstLayer[i] += inLayer[j] * firstLayerWeight[j][i];
        }
        firstLayer[i] = tanh(preFirstLayer[i]);    //tanh squashing function
    }
    //second layer
    for (int i = 0; i < 4; ++i){
        preSecondLayer[i] = 0;
        for (int j = 0; j < 4; ++j){
            preSecondLayer[i] += firstLayer[j] * secondLayerWeight[j][i];
        }
        secondLayer[i] = tanh(preSecondLayer[i]);  //tanh squashing function
    }
    //output layer
    for (int i = 0; i < 4; ++i){
        outLayer[i] = 0;
        for (int j = 0; j < 8; ++j){
            outLayer[i] += abs(secondLayer[i]);
        }
    }
}

float sigmoid(float x){
    return 1/(1+exp(-x));
}

void adjustWeight(){
    //out layer slope
    for (int i = 0; i < 4; ++i){
        if (outLayer[i] > actualOutput[i]){
            outLayerSlope[i] = 1;
        }
        else{
            outLayerSlope[i] = -1;
        }
    }
    //second layer slope
    for (int i = 0; i < 4; ++i){
        if (secondLayer[i] > 0){
            secondLayerSlope[i] = 1;
        }
        else{
            secondLayerSlope[i] = -1;
        }
        secondLayerSlope[i] *= outLayerSlope[i];
    }
    //second layer weight
    for (int i = 0; i < 4; ++i){        //first layer
        for (int j = 0; j < 4; ++j){    //second layer
            secondWeightSlope[i][j] = sigmoid(preSecondLayer[j]) * (1 - sigmoid(preSecondLayer[j])) * firstLayer[i];
            secondWeightSlope[i][j] *= secondLayerSlope[j];
            if (secondWeightSlope[i][j] > 0){   //aim is to minimize error -> adjust to opp of slope
                secondLayerWeight[i][j] -= adjustMagnitude;
                if (secondLayerWeight[i][j] < -1) secondLayerWeight[i][j] = -1;
            }
            else{
                secondLayerWeight[i][j] += adjustMagnitude;
                if (secondLayerWeight[i][j] > 1) secondLayerWeight[i][j] = 1;
            }
        }

    }
    //first layer slope
    float tmp;
    for (int i = 0; i < 4; ++i){        //first layer
        firstLayerSlope[i] = 0;
        for (int j = 0; j < 4; ++j){    //second layer
            tmp = sigmoid(preSecondLayer[j]) * (1 - sigmoid(preSecondLayer[j]));
            tmp *= secondLayerWeight[i][j] * secondLayerSlope[j];
            firstLayerSlope[i] += tmp;
        }
    }
    //first layer weight
    for (int i = 0; i < 4; ++i){        //input layer
        for (int j = 0; j < 4; ++j){    //first layer
            firstWeightSlope[i][j] = sigmoid(preFirstLayer[j]) * (1 - sigmoid(preFirstLayer[j])) * inLayer[i];
            firstWeightSlope[i][j] *= firstLayerSlope[j];
            if (firstWeightSlope[i][j] > 0){   //aim is to minimize error -> adjust to opp of slope
                firstLayerWeight[i][j] -= adjustMagnitude;
                if (firstLayerWeight[i][j] < -1) firstLayerWeight[i][j] = -1;
            }
            else{
                firstLayerWeight[i][j] += adjustMagnitude;
                if (firstLayerWeight[i][j] > 1) firstLayerWeight[i][j] = 1;
            }
        }
    }
}

int actualResult(){
    float mx = -5, type;
    for (int i = 0; i < 4; ++i){
        if (actualOutput[i] > mx){
            mx = actualOutput[i];
            type = i;
        }
    }
    return type;
}

int genResult(){
    float mx = -5, type;
    for (int i = 0; i < 4; ++i){
        if (outLayer[i] > mx){
            mx = outLayer[i];
            type = i;
        }
    }
    return type;
}

int main(){
    inputWeight();
    ifstream inFile;
    inFile.open("datacount.txt");
    inFile >> dataCount;
    inFile.close();
    int successCount = 0;
    for (int i = 1; i <= dataCount; ++i){
        inputData(i);
        calculateResult();
        if (actualResult() == genResult()) ++successCount;
        adjustWeight();
    }
    outputWeight();
    ofstream outFile;
    outFile.open("report.txt");
    outFile << "success trials: " << successCount << "\n";
    outFile.close();
    return 0;
}
