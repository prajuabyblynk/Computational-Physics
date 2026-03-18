@echo off
echo =======================================
echo Building WebAssembly with Emscripten...
echo =======================================

:: ใช้พาทอ้างอิงจากในโฟลเดอร์โปรเจกต์โดยตรง
:: -I ชี้ไปที่โฟลเดอร์ include เพื่อหา raylib.h
:: และเจาะจงใช้ไฟล์ libraylib.web.a ในโฟลเดอร์ lib
emcc src\p01.c -o docs\index.html -Os -Wall -DPLATFORM_WEB -I"raylib\include" "raylib\lib\libraylib.web.a" -s USE_GLFW=3 --preload-file assets
echo.
echo Build Complete! Check the "docs" folder.
echo =======================================
pause