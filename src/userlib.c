#include "userlib.h"


typedef char byte;

struct dirEntry {
	char name[6];
	byte sectors[26];	
};

struct directory {
	struct dirEntry entries[16];
};



void printInt(int intToPrint){
	interrupt(0x21, 0xAB, intToPrint, 0, 0);
}

int readSector(char *buf, int absSector){
	return interrupt(0x21, 0xAC, buf, absSector, 0);
}

void dir(){
	struct directory diskDir;
	char fileName[7];
	int i = 0;
	int j;
	int numSector = 0;

	readSector(&diskDir, 2);

	fileName[6] = '\0';
	for(i=0;i<16;i++){
		numSector = 0;
		if(diskDir.entries[i].name[0]){
			for(j=0;j<6;j++){
				fileName[j] = diskDir.entries[i].name[j];
			}
			
			for(j=0;j<26;j++){
				if(diskDir.entries[i].sectors[j] != 0x00){
					numSector = numSector + 1;
				}
			}
			printString(fileName);
			printString("\t\t\0");
			printInt(numSector);
			printString("\n\r\0");
		}
	}
}

int writeFile(char *fname, char *buffer, int sectors){
	return interrupt(0x21, 0x08, fname, buffer, sectors);
}

int deleteFile(char *fname){
	return interrupt(0x21, 0x07, fname, 0, 0);
}

int printString(char *str){
	return interrupt(0x21, 0x00, str, 0, 0);
}

int upgradeReadString(char *buf, int size) {
	return interrupt(0x21, 0x01, buf, size, 0);
}

int readString(char *buf) {
	return interrupt(0x21, 0x01, buf, 0, 0);
}

int readChar(char *buf) {
	interrupt(0x21, 0x11, buf, 0, 0);
	return 1;
}

int readfile(char *filename, char *buf){
	return interrupt(0x21, 0x03, filename, buf, 0);
}

int executeProgram(char* name, int segment) {
	return interrupt(0x21, 0x04, name, segment, 0);
}

void terminate() {
	interrupt(0x21, 0x05, 0, 0, 0);
}
