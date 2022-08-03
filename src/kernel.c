// declare functions
int printString(char *str);
int readChar();
int readString(char *buf);
int upgradeReadString(char *buf, int size);
void putChar(char toPrint, int row, int column, char color);
void putStr(char *toPrint, int row, int column, char color);
int readSector(char *buf, int absSector);
int modFunc(int a, int b);
int handleInterrupt21(int ax, int bx, int cx, int dx);
void printInt(int intToPrint);
int readfile(char *filename, char *buf);
int executeProgram(char* name, int segment);
void terminate();
int writeSector(char *buffer, int sector);
int deleteFile(char *fname);
int writeFile(char *fname, char *buffer, int sectors);

typedef char byte;

struct dirEntry {
	char name[6];
	byte sectors[26];	
};

struct directory {
	struct dirEntry entries[16];
};

main(){
	char buf[1024];
	int i;

	makeInterrupt21();
	/*
	char * buffer = "testphamhoangvu";
	writeSector(buffer, 2879);
	*/
	/*
	deleteFile("messag\0");
	interrupt(0x21, 0x07, "uprog1\0", 0, 0);
	*/
	
	
	//readfile("shell\0", buf);
	//writeFile("taylors\0", buf, 2);	
	//interrupt(0x21, 0x08, "messag\0", buf, 2);

	interrupt(0x21, 0x04, "shell\0", 0x2000, 0);

	// while loop
	while(1){
	}

}

/*Write the number of sectors indicated by parameter sectors from buffer into filename indicated by fname
Input: file name to write into, buffer, number of sectors written
Output: if all sectors are written, return 1; if there are no sector to write, return -1; if there are not enough sectors to write, return -2
*/
int writeFile(char *fname, char *buffer, int sectors) {
	struct directory diskDir;
	char diskMap[512];
	int i;
	int j;
	int entry = -1;
	int answer;
	int emptySector = -1;

	//Delete file in case there is a file with the same name to override later
	deleteFile(fname);
	
	if (sectors > 26) {
		sectors = 26;
	}
	answer = sectors;
	
	readSector(&diskMap, 1);
	readSector(&diskDir, 2);

	//Go through 16 entries in the directory
	for (i=0; i<16; i++){
		//Go through first byte to get empty file name
		if (!diskDir.entries[i].name[0]) {
			entry = i;
			break;		
		}			
	}
	
	//If no entry available in disk directory
	if (entry == -1) {
		return -1;
	}

	//Put 0x00 into the name of the found entry
	for (i=0; i<6; i++) {
		diskDir.entries[entry].name[i] = 0x00;
	}

	//Put the file name that we want to write into the name of the above entry
	for (i=0; i<6; i++) {
		if (fname[i] == '\0'){
			break;
		}
		diskDir.entries[entry].name[i] = fname[i];
	}	
	//Set all of the sector to blank
	for(i=0;i<26;i++){
		diskDir.entries[entry].sectors[i] = 0x00;
	}

	//Find empty sector and write into that sector until we reach the number of sectors to write or 26
	for(i=0;i<sectors;i++){
		emptySector = -1;
		for(j=0;j<512;j++){
			//Find empty sector
			if(diskMap[j] == 0x00){
				emptySector = j;
				break;
			}
		}
		//If no sector found
		if (emptySector == -1){
			answer=-2;
			break;
		}
		//Mark empty sector as used
		diskMap[emptySector] = 0xFF;
		//Write the number of empty sector into the place sector used
		diskDir.entries[entry].sectors[i] = emptySector;
		//Write into sector
		writeSector(buffer, emptySector);
		buffer = buffer + 512;
	}	

	writeSector(diskMap, 1);
	writeSector(&diskDir, 2);

	return answer;
}

/*Delete a file
Input: file name
output: file removed from disk directory and return 1; if there is no file found, return -1
*/
int deleteFile(char *fname){
	struct directory diskDir;
	char diskMap[512];
	int i;
	int j;
	int entry = -1;
	
	readSector(&diskMap, 1);
	readSector(&diskDir, 2);
	
	//Go through 16 entries in the directory
	for (i=0; i<16; i++){
		//Go through 6 first bytes to compare file name
		for (j=0; j<6; j++){
			//If a character of file name does not match, break
			if (diskDir.entries[i].name[j] != fname[j]) {
				break;
			}
			//If all 6 character in the file name match, get entry number
			if (j==5){
				entry = i;
			}
		}	
	}
	
	//If file not found
	if (entry == -1) {
		return -1;
	}

	//Modify diskMap
	for (i=0; i<26; i++) {
		j = diskDir.entries[entry].sectors[i];
		if (j == 0x00){
			break;
		}
		diskMap[j] = 0x00;
	}

	//Modify diskDir
	diskDir.entries[entry].name[0] = 0x00;
	
	writeSector(diskMap, 1);
	writeSector(&diskDir, 2);

	return 1;
}

