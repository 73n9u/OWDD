#ifndef READERS_HPP
#define READERS_HPP

#include <fcntl.h>
#include <unistd.h>
#include <unordered_map>

#include "compress.hpp"
#include "hasher.hpp"
#include "fileio.hpp"

using namespace std;

void hashToFile(const char* usbPath, size_t blockSize, string outputDir) {
    ofstream toCompress(outputDir, ios::binary | ios::app);
    int driveFile = open(usbPath, O_RDONLY);
    char buffer[blockSize];
    long long blockNumber=0;
    while(true){
        ssize_t bytesRead = pread(driveFile, buffer, blockSize, blockSize * blockNumber);

        if (bytesRead == -1) {
	  cerr << "Error reading block." << endl;
	  close(driveFile);
	  break;
        }

        if (bytesRead == 0) {
	  close(driveFile);
	  break;
        }

        string hash = calcHash(buffer, bytesRead);
        cout << hash << endl; 
        toCompress.write(hash.c_str(), 64);
        blockNumber++;
    }
    close(driveFile);
    return;
}

void blockRead(const char* usbPath, size_t blockSize, string hashfileDir, string  outputDir) {
    int counter = 0;
    int driveFile = open(usbPath, O_RDONLY);
    char buffer[blockSize];
    long blockNumDrive=0;

    ofstream toCompress(outputDir, ios::binary | ios::app);
    unordered_map<string,long> fileHashes = fileToMap(hashfileDir);
    
    while(true){
        ssize_t bytesRead = pread(driveFile, buffer, blockSize, blockSize * blockNumDrive);

        if (bytesRead == -1) {
	  cerr << "Error reading block." << endl;
	  close(driveFile);
	  break;
        }

        if (bytesRead == 0) {
	  close(driveFile);
	  break;
        }

        string hashBuf = calcHash(buffer, bytesRead);

        if (hashBuf == "076a27c79e5ace2a3d47f9dd2e83e4ff6ea8872b3c2218f66c92b89b55f36560" /*512*/ ||
	  hashBuf == "5f70bf18a086007016e948b04aed3b82103a36bea41755b6cddfaf10ace3c6ef" /*1024*/ ||
	  hashBuf == "e5a00aa9991ac8a5ee3109844d84a55583bd20572ad3ffcd42792f3c36b183ad" /*2048*/ ||
	  hashBuf == "ad7facb2586fc6e966c004d7d1d16b024f5805ff7cb47c7a85dabd8b48892ca7" /*4096*/ ||
	  hashBuf == "9f1dcbc35c350d6027f98be0f5c8b43b42ca52b7604459c0c42be3aa88913d47" /*8192*/ ||
	  hashBuf == "4fe7b59af6de3b665b67788cc2f99892ab827efae3a467342b3bb4e3bc8e5bfe" /*16384*/|| fileHashes.find(hashBuf) != fileHashes.end()){
	  blockNumDrive++;  
        }
        else {
	  cout << "Drive Hash: " << hashBuf << endl;
	  cout << "blockNumDrive: " << blockNumDrive << endl;
	  cout << "counter: " << counter << endl;
	  counter++;
	  blockNumDrive++;
	  toCompress.write(buffer, bytesRead);
	  }
    }
    close(driveFile);
    return;
}
void blockReadAdd(const char* usbPath, size_t blockSize, string hashfileDir, string  outputDir) {
    int counter = 0;
    int driveFile = open(usbPath, O_RDONLY);
    char buffer[blockSize];
    long blockNumDrive=0;

    ofstream toCompress(outputDir, ios::binary | ios::app);
    unordered_map<string,long> fileHashes = fileToMap(hashfileDir);
    
    ofstream toAddHash(hashfileDir, ios::binary | ios::app);

    while(true){
        ssize_t bytesRead = pread(driveFile, buffer, blockSize, blockSize * blockNumDrive);

        if (bytesRead == -1) {
	  cerr << "Error reading block." << endl;
	  close(driveFile);
	  break;
        }

        if (bytesRead == 0) {
	  close(driveFile);
	  break;
        }

        string hashBuf = calcHash(buffer, bytesRead);

        if (hashBuf == "076a27c79e5ace2a3d47f9dd2e83e4ff6ea8872b3c2218f66c92b89b55f36560" /*512*/ ||
	  hashBuf == "5f70bf18a086007016e948b04aed3b82103a36bea41755b6cddfaf10ace3c6ef" /*1024*/ ||
	  hashBuf == "e5a00aa9991ac8a5ee3109844d84a55583bd20572ad3ffcd42792f3c36b183ad" /*2048*/ ||
	  hashBuf == "ad7facb2586fc6e966c004d7d1d16b024f5805ff7cb47c7a85dabd8b48892ca7" /*4096*/ ||
	  hashBuf == "9f1dcbc35c350d6027f98be0f5c8b43b42ca52b7604459c0c42be3aa88913d47" /*8192*/ ||
	  hashBuf == "4fe7b59af6de3b665b67788cc2f99892ab827efae3a467342b3bb4e3bc8e5bfe" /*16384*/|| fileHashes.find(hashBuf) != fileHashes.end()){
	  blockNumDrive++;  
        }
        else {
	  cout << "Drive Hash: " << hashBuf << endl;
	  cout << "blockNumDrive: " << blockNumDrive << endl;
	  cout << "counter: " << counter << endl;
	  counter++;
	  blockNumDrive++;
	  toCompress.write(buffer, bytesRead);
	  toAddHash.write(hashBuf.c_str(), 64);
	  }
    }
    close(driveFile);
    return;
}

#endif
