#include "hash_serialiser.h"
#include "exceptions.h"
#include "hash.h"
#include <array>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

HashSerialiser::HashSerialiser(const std::string &outputPath)
    : m_fd{::open(outputPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644)},
      m_writeBuffer{} {
  if (m_fd == -1) {
    throw FileOpenException(outputPath, strerror(errno));
  }
}

HashSerialiser::~HashSerialiser() {
  try {
    flush();
  } catch (FileWriteException) {
  }
  ::close(m_fd);
}
void HashSerialiser::write(const Hash &hash) {
  uint64_t blockNum = hash.getBlockNum();
  // Extract each bit of blockNum using bit shifts to avoid
  // portability concerns in endianness.
  for (int i = 0; i < 8; i++) {
    m_writeBuffer.push_back((blockNum >> (56 - i * 8)) & 0xFF);
  }
  const auto &hashData = hash.getHash();
  m_writeBuffer.insert(m_writeBuffer.end(), hashData.begin(), hashData.end());
}

void HashSerialiser::flush() {
  if (m_writeBuffer.empty())
    return;

  ssize_t written = ::write(m_fd, m_writeBuffer.data(), m_writeBuffer.size());
  if (written == -1) {
    throw FileWriteException("Serialise file encountered an error.");
  }
  m_writeBuffer.clear();
}
