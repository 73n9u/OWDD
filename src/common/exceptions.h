#include <exception>
#include <string>

// Base OWDD exception class
class OWDDException : public std::exception {
private:
  std::string message;

public:
  explicit OWDDException(const std::string &msg) : message(msg) {}
  const char *what() const noexcept override { return message.c_str(); }
};

// Base File IO exception class
class FileIOException : public OWDDException {
public:
  explicit FileIOException(const std::string &msg)
      : OWDDException("Error with file I/O: " + msg) {}
};

class FileOpenException : public FileIOException {
public:
  explicit FileOpenException(const std::string &path, const std::string reason)
      : FileIOException("File at " + path + " failed to open: " + reason) {}
};

class FileReadException : public FileIOException {
public:
  explicit FileReadException(const std::string &msg)
      : FileIOException("Failed to read from file: " + msg) {}
};

class FileWriteException : public FileIOException {
public:
  explicit FileWriteException(const std::string &msg)
      : FileIOException("Failed to write to file: " + msg) {}
};

// Base compression exception class
class CompressionException : public OWDDException {
public:
  explicit CompressionException(const std::string &compType,
                                const int return_val, const std::string &msg)
      : OWDDException("An error occurred with compressing with : " + compType +
                      " with value: " + std::to_string(return_val) + ": " +
                      msg) {}
};

class GZIPException : public CompressionException {
public:
  explicit GZIPException(const int return_val, const std::string &msg)
      : CompressionException("GZIP", return_val, msg) {}
};
class LZMAException : public CompressionException {
public:
  explicit LZMAException(const int return_val, const std::string &msg)
      : CompressionException("LZMA", return_val, msg) {}
};
class BZIP2Exception : public CompressionException {

public:
  explicit BZIP2Exception(const int return_val, const std::string &msg)
      : CompressionException("BZIP2", return_val, msg) {}
};

// Base OpenSSL exception class
class OpenSSLException : public OWDDException {

public:
  explicit OpenSSLException(const std::string &msg, const int return_val)
      : OWDDException("An error occured with openssl function: " + msg +
                      " with return value: " + std::to_string(return_val)) {}
};
