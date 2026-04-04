#include "../common/hash.h"
#include "../common/hash_serialiser.h"
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <openssl/evp.h>
#include <unistd.h>

int openFile(std::string path, int flag) {
  int fd = ::open(path.c_str(), flag);
  if (fd == -1) {
    throw std::runtime_error("Failed to open source file");
  }
  return fd;
}

/**
 * Generate hash file from a source file/device
 *
 * @param sourcePath Path to file or device to hash
 * @param blockSize Size of each block in bytes
 * @param outputPath Path to output hash file
 */
void serialiseHashes(const std::string sourcePath, size_t blockSize,
                     const std::string outputPath) {

  // TODO: Handle potential leaks when hashSer throws but sourceDrive is
  // initialised
  HashSerialiser hashSer{outputPath};
  // Init the context to feed to the hash calculator
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

  if (mdctx == nullptr) {
    throw std::runtime_error(
        "Failed to create EVP_MD_CTX for hash calculation");
  }

  int sourceDrive = openFile(sourcePath, O_RDONLY);

  std::vector<unsigned char> buffer(blockSize);

  uint64_t currentBlock = 0;
  ssize_t bytesRead;
  while ((bytesRead = ::read(sourceDrive, buffer.data(), blockSize)) > 0) {

    Hash hash = Hash::calcHash(mdctx, buffer.data(), bytesRead, currentBlock);
    hash.printBlockNum();
    hash.printHash();
    hashSer.write(hash);

    currentBlock++;
  }
  if (bytesRead == -1) {
    EVP_MD_CTX_free(mdctx);
    close(sourceDrive);
    throw std::runtime_error("Read error on file: " + sourcePath);
  }

  EVP_MD_CTX_free(mdctx);
  close(sourceDrive);
  return;
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    throw std::runtime_error("4 Parameters required. Received only: " +
                             std::to_string(argc));
  }
  const std::string sourcePath = argv[1];
  const size_t blockSize = std::stoi(argv[2]);
  const std::string outputPath = argv[3];

  try {
    serialiseHashes(sourcePath, blockSize, outputPath);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