/*
Write from buffer to a disk sector
Input: buffer, sector
Ouput: buffer content written into the disk sector indicated
*/
int writeSector(char * buffer, int sector){
	int ax = 0x03 * 256 + 0x01;
	int relSector = modFunc(sector, 18) + 1;
	int head = modFunc(sector / 18, 2);
	int track = sector / 36;
	int cx = track * 256 + relSector;
	int dx = head * 256 + 0x00;
	int bx = buffer;
	interrupt(0x13, ax, bx, cx, dx);
	return 1;
}

/*
Terminate program
Output: program terminated, print "I'm back!" and run shell again
*/
void terminate(){
	resetSegments();
	printString("I'm back!\n\r\0");
	//Run shell again
	executeProgram("shell\0", 0x2000);
}

/*
Execute program based on file name
Input: file name, segment where the program is executed
Output: -1 if program not found, -2 if segment is not valid
*/
int executeProgram(char* name, int segment){
	char buffer[13312];
	int resultReadFile = 0;
	int i = 0;
	int offset = 0x0000;

	// check segment being used is valid
	if (modFunc(segment, 0x1000) != 0 || segment == 0x00 || segment == 0x1000 || segment >= 0xA000){
		return -2;
	}
	
	resultReadFile = readfile(name, buffer);

	//File not found
	if(resultReadFile == -1) {
		return -1;
	}

	//Put file content in memory to launch program
	for(i=0;i<13312;i++){
		putInMemory(segment, offset, buffer[i]);
		offset += 1;
	}
	
	launchProgram(segment);

}

/*
Load and print a file
Input: file name, place for file content to be placed
Output: File content is stored in the buffer, return sector that the file content is written and return -1 if file not found
*/
int readfile(char *filename, char *buf) {
	int index=0;
	int i;
	int j;
	char buffer[512];
	struct directory diskDir;
	int entry = -1;
	int indexBuf = 0;

	//Read sector 2 into disk directory
	readSector(&diskDir,2);	

	//Go through 16 entries in the directory
	for (i=0; i<16; i++){
		//Go through 6 first bytes to compare file name

		for (j=0; j<6; j++){
			if(filename[j] == 0x00){
				entry = i;
				break;
			}
			//If a character of file name does not match, break
			if (diskDir.entries[i].name[j] != filename[j]) {
				break;
			}
			//If all 6 character in the file name match, get entry number
			if (j==5){
				entry = i;
			}
		}	
	}

	//If file not found
	if (entry != -1){
		//Go through 26 bytes to get the sector that file content is written in
		for (i=0; i<26; i++) {
			
			if(diskDir.entries[entry].sectors[i] == 0x00){
				break;
			}
			//Read sector content into buffer
			readSector(&buffer, diskDir.entries[entry].sectors[i]);
			for(j=0; j<512; j++){
				buf[indexBuf] = buffer[j];
				indexBuf += 1;
			}		
		}
		// Set entry = number of sectors
		entry = i;
	}
	return entry;	
}

/*
Print a string
Input: String to be print
*/
int printString(char *str){
	int i=0;
	char al;
	char ah;
	int ax;
	
	while(str[i]){
		al = str[i];
		ah = 0x0E;
		ax = ah * 256 + al;
		interrupt(0x10, ax, 0, 0, 0);
		i++;
	}
	return i;
}

/*
Method to read a character
*/
int readChar(){
	return interrupt(0x16, 0, 0, 0, 0);
}

/*
Standard method to read string
Input: buffer char to store
*/
int readString(char *buf){
	int i=0;
	char ch[2];

	// create a string with a space
	char spaceString[2];
	spaceString[0] = 32;
	spaceString[1] = 0x00;
	
	ch[1] = 0x00;
	while(1){
		ch[0] = readChar();
		// if ENTER, then finish read string
		if (ch[0] == 0x0D){
			buf[i] = 0x00;
			break;
		}
		
		// if BACKSPACE, the cursor is moved back by 1, an empty char is printed, and the cursor is moved back by 1 again
		if (ch[0] == 0x08){
			if(i>0){
				printString(ch);
				printString(spaceString);
				printString(ch);
				i--;
				buf[i] = 0x00;
			}
		} else {
			printString(ch);
			buf[i] = ch[0];	
			i++;
		}
		
	}
	return i;
}

