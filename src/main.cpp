#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include "player.h"
#include "maze.h"
#include "timer.h"
#include "menu.h"
#include "tutorial.h"
#include "time.h"  

using namespace std;

int main() {
    const int cols = 47;
    const int rows = 25;
    const int cellSize = 40;
    const float lightRadius = 200 * cellSize;

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
    Tutorial tutorialScreen;
    while (!WindowShouldClose()) {
        if (!gameStarted && !inTutorial) {
            // Main menu
            Image menuImage = LoadImage("src/Graphic/Nar.png");
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
            // Display the best time from the file
            float bestTime = LoadBestTime();
            if (bestTime != -1) {
                Color myColor = (Color){255, 100, 100, 255};
                DrawText(TextFormat("Best Time: %.2f seconds", bestTime), screenWidth / 2 + 295, screenHeight / 2 - 170, 30, BLACK);
                DrawText(TextFormat("Best Time: %.2f seconds", bestTime), screenWidth / 2 + 285, screenHeight / 2 - 175, 30, myColor);
                
            }
            EndDrawing();

            if (IsKeyPressed(KEY_SPACE)) {
                gameStarted = true;
            }
            if (IsKeyPressed(KEY_T)) {
                inTutorial = true;
                gameStarted = false;
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
            continue;
        }

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
            DrawText(TextFormat("Your Time: %.2f seconds", gameTimer.time), 1535, 40 , 30, GREEN);
            // Save time after player wins
            SaveTimeToFile(gameTimer.time);

            string grade;
            if (gameTimer.time < 30) {
                grade = "Excellent!";
            } else if (gameTimer.time < 60) {
                grade = "Good job!";
            } else if (gameTimer.time < 90) {
                grade = "Not bad!";
            } else if (gameTimer.time < 120) {
                grade = "Try harder!";
            } else {
                grade = "KAK!!!";
            }
            int textWidth = MeasureText(grade.c_str(), 100);
            DrawText(grade.c_str(), (screenWidth - textWidth) / 2, screenHeight / 2 - 50, 100, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
