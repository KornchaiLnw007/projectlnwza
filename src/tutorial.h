#include <raylib.h>

class Tutorial {
private:
    Texture2D tutorialImage;   // รูป Tutorial 1
    Texture2D tutorialImage2;  // รูป Tutorial 2
    Rectangle backButtonRec;   // ตำแหน่งปุ่มย้อนกลับ
    bool isTutorial2Active;    // สถานะ Tutorial 2
    bool isBackButtonHovered;  // สถานะ Hover ปุ่มย้อนกลับ

public:
    Tutorial();  // Constructor
    ~Tutorial(); // Destructor
    bool Draw(int screenWidth, int screenHeight); // คืนค่า true ถ้าจะออกจาก tutorial
    bool GoToMainMenu();
    void SwitchToTutorial2();
    void ResetTutorial();
};

// Constructor โหลดรูป
Tutorial::Tutorial() {
    Image img1 = LoadImage("src/Graphic/1.png");
    tutorialImage = LoadTextureFromImage(img1);
    UnloadImage(img1);

    Image img2 = LoadImage("src/Graphic/Nargame1dukdik.png");
    tutorialImage2 = LoadTextureFromImage(img2);
    UnloadImage(img2);

    backButtonRec = {500, 900, 200, 50};  // ตำแหน่งปุ่มย้อนกลับ
    isTutorial2Active = false;
    isBackButtonHovered = false;
}

// Destructor ลบรูป
Tutorial::~Tutorial() {
    UnloadTexture(tutorialImage);
    UnloadTexture(tutorialImage2);
}

// เปลี่ยนเป็น tutorial 2
void Tutorial::SwitchToTutorial2() {
    isTutorial2Active = true;
}

// รีเซ็ตไป tutorial 1
void Tutorial::ResetTutorial() {
    isTutorial2Active = false;
}

// วาดหน้าจอ Tutorial
bool Tutorial::Draw(int screenWidth, int screenHeight) {
    BeginDrawing();
    ClearBackground(WHITE);
    
    Vector2 mousePosition = GetMousePosition();
    
    if (!isTutorial2Active) {
        // วาด Tutorial 1
        DrawTexture(tutorialImage, screenWidth / 4.5 - 20, screenHeight / 4.5 - 20, WHITE);
        DrawText("Press RIGHT ARROW to Next", screenWidth / 2 - 100, 850, 20, DARKGRAY);
    } else {
        // วาด Tutorial 2
        DrawTexture(tutorialImage2, screenWidth / 4.5 - 20, screenHeight / 4.5 - 20, WHITE);
        DrawText("Press LEFT ARROW to Back", screenWidth / 2 - 100, 850, 20, DARKGRAY);
    }
    
    DrawText("Press M to Return to Main Menu", screenWidth / 2 - 150, 900, 20, RED);
    
    EndDrawing();
    
    // ควบคุมด้วยปุ่มลูกศร
    if (IsKeyPressed(KEY_RIGHT)) {
        SwitchToTutorial2();
    } else if (IsKeyPressed(KEY_LEFT)) {
        ResetTutorial();
    } else if (IsKeyPressed(KEY_M)) {
        return true; // ออกจาก tutorial
    }
    
    return false;
}

// ฟังก์ชันเช็คว่ากดปุ่ม M เพื่อกลับเมนูหลักไหม
bool Tutorial::GoToMainMenu() {
    return IsKeyPressed(KEY_M);
}