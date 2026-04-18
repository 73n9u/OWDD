#ifndef COMPRESSORS_HPP
#define COMPRESSORS_HPP

#include <bzlib.h>
#include <lzma.h>
#include <string>
#include <zlib.h>

enum class CompressionType { GZIP, LZMA, BZIP2 };

CompressionType parseCompressionType(std::string compressionStr);

size_t compressionBound(CompressionType compType, size_t inputSize);

size_t compressGZIP(const unsigned char *input, size_t inputSize,
                    unsigned char *output, size_t outputCapacity);
size_t compressLZMA(const unsigned char *input, size_t inputSize,
                    unsigned char *output, size_t outputCapacity);
size_t compressBZIP2(const unsigned char *input, size_t inputSize,
                     unsigned char *output, size_t outputCapacity);
#endif
