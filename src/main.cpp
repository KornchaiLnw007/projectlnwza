#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include "player.h"
#include "maze.h" 
#include "timer.h" 
#include "menu.h"
#include "tutorial.h" 

using namespace std;

int main() {
    const int cols = 47; 
    const int rows = 25;  
    const int cellSize = 40;
    const float lightRadius = 3.0 * cellSize;  

    Maze maze(rows, cols);

    const int screenWidth = cols * cellSize;
    const int screenHeight = rows * cellSize;

    InitWindow(screenWidth, screenHeight, "Maze Game with Circular Lighting");
    SetTargetFPS(60);

    Player player(cellSize + cellSize / 4.0f, cellSize + cellSize / 4.0f, cellSize / 2.0f, cellSize / 2.0f, GREEN, 2.0f);
    
    Rectangle finish = {(cols - 2) * cellSize + cellSize / 4.0f, (rows - 2) * cellSize + cellSize / 4.0f, cellSize / 2.0f, cellSize / 2.0f};
    Color finishColor = GOLD;

    bool gameOver = false;
    bool gameWin = false;
    bool gameStarted = false;
    bool inTutorial = false;  // New flag for tutorial mode 

    Timer gameTimer;
    gameTimer.Reset();
    MainMenu mainMenu(screenWidth, screenHeight);
    Tutorial tutorialScreen;  // Assuming you have a tutorial class

    while (!WindowShouldClose()) {
         Image menuImage = LoadImage("src/Graphic/Nar.png"); 
                
        if (!gameStarted && !inTutorial) {
           
            
            // Main menu
            BeginDrawing();
    
            ImageResize(&menuImage, 953*1.5, 648*1.5);
            Texture2D MenuImage = LoadTextureFromImage(menuImage);  // Load the image
            UnloadImage(menuImage);
            ClearBackground(WHITE); 
           
            DrawTexture(MenuImage, 240, 10, WHITE);
            DrawText("MAZE of", screenWidth / 2 + 295, screenHeight / 2 - 360, 60, BLACK);
            DrawText("LEGENDS", screenWidth / 2 + 365, screenHeight / 2 - 280, 60, BLACK);
            DrawText("MAZE of", screenWidth / 2 + 280, screenHeight / 2 - 370, 60, GOLD);
            DrawText("LEGENDS", screenWidth / 2 + 350, screenHeight / 2 - 290, 60, GOLD);
            DrawText("> |Press 'SPACE' to start|", screenWidth / 2 + 305, screenHeight / 2 + 105, 30, BLACK);
            DrawText("> |Press 'T' for tutorial|", screenWidth / 2 + 305, screenHeight / 2 +255, 30, BLACK);
            DrawText("> |Press 'SPACE' to start|", screenWidth / 2 + 295, screenHeight / 2 + 100, 30, LIME);
            DrawText("> |Press 'T' for tutorial|", screenWidth / 2 + 295, screenHeight / 2 +250, 30, LIME);
             // Tutorial option
            EndDrawing();

            if (IsKeyPressed(KEY_SPACE)) {
                gameStarted = true;
            }
            if (IsKeyPressed(KEY_T)) {
                inTutorial = true;  // Go to tutorial when T is pressed
                gameStarted = false;  // Don't start the game
            }
            continue;
        }
        

        if (inTutorial) {
            // Display tutorial screen
            tutorialScreen.Draw(screenWidth, screenHeight);
            if (tutorialScreen.GoToMainMenu()) {
                inTutorial = false;  // Back to main menu
                gameStarted = false;
                gameOver = false;
                gameWin = false;
                gameTimer.Reset();
            }
            continue; // Skip the rest of the game logic
        }

        // If the game is running
        if (IsKeyPressed(KEY_M)) {
            maze = Maze(rows, cols);
            player.rect.x = cellSize + cellSize / 4.0f;
            player.rect.y = cellSize + cellSize / 4.0f;
            gameStarted = false;
            gameOver = false;
            gameWin = false;
            gameTimer.Reset();
        }

        if (IsKeyPressed(KEY_R)) {
            maze = Maze(rows, cols);
            player.rect.x = cellSize + cellSize / 4.0f;
            player.rect.y = cellSize + cellSize / 4.0f;
            gameOver = false;
            gameWin = false;
            player.color = GREEN;
            gameTimer.Reset();
        }

        if (!gameOver && !gameWin) {
            gameTimer.Update();
            player.Move();

            if (player.CheckCollisionWithWalls(maze.maze, rows, cols, cellSize)) {
                gameOver = true;
                player.color = RED;
                gameTimer.Stop();
            }

            if (CheckCollisionRecs(player.rect, finish)) {
                gameWin = true;
                gameTimer.Stop();
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        float centerX = player.rect.x + player.rect.width / 2;
        float centerY = player.rect.y + player.rect.height / 2;

        // Draw lighting effect
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                float blockCenterX = j * cellSize + cellSize / 2.0f;
                float blockCenterY = i * cellSize + cellSize / 2.0f;
                float distSquared = (blockCenterX - centerX) * (blockCenterX - centerX) + (blockCenterY - centerY) * (blockCenterY - centerY);
                if (distSquared <= lightRadius * lightRadius) {
                    DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, DARKBROWN);
                }
            }
        }

        // Draw walls
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (maze.isWall(i, j)) {
                    float blockCenterX = j * cellSize + cellSize / 2.0f;
                    float blockCenterY = i * cellSize + cellSize / 2.0f;
                    float distSquared = (blockCenterX - centerX) * (blockCenterX - centerX) + (blockCenterY - centerY) * (blockCenterY - centerY);
                    if (distSquared <= lightRadius * lightRadius) {
                        DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, DARKGRAY);
                    }
                }
            }
        }   

        // Draw finish block
        DrawRectangleRec(finish, finishColor);
        player.Draw();
        gameTimer.Draw(1575, 10);
        DrawText("Press 'M' to main menu", 10, 965, 30, GREEN);

        // Game over or win messages
        if (gameOver) {
            DrawText("Game Over! Press 'R' to restart", 10, 10, 30, RED);
        } else if (gameWin) {
            DrawText("You Win! Press 'R' to play again", 10, 10, 30, GREEN);
            DrawText(TextFormat("Your Time: %.2f seconds", gameTimer.time), 1570, 40 , 30, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
