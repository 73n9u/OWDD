#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <openssl/evp.h>
#include <fcntl.h>
#include <unistd.h>
#include <zlib.h>
#include <bzlib.h>
#include <lzma.h>
#include <cstring>

std::string calcHash(const char *inputStream, size_t size){

    std::stringstream outputStream; 
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr){
        return "";
    }

    const EVP_MD *md = EVP_sha256();

    EVP_DigestInit_ex(mdctx, md, nullptr);
    EVP_DigestUpdate(mdctx, inputStream, size);
    EVP_DigestFinal_ex(mdctx, hash, &hashLength);
    EVP_MD_CTX_free(mdctx);

    for(unsigned int i = 0; i < hashLength; i++){
        outputStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return outputStream.str();
}

std::string gettheline(std::string path, long blockNum){

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Unable to open the file." << std::endl;
        return ""; 
   }
    int linelength = 64;
    std::streampos position = static_cast<std::streampos>(linelength) * (blockNum - 1);
    file.seekg(position);

    char line[linelength];
    file.read(line, linelength);
    line[linelength] = '\0';

    file.close();
    return line;
}

//Function for compressing input data in GZIP format
void compressGZIP(const char* input, int inputSize, const std::string& outputFileName) {
    std::ofstream outputFile( "/home/t3nbu/zips/" + outputFileName, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    z_stream stream;
    memset(&stream, 0, sizeof(stream));

    if (deflateInit2(&stream, Z_BEST_COMPRESSION, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        std::cerr << "deflateInit2 error." << std::endl;
        return;
    }

    stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    stream.avail_in = inputSize;
    
    Bytef buffer[inputSize];

    stream.next_out = buffer;
    stream.avail_out = inputSize;

    while (true) {
        int result = deflate(&stream, Z_FINISH);
        if (result == Z_STREAM_END) {
            int bytesToWrite = inputSize- stream.avail_out;
            outputFile.write(reinterpret_cast<const char*>(buffer), bytesToWrite);
            break;
        } else if (result != Z_OK) {
            std::cerr << "deflate error." << std::endl;
            deflateEnd(&stream);
            return;
        }

        if (stream.avail_out == 0) {
            int bytesToWrite = inputSize;
            outputFile.write(reinterpret_cast<const char*>(buffer), bytesToWrite);

            stream.next_out = buffer;
            stream.avail_out = inputSize;
        }
    }

    deflateEnd(&stream);
    outputFile.close();
    return;
}

//Function for compressing input data in BZIP2 format

void compressBZIP2(const char* input, int inputSize, const char* outputFileName) {
    int result;  // Variable to store error codes

    // Open the output file for writing in binary mode
    const char* directory = "/home/t3nbu/zips/";
    char fullPath[256];  // Adjust the size as needed

    // Construct the full path
    snprintf(fullPath, sizeof(fullPath), "%s%s", directory, outputFileName);

    FILE* outputFile = fopen(fullPath, "w");

    if (!outputFile) {
        std::cerr << "Error opening the output file." << std::endl;
        return;
    }
    // Initialize the BZIP2 stream
    BZFILE* bzip2Stream = BZ2_bzWriteOpen(&result,outputFile, 9, 0, 0);

    if (bzip2Stream == NULL) {
        std::cerr << "Error initializing BZIP2 stream. Error code: " << result << std::endl;
        fclose(outputFile);
        return;
    }

    // Compress and write the data
    result = BZ_OK;
    BZ2_bzWrite(&result, bzip2Stream, const_cast<char*>(input), inputSize);

    if (result != BZ_OK) {
        std::cerr << "Error compressing data. Error code: " << result << std::endl;
        BZ2_bzWriteClose(&result, bzip2Stream, 0, NULL, NULL);
        fclose(outputFile);
        return;
    }

    BZ2_bzWriteClose(&result, bzip2Stream, 0, NULL, NULL);
    fclose(outputFile);
    return;
}

//Function for compressing input data in LZMA format

void compressLZMA(const char* input, int inputSize, const std::string& outputFileName) {
    return;
}

void blockRead(const char* usbPath, size_t blockSize) {

    int driveFile = open(usbPath, O_RDONLY);
    char buffer[blockSize];
    long long blockNum=0;
    int numDiffs = 0;
    std::map<long long, std::string> hMap;

    while(true){
        ssize_t bytesRead = pread(driveFile, buffer, blockSize, blockSize * blockNum);

        if (bytesRead == -1) {
	  std::cerr << "Error reading block." << std::endl;
	  close(driveFile);
	  break;
        }

        if (bytesRead == 0) {
	  close(driveFile);
	  break;
        }

        std::string hashBuf = calcHash(buffer, bytesRead);
        hMap[blockNum] = hashBuf;
        std::string oldHash = gettheline("/home/t3nbu/toshiba.txt", blockNum +1);

        if (oldHash != hMap[blockNum]){
	  std::cout << "\n old hash: " << oldHash << " \n new hash: " << hMap[blockNum] << std::endl;
	  std::cout <<"Block number to be sent: "<< numDiffs << std::endl;
	  numDiffs++;
	  std::string bNumStr = std::to_string(blockNum);
	  compressBZIP2(buffer, sizeof(buffer), bNumStr.c_str());
	  //compressGZIP(buffer, sizeof(buffer), std::to_string(blockNum) + ".gz");
	  //Retrieve and store the literal block|| should just call "buffer"
	  //Compress the block "compressGZIP(buffer)
	  //Figure out where to write the block to, will be written to file in WD
        }
        blockNum++;
    }
    close(driveFile);
    return;
}


int main(int argc, char* argv[]){

    if (argc < 2){
        std::cout << "Not enough arguments." << std::endl;
    }

    if (argc < 2){
        std::cout << "Too many arguments" << std::endl;
    }

    const char* path = argv[1];
    size_t blockSize= static_cast<size_t>(std::stoi(argv[2]));
    blockRead(path , blockSize);
    return 0;


}
