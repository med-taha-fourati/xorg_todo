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

SRC_WR := $(shell find $(WR_DIR) -name '*.c')
OBJ_WR := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_WR))
LIB_WR = $(LIB_DIR)/libwidget_registrar.a

SRC_WD := $(shell find $(WD_DIR) -name '*.c')
OBJ_WD := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_WD))
LIB_WD = $(LIB_DIR)/libwidgets.a

SRC_TYS := $(shell find $(TS_DIR) -name '*.c')
OBJ_TYS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_TYS))
LIB_TYS = $(LIB_DIR)/libtypes.a

SRC_MAIN = main.c app.c key_event.c $(IO_DIR)/date_cmd.c
OBJ_MAIN := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_MAIN))

all: directories $(TARGET)

$(TARGET): $(OBJ_MAIN) $(LIB_WR) $(LIB_WD) $(LIB_TYS)
	$(CC) $(OBJ_MAIN) $(LIB_WR) $(LIB_WD) $(LIB_TYS) -o $@ $(LDFLAGS)

$(LIB_WR): $(OBJ_WR)
	ar rcs $@ $^

$(LIB_WD): $(OBJ_WD)
	ar rcs $@ $^

$(LIB_TYS): $(OBJ_TYS)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(LIB_DIR)

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

manual:
	links https://tronche.com/gui/x/xlib/

.PHONY: all run clean manual directories
