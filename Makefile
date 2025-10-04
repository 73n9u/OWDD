CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lssl -lcrypto -lz -lbz2 -llzma

# Directories
SRC_HASH = src/hash
SRC_SYNC = src/sync
SRC_COMMON = src/common
BUILD_DIR = build

# Hash generation utility
HASH_GEN_TARGET = hash_generation
HASH_GEN_SRCS = $(SRC_HASH)/hash_generation.cpp \
                $(SRC_HASH)/block_reader.cpp \
                $(SRC_HASH)/hasher.cpp

# Sync utility (placeholder for later)
SYNC_TARGET = block_sync
SYNC_SRCS = $(SRC_SYNC)/block_sync.cpp

.PHONY: all clean hash sync

all: hash

hash: $(HASH_GEN_TARGET)

$(HASH_GEN_TARGET): $(HASH_GEN_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sync: $(SYNC_TARGET)

$(SYNC_TARGET): $(SYNC_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(HASH_GEN_TARGET) $(SYNC_TARGET)

help:
	@echo "Available targets:"
	@echo "  make hash     - Build hash generation utility"
	@echo "  make sync     - Build block sync utility"
	@echo "  make all      - Build all utilities (default: hash)"
	@echo "  make clean    - Remove built executables"
	@echo "  make help     - Show this help message"
