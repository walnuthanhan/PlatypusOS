// declare functions
void putChar(char toPrint, int row, int column, char color);
void putStr(char *toPrint, int row, int column, char color);

main(){
	// create string "Hello World"
	char *hello = "Hello World";
	int i;
	
	// print string "Hello World" in white on black background
	// at the top left corner of the screen
	for(i=0; i<11; i++){
		putInMemory(0xB000, 0x8000+i*2, hello[i]);
		putInMemory(0xB000, 0x8000+i*2+1, 0x0F);
	}
	
	// print "Hello World" in white on a red background
	// at the center of the screen
	for(i=0; i<11; i++){
		putChar(hello[i], 13, 35+i, 0x4F);
	}

	// print "Hello World" in red on white background
	// in the lower right corner of the screen
	putStr(hello, 25, 70, 0xF4);

	// while loop
	while(1){
	}
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

