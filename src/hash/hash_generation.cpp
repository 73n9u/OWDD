#include "../common/exceptions.h"
#include "../common/hash.h"
#include "../common/hash_serialiser.h"
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <openssl/evp.h>
#include <stdexcept>
#include <unistd.h>

/**
 * Generate hash file from a source file/device
 *
 * @param sourcePath Path to file or device to hash
 * @param blockSize Size of each block in bytes
 * @param outputPath Path to output hash file
 */
void serialiseHashes(const std::string &sourcePath, size_t blockSize,
                     const std::string &outputPath) {

  // TODO: Handle potential leaks when hashSer throws but sourceDrive is
  // initialised
  HashSerialiser hashSer{outputPath};
  // Init the context to feed to the hash calculator
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

  if (mdctx == nullptr) {
    throw OpenSSLException("Failed to create EVP_MD_CTX for hash calculation.");
  }

  int sourceDrive = ::open(sourcePath.c_str(), O_RDONLY);
  if (sourceDrive == -1) {
    EVP_MD_CTX_free(mdctx);
    throw FileOpenException(sourcePath, strerror(errno));
  }

  std::vector<unsigned char> buffer(blockSize);

  uint64_t currentBlock = 0;
  ssize_t bytesRead;
  while ((bytesRead = ::read(sourceDrive, buffer.data(), blockSize)) > 0) {

    Hash hash = Hash::calcHash(mdctx, buffer.data(), bytesRead, currentBlock);
    hash.printBlockNum();
    hash.printHash();
    try {
      hashSer.write(hash);
    } catch (const FileWriteException &e) {
      EVP_MD_CTX_free(mdctx);
      close(sourceDrive);
      throw;
    }

    currentBlock++;
  }
  if (bytesRead == -1) {
    EVP_MD_CTX_free(mdctx);
    close(sourceDrive);
    throw FileReadException(sourcePath + ": " + strerror(errno));
  }

  EVP_MD_CTX_free(mdctx);
  close(sourceDrive);
  return;
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "4 Parameters required. Received only: " << argc << "\n";
    return 1;
  }
  try {
    const std::string sourcePath = argv[1];
    const size_t blockSize = std::stoull(argv[2]);
    const std::string outputPath = argv[3];

    serialiseHashes(sourcePath, blockSize, outputPath);

  } catch (const std::invalid_argument &e) {
    std::cerr << "Unable to convert value: " << e.what() << "\n";
    return 1;

  } catch (const std::out_of_range &e) {
    std::cerr << "Value out of range: " << e.what() << "\n";
    return 1;

  } catch (const OWDDException &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
