#ifndef COMPRESSORS_HPP
#define COMPRESSORS_HPP

#include <cstring>
#include <string>

#include <bzlib.h>
#include <lzma.h>
#include <zlib.h>

void compressGZIP(const char *input, size_t inputSize,
                  const std::string &outputFileName);

void compressBZIP2(const char *input, size_t inputSize,
                   const char *outputFileName);

void compressLZMA(const char *input, size_t inputSize,
                  const std::string &outputFileName);

#endif
