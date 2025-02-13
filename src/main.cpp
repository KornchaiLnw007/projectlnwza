#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include "player.h"
#include "maze.h" 

using namespace std;

int main() {
    const int rows = 41;  // Must be odd for proper maze generation
    const int cols = 41;  // Must be odd
    const int cellSize = 20; // Size of each cell in pixels
    const float lightRadius = 3.0 * cellSize; // Radius of circular light

    // Create Maze object
    Maze maze(rows, cols);

    // Initialize raylib window
    const int screenWidth = cols * cellSize;
    const int screenHeight = rows * cellSize;

    InitWindow(screenWidth, screenHeight, "Maze Game with Circular Lighting");
    SetTargetFPS(60);

    // Create player instance
    Player player(cellSize + cellSize / 4.0f, cellSize + cellSize / 4.0f, cellSize / 2.0f, cellSize / 2.0f, GREEN, 2.0f);

    // Finish block position (top-right corner of the maze)
    Rectangle finish = {(cols - 2) * cellSize + cellSize / 4.0f, (rows - 2) * cellSize + cellSize / 4.0f, cellSize / 2.0f, cellSize / 2.0f};
    Color finishColor = GOLD;

    bool gameOver = false;
    bool gameWin = false;

    while (!WindowShouldClose()) {
        // Restart the game on 'R'
        if (IsKeyPressed(KEY_R)) {
            maze = Maze(rows, cols);  // Recreate maze object
            player.rect.x = cellSize + cellSize / 4.0f;
            player.rect.y = cellSize + cellSize / 4.0f;
            gameOver = false;
            gameWin = false;
            player.color = GREEN;  // Reset player color to green
        }

        if (!gameOver && !gameWin) {
            // Player movement
            player.Move();

            // Collision detection with walls
            if (player.CheckCollisionWithWalls(maze.maze, rows, cols, cellSize)) {
                gameOver = true;
                player.color = RED;
            }

            // Check if the player reached the finish block
            if (CheckCollisionRecs(player.rect, finish)) {
                gameWin = true;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Circular lighting effect
        float centerX = player.rect.x + player.rect.width / 2;
        float centerY = player.rect.y + player.rect.height / 2;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                float blockCenterX = j * cellSize + cellSize / 2.0f;
                float blockCenterY = i * cellSize + cellSize / 2.0f;
                
                float dist = sqrt((blockCenterX - centerX) * (blockCenterX - centerX) +
                                  (blockCenterY - centerY) * (blockCenterY - centerY));
                
                if (dist <= lightRadius) { 
                    DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, RAYWHITE); // Light up the block
                }
            }
        }

        // Draw maze walls
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (maze.isWall(i, j)) {
                    float blockCenterX = j * cellSize + cellSize / 2.0f;
                    float blockCenterY = i * cellSize + cellSize / 2.0f;
                    float dist = sqrt((blockCenterX - centerX) * (blockCenterX - centerX) +
                                      (blockCenterY - centerY) * (blockCenterY - centerY));
                    if (dist <= lightRadius) {
                        DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, BLUE);
                    }
                }
            }
        }

        // Draw finish block
        DrawRectangleRec(finish, finishColor);

        // Draw player
        player.Draw();

        // Draw game state text
        if (gameOver) {
            DrawText("Game Over! Press 'R' to restart", 10, 10, 20, RED);
        } else if (gameWin) {
            DrawText("You Win! Press 'R' to play again", 10, 10, 20, GREEN);
        } else {
            DrawText("Use W/A/S/D to move. Reach the GOLD block!", 10, 10, 20, GREEN);
            DrawText("Press 'R' to regenerate maze", 10, 40, 20, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
