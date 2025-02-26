#include <raylib.h>
#include <vector>
using namespace std;

class Player {
public:
    Rectangle rect;
    Color color;
    float speed;
    Sound walkSound;  // เพิ่มเสียงเดิน
    bool isWalking;

    bool shieldActive;
    bool speedBoostActive;
    float shieldTime;
    float speedBoostTime;
    float shieldCooldownTime;
    float speedBoostCooldownTime;
    bool shieldCooldownStarted;
    bool speedBoostCooldownStarted;

    Player(float startX, float startY, float width, float height, Color startColor, float startSpeed)
        : rect({startX, startY, width, height}), color(startColor), speed(startSpeed), isWalking(false),
          shieldActive(false), speedBoostActive(false), shieldTime(0.0f), speedBoostTime(0.0f),
          shieldCooldownTime(0.0f), speedBoostCooldownTime(0.0f),
          shieldCooldownStarted(false), speedBoostCooldownStarted(false) {
        walkSound = LoadSound("src/sound/walk.mp3");
    }

    void Move() {
        float moveSpeed = speed;
        if (speedBoostActive) moveSpeed *= 2.0f;
        if (shieldActive) moveSpeed *= 0.5f;
        
        float newX = rect.x;
        float newY = rect.y;

        Vector2 previousPosition = { newX, newY };
        bool moving = false;
        if (IsKeyDown(KEY_W)) { newY -= 1.5 * moveSpeed; moving = true; }
        if (IsKeyDown(KEY_S)) { newY += 1.5 * moveSpeed; moving = true; }
        if (IsKeyDown(KEY_A)) { newX -= 1.5 * moveSpeed; moving = true; }
        if (IsKeyDown(KEY_D)) { newX += 1.5 * moveSpeed; moving = true; }

        if (newX < 0) newX = 0;
        if (newY < 0) newY = 0;
        if (newX + rect.width > GetScreenWidth()) newX = GetScreenWidth() - rect.width;
        if (newY + rect.height > GetScreenHeight()) newY = GetScreenHeight() - rect.height;
            
        rect.x = newX;
        rect.y = newY;

        if (rect.x == previousPosition.x && rect.y == previousPosition.y) moving = false;
        
        if (moving) {
            if (!IsSoundPlaying(walkSound)) PlaySound(walkSound);
            isWalking = true;
        } else {
            if (isWalking) {
                StopSound(walkSound);
                isWalking = false;
            }
        }
    }

    bool CheckCollisionWithWalls(const vector<vector<int>>& maze, int rows, int cols, int cellSize) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (maze[i][j] == 1) {
                    Rectangle wallRect = {j * cellSize, i * cellSize, cellSize, cellSize};
                    if (CheckCollisionRecs(rect, wallRect)) {
                        if (!shieldActive) {
                            Sound deathSound = LoadSound("src/sound/death.mp3");
                            PlaySound(deathSound);
                            ResetCooldowns();
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    void ResetCooldowns() {
        shieldCooldownTime = 0.0f;
        speedBoostCooldownTime = 0.0f;
        shieldCooldownStarted = false;
        speedBoostCooldownStarted = false;
    }

    void ActivateShield(float duration) {
        if (!shieldCooldownStarted && !speedBoostActive) {
            shieldActive = true;
            shieldTime = duration;
        }
    }
    
    void ActivateSpeedBoost(float duration) {
        if (!speedBoostCooldownStarted && !shieldActive) {
            speedBoostActive = true;
            speedBoostTime = duration;
        }
    }
    
    void DeactivateShield() {
        shieldActive = false;
        shieldTime = 0.0f;
        if (!shieldCooldownStarted) {
            shieldCooldownStarted = true;
            shieldCooldownTime = 35;
        }
    }

    void DeactivateSpeedBoost() {
        speedBoostActive = false;
        speedBoostTime = 0.0f;
        if (!speedBoostCooldownStarted) {
            speedBoostCooldownStarted = true;
            speedBoostCooldownTime = 20;
        }
    }

    void Update(float deltaTime) {
        if (shieldActive) {
            shieldTime -= deltaTime;
            if (shieldTime <= 0.0f) {
                DeactivateShield();
            }
        }

        if (speedBoostActive) {
            speedBoostTime -= deltaTime;
            if (speedBoostTime <= 0.0f) {
                DeactivateSpeedBoost();
            }
        }

        if (shieldCooldownStarted && shieldCooldownTime > 0.0f) {
            shieldCooldownTime -= deltaTime;
            if (shieldCooldownTime <= 0.0f) shieldCooldownStarted = false;
        }

        if (speedBoostCooldownStarted && speedBoostCooldownTime > 0.0f) {
            speedBoostCooldownTime -= deltaTime;
            if (speedBoostCooldownTime <= 0.0f) speedBoostCooldownStarted = false;
        }
    }

    void Draw() {
        Color playerColor = shieldActive ? WHITE : color;
        DrawRectangleRec(rect, playerColor);
        if (speedBoostActive) DrawText("Speed Boost!", rect.x + rect.width + 10, rect.y, 20, YELLOW);
        if (shieldCooldownTime > 0.0f) DrawText(TextFormat("Shield Cooldown: %.1f", shieldCooldownTime), rect.x, rect.y - 20, 20, WHITE);
        if (speedBoostCooldownTime > 0.0f) DrawText(TextFormat("Speed Boost Cooldown: %.1f", speedBoostCooldownTime), rect.x, rect.y - 40, 20, WHITE);
    }

    void UnloadSounds() {
        UnloadSound(walkSound);
    }
};
