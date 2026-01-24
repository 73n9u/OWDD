#ifndef COMPRESSORS_HPP
#define COMPRESSORS_HPP

#include <cstring>

#include <bzlib.h>
#include <lzma.h>
#include <string>
#include <zlib.h>

enum class CompressionType { GZIP, LZMA, BZIP2 };

CompressionType parseCompressionType(std::string compressionStr);

void compressGZIP(const char *input, size_t inputSize);

void compressLZMA(const char *input, size_t inputSize);

void compressBZIP2(const char *input, size_t inputSize);
#endif
