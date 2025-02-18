#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <fstream>

using namespace std;

void SaveTimeToFile(float time) {
    ofstream outFile("time.txt");
    if (outFile.is_open()) {
        outFile << time;
        outFile.close();
    } else {
        cout << "Unable to open file for saving time." << endl;
    }
}

float LoadBestTime() {
    ifstream inFile("time.txt");
    if (inFile.is_open()) {
        float bestTime;
        inFile >> bestTime;
        inFile.close();
        return bestTime;
    } else {
        return -1;  // ถ้าไม่สามารถเปิดไฟล์หรือไม่มีข้อมูล
    }
}

#endif // TIME_H
