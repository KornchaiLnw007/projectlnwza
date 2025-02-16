#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <raylib.h>

class Tutorial {
public:
    void Draw(int screenWidth, int screenHeight);
    bool GoToMainMenu();
};
void Tutorial::Draw(int screenWidth, int screenHeight) {
    BeginDrawing();
    ClearBackground(PINK);
    
    // Draw tutorial instructions here
    DrawText("Welcome to the Tutorial!", screenWidth / 2 - 200, screenHeight / 2 - 40, 40, DARKGRAY);
    DrawText("Use W/A/S/D to move around", screenWidth / 2 - 150, screenHeight / 2, 20, DARKGRAY);
    DrawText("Reach the gold block to win", screenWidth / 2 - 150, screenHeight / 2 + 30, 20, DARKGRAY);
    DrawText("Press 'M' to return to main menu", screenWidth / 2 - 150, screenHeight / 2 + 60, 20, DARKGRAY);
    
    EndDrawing();
}

bool Tutorial::GoToMainMenu() {
    if (IsKeyPressed(KEY_M)) {
        return true;
    }
    return false;
}
#endif
