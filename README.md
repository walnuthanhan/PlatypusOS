# PlatypusOS

- [I. Introduction](#i-introduction)
- [II. Setup Process](#ii-setup-process)
- [III. User commands supported](#iii-user-commands-supported)
- [IV. Bugs &amp; Limitations](#iv-bugs--amp--limitations)
- [V. Implementation](#v-implementation)
- [VI. Conclusion](#vi-conclusion)

## I. Introduction

**PlatypusOS** is a simple 16-bit operating system implemented by Vu Pham and Han Trinh as part of the COMP 354, Operating System class at Dickinson College. The operating system tries to mimic the basic functionalities of many popular OS. PlatypusOS contains a shell program with a command line interface and a simple text editor program, which can be run through the shell. This operation system can also implement simple functions, such as: print output strings and integers to the screen, read users&#39; keyboard inputs, read data from a disk sector, write data to a disk sector, execute a program, terminate a program as well as read, write, and delete files. Moreover, users can copy a file and list all existing files on the disk through the shell command line program. This project also allows programs to be run concurrently using round-robin scheduling algorithms – functionalities such as memory management, time-sharing, and process management are implemented to make sure multiprogramming run smoothly. Through the shell command lines, users can see a list of all the processes currently running or kill a running process


## II. Setup Process

1. Have an environment to run the OS (one suggestion would be to install VirtualBox and download the virtual machine disk for MacOS High Sierra 10.13, connect the disk to VirtualBox and run it)

2. Install XQuartz

3. Download the Comp354Projects.zip file, unzip it.

4. Customize the PathSetter.bash file –change the value provided to COMP354 HOME in the bash file with the Comp354Projects directory path.

5. Open XQuartz, change directory to the _project_ folder inside Comp354Projects folder.

6. Type command _./compileOS.bash_, choose the Begin Simulation option (or just press ENTER)

## III. User commands supported

- _type \<file\>_: Display the content of _\<file\>_ on the screen. Print an error message if _\<file>_ does not exist.

- _execute \<file\>_: Execute the C program _\<file>_. Print an error message if _\<file>_ does not exist.

- _delete \<file\>:_ Delete _\<file>_ from the disk image. Print an error message if _\<file>_ does not exist.

- _copy \<src\> \<dest\>:_ Make a copy of file _\<src>_ as the new file _\<dest>._ Print an error message if _\<src>_ does not exist, or if the disk directory is full, or if the disk is full.

- _dir:_ Display a list of files currently stored on the disk with the size and the sector.

- _ps:_ Display the names and memory segment indices of currently executing proccesses.

- _kill \<seg\>:_ Kill the process that is executing in the segment with the specified index. Print an error message if the index is out of bound, or if there is no process currently running in that segment.

- _restore \<file\>:_ Restore deleted _\<file>_ from the disk image. Print an error message if _\<file>_ does not exist or if the data is not intact.

## IV. Bugs &amp; Limitations

1. Filenames have a length limiation – each filename cannot be longer than 6 characters, therefore, when users input a filename with a length larger than 6, only the first 6 characters will be considered.

2. Folder functionality is not implemented, so all files are stored at the "root folder" of the disk.

3. The shell has some bugs regarding users&#39; command line inputs because it is impossible to determine the length of the input that users put it. For example, all command lines that start with "dir" will be recognized.

4. Command _execute \<file\>_ cannot recognize if _\<file\>_ is a C program or not before executing it. Therefore, if a non-C program or a text file is used for this command, it will not go well!

5. Command _kill \<seg\>_ has some bugs. Even though that when the command is invoked, the killed process will no longer appear in the list of currently executing processes, the process is still executed in some way. Also, trying to use the command while 3 or more processes are executing will make the shell freezes.

6. Command _restore \<file\>_ cannot check the first character of filename, because that character is set to 0x00 after the file is deleted. Thus, if the deleted file is called “messag”, a command like “restore fessage” can still restore the “messag” file, though the new file name will be “fessage”.


## V. Implementation

**PlatypusOS** is implemented through 6 small projects on MacOS High Sierra 10.13 based on instructions, exisiting assembly code, bochs configuration file, floppy images, and a loadfile C program given by the instructor. Below are the files that are implemented and their details:

**compileOS.bash** is a shell script used to compile and run the kernel. When the file runs, it will assemble the assembly file _lib.asm_ and _kernel.asm_, compile all the files written in C (including the kernel, the shell, and the text editor program that are written), link object files into executable files. It will also compile _loadfile.c_, which will be used to copy executable files mentioned in the previous sentence or text messages to the disk image. The shell script also contains command lines to create a new disk image, add disk map, disk directory, compiled kernel file, and bootload program to the appropriate sectors inside that disk image. After all that, the script will boot the bochs.

**kernel.c** is the kernel written in programming language C. First, _putChar_ and _putString_ functions are used to print characters on the screen by writing bytes through function _putInMemory,_ which is already implemented in the assembly file _kernel.asm_. There is the _printString_ function, which will print strings from left to right, starting from where the last string is printed – the function is implemented using BIOS interrupt 0x10, and will stop when it reaches the null terminator. Then, the _printInt_ function is built, which will print an integer, based on _printString._ Meanwhile, function _readChar_ is implemented to read a character from the keyboard using BIOS interrupt 0x16, and from that _readString_ is written, which will read multiple characters into a buffer until ENTER key is pressed. _upgradeReadString,_ which is the upgraded version of _readString_ that can avoid Buffer Overflow_,_ is also created, with the difference is that _upgradeReadString_ will receive an extra parameter indiciating the size of the buffer. To read or write data from a sector of the disk, BIOS interrupt 0x13 is used – parameter _AH_ of _readSector_ would be 0x02 to indicate that this function will be used to read sector, while that value would be 0x03 for _writeSector_ to indicate the function will be used to write into a sector. To parse the disk directory easily, data structure _directory_ is used. The structure contains 16 directory entries – each of those entries is represented by data structure _dirEntry_, with the first 6 bytes containing the file name, and the remaining 26 bytes storing the index of the sectors that make up the file. To read a file using function _readfile,_ the disk directory is searched for the file name, then transfer the data from the sectors into a buffer. To delete a file using function _deleteFile_, the data in disk directory and disk map is cleared out appropriately and the first character of the file name is set as 0x00, while to write a new file using function _writeFile_, empty sectors are determined using the disk map, write the data into sectors, and map the file name with sector indices in the disk directory. To restore a deleted file, the function _restoreFile_ can be used, the previous deleted file will be determined by the second characters to the sixth characters of filename, and the file will be restored if its data is intact.

Function _executeProgram_ is used to execute programs by using _initializeProgram_, while _terminate_ is used to terminate programs – both access global variables and data structure in _proc.c_ through _setKernelDataSegment_ and _restoreDataSegment_. There are 3 functions that are related to concurrency: _yield_ gives up the remainder of the process&#39; time slice by using interrupt 0x08, _showProcesses_ displays a list of all running procceses, and _kill_ kills the process executing in the specified segment index.

To invoke the functions above from user-written programs outside the kernel, a system call interface is implemented by creating an ISR for user-defined interrupt 0x21, which is normally used to provide OS specific services. The interrupt vector and the ISR are set up through the exisiting assembly code in _kernel.asm_, therefore, only the function _handleInterrupt21_ needs to be defined, and it will be called whenever interrupt 0x21 occurs. There are 4 paramaters for _handleInterrupt21 – ax_ will be used to indicate which service is re

ested, and if the value of _ax_ matches, it will invoke the appropriate functions (that are defined in the kernel), while _bx, cx, dx_ will be used to pass parameters to those functions.

<div align="center">

| **ax** | **function** | **ax** | **function** | **ax** | **function** |
| --- | --- | --- | --- | --- | --- |
| 0x00 | printString | 0x11 | readChar | 0x01 | readString |
| 0x03 | readfile | 0x04 | executeProgram | 0x05 | terminate |
| 0x07 | deleteFile | 0x08 | writeFile | 0x09 | yield |
| 0x0A | showProcesses | 0x0B | kill | 0xAB | printInt |
| 0xAC | readSector | 0xCC | restoreFile |

**Table 1:** Value of ax corresponding to specific service
  
</div>

**userlib.c** and **userlib.h** are files representing a user library that users can use to invoke functions provided by the kernel. This library provides access to the OS services by calling an interrupt 0x21 and providing the _ax_ value as well as appropriate parameters so that the _handlerInterrupt21_ in _kernel.c_ can choose the right functions. Those functions in the libraries serve as wrappers around the system calls to help it easier for users to write code.

**proc.c** and **proc.h** are used to implement data structure for multiprogramming. There is the _PCB_ data structure, which represents a process control block that contains information such as name (of length 6), state of the process control block, segment, and stack pointer. Interger array _memoryMap_ is usedas a memory segment map, a list of corresponding PCB called _pcbPool_, an idle process control block, a pointer to the running process, as well as a ready queue implemented in the form of a linked list. Functions are implemented to initialize the data structure, handle the memory segment map, the ready queue, and the process control blocks. Furthermore, additional test cases for our implementations mentioned above are added to file **testproc.c**.

**texteditor.c** is a simple text editor program that can be used through the shell command line by typing the command "_execute texted"_. The text editor is implemented using many OS services in _userlib.c_ – it will read lines of text from the users until users type CTRL+D followed by the ENTER key, then the program will save the file and terminate_._


## VI. Conclusion

Overall, **PlatypusOS** is a low-level operating system that can be used to store data of small size or execute some simple programs concurrently. While the project is definitely not perfect, it gives us an unique chance to see how an operating system works and implement some simple OS functionality.