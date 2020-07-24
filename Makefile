SRC_DIR = src
BIN_DIR = build

ALL = $(BIN_DIR)/non-concurrent $(BIN_DIR)/global-lock $(BIN_DIR)/smart-lock
COMMON = $(SRC_DIR)/common.h $(SRC_DIR)/generate-ops.c $(SRC_DIR)/generate-ops.h $(SRC_DIR)/main.c $(SRC_DIR)/seg-tree-interface.h $(SRC_DIR)/worker.c

all: $(ALL)

$(BIN_DIR)/non-concurrent: $(COMMON) $(SRC_DIR)/non-concurrent/seg-tree.c $(SRC_DIR)/non-concurrent/seg-tree.h
	gcc -o $(BIN_DIR)/non-concurrent $(SRC_DIR)/main.c $(SRC_DIR)/generate-ops.c $(SRC_DIR)/non-concurrent/seg-tree.c -Wall -pthread

$(BIN_DIR)/global-lock: $(COMMON) $(SRC_DIR)/global-lock/seg-tree.c $(SRC_DIR)/global-lock/seg-tree.h
	gcc -o $(BIN_DIR)/global-lock $(SRC_DIR)/main.c $(SRC_DIR)/generate-ops.c $(SRC_DIR)/global-lock/seg-tree.c -Wall -pthread

$(BIN_DIR)/smart-lock: $(COMMON) $(SRC_DIR)/smart-lock/seg-tree.c $(SRC_DIR)/smart-lock/seg-tree.h
	gcc -o $(BIN_DIR)/smart-lock $(SRC_DIR)/main.c $(SRC_DIR)/generate-ops.c $(SRC_DIR)/smart-lock/seg-tree.c -Wall -pthread

clean:
	rm -f $(ALL)

