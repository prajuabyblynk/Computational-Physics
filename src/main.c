#include "raylib.h"
#include <math.h> // สำหรับใช้ sin(), cos()

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// --- การตั้งค่าทางฟิสิกส์และหน้าจอ ---
const float G = 9.8f;      // แรงโน้มถ่วง (m/s^2)
const int SCALE = 10;      // อัตราส่วน: 1 เมตร = 10 พิกเซล

// 1. ตัวแปรโครงสร้างสำหรับลูกบอล
typedef struct {
    Vector2 position;      // ตำแหน่งปัจจุบัน (พิกเซล)
    Vector2 initialVel;    // ความเร็วต้น (m/s)
    float time;             // เวลาที่ผ่านไป (วินาที)
    bool isMoving;          // กำลังเคลื่อนที่อยู่ไหม
} Projectile;

// ตัวแปร Global
Projectile ball = { 0 };

// ประกาศฟังก์ชันไว้ด้านบน
void UpdateDrawFrame(void);
void ResetProjectile(void);

int main(void) {
    // กำหนดขนาดเริ่มต้น (สำหรับรันบน Windows ปกติ)
    InitWindow(1280, 720, "Raylib Physics Simulation - Projectile Motion");
    SetWindowState(FLAG_WINDOW_RESIZABLE); // ย่อขยายหน้าต่างได้

    // ตั้งค่าลูกบอลเริ่มต้น
    ResetProjectile();

#if defined(PLATFORM_WEB)
    // สำหรับรันบน Web
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    // สำหรับรันบน Windows (Dev-C++)
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();
    return 0;
}

// ฟังก์ชันสำหรับรีเซ็ตค่าลูกบอลไปที่จุดเริ่มต้น
void ResetProjectile(void) {
    ball.position = (Vector2){ 50, GetScreenHeight() - 50 }; // เริ่มจากมุมซ้ายล่าง
    float v0 = 50.0f;     // ความเร็วต้น (m/s)
    float angle = 45.0f;  // มุมยิง (องศา)
    
    // คำนวณความเร็วต้นแยกแกน X, Y (m/s)
    ball.initialVel.x = v0 * cos(angle * PI / 180.0f);
    ball.initialVel.y = v0 * sin(angle * PI / 180.0f);
    
    ball.time = 0.0f;
    ball.isMoving = false;
}

// ฟังก์ชันหลักที่ใช้คำนวณและวาดหน้าจอ
void UpdateDrawFrame(void) {
    
    // --- ส่วนที่ 1: ตรวจสอบและปรับขนาดหน้าจอ (เฉพาะบนเว็บ) ---
#if defined(PLATFORM_WEB)
    int browserWidth = EM_ASM_INT(return window.innerWidth);
    int browserHeight = EM_ASM_INT(return window.innerHeight);
    if (GetScreenWidth() != browserWidth || GetScreenHeight() != browserHeight) {
        SetWindowSize(browserWidth, browserHeight);
    }
#endif

    // --- ส่วนที่ 2: อัปเดตตรรกะทางฟิสิกส์ ---
    if (IsKeyPressed(KEY_SPACE) && !ball.isMoving) {
        ball.isMoving = true;
    }
    if (IsKeyPressed(KEY_R)) {
        ResetProjectile();
    }

    if (ball.isMoving) {
        float dt = GetFrameTime(); // เวลาที่ผ่านไปในเฟรมนี้ (วินาที)
        ball.time += dt;
        
        // คำนวณตำแหน่งใหม่ (เป็นเมตร) ตามสูตรฟิสิกส์
        float x = ball.initialVel.x * ball.time;
        float y = (ball.initialVel.y * ball.time) - (0.5f * G * ball.time * ball.time);
        
        // แปลงหน่วยเมตรเป็นพิกเซล และตั้งตำแหน่งบนหน้าจอ (แกน Y ของจอจะคว่ำลง)
        ball.position.x = 50 + (x * SCALE);
        ball.position.y = (GetScreenHeight() - 50) - (y * SCALE);
        
        // ถ้าลูกบอลตกถึงพื้น ให้หยุดเคลื่อนที่
        if (ball.position.y >= GetScreenHeight() - 50) {
            ball.position.y = GetScreenHeight() - 50;
            ball.isMoving = false;
        }
    }

    // --- ส่วนที่ 3: วาดกราฟิก ---
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // วาดพื้น
    DrawLine(0, GetScreenHeight() - 50, GetScreenWidth(), GetScreenHeight() - 50, BLACK);
    
    // วาดลูกบอล
    DrawCircleV(ball.position, 15, RED);
    
    // วาดข้อความอธิบาย
    DrawText("Projectile Motion Simulation", 10, 10, 20, DARKGRAY);
    DrawText("Press [SPACE] to launch, [R] to reset", 10, 35, 16, GRAY);
    DrawText(TextFormat("Time: %.2f s", ball.time), 10, 60, 16, BLACK);
    DrawText(TextFormat("Position: (%.1f, %.1f) m", (ball.position.x - 50)/SCALE, (GetScreenHeight() - 50 - ball.position.y)/SCALE), 10, 80, 16, BLACK);

    EndDrawing();
}