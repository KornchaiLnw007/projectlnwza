#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include "player.h"
#include "maze.h" 
#include "timer.h"  

using namespace std;

int main() {
    const int rows = 41; 
    const int cols = 41;
    const int cellSize = 20;
    const float lightRadius = 4.0 * cellSize;

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
    bool gameStarted = false;  // ตัวแปรเพื่อตรวจสอบว่าเกมเริ่มแล้วหรือยัง

    Timer gameTimer;
    gameTimer.Reset();

    while (!WindowShouldClose()) {
        if (!gameStarted) {
            // แสดงหน้าจอหลัก
            BeginDrawing();
            ClearBackground(GRAY);
            DrawText("Maze Game", screenWidth / 2 - 100, screenHeight / 2 - 40, 40, GOLD);
            DrawText("Press SPACE to start", screenWidth / 2 - 100, screenHeight / 2 + 10, 20, DARKGRAY);
            EndDrawing();

            // ถ้ากด Spacebar, เกมเริ่ม
            if (IsKeyPressed(KEY_SPACE)) {
                gameStarted = true;
            }
            continue; // ข้ามขั้นตอนอื่นๆ ที่จะไปที่หน้าจอเกม
        }

        // ถ้ากด M จะกลับไปที่หน้าจอหลัก
        if (IsKeyPressed(KEY_M)) {
            gameStarted = false;  // เปลี่ยนสถานะกลับไปที่หน้าจอหลัก
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

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                float blockCenterX = j * cellSize + cellSize / 2.0f;
                float blockCenterY = i * cellSize + cellSize / 2.0f;
                
                float distSquared = (blockCenterX - centerX) * (blockCenterX - centerX) + (blockCenterY - centerY) * (blockCenterY - centerY);
                if (distSquared <= lightRadius * lightRadius) {
                    DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, RAYWHITE);
                }
            }
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (maze.isWall(i, j)) {
                    float blockCenterX = j * cellSize + cellSize / 2.0f;
                    float blockCenterY = i * cellSize + cellSize / 2.0f;
                    float distSquared = (blockCenterX - centerX) * (blockCenterX - centerX) + (blockCenterY - centerY) * (blockCenterY - centerY);
                    if (distSquared <= lightRadius * lightRadius) {
                        DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, BLUE);
                    }
                }
            }
        }

        DrawRectangleRec(finish, finishColor);

        player.Draw();

        gameTimer.Draw(610, 10);

        if (gameOver) {
            DrawText("Game Over! Press 'R' to restart", 10, 10, 20, RED);
        } else if (gameWin) {
            DrawText("You Win! Press 'R' to play again", 10, 10, 20, GREEN);
            DrawText(TextFormat("Time: %.2f seconds", gameTimer.time), 10, 40, 20, GREEN);
        } else {
            DrawText("Use W/A/S/D to move. Reach the GOLD block!", 10, 10, 20, GREEN);
            DrawText("Press 'R' to regenerate maze", 10, 40, 20, GREEN);
            DrawText("Press 'M' to main menu", 10, 795, 20, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
