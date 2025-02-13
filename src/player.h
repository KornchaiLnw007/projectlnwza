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
            if (IsKeyDown(KEY_W)) rect.y -= speed;
            if (IsKeyDown(KEY_S)) rect.y += speed;
            if (IsKeyDown(KEY_A)) rect.x -= speed;
            if (IsKeyDown(KEY_D)) rect.x += speed;
        }
    
        bool CheckCollisionWithWalls(const vector<vector<int>>& maze, int rows, int cols, int cellSize) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (maze[i][j] == 1) { // Wall
                        Rectangle wallRect = {j * cellSize, i * cellSize, cellSize, cellSize};
                        if (CheckCollisionRecs(rect, wallRect)) {
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