#pragma once
#include <algorithm>
#include <bzlib.h>
#include <cstring>
#include <lzma.h>
#include <stdexcept>
#include <string>
#include <zlib.h>

#include "compressors.h"
#include "exceptions.h"

CompressionType parseCompressionType(std::string compressionStr) {
  std::string upper = compressionStr;
  std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
  if (upper == "GZIP")
    return CompressionType::GZIP;
  if (upper == "LZMA")
    return CompressionType::LZMA;
  if (upper == "BZIP2")
    return CompressionType::BZIP2;
  throw std::invalid_argument(
      "Unknown compression type: " + compressionStr +
      ". Supported options are: GZIP, BZIP2, and LZMA.");
}

size_t compressionBound(CompressionType compType, size_t inputSize) {
  switch (compType) {
  case CompressionType::GZIP:
    return deflateBound(Z_NULL, inputSize);
  case CompressionType::LZMA:
    return lzma_stream_buffer_bound(inputSize);
  case CompressionType::BZIP2:
    // We add a 1% headroom of the inputSize to output buffer
    // plus an additional 600 bytes of size according to:
    // https://sourceware.org/pub/bzip2/docs/v101/manual_3.html
    return inputSize + (inputSize / 100) + 600;
  }
}

size_t compressBZIP2(const unsigned char *input, size_t inputSize,
                     unsigned char *output, size_t outputCapacity) {

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
    throw BZIP2Exception(bz_res, "Failed to initialise BZIP2 stream.");
  }

  // Set the streams data input pointer and size
  stream.next_in = const_cast<char *>(reinterpret_cast<const char *>(input));
  stream.avail_in = inputSize;

  // Set the streams data output pointer and size
  stream.next_out = reinterpret_cast<char *>(output);
  // TODO: this can worst case be larger than the input because it's shit.
  // Increase based on spec.
  stream.avail_out = outputCapacity;

  // Compress the entire input
  bz_res = BZ2_bzCompress(&stream, BZ_FINISH);

  if (bz_res != BZ_STREAM_END) {
    BZ2_bzCompressEnd(&stream);
    throw BZIP2Exception(bz_res, "BZIP2 compression failed to complete.");
  }
  // TODO: pass compressionRatio to caller
  double compressionRatio = static_cast<float>(stream.total_out_lo32) /
                            static_cast<float>(stream.total_in_lo32);
  BZ2_bzCompressEnd(&stream);
  return stream.total_out_lo32;
}

size_t compressGZIP(const unsigned char *input, size_t inputSize,
                    unsigned char *output, size_t outputCapacity) {

  // Init z_stream object
  z_stream stream = {};
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;

  // Set compression level as best compression, giving us zlibs best effort
  // compression
  int z_res = deflateInit(&stream, Z_BEST_COMPRESSION);

  if (z_res != Z_OK) {
    throw GZIPException(z_res, "Failed to initialise GZIP stream.");
  }

  // Define the next in as a cast of the input as z_streams require
  // next inputs to be Bytef types
  stream.next_in = const_cast<Bytef *>(input);
  stream.avail_in = static_cast<uInt>(inputSize);

  stream.next_out = output;
  stream.avail_out = static_cast<uInt>(outputCapacity);

  // Compress the input in the stream in one pass until it's finished
  z_res = deflate(&stream, Z_FINISH);

  // If we didn't reach the end of the stream without an error
  if (z_res != Z_STREAM_END) {
    deflateEnd(&stream);
    throw GZIPException(z_res, "GZIP compression failed to complete.");
  }

  // TODO: pass compressionRatio to caller
  double compressionRatio = static_cast<float>(stream.total_out) /
                            static_cast<float>(stream.total_in);

  deflateEnd(&stream);
  return stream.total_out;
}

size_t compressLZMA(const unsigned char *input, size_t inputSize,
                    unsigned char *output, size_t outputCapacity) {

  lzma_stream stream = LZMA_STREAM_INIT;

  lzma_ret ret =
      lzma_easy_encoder(&stream, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64);

  if (ret != LZMA_OK) {
    throw LZMAException(ret, "Failed to initialise LZMA stream.");
  }

  stream.next_in = input;
  stream.avail_in = inputSize;

  stream.next_out = output;
  stream.avail_out = outputCapacity;

  ret = lzma_code(&stream, LZMA_FINISH);

  if (ret != LZMA_STREAM_END) {
    lzma_end(&stream);
    throw LZMAException(ret, "LZMA compression failed to complete.");
  }
  // TODO: pass compressionRatio to caller
  double compressionRatio = static_cast<float>(stream.total_out) /
                            static_cast<float>(stream.total_in);
  lzma_end(&stream);
  return stream.total_out;
}
