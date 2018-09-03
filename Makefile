

CC=gcc
LDFLAGS=-lpthread
CFLAGS=-g -fPIC -Wno-deprecated
SHARED = -shared

TOP_DIR=.
BV_INC=-I $(TOP_DIR)/include
BV_SRC = $(wildcard $(TOP_DIR)/src/*.c)
BV_OBJ_DIR = $(patsubst %.c,%.o,$(BV_SRC))
BV_OBJ_NAME = $(notdir $(BV_OBJ_DIR))
BV_OBJ = $(addprefix $(TOP_DIR)/,$(BV_OBJ_NAME))
BV_FILE = $(filter %$(*F).c, $(BV_SRC))
TARGET=$(TOP_DIR)/lib/libBv.so

all:
	$(CC) -c $(BV_FILE) $(BV_INC) $(CFLAGS)
	$(CC) -o $(TARGET) $(BV_OBJ) $(CFLAGS) $(LDFLAGS) $(SHARED)
clean:
	rm $(BV_OBJ_NAME)
	rm $(TARGET)