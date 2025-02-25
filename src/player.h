#include <raylib.h>
#include <vector>
using namespace std;

class Player {
public:
    Rectangle rect;
    Color color;
    float speed;
    Sound walkSound;  // เพิ่มเสียงเดิน
    bool isWalking;   // เช็คว่ากำลังเดินอยู่หรือไม่
    
    // คอนสตรักเตอร์ (Constructor)
    Player(float startX, float startY, float width, float height, Color startColor, float startSpeed)
        : rect({startX, startY, width, height}), color(startColor), speed(startSpeed), isWalking(false) {
        walkSound = LoadSound("src/sound/walk.mp3");  // โหลดเสียงเดิน
    }

    // ฟังก์ชันเคลื่อนที่
    void Move() {
        bool moving = false;
        
        if (IsKeyDown(KEY_W)) { rect.y -= 1.5 * speed; moving = true; }
        if (IsKeyDown(KEY_S)) { rect.y += 1.5 * speed; moving = true; }
        if (IsKeyDown(KEY_A)) { rect.x -= 1.5 * speed; moving = true; }
        if (IsKeyDown(KEY_D)) { rect.x += 1.5 * speed; moving = true; }

        // เล่นเสียงเดินเฉพาะตอนเริ่มเดิน
        if (moving) {
            if (!IsSoundPlaying(walkSound)) { // ถ้าเสียงหยุด ให้เล่นใหม่
                PlaySound(walkSound);
            }
            isWalking = true;
        } 
        else { // ถ้าหยุดเดิน ให้หยุดเสียง
            if (isWalking) {
                StopSound(walkSound);
                isWalking = false;
            }
        }
    }

    // เช็คการชนกับกำแพง
    bool CheckCollisionWithWalls(const vector<vector<int>>& maze, int rows, int cols, int cellSize) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (maze[i][j] == 1) { // Wall
                    Rectangle wallRect = {j * cellSize, i * cellSize, cellSize, cellSize};
                    if (CheckCollisionRecs(rect, wallRect)) {
                        Sound deathSound = LoadSound("src/sound/death.mp3");
                        PlaySound(deathSound);
                        return true;
                    }
                }
            }
        }
        return false; // No collision
    }

    // ฟังก์ชันวาดตัวละคร
    void Draw() {
        DrawRectangleRec(rect, color);
    }

    // ทำความสะอาดเสียงเมื่อเลิกใช้
    void UnloadSounds() {
        UnloadSound(walkSound);
    }
};
