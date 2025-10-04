#ifndef BLOCK_READER_HPP
#define BLOCK_READER_HPP

#include <cstddef>
#include <functional>
#include <sys/types.h>

int openForRead(const char *path);
ssize_t readBlock(int fd, char *buffer, size_t blockSize,
                  long long blockNumber);
void iterateBlocks(
    const char *path, size_t blockSize,
    std::function<void(const char *, size_t, long long)> callback);
#endif
