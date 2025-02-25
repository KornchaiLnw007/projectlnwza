#include <raylib.h>

class Tutorial {    
private:
    Texture2D tutorialImage;
    Texture2D tutorialImage2;
    Rectangle buttonRec;
    Rectangle backButtonRec;
    bool isTutorial2Active;
    bool isButtonHovered;
    bool isBackButtonHovered;
    bool exitTutorial;  // ตัวแปรใหม่เพื่อเช็คว่าเราจะออกจาก tutorial ไหม

public:
    Tutorial();
    ~Tutorial();
    bool Draw(int screenWidth, int screenHeight);  // เปลี่ยนจาก void -> bool เพื่อตรวจสอบว่าออกจาก tutorial หรือไม่
    bool GoToMainMenu();
    bool IsButtonClicked(Vector2 mousePosition); 
    bool IsBackButtonClicked(Vector2 mousePosition);
    void SwitchToTutorial2();
    void ResetTutorial();
    void TurnbackToMainMenu();
};

// Constructor
Tutorial::Tutorial() {
    Image img1 = LoadImage("src/Graphic/1.png");
    tutorialImage = LoadTextureFromImage(img1);
    UnloadImage(img1);

    Image img2 = LoadImage("src/Graphic/Nargame1dukdik.png");
    tutorialImage2 = LoadTextureFromImage(img2);
    UnloadImage(img2);

    buttonRec = {1180, 900, 200, 50};
    backButtonRec = {500, 900, 200, 50};
    isTutorial2Active = false;
    isButtonHovered = false;
    isBackButtonHovered = false;
    exitTutorial = false;  // เริ่มต้นไม่ออกจาก tutorial
}

// Destructor
Tutorial::~Tutorial() {
    UnloadTexture(tutorialImage);
    UnloadTexture(tutorialImage2);
}

// ตรวจสอบการคลิกปุ่ม
bool Tutorial::IsButtonClicked(Vector2 mousePosition) {
    isButtonHovered = CheckCollisionPointRec(mousePosition, buttonRec);
    return isButtonHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

bool Tutorial::IsBackButtonClicked(Vector2 mousePosition) {
    isBackButtonHovered = CheckCollisionPointRec(mousePosition, backButtonRec);
    return isBackButtonHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

// เปลี่ยนเป็น tutorial 2
void Tutorial::SwitchToTutorial2() {
    isTutorial2Active = true;
}

// รีเซ็ตไป tutorial 1
void Tutorial::ResetTutorial() {
    isTutorial2Active = false;
}

// ออกจาก tutorial และกลับไป Main Menu
void Tutorial::TurnbackToMainMenu() {
    exitTutorial = true;  // ตั้งค่าว่าเราจะออกจาก tutorial
}

// วาดหน้าจอ Tutorial และตรวจสอบว่าต้องออกไหม
bool Tutorial::Draw(int screenWidth, int screenHeight) {
    BeginDrawing();
    ClearBackground(WHITE);

    Vector2 mousePosition = GetMousePosition();
    
    if (!isTutorial2Active) {
        // วาด Tutorial 1
        Rectangle sourceRec = {0, 0, (float)tutorialImage.width, (float)tutorialImage.height};
        Rectangle destRec = {screenWidth / 4.5f - 20, screenHeight / 4.5f - 20, 1280, 720};
        Vector2 origin = {100, 75};
        DrawTexturePro(tutorialImage, sourceRec, destRec, origin, 0.0f, WHITE);

        // ปุ่ม Next
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

        // ปุ่มย้อนกลับ
        Rectangle currentBack = backButtonRec;
        if (CheckCollisionPointRec(mousePosition, backButtonRec)) {
            currentBack.x -= 5;
            currentBack.y -= 5;
            currentBack.width += 10;
            currentBack.height += 10;
            DrawRectangleRec(currentBack, LIGHTGRAY);
        } else {
            DrawRectangleRec(currentBack, DARKGRAY);
        }
        DrawRectangleRec(backButtonRec, DARKGRAY);
        DrawText("Return", screenWidth / 2 - 426, 915, 20, WHITE);

        // ตรวจสอบการคลิกปุ่ม
        if (IsButtonClicked(mousePosition)) {
            SwitchToTutorial2();
        }

        if (IsBackButtonClicked(mousePosition)) {  
            TurnbackToMainMenu();
        }
    } else {
        // วาด Tutorial 2
        Rectangle sourceRec = {0, 0, (float)tutorialImage2.width, (float)tutorialImage2.height};
        Rectangle destRec = {screenWidth / 4.5f - 20, screenHeight / 4.5f - 20, 1280, 720};
        Vector2 origin = {100, 75};
        DrawTexturePro(tutorialImage2, sourceRec, destRec, origin, 0.0f, WHITE);
        DrawText("Welcome to Tutorial 2!", screenWidth / 2 - 200, screenHeight / 2, 40, DARKGRAY);
    }

    EndDrawing();

    return exitTutorial;  // คืนค่า true ถ้าต้องการออกจาก tutorial
}

// เช็คว่ากดปุ่ม M เพื่อกลับเมนูหลักไหม
bool Tutorial::GoToMainMenu() {
    if (IsKeyPressed(KEY_M)) {
        ResetTutorial();
        return true;
    }
    return false;
}
