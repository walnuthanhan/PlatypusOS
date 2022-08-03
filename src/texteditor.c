#include "userlib.h"

/*
Text editor implementation
Call by: execute texted
*/

main(){
	char filename[1000];
	int i=0;
	char ch1[2];
	char ch2[2];
	char file[13312];
	char spaceString[2];

	ch1[1] = '\0';
	ch2[1] = '\0';

	// create a string with a space
	spaceString[0] = 32;
	spaceString[1] = 0x00;

	printString("Enter file name: \0");
	readString(filename);
	printString("\n\r\0");

	while(1){
		readChar(ch1);
		// if Ctrl D, then finish read string
		if (ch1[0] == 0x04){
			readChar(ch2);
			if(ch2[0] == 0x0D){
				file[i] = 0x00;
				writeFile(filename, file, i/512+1);
				printString("\n\r\0");
				terminate();
			} else {
				continue;
			}
		}
		
		// if BACKSPACE, the cursor is moved back by 1, an empty char is printed, and the cursor is moved back by 1 again
		if (ch1[0] == 0x08){
			if(i>0){
				printString(ch1);
				printString(spaceString);
				printString(ch1);
				i--;
				file[i] = 0x00;
			}
		} else if (i<13311){
			printString(ch1);
			file[i] = ch1[0];	
			i++;
		}
		
	}
}