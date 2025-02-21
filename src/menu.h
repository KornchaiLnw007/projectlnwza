#include <raylib.h>
#include <string>
#include "time.h"  // เพิ่มการใช้งาน time.h

class MainMenu {
     private:
    int screenWidth;
    int screenHeight;
    float bestTime;
    public:
        MainMenu(int screenWidth, int screenHeight);
        void Update(bool& gameStarted);
        void Draw();
};

MainMenu::MainMenu(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight) {
    bestTime = LoadBestTime();  // โหลดเวลาที่ดีที่สุดจากไฟล์เมื่อเมนูเริ่ม
}

void MainMenu::Update(bool& gameStarted) {
    // Handle the start of the game if the SPACE key is pressed
    if (IsKeyPressed(KEY_SPACE)) {
        gameStarted = true;
    }
}

