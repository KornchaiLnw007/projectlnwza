#include <raylib.h>

class Tutorial2 {
    private:
        Texture2D tutorialImage; // เพิ่มตัวแปรเก็บรูปภาพ
    public:
        Tutorial2();  // Constructor
        ~Tutorial2(); // Destructor
        void Draw(int screenWidth, int screenHeight);
        bool GoToMainMenu();
    };
    
    // Constructor โหลดรูป
    
    
    // Destructor ลบรูป
    Tutorial2::~Tutorial2() {
        UnloadTexture(tutorialImage);
    }
    
    // วาดหน้าจอ Tutorial
    void Tutorial2::Draw(int screenWidth, int screenHeight) {
        BeginDrawing();
        ClearBackground(WHITE);
        
        Rectangle sourceRec = {0, 0, (float)tutorialImage.width, (float)tutorialImage.height};
        Rectangle destRec = {screenWidth / 4.5 - 20, screenHeight / 4.5 - 20, 1280, 720};
        Vector2 origin = {100, 75}; // จุดหมุนตรงกลางรูป
        float rotation = 0.0f;
    
        DrawTexturePro(tutorialImage, sourceRec, destRec, origin, rotation, WHITE);
    
        
        // คำอธิบาย
        DrawText("Welcome to the Tutorial!", screenWidth / 2 - 200, screenHeight / 2 - 40, 40, DARKGRAY);
        DrawText("Use W/A/S/D to move around", screenWidth / 2 - 150, screenHeight / 2, 20, DARKGRAY);
        DrawText("Reach the gold block to win", screenWidth / 2 - 150, screenHeight / 2 + 30, 20, DARKGRAY);
        DrawText("Press 'M' to return to main menu", screenWidth / 2 - 150, screenHeight / 2 + 60, 20, DARKGRAY);
        
        EndDrawing();
    }
    
    // กลับเมนูหลัก
    bool Tutorial::GoToMainMenu() {
        return IsKeyPressed(KEY_M);
    }