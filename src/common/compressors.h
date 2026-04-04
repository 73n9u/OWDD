#ifndef COMPRESSORS_HPP
#define COMPRESSORS_HPP

#include <bzlib.h>
#include <lzma.h>
#include <string>
#include <zlib.h>

enum class CompressionType { GZIP, LZMA, BZIP2 };

CompressionType parseCompressionType(std::string compressionStr);

void compressGZIP(const unsigned char *input, size_t inputSize,
                  unsigned char *output);

void compressLZMA(const unsigned char *input, size_t inputSize,
                  unsigned char *output);

void compressBZIP2(const unsigned char *input, size_t inputSize,
                   unsigned char *output);
#endif
