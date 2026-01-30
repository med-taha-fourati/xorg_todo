CC = clang
CFLAGS = -Wall -g
LDFLAGS = -lX11
TARGET = xorg_todo
SRC = main.c app.c key_event.c textbox.c button.c widget_registrar.c widget_textbox.c widget_button.c box.c rounded_box.c
OBJ = main.o
TARGET_LIB = lib
TARGET_DIR = build

all: $(%.c)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET_DIR)/$(TARGET) $(LDFLAGS)

run: all
	./$(TARGET_DIR)/$(TARGET)

manual:
	links https://tronche.com/gui/x/xlib/
