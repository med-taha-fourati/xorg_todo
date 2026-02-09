CC = clang
CFLAGS = -Wall -g -I.
LDFLAGS = -lX11

BUILD_DIR = build
LIB_DIR = $(BUILD_DIR)/lib
TARGET = $(BUILD_DIR)/xorg_todo

WR_DIR = widget_registrar
WD_DIR = widgets
IO_DIR = interops
TS_DIR = types
LST_DIR = $(TS_DIR)/list

SRC_WR = $(WR_DIR)/widget_registrar.c $(WR_DIR)/widget_textbox.c $(WR_DIR)/widget_button.c $(WR_DIR)/widget_label.c
OBJ_WR = $(SRC_WR:%.c=$(BUILD_DIR)/%.o)
LIB_WR = $(LIB_DIR)/libwidget_registrar.a

SRC_WD = $(WD_DIR)/textbox.c $(WD_DIR)/button.c $(WD_DIR)/box.c $(WD_DIR)/rounded_box.c $(WD_DIR)/label.c
OBJ_WD = $(SRC_WD:%.c=$(BUILD_DIR)/%.o)
LIB_WD = $(LIB_DIR)/libwidgets.a

SRC_TYS = $(LST_DIR)/list.c
OBJ_TYS = $(SRC_TYS:%.c=$(BUILD_DIR)/%.o)
LIB_TYS = $(LIB_DIR)/libtypes.a

SRC_MAIN = main.c app.c key_event.c $(IO_DIR)/date_cmd.c
OBJ_MAIN = $(SRC_MAIN:%.c=$(BUILD_DIR)/%.o)

all: directories $(TARGET)

$(TARGET): $(OBJ_MAIN) $(LIB_WR) $(LIB_WD) $(LIB_TYS)
	$(CC) $(OBJ_MAIN) $(LIB_WR) $(LIB_WD) -o $@ $(LDFLAGS)

$(LIB_WR): $(OBJ_WR)
	ar rcs $@ $^

$(LIB_WD): $(OBJ_WD)
	ar rcs $@ $^

$(LIB_LST): $(OBJ_LST)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(WR_DIR)
	mkdir -p $(BUILD_DIR)/$(WD_DIR)
	mkdir -p $(BUILD_DIR)/$(IO_DIR)
	mkdir -p $(BUILD_DIR)/$(LST_DIR)
	mkdir -p $(LIB_DIR)

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

manual:
	links https://tronche.com/gui/x/xlib/

.PHONY: all run clean manual directories
