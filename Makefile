# เปลี่ยนพาทนี้ให้ชี้ไปที่โฟลเดอร์ raylib ในเครื่องของคุณ
RAYLIB_PATH = ./raylib

PROJECT_NAME = index
SOURCE = src/main.c
ASSETS_PATH = assets
OUT_DIR = docs

CC = emcc
CFLAGS = -Os -Wall -I$(RAYLIB_PATH)/src -DPLATFORM_WEB
LDFLAGS = $(RAYLIB_PATH)/src/libraylib.a -s USE_GLFW=3 --preload-file $(ASSETS_PATH)

all: $(OUT_DIR)/$(PROJECT_NAME).html

$(OUT_DIR)/$(PROJECT_NAME).html: $(SOURCE)
	$(CC) $(SOURCE) -o $(OUT_DIR)/$(PROJECT_NAME).html $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT_DIR)/$(PROJECT_NAME).*