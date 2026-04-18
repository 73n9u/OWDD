#ifndef DECOMPRESSORS_HPP
#define DECOMPRESSORS_HPP

#include <bzlib.h>
#include <lzma.h>
#include <string>
#include <zlib.h>

enum class DecompressionType { GZIP, LZMA, BZIP2 };

DecompressionType parseDecompressionType(std::string compressionStr);

size_t decompressGZIP(const unsigned char *input, size_t inputSize,
                      unsigned char *output, size_t outputCapacity);
size_t decompressLZMA(const unsigned char *input, size_t inputSize,
                      unsigned char *output, size_t outputCapacity);
size_t decompressBZIP2(const unsigned char *input, size_t inputSize,
                       unsigned char *output, size_t outputCapacity);
#endif
