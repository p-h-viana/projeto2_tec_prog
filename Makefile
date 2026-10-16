PREFIX = /usr/local
LIBNAME = quimica
SRC_DIR = src
APP_DIR = app
TEST_DIR = tests
BUILD = build
LIB_DIR = lib
BIN_DIR = bin
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD)/%.o)
DEP = $(OBJ:.o=.d)
HEADERS = $(wildcard include/*.h)

LIB_STATIC = $(LIB_DIR)/lib$(LIBNAME).a
LIB_DYNAMIC = $(LIB_DIR)/lib$(LIBNAME).so

APP_STATIC = $(BIN_DIR)/app_static
APP_DYNAMIC = $(BIN_DIR)/app_dynamic

TEST_SRC = $(wildcard $(TEST_DIR)/test_*.c)
TEST_BIN = $(TEST_SRC:$(TEST_DIR)/%.c=$(BIN_DIR)/%)