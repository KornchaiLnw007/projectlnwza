#include <raylib.h>

class Tutorial {
private:
    Texture2D tutorialImages[5];  // อาร์เรย์เก็บภาพ tutorial 5 หน้า
    Rectangle backButtonRec;       // ตำแหน่งปุ่มย้อนกลับ
    int currentTutorialPage;       // หน้าปัจจุบัน
    bool isBackButtonHovered;      // สถานะ Hover ปุ่มย้อนกลับ

public:
    Tutorial();  // Constructor
    ~Tutorial(); // Destructor
    bool Draw(int screenWidth, int screenHeight); // คืนค่า true ถ้าจะออกจาก tutorial
    bool GoToMainMenu();
    void NextPage();
    void PreviousPage();
    void ResetTutorial();
};

// Constructor โหลดรูปภาพ
Tutorial::Tutorial() {
    // โหลดภาพ tutorial ทั้งหมด
    for (int i = 0; i < 5; i++) {
        Image img = LoadImage(TextFormat("src/Graphic/tutorial%d.png", i + 1)); // โหลดภาพตามหมายเลข
        tutorialImages[i] = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    backButtonRec = {500, 900, 200, 50};  // ตำแหน่งปุ่มย้อนกลับ
    currentTutorialPage = 0;  // เริ่มที่หน้า tutorial 1
    isBackButtonHovered = false;
}

// Destructor ลบรูป
Tutorial::~Tutorial() {
    // ลบภาพทั้งหมด
    for (int i = 0; i < 5; i++) {
        UnloadTexture(tutorialImages[i]);
    }
}

// ไปที่หน้าถัดไป
void Tutorial::NextPage() {
    if (currentTutorialPage < 4) {  // ถ้ายังไม่ถึงหน้าสุดท้าย
        currentTutorialPage++;
    }
}

// ไปที่หน้าก่อนหน้า
void Tutorial::PreviousPage() {
    if (currentTutorialPage > 0) {  // ถ้ายังไม่ถึงหน้าต้น
        currentTutorialPage--;
    }
}

// รีเซ็ตกลับไปหน้าแรก
void Tutorial::ResetTutorial() {
    currentTutorialPage = 0;  // กลับไปที่หน้าแรก
}

// วาดหน้าจอ Tutorial
bool Tutorial::Draw(int screenWidth, int screenHeight) {
    BeginDrawing();
    ClearBackground(WHITE);
    
    Vector2 mousePosition = GetMousePosition();
    
    // วาดหน้าปัจจุบัน
    DrawTexture(tutorialImages[currentTutorialPage], screenWidth / 4.5 + 60, screenHeight / 4.5 - 20, WHITE);
    
    // ข้อความที่บอกให้กดลูกศรเพื่อไปหน้าถัดไป
    if (currentTutorialPage < 4) {
        DrawText("Press RIGHT ARROW to Next [->]", screenWidth / 2 - 100, 800, 20, DARKGRAY);
    }
    
    // ข้อความถ้าอยู่หน้าแรกจะให้กดปุ่มลูกศรซ้าย
    if (currentTutorialPage > 0) {
        DrawText("Press  LEFT ARROW to Back [<-]", screenWidth / 2 - 100, 850, 20, DARKGRAY);
    }

    // ข้อความเพื่อกลับสู่เมนูหลัก
    DrawText("Press M to Return to Main Menu", screenWidth / 2 - 100, 900, 20, RED);
    
    EndDrawing();
    
    // ควบคุมการเปลี่ยนหน้า
    if (IsKeyPressed(KEY_RIGHT)) {
        NextPage();
    } else if (IsKeyPressed(KEY_LEFT)) {
        PreviousPage();
    } else if (IsKeyPressed(KEY_M)) {
        return true; // ออกจาก tutorial
    }
    
    return false;
}

// ฟังก์ชันเช็คว่ากดปุ่ม M เพื่อกลับเมนูหลักไหม
bool Tutorial::GoToMainMenu() {
    return IsKeyPressed(KEY_M);
}
