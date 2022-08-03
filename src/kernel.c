// declare functions
int printString(char *str);
int readChar();
int readString(char *buf);
void putChar(char toPrint, int row, int column, char color);
void putStr(char *toPrint, int row, int column, char color);
int readSector(char *buf, int absSector);
int modFunc(int a, int b);
int handleInterrupt21(int ax, int bx, int cx, int dx);

main(){
	char *buf = "*****\0";
	//char ch;
	//char line[20];
	char buffer[512];
	char line[80];
	char ch[1];	

	printString("Hello World\0");
	printString("\n\r\0");

	// read the character

/*	printString("Type a char: \0");
	ch = readChar();
	buf[2] = ch;
	printString("Read: \0");
	printString(buf);
	printString("\n\r\0");

	// read the string
	printString("Enter a line: \0");
	readString(line);
	printString("\n\r\0");
	printString(line);	
*/
	/*readSector(buffer, 30);
	printString(buffer);*/
/*
	makeInterrupt21();
	interrupt(0x21, 0x00, 0, 0, 0);
*/
	makeInterrupt21();
	
	interrupt(0x21, 0x00, "Type>\0", 0, 0);
	interrupt(0x21, 0x11, ch, 0, 0);
//	line[0] = 0x41;
	line[0] = ch[0];
	line[1] = 0x00;
//	printString(line);
	interrupt(0x21, 0x00, line, 0, 0);
	
	// while loop
	while(1){
	}

}

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

int readChar(){
	return interrupt(0x16, 0, 0, 0, 0);
}

int readString(char *buf){
	int i=0;
	char ch[2];
	ch[1] = 0x00;
	while(1){
		ch[0] = readChar();
		if (ch[0] == 0x0D){
			buf[i] = 0x00;
			break;
		}
		buf[i] = ch[0];
		printString(ch);		
		i++;
		
	}
	return i;
}

int readSector(char *buf, int absSector){
	int ax = 0x02 * 256 + 0x01;
	int relSector = modFunc(absSector, 18) + 1;
	int head = modFunc(absSector / 18, 2);
	int track = absSector / 36;
	int cx = track * 256 + relSector;
	int dx = head * 256 + 0x00;
	int bx = &buf;
	interrupt(0x13, ax, bx, cx, dx);
	return 1;
}

int handleInterrupt21(int ax, int bx, int cx, int dx){
	char *b;
	if (ax == 0x00){
		//printString("Quit Interrupting!");
		//return 0;
		return printString(bx);
	}
	if (ax == 0x11){
		//printString("read char");
		b = bx;
		b[0] = readChar();
		return 1;
	}
	if (ax == 0x01){
		return readString(bx);
	}
	return -1;
}

int modFunc(int a, int b){
	int c = a/b;
	return a - c*b;
}

// input: the character to be printed, row, column, and the color
// output: the character printed in the specific color and the row and column provided
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

// input: the string to be printed, row, column, and the color
// output: the entire string printed in the specific color and the row and column provided
void putStr(char *toPrint, int row, int column, char color){
	int i=0;

	// print each character in the string
	while(toPrint[i]){
		putChar(toPrint[i], row, column+i, color);
		i++;
	}
}

