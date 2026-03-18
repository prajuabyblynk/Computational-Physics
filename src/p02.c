#include "raylib.h"
#include <stdio.h>
#include <math.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// กำหนดขนาดหน้าจอ
#define SCREEN_WIDTH 1260
#define SCREEN_HEIGHT 760


// --- Modern Color Palette ---
Color ColorBg        = { 243, 244, 246, 255 }; 
Color ColorPanel     = { 255, 255, 255, 255 }; 
Color ColorText      = { 31, 41, 55, 255 };    
Color ColorTextMuted = { 107, 114, 128, 255 }; 
Color ColorPrimary   = { 59, 130, 246, 255 };  
Color ColorPrimHover = { 37, 99, 235, 255 };   
Color ColorSecondary = { 209, 213, 219, 255 }; 
Color ColorSecHover  = { 156, 163, 175, 255 }; 
Color ColorOutputBg  = { 249, 250, 251, 255 }; 

// พารามิเตอร์ฟิสิกส์
float v0 = 18.0f; 
float h = 50.0f;  
float g = 9.8f;   

// ผลลัพธ์
float v_impact = 0.0f;      
float theta_impact = 0.0f;  
float time_trajectory = 0.0f; 

Texture2D problemImg;

// ฟังก์ชันคำนวณฟิสิกส์
void CalculatePhysics() {
    if (h <= 0 || g <= 0) {
        v_impact = v0; theta_impact = 0.0f; time_trajectory = 0.0f;
        return;
    }
    float vy_impact = sqrtf(2.0f * g * h);
    v_impact = sqrtf(v0 * v0 + 2.0f * g * h);
    theta_impact = atan2f(-vy_impact, v0) * (180.0f / PI);
    time_trajectory = vy_impact / g;
}

// UI Components
bool DrawModernButton(Rectangle bounds, const char* text, Color baseColor, Color hoverColor) {
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    bool isClicked = isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    DrawRectangleRounded(bounds, 0.2f, 10, isHovered ? hoverColor : baseColor);
    int textWidth = MeasureText(text, 20);
    DrawText(text, bounds.x + (bounds.width / 2) - (textWidth / 2), bounds.y + (bounds.height / 2) - 10, 20, WHITE);
    return isClicked;
}

void DrawModernSpinner(const char* label, float* value, float step, float minVal, Rectangle bounds) {
    DrawText(label, bounds.x, bounds.y - 25, 18, ColorTextMuted);
    DrawRectangleRounded(bounds, 0.2f, 10, WHITE);
    DrawRectangleRoundedLines(bounds, 0.2f, 10, ColorSecondary); 
    char valText[32]; snprintf(valText, sizeof(valText), "%.1f", *value);
    DrawText(valText, bounds.x + 15, bounds.y + 12, 20, ColorText);
    Rectangle btnMinus = { bounds.x + bounds.width - 80, bounds.y + 5, 35, bounds.height - 10 };
    if (DrawModernButton(btnMinus, "-", ColorSecondary, ColorSecHover)) { *value -= step; if (*value < minVal) *value = minVal; }
    Rectangle btnPlus = { bounds.x + bounds.width - 40, bounds.y + 5, 35, bounds.height - 10 };
    if (DrawModernButton(btnPlus, "+", ColorSecondary, ColorSecHover)) { *value += step; }
}

void DrawOutputBox(const char* label, float value, const char* unit, Rectangle bounds) {
    DrawText(label, bounds.x, bounds.y - 25, 18, ColorTextMuted);
    DrawRectangleRounded(bounds, 0.2f, 10, ColorOutputBg);
    DrawRectangleRoundedLines(bounds, 0.2f, 10, ColorSecondary);
    char valText[64]; snprintf(valText, sizeof(valText), "%.2f  %s", value, unit);
    DrawText(valText, bounds.x + 15, bounds.y + 12, 20, ColorPrimary);
}

// --- ฟังก์ชันหลักสำหรับการวาดแต่ละเฟรม ---
void UpdateDrawFrame(void) {
    BeginDrawing();
    ClearBackground(ColorBg); 

    // แผงซ้าย (รูปภาพ)
    Rectangle imgPanel = { 20, 20, 720, 720 };
    DrawRectangleRounded(imgPanel, 0.05f, 10, ColorPanel);
    DrawRectangleRoundedLines(imgPanel, 0.05f, 10, ColorSecondary);
    
    if (problemImg.id != 0) {
        float scale = (imgPanel.width - 40) / problemImg.width;
        DrawTextureEx(problemImg, (Vector2){ imgPanel.x + 20, imgPanel.y + 20 }, 0.0f, scale, WHITE);
    }

    // แผงขวา (UI)
    Rectangle uiPanel = { 760, 20, 480, 720 };
    DrawRectangleRounded(uiPanel, 0.05f, 10, ColorPanel);
    DrawRectangleRoundedLines(uiPanel, 0.05f, 10, ColorSecondary);

    float uiX = uiPanel.x + 40;
    float startY = uiPanel.y + 40;

    DrawText("SIMULATION PARAMETERS", uiX, startY, 24, ColorText);
    DrawLine(uiX, startY + 35, uiX + 400, startY + 35, ColorSecondary);

    DrawModernSpinner("Initial Velocity (v0) [m/s]", &v0, 0.5f, 0.0f, (Rectangle){ uiX, startY + 80, 400, 45 });
    DrawModernSpinner("Height (h) [m]", &h, 1.0f, 0.0f, (Rectangle){ uiX, startY + 160, 400, 45 });
    DrawModernSpinner("Gravity (g) [m/s^2]", &g, 0.1f, 0.1f, (Rectangle){ uiX, startY + 240, 400, 45 });

    if (DrawModernButton((Rectangle){ uiX, startY + 310, 195, 50 }, "CALCULATE", ColorPrimary, ColorPrimHover)) CalculatePhysics();
    if (DrawModernButton((Rectangle){ uiX + 205, startY + 310, 195, 50 }, "RESET", ColorTextMuted, ColorText)) { v0 = 18.0f; h = 50.0f; g = 9.8f; CalculatePhysics(); }

    DrawLine(uiX, startY + 390, uiX + 400, startY + 390, ColorSecondary);
    DrawText("CALCULATED RESULTS", uiX, startY + 410, 24, ColorText);

    DrawOutputBox("Impact Velocity (v)", v_impact, "m/s", (Rectangle){ uiX, startY + 470, 400, 45 });
    DrawOutputBox("Impact Angle (theta)", theta_impact, "degrees", (Rectangle){ uiX, startY + 540, 400, 45 });
    DrawOutputBox("Trajectory Time (t)", time_trajectory, "s", (Rectangle){ uiX, startY + 610, 400, 45 });

    EndDrawing();
}

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT); 
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Professional Physics Simulation");
    
    problemImg = LoadTexture("assets/p01_proj.png");
    CalculatePhysics();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    UnloadTexture(problemImg);
    CloseWindow();
    return 0;
}