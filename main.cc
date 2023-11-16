#include "compress.hpp"
#include "hasher.hpp"
#include "fileio.hpp"
#include "readers.hpp"

using namespace std;

int main(){
    string localDriveDir= {"/dev/sdc"};
//    int hashPartitionNum[] = {2,3,4,5};
//    int readPartitionNum[] = {3,4,5,6};
//    int oldUbuntuVer[] = {16,18,20,22};
//    int newUbuntuVer[] = {18,20,22,23};
//    int readPartitionNum16[] = {4,5,6};
//    int newUbuntuVer16[] = {20,22,23};
//    int readPartitionNum18[] = {5,6};
//    int newUbuntuVer18[] = {22,23};
    int blocks[] = {512, 1024, 2048, 4096, 8192, 16384};
    string hashfileDir = "/media/t3nbu/redStorage/";
    string outputDir = "/media/t3nbu/redStorage/addeddata/";
//-------------------------------BLOCK HASH--------------------------//


//    for (int i = 0; i < 4; ++i){
//        for (int j = 0; j < 6; ++j){
//	  hashToFile((localDriveDir + to_string(hashPartitionNum[i])).c_str(),
//		   blocks[j],
//		   hashfileDir + to_string(blocks[j]) + "/" + to_string(oldUbuntuVer[i]) + "/" + to_string(oldUbuntuVer[i]) + to_string(blocks[j]) + ".txt");
//
//        }
//    }

//-------------------------------BLOCK COMPARE-----------------------//

//16 to 18
//    for (int i = 0; i < 4; ++i){
//        for (int j = 0; j < 6; ++j){
//	  blockReadAdd((localDriveDir + to_string(readPartitionNum[i])).c_str(),
//		  blocks[j],
//		  hashfileDir + to_string(blocks[j]) + "/" + to_string(oldUbuntuVer[i]) + "/" + to_string(oldUbuntuVer[i]) + to_string(blocks[j]) + ".txt",
//		  outputDir + to_string(blocks[j]) + "/" + to_string(oldUbuntuVer[i]) + "/" + to_string(oldUbuntuVer[i]) + "to" + to_string(newUbuntuVer[i]) + ".txt");
//
//        }
//    }

//I need 16to18, 16w18to20,16w18w20to22, 16w18w20w22to23
//I need 18to20, 18w20to22, 18w20w22to23
//I need 20to22, w20w22to23

    //16 to 18
        for (int j = 0; j < 6; ++j){
	  blockReadAdd((localDriveDir + "3").c_str(),
		  blocks[j],
		  hashfileDir + to_string(blocks[j]) + "/16/16"+ to_string(blocks[j]) + ".txt",
		  outputDir + to_string(blocks[j]) + "16to18.txt");

        }

    //16w18 to 20
        for (int j = 0; j < 6; ++j){
	  blockReadAdd((localDriveDir + "4").c_str(),
		  blocks[j],
		  hashfileDir + to_string(blocks[j]) + "/16/16"+ to_string(blocks[j]) + ".txt",
		  outputDir + to_string(blocks[j]) + "16w18to20.txt");

        }

    //16w18w20 to 22
        for (int j = 0; j < 6; ++j){
	  blockReadAdd((localDriveDir + "5").c_str(),
		  blocks[j],
		  hashfileDir + to_string(blocks[j]) + "/16/16"+ to_string(blocks[j]) + ".txt",
		  outputDir + to_string(blocks[j]) + "16w18w20to22.txt");

        }

    //16w18w20w22 to 23
        for (int j = 0; j < 6; ++j){
	  blockReadAdd((localDriveDir + "6").c_str(),
		  blocks[j],
		  hashfileDir + to_string(blocks[j]) + "/16/16"+ to_string(blocks[j]) + ".txt",
		  outputDir + to_string(blocks[j]) + "16w18w20w22to23.txt");

        }

    //18 to 20
        for (int j = 0; j < 6; ++j){
	  blockReadAdd((localDriveDir + "4").c_str(),
		  blocks[j],
		  hashfileDir + to_string(blocks[j]) + "/18/18"+ to_string(blocks[j]) + ".txt",
		  outputDir + to_string(blocks[j]) + "18to20.txt");

        }

    //18w20 to 22
        for (int j = 0; j < 6; ++j){
	  blockReadAdd((localDriveDir + "5").c_str(),
		  blocks[j],
		  hashfileDir + to_string(blocks[j]) + "/18/18"+ to_string(blocks[j]) + ".txt",
		  outputDir + to_string(blocks[j]) + "18w20to22.txt");

        }

    //18w20w22 to 23
        for (int j = 0; j < 6; ++j){
	  blockReadAdd((localDriveDir + "6").c_str(),
		  blocks[j],
		  hashfileDir + to_string(blocks[j]) + "/18/18"+ to_string(blocks[j]) + ".txt",
		  outputDir + to_string(blocks[j]) + "18w20w22to23.txt");

        }


        //20 to 22
        for (int j = 0; j < 6; ++j){
	  blockReadAdd((localDriveDir + "5").c_str(),
		  blocks[j],
		  hashfileDir + to_string(blocks[j]) + "/20/20"+ to_string(blocks[j]) + ".txt",
		  outputDir + to_string(blocks[j]) + "20to22.txt");

        }


        //20w22 to 23
        for (int j = 0; j < 6; ++j){
	  blockReadAdd((localDriveDir + "6").c_str(),
		  blocks[j],
		  hashfileDir + to_string(blocks[j]) + "/20/20"+ to_string(blocks[j]) + ".txt",
		  outputDir + to_string(blocks[j]) + "20w22to23.txt");

        }


//I need 16 ---> 20,22,23

//    for (int i = 0; i < 3; ++i){
//        for (int j = 0; j < 6; ++j){
//	  blockRead((localDriveDir + to_string(readPartitionNum16[i])).c_str(),
//		  blocks[j],
//		  hashfileDir + to_string(blocks[j]) + "/16/16" + to_string(blocks[j]) + ".txt",
//		  outputDir + to_string(blocks[j]) + "/16/16to" + to_string(newUbuntuVer16[i]) + ".txt");
//
//        }
//    }
//
////I need 18 ---> 22,23
//
//    for (int i = 0; i < 2; ++i){
//        for (int j = 0; j < 6; ++j){
//	  blockRead((localDriveDir + to_string(readPartitionNum18[i])).c_str(),
//		  blocks[j],
//		  hashfileDir + to_string(blocks[j]) + "/18/18" + to_string(blocks[j]) + ".txt",
//		  outputDir + to_string(blocks[j]) + "/18/18to" + to_string(newUbuntuVer18[i]) + ".txt");
//
//        }
//    }
//    
//
////I need 20 ---> 23
//
//
//for (int j = 0; j < 6; ++j){
//    blockRead("/dev/sdc6",
//	    blocks[j],
//	    hashfileDir + to_string(blocks[j]) + "/20" + to_string(blocks[j]) + ".txt",
//	    outputDir + to_string(blocks[j]) + "/20/20to23.txt");
//
//        }

    
//------------------------------BLOCK COMPARE ADD--------------------//
//I need 16to18, 16w18to20,16w18w20to22, 16w18w20w22to23
//I need 18to20, 18w20to22, 18w20w22to23
//I need 20to22, w20w22to23
//
//    //18 to 20
//        for (int j = 0; j < 6; ++j){
//	  blockRead((localDriveDir + "4").c_str(),
//		  blocks[j],
//		  hashfileDir + to_string(blocks[j]) + "/18/18"+ to_string(blocks[j]) + ".txt",
//		  outputDir + to_string(blocks[j]) + "18to20.txt");
//
//        }
//
//    //18w20 to 22
//        for (int j = 0; j < 6; ++j){
//	  blockRead((localDriveDir + "5").c_str(),
//		  blocks[j],
//		  hashfileDir + to_string(blocks[j]) + "/18/18"+ to_string(blocks[j]) + ".txt",
//		  outputDir + to_string(blocks[j]) + "18w20to22.txt");
//
//        }
//
//    //18w20w22 to 23
//        for (int j = 0; j < 6; ++j){
//	  blockRead((localDriveDir + "6").c_str(),
//		  blocks[j],
//		  hashfileDir + to_string(blocks[j]) + "/18/18"+ to_string(blocks[j]) + ".txt",
//		  outputDir + to_string(blocks[j]) + "18w20w22to23.txt");
//
//        }
//
//
//        //20 to 22
//        for (int j = 0; j < 6; ++j){
//	  blockRead((localDriveDir + "5").c_str(),
//		  blocks[j],
//		  hashfileDir + to_string(blocks[j]) + "/20/20"+ to_string(blocks[j]) + ".txt",
//		  outputDir + to_string(blocks[j]) + "20to22.txt");
//
//        }
//
//
//        //20w22 to 23
//        for (int j = 0; j < 6; ++j){
//	  blockRead((localDriveDir + "6").c_str(),
//		  blocks[j],
//		  hashfileDir + to_string(blocks[j]) + "/20/20"+ to_string(blocks[j]) + ".txt",
//		  outputDir + to_string(blocks[j]) + "20w22to23.txt");
//
//        }
//-------------------------------------------------------------------//

    //arch versions:
    //archlinux-2023.08.01-x86_64.iso
    //archlinux-2023.09.01-x86_64.iso
    //archlinux-2023.10.04-x86_64.iso

    //ubuntu versions:
    //ubuntu-16.04.7-desktop-amd64.iso 
    //ubuntu-18.04.6-desktop-amd64.iso
    //ubuntu-20.04.6-desktop-amd64.iso
    //ubuntu-22.04.2-desktop-amd64.iso
    //ubuntu-23.10.0-desktop-amd64.iso
    

    return 0;
}
