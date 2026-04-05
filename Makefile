CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wunused-function -Wunused-label -Wunused-variable -Wextra -O2
LDFLAGS = -lssl -lcrypto -lz -lbz2 -llzma

# Directories
SRC_HASH = src/hash
SRC_SYNC = src/sync
SRC_COMMON = src/common
BUILD_DIR = build

# Common src
COMMON_SRCS = 	$(SRC_COMMON)

# Hash generation utility
HASH_GEN_TARGET = 	$(BUILD_DIR)/hash_generation
HASH_GEN_SRCS = 	$(SRC_HASH)/hash_generation.cpp \
			$(COMMON_SRCS)/hash.cpp \
			$(SRC_COMMON)/hash_serialiser.cpp

# Sync utility
SYNC_TARGET = 	$(BUILD_DIR)/block_sync
SYNC_SRCS = 	$(SRC_SYNC)/hash_sync.cpp \
		$(COMMON_SRCS)/hash.cpp \
		$(COMMON_SRCS)/compressors.cpp \
		$(SRC_COMMON)/hash_deserialiser.cpp

.PHONY: all clean hash sync help

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(HASH_GEN_TARGET): $(HASH_GEN_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(SYNC_TARGET): $(SYNC_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


hash: $(HASH_GEN_TARGET)
sync: $(SYNC_TARGET)
all: hash sync

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "Available targets:"
	@echo "  make hash     - Build hash generation utility"
	@echo "  make sync     - Build block sync utility"
	@echo "  make all      - Build all utilities"
	@echo "  make clean    - Remove built executables"
	@echo "  make help     - Show this help message"
