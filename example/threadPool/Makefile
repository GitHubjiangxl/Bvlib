
include ../Makefile.param

CUR_DIR=$(PWD)
BV_SRC = $(wildcard $(CUR_DIR)/*.c)
BV_OBJ_DIR = $(patsubst %.c,%.o,$(BV_SRC))
BV_OBJ_NAME = $(notdir $(BV_OBJ_DIR))
BV_OBJ = $(addprefix $(TOP_DIR)/bin/,$(BV_OBJ_NAME))
BV_FILE = $(filter %$(*F).c, $(BV_SRC))
TARGET=$(TOP_DIR)/bin/ThreadPool
all:
	echo "$(CUR_DIR)"
	$(CC) $(BV_FILE) -o $(TARGET) $(CFLAGS) $(LDFLAGS) $(INC_DIR) $(LIB_DIR)
clean:
	rm $(TARGET)