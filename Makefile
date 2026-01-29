CC = clang
CFLAGS = -Wall -g
LDFLAGS = -lX11
TARGET = xorg_todo
SRC = main.c key_event.c textbox.c
OBJ = main.o
TARGET_LIB = lib
TARGET_DIR = build

all: $(%.c)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET_DIR)/$(TARGET) $(LDFLAGS)

run: all
	./$(TARGET_DIR)/$(TARGET)

manual:
	links https://tronche.com/gui/x/xlib/
