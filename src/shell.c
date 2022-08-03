#include "userlib.h"

main(){
	char command[10000];
	char file[13312];
	int result = 0;
	while(1){
		//Print "Shell>"
		//interrupt(0x21, 0x00, "Shell>\0", 0, 0);
		printString("Shell>\0");

		//Receive user input
		//interrupt(0x21, 0x01, command, 0, 0);
		readString(command);
		//interrupt(0x21, 0x00, "\n\r\0", 0, 0);
		printString("\n\r\0");

		//Command Type
		if(command[0] == 't' && command[1] == 'y' && command[2] == 'p' && command[3] == 'e'){
			//Read file
			//result = interrupt(0x21, 0x03, command+5, file, 0);
			result = readfile(command+5, file);
			
			if(result == -1){
				//File not found
				//interrupt(0x21, 0x00, "Error! The file does not exist on the disk.\n\r\0", 0, 0);
				printString("Error! The file does not exist on the disk.\n\r\0");
			} else {
				//Print file content
				//interrupt(0x21, 0x00, file, 0, 0);
				printString(file);
			}
		
		} 
		//Command Execute
		else if(command[0] == 'e' && command[1] == 'x'&& command[2] == 'e' && command[3] == 'c' && command[4] == 'u' && command[5] == 't' && command[6] == 'e'){
			//Execute file
			//result = interrupt(0x21, 0x04, command+8, 0x2000, 0);
			result = executeProgram(command+8, 0x2000);

			if(result == -1){
				//File not found
				//interrupt(0x21, 0x00, "Error! The file does not exist on the disk.\n\r\0", 0, 0);
				printString("Error! The file does not exist on the disk.\n\r\0");

			}
		} else {
			//If command is not one of the above commands
			//interrupt(0x21, 0x00, "Unrecognized command.\n\r\0", 0, 0);
			printString("Unrecognized command.\n\r\0");

		}
	}
}
