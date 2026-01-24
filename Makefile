CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wunused-function -Wunused-label -Wunused-variable -Wextra -O2
LDFLAGS = -lssl -lcrypto -lz -lbz2 -llzma

# Directories
SRC_HASH = src/hash
SRC_SYNC = src/sync
SRC_COMMON = src/common
BUILD_DIR = build

# Hash generation utility
HASH_GEN_TARGET = $(BUILD_DIR)/hash_generation
HASH_GEN_SRCS = $(SRC_HASH)/hash_generation.cpp \
                $(SRC_HASH)/hasher.cpp

# Sync utility
SYNC_TARGET = $(BUILD_DIR)/block_sync
SYNC_SRCS = $(SRC_SYNC)/block_sync.cpp \
            $(SRC_SYNC)/detect_diff.cpp \
            $(SRC_SYNC)/hash_reader.cpp \
            $(SRC_HASH)/block_reader.cpp \
            $(SRC_HASH)/hasher.cpp

.PHONY: all clean hash sync

all: hash sync

hash: $(HASH_GEN_TARGET)

$(HASH_GEN_TARGET): $(HASH_GEN_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sync: $(SYNC_TARGET)

$(SYNC_TARGET): $(SYNC_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "Available targets:"
	@echo "  make hash     - Build hash generation utility"
	@echo "  make sync     - Build block sync utility"
	@echo "  make all      - Build all utilities (default: hash)"
	@echo "  make clean    - Remove built executables"
	@echo "  make help     - Show this help message"