/*
Upgraded version of read string - user will indicate the size of the expected string, and the method will read only a string of that size or smaller
Input: buffer char to store, size of the buffer
*/
int upgradeReadString(char *buf, int size){
	int i=0;
	char ch[2];
	ch[1] = 0x00;
	while(1){
		ch[0] = readChar();
		// if ENTER, then finish read string
		if (ch[0] == 0x0D){
			if (i < size) {
				buf[i] = 0x00;
			} else {
				buf[size-1] = 0x00;
			}
			break;
		}
		
		// if BACKSPACE, the cursor is moved back by 1, an empty char is printed, and the cursor is moved back by 1 again
		if (ch[0] == 0x08){
			if(i>0){
				printString(ch);
				printString(" ");
				printString(ch);
				i--;
				if (i < size) {
					buf[i] = 0x00;
				}
			}
		} else {
			printString(ch);
			if (i < size) {
				buf[i] = ch[0];
			}	
			i++;
		}
		
	}
	if (i < size) {
		return i;
	} else{
		return size;
	}
}

/*
Read a sector from Disk via the BIOS
Input: buffer char to store, and absolute sector number of the disk to be read
*/
int readSector(char *buf, int absSector){
	int ax = 0x02 * 256 + 0x01;
	int relSector = modFunc(absSector, 18) + 1;
	int head = modFunc(absSector / 18, 2);
	int track = absSector / 36;
	int cx = track * 256 + relSector;
	int dx = head * 256 + 0x00;
	int bx = buf;
	interrupt(0x13, ax, bx, cx, dx);
	return 1;
}

/*
A method to handle interrupt 21.
Input: ax (code), bx (buffer address), cx (max buffer size), and dx
*/
int handleInterrupt21(int ax, int bx, int cx, int dx){
	char *b;
	if (ax == 0x00){
		//printString("Quit Interrupting!");
		//return 0;
		b = bx;
		return printString(b);
	}
	if (ax == 0x11){
		b = bx;
		b[0] = readChar();
		return 1;
	}
	if (ax == 0x01){
		// if size is specified, use upgraded ReadString
		b = bx;
		if (cx != 0) {
			return upgradeReadString(b, cx);
		}
		return readString(b);
	}
	if (ax == 0x03){
		b = bx;
		return readfile(b, cx);
	}
	if (ax == 0x04){
		b = bx;
		return executeProgram(b, cx);
	}
	if (ax == 0x05){
		terminate();
	}
	if (ax == 0x07){
		b = bx;
		return deleteFile(b);
	}
	if (ax == 0x08){
		b=bx;
		return writeFile(b, cx, dx);
	}
	if (ax == 0xAB){
		printInt(bx);
		return 1;
	}
	if (ax == 0xAC){
		b=bx;
		return readSector(b,cx);
	}
	return -1;
}

/*
Print out an integer
Input: an integer to be printed
*/
void printInt(int intToPrint) {
	char ch[2];
	int c = modFunc(intToPrint, 10);
	int d = intToPrint/10;
	
	ch[1] = 0x00;

	if (d != 0) {
		printInt(d);
	}
	ch[0] = c + 48;
	printString(ch);
}

/*
Take the output of a%b (a MOD b)
*/
int modFunc(int a, int b){
	int c = a/b;
	return a - c*b;
}

/* 
input: the character to be printed, row, column, and the color
output: the character printed in the specific color and the row and column provided
*/
void putChar(char toPrint, int row, int column, char color){
	int segment = 0xB000;
	int offset;

	// calculate offset
	offset = 0x8000 + 80*(row-1)*2 + 2*(column-1);

	// put character in memory
	putInMemory(segment, offset, toPrint);

	// add color
	putInMemory(segment, offset + 1, color);
}

/*
Input: the string to be printed, row, column, and the color
Output: the entire string printed in the specific color and the row and column provided
*/
void putStr(char *toPrint, int row, int column, char color){
	int i=0;

	// print each character in the string
	while(toPrint[i]){
		putChar(toPrint[i], row, column+i, color);
		i++;
	}
}

