#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <unistd.h>
#include <unordered_map>

std::string calcHash(const char *inputStream, size_t size) {

  std::stringstream outputStream;
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int hashLength;
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

  if (mdctx == nullptr) {
    return "";
  }

  const EVP_MD *md = EVP_sha256();

  EVP_DigestInit_ex(mdctx, md, nullptr);
  EVP_DigestUpdate(mdctx, inputStream, size);
  EVP_DigestFinal_ex(mdctx, hash, &hashLength);

  EVP_MD_CTX_free(mdctx);

  for (unsigned int i = 0; i < hashLength; i++) {
    outputStream << std::hex << std::setw(2) << std::setfill('0')
                 << static_cast<int>(hash[i]);
  }
  return outputStream.str();
}

void blockRead(const char *usbPath, size_t blockSize) {
  int driveFile = open(usbPath, O_RDONLY);
  char buffer[blockSize];
  long long blockNumber = 0;
  while (true) {
    ssize_t bytesRead =
        pread(driveFile, buffer, blockSize, blockSize * blockNumber);

    if (bytesRead == -1) {
      std::cerr << "Error reading block." << std::endl;
      close(driveFile);
      break;
    }

    if (bytesRead == 0) {
      close(driveFile);
      break;
    }

    std::cout << calcHash(buffer, bytesRead);
    blockNumber++;
  }
  close(driveFile);
  return;
}
int main(int argc, char *argv[]) {

  if (argc < 3) {
    std::cout << "Not enough arguments." << std::endl;
  }

  if (argc > 3) {
    std::cout << "Too many arguments" << std::endl;
  }

  size_t secondArg = static_cast<size_t>(std::stoi(argv[2]));
  blockRead(argv[1], secondArg);

  return 0;
}
