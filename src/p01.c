#include "raylib.h"
#include <math.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// --- Global Variables ---
const int screenWidth = 1200;
const int screenHeight = 600;

Texture2D problemImage;
float h = 50.0f;       
float v0 = 18.0f;      
const float g = 9.8f;  

void UpdateDrawFrame(void) {
    // 1. Input Handling
    if (IsKeyDown(KEY_UP)) h += 0.1f;
    if (IsKeyDown(KEY_DOWN)) h -= 0.1f;
    if (IsKeyDown(KEY_RIGHT)) v0 += 0.1f;
    if (IsKeyDown(KEY_LEFT)) v0 -= 0.1f;

    if (h < 0.0f) h = 0.0f;
    if (v0 < 0.0f) v0 = 0.0f;

    // 2. Math Calculations
    float v_final = sqrtf((v0 * v0) + (2.0f * g * h));
    float vy_final = sqrtf(2.0f * g * h); 
    float theta_rad = atan2f(-vy_final, v0);
    float theta_deg = theta_rad * (180.0f / PI);

    // 3. Render
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // --- LEFT PANEL: 70% ---
    float leftPanelWidth = screenWidth * 0.70f;
    
    if (problemImage.id != 0) { 
        // คำนวณ Scale ให้พอดีกับความกว้าง 70% หรือความสูงของจอ
        float scaleX = (leftPanelWidth - 40) / (float)problemImage.width; // ลบ 40 เพื่อเผื่อขอบขวา-ซ้าย
        float scaleY = (screenHeight - 40) / (float)problemImage.height; // ลบ 40 เพื่อเผื่อขอบบน-ล่าง
        float imageScale = fminf(scaleX, scaleY);
        
        // --- ปรับตรงนี้ให้ชิดบนซ้าย ---
        float imageXPos = 20.0f; // ระยะห่างจากขอบซ้าย 20 พิกเซล
        float imageYPos = 20.0f; // ระยะห่างจากขอบบน 20 พิกเซล
        
        DrawTextureEx(problemImage, (Vector2){imageXPos, imageYPos}, 0.0f, imageScale, WHITE);
    } else {
        DrawText("IMAGE NOT FOUND", (int)leftPanelWidth/2 - 100, screenHeight/2 - 20, 20, RED);
    }

    // เส้นแบ่งแผงควบคุม
    DrawLine((int)leftPanelWidth, 0, (int)leftPanelWidth, screenHeight, LIGHTGRAY);

    // --- RIGHT PANEL: 30% ---
    int rightStartX = (int)leftPanelWidth + 20;
    DrawText("INTERACTIVE SOLVER", rightStartX, 30, 24, DARKBLUE);
    DrawLine(rightStartX, 65, screenWidth - 20, 65, DARKBLUE);

    DrawText("1. Adjust Variables (INPUTS):", rightStartX, 85, 18, DARKGRAY);
    DrawText("[UP/DOWN] Height (h)", rightStartX + 10, 120, 16, GRAY);
    DrawText(TextFormat("h = %.1f m", h), rightStartX + 10, 140, 28, BLACK);

    DrawText("[LEFT/RIGHT] Velocity (v0)", rightStartX + 10, 200, 16, GRAY);
    DrawText(TextFormat("v0 = %.1f m/s", v0), rightStartX + 10, 220, 28, BLACK);

    DrawLine(rightStartX, 320, screenWidth - 20, 320, LIGHTGRAY);
    DrawText("2. Calculated Answers (OUTPUTS):", rightStartX, 340, 18, DARKGREEN);

    DrawText("Final Velocity (v):", rightStartX + 10, 380, 18, DARKGRAY);
    DrawText(TextFormat("%.2f m/s", v_final), rightStartX + 10, 405, 36, RED);

    DrawText("Impact Angle (theta):", rightStartX + 10, 470, 18, DARKGRAY);
    DrawText(TextFormat("%.2f deg", theta_deg), rightStartX + 10, 495, 36, RED);

    EndDrawing();
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Physics Solver: Top-Left Image");

    // โหลด Texture
    problemImage = LoadTexture("assets/p01_proj.png");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    UnloadTexture(problemImage); 
    CloseWindow();
    return 0;
}