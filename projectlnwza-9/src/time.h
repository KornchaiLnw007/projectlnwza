#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ประกาศ prototype ของ LoadBestTime ก่อน
float LoadBestTime();

void SaveTimeToFile(float time) {
    float bestTime = LoadBestTime();
    
    // ถ้ายังไม่มีข้อมูลหรือเวลาที่เล่นได้ดีกว่าเวลาที่ดีที่สุดเดิม ให้บันทึกใหม่
    if (bestTime == -1 || time < bestTime) {
        ofstream outFile("time.txt", ios::trunc);  // เปิดไฟล์แบบเขียนทับ
        if (outFile.is_open()) {
            outFile << time << endl;  // บันทึกเวลาใหม่
            outFile.close();
        } else {
            cout << "Unable to open file for saving time." << endl;
        }
    }
}

float LoadBestTime() {
    ifstream inFile("time.txt");
    float bestTime = -1;
    float time;

    if (inFile.is_open()) {
        if (inFile >> bestTime) {
            // อ่านค่าเวลาที่ดีที่สุดจากไฟล์ (ถ้ามี)
        }
        inFile.close();
    }
    
    return bestTime;
}

#endif