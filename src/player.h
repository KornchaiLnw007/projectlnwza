#include <raylib.h>
#include <vector>
using namespace std;

class Player {
    public:
        Rectangle rect;
        Color color;
        float speed;
        bool shieldActive;  // สถานะการเปิดโล่
        bool speedBoostActive;  // สถานะการเปิด Speed Boost
        float shieldTime;  // เวลาของโล่
        float speedBoostTime;  // เวลาของ Speed Boost
        float shieldCooldownTime;  // เวลาคูลดาวน์ของโล่
        float speedBoostCooldownTime;  // เวลาคูลดาวน์ของ Speed Boost
        bool shieldCooldownStarted;  // เช็คว่าเริ่มคูลดาวน์โล่แล้ว
        bool speedBoostCooldownStarted;  // เช็คว่าเริ่มคูลดาวน์ Speed Boost แล้ว

        Player(float startX, float startY, float width, float height, Color startColor, float startSpeed)
            : rect({startX, startY, width, height}), color(startColor), speed(startSpeed),
              shieldActive(false), speedBoostActive(false), shieldTime(0.0f), speedBoostTime(0.0f),
              shieldCooldownTime(0.0f), speedBoostCooldownTime(0.0f), 
              shieldCooldownStarted(false), speedBoostCooldownStarted(false) {}

        void Move() {
            float moveSpeed = speed;
            if (speedBoostActive) {
                moveSpeed *= 2.0f;  // เพิ่มความเร็ว 2 เท่าตอนที่ Speed Boost เปิดใช้งาน
            }
            if(shieldActive){
                if (IsKeyDown(KEY_W)) rect.y -= 0.5* moveSpeed;
                if (IsKeyDown(KEY_S)) rect.y += 0.5* moveSpeed;
                if (IsKeyDown(KEY_A)) rect.x -= 0.5* moveSpeed;
                if (IsKeyDown(KEY_D)) rect.x += 0.5* moveSpeed;
            }else{
                if (IsKeyDown(KEY_W)) rect.y -= 1.5 * moveSpeed;
                if (IsKeyDown(KEY_S)) rect.y += 1.5 * moveSpeed;
                if (IsKeyDown(KEY_A)) rect.x -= 1.5 * moveSpeed;
                if (IsKeyDown(KEY_D)) rect.x += 1.5 * moveSpeed;
            }

        }

        bool CheckCollisionWithWalls(const vector<vector<int>>& maze, int rows, int cols, int cellSize) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (maze[i][j] == 1) { // Wall
                        Rectangle wallRect = {j * cellSize, i * cellSize, cellSize, cellSize};
                        if (CheckCollisionRecs(rect, wallRect)) {
                            // ตรวจสอบสถานะโล่ หากเปิดใช้งานโล่ไม่ให้เกิดการชน
                            if (!shieldActive) {
                                Sound deathSound = LoadSound("src/sound/death.mp3");
                                PlaySound(deathSound);
                                ResetCooldowns();  // รีเซ็ตการคูลดาวน์เมื่อผู้เล่นตาย
                                return true; // Collision detected
                            }
                        }
                    }
                }
            }
            return false; // No collision
        }

        void ResetCooldowns() {
            // รีเซ็ตการคูลดาวน์เมื่อผู้เล่นตาย
            shieldCooldownTime = 0.0f;
            speedBoostCooldownTime = 0.0f;
            shieldCooldownStarted = false;
            speedBoostCooldownStarted = false;
        }

        void ActivateShield(float duration) {
            if (!shieldCooldownStarted && !speedBoostActive) {  // ตรวจสอบว่าโล่ไม่ได้อยู่ในคูลดาวน์และ Speed Boost ยังไม่ได้เปิดใช้งาน
                shieldActive = true;
                shieldTime = duration;  // ตั้งเวลาให้โล่ใช้งาน
            }
        }
        
        void ActivateSpeedBoost(float duration) {
            if (!speedBoostCooldownStarted && !shieldActive) {  // ตรวจสอบว่า Speed Boost ไม่อยู่ในคูลดาวน์และโล่ไม่ได้เปิดใช้งาน
                speedBoostActive = true;
                speedBoostTime = duration;  // ตั้งเวลาให้ Speed Boost ใช้งาน
            }
        }
        

        void DeactivateShield() {
            shieldActive = false;
            shieldTime = 0.0f; // Reset shield time when deactivated
            // เริ่มคูลดาวน์โล่เมื่อโล่ถูกปิด
            if (!shieldCooldownStarted) {
                shieldCooldownStarted = true;
                shieldCooldownTime = 35;  // กำหนดเวลาคูลดาวน์
            }
        }

        void DeactivateSpeedBoost() {
            speedBoostActive = false;
            speedBoostTime = 0.0f; // Reset speed boost time when deactivated
            // เริ่มคูลดาวน์ Speed Boost เมื่อหมดเวลา
            if (!speedBoostCooldownStarted) {
                speedBoostCooldownStarted = true;
                speedBoostCooldownTime = 20;  // กำหนดเวลาคูลดาวน์
            }
        }

        void Update(float deltaTime) {
            // อัพเดทเวลาของโล่
            if (shieldActive) {
                shieldTime -= deltaTime;
                if (shieldTime <= 0.0f) {
                    shieldActive = false;  // ปิดโล่เมื่อหมดเวลา
                    DeactivateShield();  // เริ่มคูลดาวน์
                }
            }

            // อัพเดทเวลาของ Speed Boost
            if (speedBoostActive) {
                speedBoostTime -= deltaTime;
                if (speedBoostTime <= 0.0f) {
                    speedBoostActive = false;  // ปิด Speed Boost เมื่อหมดเวลา
                    DeactivateSpeedBoost();  // เริ่มคูลดาวน์
                }
            }

            // ลดเวลาคูลดาวน์
            if (shieldCooldownStarted && shieldCooldownTime > 0.0f) {
                shieldCooldownTime -= deltaTime;  // ลดเวลาคูลดาวน์โล่
                if (shieldCooldownTime <= 0.0f) {
                    shieldCooldownTime = 0.0f;
                    shieldCooldownStarted = false;  // สิ้นสุดคูลดาวน์โล่
                }
            }

            if (speedBoostCooldownStarted && speedBoostCooldownTime > 0.0f) {
                speedBoostCooldownTime -= deltaTime;  // ลดเวลาคูลดาวน์ Speed Boost
                if (speedBoostCooldownTime <= 0.0f) {
                    speedBoostCooldownTime = 0.0f;
                    speedBoostCooldownStarted = false;  // สิ้นสุดคูลดาวน์ Speed Boost
                }
            }
        }

        void Draw() {
            // เมื่อโล่เปิดใช้งาน เปลี่ยนสีตัวละครเป็นสีขาว
            Color playerColor = shieldActive ? WHITE : color;
            DrawRectangleRec(rect, playerColor); // วาดตัวละครด้วยสีที่เปลี่ยนไป

            // Display Speed Boost status if active
            if (speedBoostActive) {
                DrawText("Speed Boost!", rect.x + rect.width + 10, rect.y, 20, YELLOW);
            }

            // แสดงเวลาคูลดาวน์
            if (shieldCooldownTime > 0.0f) {
                DrawText(TextFormat("Shield Cooldown: %.1f", shieldCooldownTime), rect.x, rect.y - 20, 20, WHITE);
            }

            if (speedBoostCooldownTime > 0.0f) {
                DrawText(TextFormat("Speed Boost Cooldown: %.1f", speedBoostCooldownTime), rect.x, rect.y - 40, 20, WHITE);
            }
        }
};