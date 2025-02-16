#include <raylib.h>

class MainMenu {
    public:
    MainMenu(int screenWidth, int screenHeight);
    void Update(bool& gameStarted);
    void Draw();

    private:
    int screenWidth;
    int screenHeight;
};

MainMenu::MainMenu(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

void MainMenu::Update(bool& gameStarted) {
    // Handle the start of the game if the SPACE key is pressed
    if (IsKeyPressed(KEY_SPACE)) {
        gameStarted = true;
    }
}

void MainMenu::Draw() {
    // Draw the main menu screen
    BeginDrawing();
    ClearBackground(GRAY);
    DrawText("Maze Game", screenWidth / 2 - 100, screenHeight / 2 - 40, 40, GOLD);
    DrawText("Press SPACE to start", screenWidth / 2 - 100, screenHeight / 2 + 10, 20, DARKGRAY);
    EndDrawing();
}
