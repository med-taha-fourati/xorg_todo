CC = clang
CFLAGS = -Wall -g
LDFLAGS = -lX11
TARGET = xorg_todo
WIDGET_REGISTRAR_DIR = widget_registrar
WIDGETS_DIR = widgets
SRC_WR = $(WIDGET_REGISTRAR_DIR)/widget_registrar.c $(WIDGET_REGISTRAR_DIR)/widget_textbox.c $(WIDGET_REGISTRAR_DIR)/widget_button.c $(WIDGET_REGISTRAR_DIR)/widget_label.c
SRC_WD = $(WIDGETS_DIR)/textbox.c $(WIDGETS_DIR)/button.c $(WIDGETS_DIR)/box.c $(WIDGETS_DIR)/rounded_box.c $(WIDGETS_DIR)/label.c
SRC = main.c app.c key_event.c $(SRC_WR) $(SRC_WD) interops/date_cmd.c
OBJ = main.o
TARGET_LIB = lib
TARGET_DIR = build

all: $(%.c)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET_DIR)/$(TARGET) $(LDFLAGS) $(PROC)

run: all
	./$(TARGET_DIR)/$(TARGET)

manual:
	links https://tronche.com/gui/x/xlib/
