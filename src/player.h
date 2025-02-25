#include<raylib.h>
using namespace std;

class Player {
    public:
        Rectangle rect;
        Color color;
        float speed;
    
        Player(float startX, float startY, float width, float height, Color startColor, float startSpeed)
            : rect({startX, startY, width, height}), color(startColor), speed(startSpeed) {}
    
        void Move() {
            if (IsKeyDown(KEY_W)) rect.y -= 1.5*speed;
            if (IsKeyDown(KEY_S)) rect.y += 1.5*speed;
            if (IsKeyDown(KEY_A)) rect.x -= 1.5*speed;
            if (IsKeyDown(KEY_D)) rect.x += 1.5*speed;
        }
    
        bool CheckCollisionWithWalls(const vector<vector<int>>& maze, int rows, int cols, int cellSize) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (maze[i][j] == 1) { // Wall
                        Rectangle wallRect = {j * cellSize, i * cellSize, cellSize, cellSize};
                        if (CheckCollisionRecs(rect, wallRect)) {
                            Sound deathSound = LoadSound("src/sound/death.mp3");
                            PlaySound(deathSound);
                            return true; // Collision detected
                        }
                    }
                }
            }
            return false; // No collision
        }
    
        void Draw() {
            DrawRectangleRec(rect, color);
        }
};