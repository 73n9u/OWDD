#include "../common/compressors.h"
#include "../common/hash.h"
#include "../common/hash_deserialiser.h"
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <zlib.h>

// Open hash file
// Open dest drive
// Read hash at bNum in file and drive
// Compare
// If same, bNum++
// If diff, compress data

std::vector<Hash> deserialiseHashes(const std::string hashFile,
                                    const std::string sourceDrive,
                                    const std::string outputPath,
                                    const ssize_t blockSize,
                                    CompressionType compType) {
  std::cout << "Are you getting here?\n";
  uint64_t currentBlock{0};
  std::vector<Hash> hashes{};
  std::vector<unsigned char> buffer(blockSize);
  // unsigned char diskBuffer[blockSize];
  //  Initialise deserialiser for input hash file
  HashDeserialiser hashDeser{hashFile};
  std::cout << "How about after deser init?\n";

  // Open destDrive
  int sd = ::open(sourceDrive.c_str(), O_RDONLY);
  if (sd == -1) {
    throw std::runtime_error(std::string("Failed to open source drive: ") +
                             sourceDrive);
  }
  std::cout << "After source drive opening?\n";

  std::ofstream of(outputPath);
  // Init the context to feed to the hash calculator
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

  if (mdctx == nullptr) {
    throw std::runtime_error(
        "Failed to create EVP_MD_CTX for hash calculation");
  }

  ssize_t bytesRead;

  Hash hashFileHash{{}, 0};
  while (hashDeser.read(hashFileHash) &&
         ((bytesRead = ::read(sd, buffer.data(), blockSize)) > 0)) {
    Hash driveHash =
        Hash::calcHash(mdctx, buffer.data(), bytesRead, currentBlock);
    if ((hashFileHash.getHash() != driveHash.getHash()) ||
        (hashFileHash.getBlockNum() != driveHash.getBlockNum())) {
      // Something about the two hashes is different. Compress block based on
      // compType
      std::cout << "The hashes at blockNum: " << currentBlock
                << " differ. Compressing with: " << currentBlock << '\n';
      unsigned char compressedData[blockSize];
      switch (compType) {
      case CompressionType::GZIP:
        compressGZIP(buffer.data(), blockSize, compressedData);
        break;
      case CompressionType::LZMA:
        compressLZMA(buffer.data(), blockSize, compressedData);
        break;
      case CompressionType::BZIP2:
        compressBZIP2(buffer.data(), blockSize, compressedData);
        break;
      }
      std::cout << "Compressed data into output buffer.\n";
      of << compressedData;
    } else {
      std::cout << "Blocks hash values match\n";
    }
    // Track current blockNum and create new Hash object from
    hashFileHash.printBlockNum();
    hashFileHash.printHash();
    // hashes.push_back(hashFileHash);
    currentBlock++;
  }
  return hashes;
}
int main(int argc, char *argv[]) {
  if (argc < 6) {
    std::cerr << ("Usage: program <compression_type>\n");
    return 1;
  }
  try {
    const std::string hashFilePath = argv[1];
    const std::string sourceDrive = argv[2];
    const std::string outputPath = argv[3];
    const size_t blockSize = std::stoull(argv[4]);
    CompressionType compression = parseCompressionType(argv[5]);

    deserialiseHashes(hashFilePath, sourceDrive, outputPath, blockSize,
                      compression);
  } catch (const std::invalid_argument &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
