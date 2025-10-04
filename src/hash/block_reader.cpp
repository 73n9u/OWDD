#include "block_reader.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>

/**
 * Open a file or device for reading
 *
 * @param path Path to the file or device
 * @return File descriptor on success
 * @throws std::runtime_error if open fails
 */
int openForRead(const char *path) {
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    throw std::runtime_error(std::string("Failed to open file: ") +
                             std::string(path) + " - " +
                             std::string(strerror(errno)));
  }
  return fd;
}

/**
 * Read a single block from a file descriptor
 *
 * @param fd File descriptor (from openForRead)
 * @param buffer Buffer to store the read data
 * @param blockSize Size of each block in bytes
 * @param blockNumber Block number to read (0-indexed)
 * @return Number of bytes actually read, 0 on EOF, -1 on error
 */
ssize_t readBlock(int fd, char *buffer, size_t blockSize,
                  long long blockNumber) {
  off_t offset = blockSize * blockNumber;
  ssize_t bytesRead = pread(fd, buffer, blockSize, offset);

  if (bytesRead == -1) {
    throw std::runtime_error(std::string("Failed to read block ") +
                             std::to_string(blockNumber) + ": " +
                             std::string(strerror(errno)));
  }

  return bytesRead;
}

/**
 * Iterate through all blocks in a file and execute callback for each
 *
 * @param path Path to the file or device
 * @param blockSize Size of each block in bytes
 * @param callback Function called for each block with (buffer, bytesRead, blockNumber)
 * @throws std::runtime_error on I/O errors
 */
void iterateBlocks(
    const char *path, size_t blockSize,
    std::function<void(const char *, size_t, long long)> callback) {

  int fd = openForRead(path);
  char *buffer = new char[blockSize];
  long long blockNumber = 0;

  try {
    while (true) {
      ssize_t bytesRead = readBlock(fd, buffer, blockSize, blockNumber);

      if (bytesRead == 0) {
        // End of file
        break;
      }

      // Call the callback with block data
      callback(buffer, static_cast<size_t>(bytesRead), blockNumber);
      blockNumber++;
    }
  } catch (...) {
    delete[] buffer;
    close(fd);
    throw;
  }

  delete[] buffer;
  close(fd);
}
