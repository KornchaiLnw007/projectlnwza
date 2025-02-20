#include <raylib.h>

class Tutorial {    
private:
    Texture2D tutorialImage;   // รูป Tutorial 1
    Texture2D tutorialImage2;  // รูป Tutorial 2
    Rectangle buttonRec;       // ตำแหน่งปุ่ม
    Rectangle backButtonRec;                                             // ตำแหน่งปุ่มย้อนกลับ
    bool isTutorial2Active;    // สถานะ Tutorial 2
    bool isButtonHovered;      // สถานะ Hover
    bool isBackButtonHovered;                                          // สถานะ Hover ปุ่มย้อนกลับ
public:
    Tutorial();                // Constructor
    ~Tutorial();               // Destructor
    void Draw(int screenWidth, int screenHeight);
    bool GoToMainMenu();
    bool IsButtonClicked(Vector2 mousePosition); 
    bool IsBackButtonClicked(Vector2 mousePosition);                  // ฟังก์ชันตรวจสอบการคลิกปุ่มย้อนกลับ
    void SwitchToTutorial2();
    void ResetTutorial();
    void TurnbackToMainMenu();
};

// Constructor โหลดรูป
Tutorial::Tutorial() {
    Image img1 = LoadImage("src/Graphic/1.png");
    tutorialImage = LoadTextureFromImage(img1);
    UnloadImage(img1);

    Image img2 = LoadImage("src/Graphic/Nargame1dukdik.png");
    tutorialImage2 = LoadTextureFromImage(img2);
    UnloadImage(img2);

    buttonRec = {1180, 900, 200, 50}; // ตำแหน่งปุ่ม
    backButtonRec = {500, 900, 200, 50};                                           // ตำแหน่งปุ่มย้อนกลับ
    isTutorial2Active = false;
    isButtonHovered = false;  // ค่าเริ่มต้นคือไม่ได้ชี้ปุ่ม
    isBackButtonHovered = false;                                                  // ค่าเริ่มต้นคือไม่ได้ชี้ปุมย้อนกลับ
}

// Destructor ลบรูป
Tutorial::~Tutorial() {
    UnloadTexture(tutorialImage);
    UnloadTexture(tutorialImage2);
}

// ฟังก์ชันตรวจสอบการคลิกปุ่ม
bool Tutorial::IsButtonClicked(Vector2 mousePosition) {
    isButtonHovered = CheckCollisionPointRec(mousePosition, buttonRec);
    return isButtonHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

bool Tutorial::IsBackButtonClicked(Vector2 mousePosition) {                          // ตรวจสอบการคลิกปุ่มย้อนกลับ ??
    isBackButtonHovered = CheckCollisionPointRec(mousePosition, backButtonRec);
    return isBackButtonHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
// ฟังก์ชันสำหรับเปลี่ยนไป tutorial 2
void Tutorial::SwitchToTutorial2() {
    isTutorial2Active = true;
}

// ฟังก์ชันรีเซ็ตไป tutorial 1
void Tutorial::ResetTutorial() {
    isTutorial2Active = false;
}

// ฟังก์ชันสำหรับเปลี่ยนไปหน้าหลัก
void Tutorial::TurnbackToMainMenu() {
    isTutorial2Active = false;
}

// วาดหน้าจอ Tutorial
void Tutorial::Draw(int screenWidth, int screenHeight) {
    BeginDrawing();
    ClearBackground(WHITE);

    // ตรวจสอบตำแหน่งเมาส์
    Vector2 mousePosition = GetMousePosition();
   

    if (!isTutorial2Active) {
        // ===== วาด Tutorial 1 =====
        Rectangle sourceRec = {0, 0, (float)tutorialImage.width, (float)tutorialImage.height};
        Rectangle destRec = {screenWidth / 4.5f - 20, screenHeight / 4.5f - 20, 1280, 720};
        Vector2 origin = {100, 75};
        DrawTexturePro(tutorialImage, sourceRec, destRec, origin, 0.0f, WHITE);

        // ===== วาดปุ่มพร้อมเอฟเฟกต์ Hover =====      
        Rectangle currentButton = buttonRec;
        if (CheckCollisionPointRec(mousePosition, buttonRec)) {
            currentButton.x -= 5;
            currentButton.y -= 5;
            currentButton.width += 10;
            currentButton.height += 10;
            DrawRectangleRec(currentButton, LIGHTGRAY);
        } else {
            DrawRectangleRec(currentButton, DARKGRAY);
        }

        DrawRectangleRec(buttonRec, DARKGRAY);
        DrawText("Go to Next Screen", screenWidth / 2 + 245, 915, 20, WHITE);

        Rectangle Currentback = backButtonRec;                                            
        if (CheckCollisionPointRec(mousePosition, backButtonRec)) {
            Currentback.x -= 5;
            Currentback.y -= 5;
            Currentback.width += 10;
            Currentback.height += 10;
            DrawRectangleRec(Currentback, LIGHTGRAY);
        } else {
            DrawRectangleRec(Currentback, DARKGRAY);
        }

        DrawRectangleRec(backButtonRec, DARKGRAY);
        DrawText("Return", screenWidth / 2 - 426, 915, 20, WHITE);

        // ตรวจสอบการคลิกปุ่ม
        if (IsButtonClicked(mousePosition)) {
            SwitchToTutorial2();
        }

         if(IsBackButtonClicked(mousePosition)) {                                          // ตรวจสอบการคลิกปุ่มย้อนกลับ
            TurnbackToMainMenu();
        }
    } else {
        // ===== วาด Tutorial 2 =====
        Rectangle sourceRec = {0, 0, (float)tutorialImage2.width, (float)tutorialImage2.height};
        Rectangle destRec = {screenWidth / 4.5f - 20, screenHeight / 4.5f - 20, 1280, 720};
        Vector2 origin = {100, 75};
        DrawTexturePro(tutorialImage2, sourceRec, destRec, origin, 0.0f, WHITE);
        DrawText("Welcome to Tutorial 2!", screenWidth / 2 - 200, screenHeight / 2, 40, DARKGRAY);
    }

    EndDrawing();
}

// ฟังก์ชันในการย้อนกลับไปหน้าหลัก
bool Tutorial::GoToMainMenu() {
    if (IsKeyPressed(KEY_M) ) {
        ResetTutorial();
        return true;
    }else {
        return false;
    }
}
