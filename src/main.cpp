#include "raylib.h"

// ขนาดของตัวละคร
const float playerWidth = 50;
const float playerHeight = 50;

//ขนาดของสิ่งกีดขวาง
const float obstacleWidth = 100.0f;
const float obstacleHeight = 40.0f;


// ตัวแปรสำหรับการเคลื่อนไหว
Vector2 playerPosition = { 100.0f, 400.0f }; // ตำแหน่งเริ่มต้นของตัวละคร
float playerSpeed = 5.0; // ความเร็วในการเคลื่อนไหว
float gravity = 0.5;     // ความแรงของแรงโน้มถ่วง
float jumpSpeed = -12.0; // ความเร็วในการกระโดด
float velocityY = 0.0;   // ความเร็วในแนว Y (แนวตั้ง)

bool isJumping = false;   // ตรวจสอบว่าอยู่ในระหว่างกระโดดหรือไม่

Rectangle obstacle = { 400.0,500,obstacleWidth,obstacleHeight,};

bool CheckCollisionWithObstacle(Vector2 playerPos, Rectangle obstacle) {
    return (playerPos.x + playerWidth > obstacle.x && playerPos.x < obstacle.x + obstacleWidth &&
            playerPos.y + playerHeight > obstacle.y && playerPos.y < obstacle.y + obstacleHeight);
}

int main() {
    // เริ่มต้นการใช้งาน raylib
    InitWindow(800, 600, "GGEZ");

    // ตั้งค่าการแสดงผล
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // คำนวณการเคลื่อนไหวของตัวละคร
        if (IsKeyDown(KEY_RIGHT)) {
            playerPosition.x += playerSpeed; // เคลื่อนไปขวา
        }
        if (IsKeyDown(KEY_LEFT)) {
            playerPosition.x -= playerSpeed; // เคลื่อนไปซ้าย
        }

        // การกระโดด
        if (IsKeyPressed(KEY_SPACE) && !isJumping) {
            velocityY = jumpSpeed; // เริ่มกระโดด
            isJumping = true; // ตั้งค่าตัวละครว่าอยู่ในระหว่างการกระโดด
        }

        // อัพเดตตำแหน่ง Y ด้วยแรงโน้มถ่วง
        playerPosition.y += velocityY;
        velocityY += gravity; // เพิ่มแรงโน้มถ่วงลงมา

        // ตรวจสอบว่าตัวละครอยู่บนพื้นหรือไม่
        if (playerPosition.y >= 600-playerHeight) { // พื้นที่พื้นคือ y = 400
            playerPosition.y = 600-playerHeight; // ตั้งให้ตำแหน่ง Y กลับไปที่พื้น
            velocityY = 0.0; // รีเซ็ตความเร็วในแนว Y
            isJumping = false; // รีเซ็ตสถานะการกระโดด
        }

        //ตรวจสอบว่าตัวละครชนกำเเพงหรือไม่
        if (playerPosition.x >= 800-playerWidth){
            playerPosition.x = 800-playerWidth;
        }
        if(playerPosition.x <= 0){
            playerPosition.x = 0;
        } 

        // ตรวจสอบการชนกับสิ่งกีดขวาง
        if (CheckCollisionWithObstacle(playerPosition, obstacle)) {
            // หากชนกับสิ่งกีดขวาง, ตัวละครจะไม่สามารถขยับผ่านมันได้
            if (IsKeyDown(KEY_RIGHT)) {
                playerPosition.x -= playerSpeed; // หยุดการเคลื่อนไหวไปขวา
            }
            if (IsKeyDown(KEY_LEFT)) {
                playerPosition.x += playerSpeed; // หยุดการเคลื่อนไหวไปซ้าย
            }
            if (velocityY < 0) { // ถ้าตัวละครกำลังตกลง
                playerPosition.y = obstacle.y + playerHeight; // ให้ตัวละครยืนบนสิ่งกีดขวาง
                velocityY = 0.0f; // หยุดการเคลื่อนไหวในแนว Y
                isJumping = false; // ตั้งค่าการกระโดดให้เป็น false
            }else if(velocityY > 0){
                playerPosition.y =obstacle.y -playerHeight;
                velocityY =0;
            }
             
        }
    
        // เริ่มต้นการแสดงผล
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // วาดตัวละคร (เป็นสี่เหลี่ยม)
        DrawRectangleV(playerPosition, (Vector2){playerWidth, playerHeight}, BLUE);

         // วาดสิ่งกีดขวาง (เป็นสี่เหลี่ยม)
        DrawRectangleRec(obstacle, RED);

        // แสดงข้อความ
        DrawText("Demo", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    // ปิดการใช้งาน raylib
    CloseWindow();

    return 0;
}
