#pragma once
#include <bzlib.h>
#include <cstring>
#include <iostream>
#include <lzma.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <zlib.h>

#include "compressors.hpp"

CompressionType parseCompressionType(std::string compressionStr) {}

void compressGZIP(const char *input, size_t inputSize, char *output) {

  // Init z_stream object
  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;

  // Set compression level, compression type, windows bits, memLevel and
  // strategy
  int z_res = deflateInit2(&stream, Z_BEST_COMPRESSION, Z_DEFLATED, 15, 8,
                           Z_DEFAULT_STRATEGY);

  if (z_res != Z_OK) {
    throw std::runtime_error(std::string("Failed to initialize GZIP stream"));
  }

  // Define the next in as a cast of the input as z_streams require
  // next inputs to be Bytef types
  stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input));
  stream.avail_in = inputSize;

  stream.next_out = reinterpret_cast<Bytef *>(output);
  stream.avail_out = inputSize;

  // Compress the input in the stream in one pass until it's finished
  z_res = deflate(&stream, Z_FINISH);

  // If we didn't reach the end of the stream without an error
  if (z_res != Z_STREAM_END) {
    deflateEnd(&stream);
    throw std::runtime_error(std::string("Failed to compress GZIP stream"));
  }

  deflateEnd(&stream);
}

void compressLZMA(const char *input, size_t inputSize, char *output) {

  lzma_stream stream = LZMA_STREAM_INIT;

  lzma_ret ret =
      lzma_easy_encoder(&stream, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64);

  if (ret != LZMA_OK) {
    throw std::runtime_error(std::string("Error initialising LZMA stream."));
  }

  stream.next_in = reinterpret_cast<const uint8_t *>(input);
  stream.avail_in = inputSize;

  stream.next_out = reinterpret_cast<uint8_t *>(output);
  stream.avail_out = inputSize;

  ret = lzma_code(&stream, LZMA_FINISH);

  if (ret != LZMA_STREAM_END) {
    lzma_end(&stream);
    throw std::runtime_error(std::string("Error compressing LZMA stream."));
  }
  lzma_end(&stream);
}

void compressBZIP2(const char *input, size_t inputSize, char *output) {

  // Initialize the stream with BZIP2 default memory allocators
  bz_stream stream;
  stream.bzalloc = NULL;
  stream.bzfree = NULL;
  stream.opaque = NULL;

  // verbosity: Set to 0 for nill debugging output of compression
  // workFactor: Set to 0 for standard fallback for repetitive data in stream
  int bz_res = BZ2_bzCompressInit(&stream, 9, 1, 0);

  // Check for errors in bz_res initialisation
  if (bz_res != BZ_OK) {
    throw std::runtime_error(std::string("Error initializing BZIP2 stream."));
  }

  // Set the streams data input pointer and size
  stream.next_in = const_cast<char *>(input);
  stream.avail_in = inputSize;

  // Set the streams data output pointer and size
  stream.next_out = output;
  stream.avail_out = inputSize;

  // Compress the entire input
  bz_res = BZ2_bzCompress(&stream, BZ_FINISH);

  if (bz_res != BZ_STREAM_END) {
    BZ2_bzCompressEnd(&stream);
    throw std::runtime_error(std::string("BZIP compression failed."));
  }
  BZ2_bzCompressEnd(&stream);
}
