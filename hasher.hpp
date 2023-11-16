#ifndef HASHER_HPP
#define HASHER_HPP


#include <openssl/evp.h>
#include <string>
#include <iomanip>

using namespace std;

string calcHash(const char *inputStream, size_t size){

    stringstream outputStream; 
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
        outputStream << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
    }
    return outputStream.str();
}

#endif
