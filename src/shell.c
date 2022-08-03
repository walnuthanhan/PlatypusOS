#include "userlib.h"

main(){
	char command[10000];
	char file[13312];
	int result = 0;
	char arg1[1000];
	int count = 0;

	enableInterrupts();

	while(1){
		printString("Shell>\0");

		//Receive user input
		readString(command);
		printString("\n\r\0");

		//Command Type
		if(command[0] == 't' && command[1] == 'y' && command[2] == 'p' && command[3] == 'e' && command[4] == ' '){
			//Read file
			result = readfile(command+5, file);
			
			if(result == -1){
				//File not found
				printString("Error! The file does not exist on the disk.\n\r\0");
			} else {
				//Print file content
				printString(file);
				printString("\n\r\0");
			}
		
		} 
		//Command Execute
		else if(command[0] == 'e' && command[1] == 'x'&& command[2] == 'e' && command[3] == 'c' && command[4] == 'u' && command[5] == 't' && command[6] == 'e' && command[7] == ' '){
			//Execute file
			//result = interrupt(0x21, 0x04, command+8, 0x2000, 0);
			result = executeProgram(command+8);

			if(result == -1){
				//File not found
				//interrupt(0x21, 0x00, "Error! The file does not exist on the disk.\n\r\0", 0, 0);
				printString("Error! The file does not exist on the disk.\n\r\0");

			}
		}
		//Command Delete 
		else if(command[0] == 'd' && command[1] == 'e'&& command[2] == 'l' && command[3] == 'e' && command[4] == 't' && command[5] == 'e' && command[6] == ' ') {
			result = deleteFile(command+7);
			if (result == -1) {
				printString("File not found.\n\r\0");
			}
		} 
		//Command Copy 
		else if(command[0] == 'c' && command[1] == 'o' && command[2] == 'p' && command[3] == 'y' && command[4] == ' ') {
			count = 0;
			while(command[5+count] != ' '){
				arg1[count] = command[5+count];
				count = count + 1;
			}
			arg1[count] = '\0';
			result = readfile(arg1, file);
			if(result == -1){
				printString("File not found.\n\r\0");
			} else {
				result = writeFile(command+6+count, file, result);
				if(result == -1){
					printString("Disk directory is full.\n\r\0");
				} 
				if(result == -2){
					printString("Disk is full.\n\r\0");
				}
			}
		}
		//Command dir
		else if(command[0] == 'd' && command[1] == 'i' && command[2] == 'r') {
			dir();
		}
		//Command ps
		else if(command[0] == 'p' && command[1] == 's'){
		  	showProcesses();
		}
		//Command kill
		else if(command[0] == 'k' && command[1] == 'i' && command[2] == 'l'&& command[3] == 'l'&& command[4] == ' ') {
			arg1[0] = command[5];
		  	if(arg1[0] == '0'){
		   		result = kill(0);
		  	} else if(arg1[0] == '1'){
				result = kill(1);
			} else if(arg1[0] == '2'){
		    		result = kill(2);
		  	}else if(arg1[0] == '3'){
		    		result = kill(3);
		  	}else if(arg1[0] == '4'){
		    		result = kill(4);
		  	}else if(arg1[0] == '5'){
		    		result = kill(5);
		  	}else if(arg1[0] == '6'){
		    		result = kill(6);
		  	}else if(arg1[0] == '7'){
		    		result = kill(7);
		  	} else{
		    		printString("That segment does not exist\n\r\0");
				continue;
		  	}
		  	if(result == 1){
		    		printString("Process killed successfully\n\r\0"); 
		  	} else{
		    		printString("No process currently running in that segment\n\r\0");
		  	}
		}
		//Command Restore
		else if(command[0] == 'r' && command[1] == 'e'&& command[2] == 's' && command[3] == 't' && command[4] == 'o' && command[5] == 'r' && command[6] == 'e' && command[7] == ' ') {
			result = restoreFile(command+8);
			if (result == -1) {
				printString("File not found.\n\r\0");
			} else if (result == -2){
			  printString("Sector is overwritten. Cannot restore file.\n\r\0");
			}
		}
		else {
			//If command is not one of the above commands
			//interrupt(0x21, 0x00, "Unrecognized command.\n\r\0", 0, 0);
			printString("Unrecognized command.\n\r\0");

		}
	}
}
