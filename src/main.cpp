#include <raylib.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>
#include "player.h"

using namespace std;

// Define directions: right, down, left, up
const int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

// Maze generation using DFS
void generateMaze(vector<vector<int>>& maze, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            maze[i][j] = 1; // 1 represents a wall
        }
    }

    stack<pair<int, int>> st;
    st.push({1, 1});
    maze[1][1] = 0; // 0 represents a path

    random_device rd;
    mt19937 rng(rd());

    while (!st.empty()) {
        pair<int, int> current = st.top();
        st.pop();

        vector<int> dirs = {0, 1, 2, 3};
        shuffle(dirs.begin(), dirs.end(), rng);

        for (int dir : dirs) {
            int ni = current.first + directions[dir][0] * 2;
            int nj = current.second + directions[dir][1] * 2;

            if (ni >= 1 && ni < rows && nj >= 1 && nj < cols && maze[ni][nj] == 1) {
                maze[ni][nj] = 0;
                maze[current.first + directions[dir][0]][current.second + directions[dir][1]] = 0;
                st.push({ni, nj});
            }
        }
    }
}


int main() {
    const int rows = 41;  // Must be odd for proper maze generation
    const int cols = 41;  // Must be odd
    const int cellSize = 20; // Size of each cell in pixels
    const float lightRadius = 3.0 * cellSize; // Radius of circular light

    vector<vector<int>> maze(rows, vector<int>(cols, 1));
    generateMaze(maze, rows, cols);

    // Initialize raylib
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
            generateMaze(maze, rows, cols);
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
            if (player.CheckCollisionWithWalls(maze, rows, cols, cellSize)) {
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
                if (maze[i][j] == 1) {
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
