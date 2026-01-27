CC = clang
CFLAGS = -Wall -g
LDFLAGS = -lX11
TARGET = xorg_todo
SRC = main.c key_event.c button.c
OBJ = main.o
TARGET_LIB = lib
TARGET_DIR = build

all: $(%.c)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET_DIR)/$(TARGET) $(LDFLAGS)

run: all
	./$(TARGET_DIR)/$(TARGET)
