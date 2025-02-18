#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void SaveTimeToFile(float time) {
    // บันทึกทุกครั้งที่มีการเล่นจบ
    ofstream outFile("time.txt", ios::app);  // เปิดไฟล์ในโหมด append (เพิ่มข้อมูล)
    if (outFile.is_open()) {
        outFile << time << endl;  // บันทึกเวลาใหม่ในบรรทัดใหม่
        outFile.close();
    } else {
        cout << "Unable to open file for saving time." << endl;
    }
}

float LoadBestTime() {
    ifstream inFile("time.txt");
    float bestTime = -1;
    float time;

    if (inFile.is_open()) {
        while (inFile >> time) {
            if (bestTime == -1 || time < bestTime) {
                bestTime = time;  // ค้นหาเวลาที่เร็วที่สุด
            }
        }
        inFile.close();
    } else {
        cout << "Unable to open file for loading time." << endl;
    }

    return bestTime;
}

#endif
