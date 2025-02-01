#include <raylib.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>

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
    const int playerWidth = cellSize / 2; // Width of the player rectangle
    const int playerHeight = cellSize / 2; // Height of the player rectangle
    const int fovWidth = 5 * cellSize; // Width of the rectangular FOV (field of view)
    const int fovHeight = 5 * cellSize; // Height of the rectangular FOV

    vector<vector<int>> maze(rows, vector<int>(cols, 1));
    generateMaze(maze, rows, cols);

    // Initialize raylib
    const int screenWidth = cols * cellSize;
    const int screenHeight = rows * cellSize;

    InitWindow(screenWidth, screenHeight, "Maze Game with Finish Block");
    SetTargetFPS(60);

    // Define player attributes (rectangle)
    Rectangle player = {cellSize + cellSize / 4.0f, cellSize + cellSize / 4.0f, playerWidth, playerHeight}; // Starting position
    Color playerColor = GREEN;

    // Finish block position (top-right corner of the maze)
    Rectangle finish = {(cols - 2) * cellSize + cellSize / 4.0f, (rows - 2) * cellSize + cellSize / 4.0f, cellSize / 2.0f, cellSize / 2.0f};
    Color finishColor = GOLD;

    bool gameOver = false;
    bool gameWin = false;

    while (!WindowShouldClose()) {
        // Restart the game on 'R'
        if (IsKeyPressed(KEY_R)) {
            generateMaze(maze, rows, cols);
            player.x = cellSize + cellSize / 4.0f;
            player.y = cellSize + cellSize / 4.0f;
            gameOver = false;
            gameWin = false;
            playerColor = GREEN;  // Reset player color to green
        }

        if (!gameOver && !gameWin) {
            // Player movement
            if (IsKeyDown(KEY_W)) player.y -= 2.0f;
            if (IsKeyDown(KEY_S)) player.y += 2.0f;
            if (IsKeyDown(KEY_A)) player.x -= 2.0f;
            if (IsKeyDown(KEY_D)) player.x += 2.0f;

            // Collision detection with walls (check if any part of the rectangle hits the wall)
            int cellX = static_cast<int>(player.x / cellSize);
            int cellY = static_cast<int>(player.y / cellSize);

            // Check if the player hits any part of the wall (sodium blue color)
            if (!gameOver) {
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        if (maze[i][j] == 1) { // Check if it's a wall
                            Rectangle wallRect = {j * cellSize, i * cellSize, cellSize, cellSize};
                            if (CheckCollisionRecs(player, wallRect)) { // Collision with wall
                                gameOver = true;
                                playerColor = RED; // Change player color to red to indicate death
                            }
                        }
                    }
                }
            }

            // Check if the player reached the finish block
            if (CheckCollisionRecs(player, finish)) {
                gameWin = true;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK); // Fill the background with black

        // Darken the entire screen, then reveal the light area around the player (rectangular FOV)
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                // Calculate distance from player to each block
                int dx = abs(j - static_cast<int>(player.x / cellSize));
                int dy = abs(i - static_cast<int>(player.y / cellSize));

                // If within the radius of light, reveal the block
                if (dx <= fovWidth / cellSize && dy <= fovHeight / cellSize) {
                    float dist = sqrt(dx * dx + dy * dy);
                    if (dist <= fovWidth / 2.0f) {
                        // Reveal this block (wall or path) in the light area
                        DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, RAYWHITE); // Light up the block
                    }
                }
            }
        }

        // Draw maze in the lighted area (Blue for walls)
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (maze[i][j] == 1) {
                    // Only draw the wall if it's inside the illuminated area
                    int dx = abs(j - static_cast<int>(player.x / cellSize));
                    int dy = abs(i - static_cast<int>(player.y / cellSize));
                    if (dx <= fovWidth / cellSize && dy <= fovHeight / cellSize) {
                        float dist = sqrt(dx * dx + dy * dy);
                        if (dist <= fovWidth / 2.0f) {
                            DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, BLUE); // Wall in blue
                        }
                    }
                }
            }
        }

        // Draw finish block
        DrawRectangleRec(finish, finishColor);

        // Draw player as a rectangle
        DrawRectangleRec(player, playerColor);

        // Draw instructions and game state
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
