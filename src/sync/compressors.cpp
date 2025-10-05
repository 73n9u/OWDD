#pragma once
#include <cstring>
#include <fstream>
#include <iostream>

#include <bzlib.h>
#include <lzma.h>
#include <zlib.h>

using namespace std;

void compressGZIP(const char *input, size_t inputSize,
                  const string &outputFileName) {
  ofstream outputFile("/home/t3nbu/testresults/4096/object/GZIP/" +
                          outputFileName,
                      ios::binary);
  if (!outputFile.is_open()) {
    cerr << "Error opening output file." << endl;
    return;
  }

  z_stream stream;
  memset(&stream, 0, sizeof(stream));

  if (deflateInit2(&stream, Z_BEST_COMPRESSION, Z_DEFLATED, 31, 8,
                   Z_DEFAULT_STRATEGY) != Z_OK) {
    cerr << "deflateInit2 error." << endl;
    return;
  }

  stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input));
  stream.avail_in = inputSize;

  Bytef buffer[inputSize];

  stream.next_out = buffer;
  stream.avail_out = inputSize;

  while (true) {
    int result = deflate(&stream, Z_FINISH);
    if (result == Z_STREAM_END) {
      int bytesToWrite = inputSize - stream.avail_out;
      outputFile.write(reinterpret_cast<const char *>(buffer), bytesToWrite);
      break;
    } else if (result != Z_OK) {
      cerr << "deflate error." << endl;
      deflateEnd(&stream);
      return;
    }

    if (stream.avail_out == 0) {
      int bytesToWrite = inputSize;
      outputFile.write(reinterpret_cast<const char *>(buffer), bytesToWrite);

      stream.next_out = buffer;
      stream.avail_out = inputSize;
    }
  }

  deflateEnd(&stream);
  outputFile.close();
}

void compressBZIP2(const char *input, size_t inputSize,
                   const char *outputFileName) {
  int result;

  const char *directory = "/home/t3nbu/testresults/4096/object/BZIP2/";
  char fullPath[256];
  snprintf(fullPath, sizeof(fullPath), "%s%s", directory, outputFileName);

  FILE *outputFile = fopen(fullPath, "w");

  if (!outputFile) {
    cerr << "Error opening the output file." << endl;
    return;
  }
  BZFILE *bzip2Stream = BZ2_bzWriteOpen(&result, outputFile, 9, 0, 0);

  if (bzip2Stream == NULL) {
    cerr << "Error initializing BZIP2 stream. Error code: " << result << endl;
    fclose(outputFile);
    return;
  }

  result = BZ_OK;
  BZ2_bzWrite(&result, bzip2Stream, const_cast<char *>(input), inputSize);

  if (result != BZ_OK) {
    cerr << "Error compressing data. Error code: " << result << endl;
    BZ2_bzWriteClose(&result, bzip2Stream, 0, NULL, NULL);
    fclose(outputFile);
    return;
  }

  BZ2_bzWriteClose(&result, bzip2Stream, 0, NULL, NULL);
  fclose(outputFile);
}

void compressLZMA(const char *input, size_t inputSize,
                  const string &outputFileName) {
  lzma_stream stream = LZMA_STREAM_INIT;

  ofstream outputFile("/home/t3nbu/testresults/4096/object/LZMA/" +
                          outputFileName,
                      ios::binary);

  if (!outputFile.is_open()) {
    cerr << "Error opening the output file." << endl;
    return;
  }

  lzma_ret ret =
      lzma_easy_encoder(&stream, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64);

  if (ret != LZMA_OK) {
    cerr << "Error initializing LZMA encoder. Error code: " << ret << endl;
    outputFile.close();
    return;
  }

  stream.next_in = reinterpret_cast<const uint8_t *>(input);
  stream.avail_in = inputSize;

  uint8_t outputBuffer[BUFSIZ];
  stream.next_out = outputBuffer;
  stream.avail_out = sizeof(outputBuffer);

  while (true) {
    ret = lzma_code(&stream, LZMA_FINISH);

    if (ret == LZMA_STREAM_END) {
      outputFile.write(reinterpret_cast<const char *>(outputBuffer),
                       sizeof(outputBuffer) - stream.avail_out);
      break;
    } else if (ret != LZMA_OK) {
      cerr << "Error compressing data. Error code: " << ret << endl;
      lzma_end(&stream);
      outputFile.close();
      return;
    }

    outputFile.write(reinterpret_cast<const char *>(outputBuffer),
                     sizeof(outputBuffer) - stream.avail_out);
    stream.next_out = outputBuffer;
    stream.avail_out = sizeof(outputBuffer);
  }

  lzma_end(&stream);
  outputFile.close();
}
