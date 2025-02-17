#include <raylib.h>

class Tutorial {
private:
    Texture2D tutorialImage;  // เพิ่มตัวแปรเก็บรูปภาพ
    Rectangle buttonRec;  // ตัวแปรเก็บตำแหน่งของปุ่ม
    bool isTutorial2Active; // ตัวแปรบ่งชี้ว่าเราควรแสดง tutorial 2 หรือไม่
public:
    Tutorial();  // Constructor
    ~Tutorial(); // Destructor
    void Draw(int screenWidth, int screenHeight);
    bool GoToMainMenu();
    bool IsButtonClicked(Vector2 mousePosition); // ฟังก์ชันตรวจสอบการคลิกปุ่ม
    void SwitchToTutorial2(); // ฟังก์ชันสำหรับเปลี่ยนไป tutorial 2
    void ResetTutorial(); // ฟังก์ชันรีเซ็ตไป tutorial 1
};

// Constructor โหลดรูป
Tutorial::Tutorial() {
    Image img = LoadImage("src/Graphic/1.png"); 
    tutorialImage = LoadTextureFromImage(img);
    UnloadImage(img);

    // กำหนดตำแหน่งและขนาดของปุ่ม
    buttonRec = { 800, 500, 200, 50 };  // ปุ่มอยู่ที่ตำแหน่ง 800, 500
    isTutorial2Active = false; // เริ่มต้นที่ tutorial 1
}

// Destructor ลบรูป
Tutorial::~Tutorial() {
    UnloadTexture(tutorialImage);
}

// ฟังก์ชันตรวจสอบการคลิกปุ่ม
bool Tutorial::IsButtonClicked(Vector2 mousePosition) {
    if (CheckCollisionPointRec(mousePosition, buttonRec)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return true;
        }
    }
    return false;
}

// ฟังก์ชันสำหรับเปลี่ยนไป tutorial 2
void Tutorial::SwitchToTutorial2() {
    isTutorial2Active = true;  // เปลี่ยนไปที่ tutorial 2
}

// ฟังก์ชันรีเซ็ตไป tutorial 1
void Tutorial::ResetTutorial() {
    isTutorial2Active = false;  // รีเซ็ตไป tutorial 1
}

// วาดหน้าจอ Tutorial
void Tutorial::Draw(int screenWidth, int screenHeight) {
    BeginDrawing();
    ClearBackground(WHITE);

    if (!isTutorial2Active) {
        // วาด tutorial 1
        Rectangle sourceRec = {0, 0, (float)tutorialImage.width, (float)tutorialImage.height};
        Rectangle destRec = {screenWidth / 4.5 - 20, screenHeight / 4.5 - 20, 1280, 720};
        Vector2 origin = {100, 75}; // จุดหมุนตรงกลางรูป
        float rotation = 0.0f;

        DrawTexturePro(tutorialImage, sourceRec, destRec, origin, rotation, WHITE);

        // วาดปุ่ม
        DrawRectangleRec(buttonRec, DARKGRAY);
        DrawText("Go to Next Screen", screenWidth / 2 - 90, screenHeight / 1.5f + 15, 20, WHITE);

        // ตรวจสอบการคลิกปุ่ม
        Vector2 mousePosition = GetMousePosition();
        if (IsButtonClicked(mousePosition)) {
            SwitchToTutorial2(); // ไปที่ tutorial 2 ทันที
        }
    } else {
        // วาด tutorial 2
        DrawText("This is tutorial 2!", screenWidth / 2 - 100, screenHeight / 2, 40, DARKGRAY);
    }

    EndDrawing();
}

// ฟังก์ชันในการย้อนกลับไปหน้าหลัก
bool Tutorial::GoToMainMenu() {
    // ตรวจสอบการกดปุ่ม 'M' เพื่อกลับไปหน้าหลัก
    if (IsKeyPressed(KEY_M)) {
        ResetTutorial();  // รีเซ็ตกลับไป tutorial 1
        return true; // กลับไปที่หน้าหลัก
    }

    return false;
}
