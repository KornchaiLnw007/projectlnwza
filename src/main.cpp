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
    InitAudioDevice(); 
    const int cols = 47;
    const int rows = 25;
    const int cellSize = 40;
    const float lightRadius = 3 * cellSize;

    Maze maze(rows, cols);
    const int screenWidth = cols * cellSize;
    const int screenHeight = rows * cellSize;

    InitWindow(screenWidth, screenHeight, "Maze Game");
    SetTargetFPS(60);

    Player player(cellSize + cellSize / 4.0f, cellSize + cellSize / 4.0f, cellSize / 2.0f, cellSize / 2.0f, GREEN, 2.0f);
    Rectangle finish = {(cols - 2) * cellSize + cellSize / 4.0f, (rows - 2) * cellSize + cellSize / 4.0f, cellSize / 2.0f, cellSize / 2.0f};
    Color finishColor = GOLD;

    bool gameOver = false;
    bool gameWin = false;
    bool gameStarted = false;
    bool inTutorial = false;  // New flag for tutorial mode 
    

    Music mainMusic = LoadMusicStream("src/sound/mainsong.mp3");
    Music gameMusic = LoadMusicStream("src/sound/gamesong.mp3");
    PlayMusicStream(mainMusic);
    SetMusicVolume(mainMusic, 1.1f);
    SetMusicVolume(gameMusic, 1.5f);

    Timer gameTimer;
    gameTimer.Reset();
    MainMenu mainMenu(screenWidth, screenHeight);
    Tutorial tutorialScreen;
    Image menuImage = LoadImage("src/Graphic/menu.png");
    //ImageResize(&menuImage, 953*1.5, 648*1.5);
    Texture2D MenuImage = LoadTextureFromImage(menuImage);
    UnloadImage(menuImage); // ไม่ต้องใช้ Image แล้ว

    while (!WindowShouldClose()) {
        UpdateMusicStream(mainMusic); // อัปเดตเพลงเมนู
        UpdateMusicStream(gameMusic); // อัปเดตเพลงเกม
        if (!gameStarted && !inTutorial) {
            BeginDrawing();
            ClearBackground(WHITE); 
            DrawTexture(MenuImage, 0, 0, WHITE);
            DrawText("MAZE of", screenWidth / 2 + 330, screenHeight / 2 - 360, 100, BLACK);
            DrawText("LEGENDS", screenWidth / 2 + 400, screenHeight / 2 - 280, 100, BLACK);
            DrawText("MAZE of", screenWidth / 2 + 315, screenHeight / 2 - 370, 100, GOLD);
            DrawText("LEGENDS", screenWidth / 2 + 385, screenHeight / 2 - 290, 100, GOLD);
            DrawText("> |Press 'SPACE' to start|", screenWidth / 2 + 305, screenHeight / 2 + 105, 30, BLACK);
            DrawText("> |Press 'T' for tutorial|", screenWidth / 2 + 305, screenHeight / 2 +255, 30, BLACK);
            DrawText("> |Press 'SPACE' to start|", screenWidth / 2 + 295, screenHeight / 2 + 100, 30, LIME);
            DrawText("> |Press 'T' for tutorial|", screenWidth / 2 + 295, screenHeight / 2 +250, 30, LIME);
            // Display the best time from the file
            float bestTime = LoadBestTime();
            if (bestTime != -1) {
                Color myColor = (Color){255, 100, 100, 255};
                DrawText(TextFormat("Best Time: %.2f seconds", bestTime), screenWidth / 2 + 395, screenHeight / 2 - 170, 30, BLACK);
                DrawText(TextFormat("Best Time: %.2f seconds", bestTime), screenWidth / 2 + 385, screenHeight / 2 - 175, 30, myColor);
                
            }
            EndDrawing();

            if (IsKeyPressed(KEY_SPACE)) {
                gameStarted = true;
                StopMusicStream(mainMusic);
                PlayMusicStream(gameMusic);
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
                if (!IsMusicStreamPlaying(mainMusic)) {
                    PlayMusicStream(mainMusic);
                }

            }
            continue;
        }

        if (IsKeyPressed(KEY_M)) {
            gameStarted = false;
            gameOver = false;
            gameWin = false;
            maze = Maze(rows, cols);
            player.rect.x = cellSize + cellSize / 4.0f;
            player.rect.y = cellSize + cellSize / 4.0f;
            gameTimer.Reset();
            StopMusicStream(gameMusic); // หยุดเพลงเกม
            PlayMusicStream(mainMusic); // เล่นเพลงเมนูใหม่
            player.color = GREEN;
            player.DeactivateShield();
            player.DeactivateSpeedBoost();
            player.ResetCooldowns();
        }

        if (IsKeyPressed(KEY_R)) {
            maze = Maze(rows, cols);
            player.rect.x = cellSize + cellSize / 4.0f;
            player.rect.y = cellSize + cellSize / 4.0f;
            gameOver = false;
            gameWin = false;
            player.color = GREEN;
            gameTimer.Reset();
            player.DeactivateShield();
            player.DeactivateSpeedBoost();
            player.ResetCooldowns();
        }

        if (IsKeyPressed(KEY_Q)) {  // กด Q เพื่อเปิดโล่
            player.ActivateShield(3.0f);  // โล่เปิดใช้งานเป็นเวลา 3 วินาที
        }
        
        if (IsKeyPressed(KEY_E)) {  // กด E เพื่อเปิด Speed Boost
            player.ActivateSpeedBoost(3.0f);  // Speed Boost เปิดใช้งานเป็นเวลา 3 วินาที
        }
        
        if (gameOver || gameWin) {
            // Deactivate player skills if the game is over
            player.DeactivateShield();
            player.DeactivateSpeedBoost();
            player.ResetCooldowns();
        }
        
        player.Update(GetFrameTime());  // อัพเดทสถานะของสกิล

        if (!gameOver && !gameWin) {
            gameTimer.Update();
            player.Move();

            // Check if player has used Speed Boost or Shield, and handle them accordingly
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
        DrawText("Press 'E' to use speed boost", 500, 965, 30, GREEN);
        DrawText("Press 'Q' to use sheild", 1000, 965, 30, GREEN);
        
        // Game over or win messages
        if (gameOver) {
            int textWidth = MeasureText("Game Over!", 100);
            int textWidth2 = MeasureText("Press 'R' to play again", 30);
            DrawText("Game Over!", (screenWidth - textWidth) / 2, screenHeight / 2-50, 100, RED);
            DrawText("Press 'R' to play again", (screenWidth - textWidth2) / 2, screenHeight / 2 + 50, 30, RED);
        } else if (gameWin) {
            DrawText("Press 'R' to play again", 10, 10, 30, GREEN);
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
            int timeWidth = MeasureText(TextFormat("Your Time: %.2f seconds", gameTimer.time), 30);
            
            DrawText(grade.c_str(), (screenWidth - textWidth) / 2, screenHeight / 2 - 50, 100, GREEN);
            DrawText(TextFormat("Your Time: %.2f seconds", gameTimer.time), (screenWidth - timeWidth) / 2, screenHeight / 2 +50 , 30, GREEN);

        }

        EndDrawing();
    }
    UnloadMusicStream(mainMusic);
    UnloadMusicStream(gameMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;

}