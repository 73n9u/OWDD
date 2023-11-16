#ifndef FILEIO_HPP
#define FILEIO_HPP


#include <iostream>
#include <fstream>
#include <iomanip>
#include <unordered_map>

using namespace std;

void appendCharArrayToFile(const char* filename, const char* data, streamsize size) {
    ofstream file(filename, ios::binary | ios::app); 

    if (!file.is_open()) {
        cerr << "Error opening the file: " << filename << endl;
        return;
    }

    file.write(data, size);

    if (!file.good()) {
        cerr << "Error writing to the file: " << filename << endl;
    }
    file.close();
}


unordered_map<string,long> fileToMap(const string& filename) {

    unordered_map<string,long> dataMap;
    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return dataMap; 
    }

    long ordinal = 0;
    char buffer[64];

    while (file.read(buffer, sizeof(buffer))) {

        string data(buffer, sizeof(buffer));
        dataMap[data] = ordinal;

        ordinal++;
    }

    file.close();
    return dataMap;
}

string getFileLine(string path, long blockNum){

    ifstream file(path);
    if (!file.is_open()) {
        cout << "Unable to open the file." << endl;
        return ""; 
   }
    int linelength = 64;
    streampos position = static_cast<streampos>(linelength) * (blockNum - 1);
    file.seekg(position);

    char line[linelength];
    file.read(line, linelength);
    line[linelength] = '\0';

    file.close();
    return line;
}

//void writeToFile(const string& directoryPath, const string& filename, istream& dataStream, const size_t inputSize) {
//    string fullPath = directoryPath + "/" + filename;
//    ofstream outputFile(fullPath, ios::binary);
//
//    if (!outputFile) {
//        cerr << "Error opening the file for writing." << endl;
//        return;
//    }
//
//    char buffer[inputSize];
//    while (dataStream.read(buffer, sizeof(buffer))) {
//        outputFile.write(buffer, dataStream.gcount());
//    }
//
//    if (!outputFile) {
//        cerr << "Error writing to the file." << endl;
//        return;
//    }
//
//    outputFile.close();
//    return;
//}

//bool containsValue(const map<string,long> &mapObj, const string &value){
//    bool result = false;
//    for (const auto &pair : mapObj)
//    {
//        if (pair.first == value)
//        {
//            result = true;
//            break;
//        }
//    }
//    return result;
//}

#endif
