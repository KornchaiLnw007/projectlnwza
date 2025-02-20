#include <raylib.h>
#include <string>
#include "time.h"  // เพิ่มการใช้งาน time.h

class MainMenu {
     private:
    int screenWidth;
    int screenHeight;
    
    public:
<<<<<<< HEAD
    MainMenu(int screenWidth, int screenHeight);
    void Update(bool& gameStarted);
    void Draw();
=======
        MainMenu(int screenWidth, int screenHeight);
        void Update(bool& gameStarted);
        void Draw();

    private:
        int screenWidth;
        int screenHeight;
        float bestTime;
>>>>>>> c04810484831f678b356c2e8b205ce10b363cfd5
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

void MainMenu::Draw() {
    BeginDrawing();
    ClearBackground(GRAY);

    // แสดงชื่อเกม
    DrawText("Maze Game", screenWidth / 2 - 100, screenHeight / 2 - 40, 40, GOLD);
    DrawText("Press SPACE to start", screenWidth / 2 - 100, screenHeight / 2 + 10, 20, DARKGRAY);

    // แสดงเวลาเร็วที่สุด
    if (bestTime >= 0) {
        DrawText(TextFormat("Best Time: %.2f seconds", bestTime), screenWidth / 2 - 100, screenHeight / 2 + 50, 20, GREEN);
    } else {
        DrawText("No best time recorded", screenWidth / 2 - 100, screenHeight / 2 + 50, 20, RED);
    }

    EndDrawing();
}
