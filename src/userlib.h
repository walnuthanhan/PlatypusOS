#ifndef USERLIB_H_
#define USERLIB_H_

int printString(char *str);
int upgradeReadString(char *buf, int size);
int readString(char *buf);
int readChar();
int readfile(char *filename, char *buf);
int executeProgram(char* name);
void terminate();
int deleteFile(char *fname);
int writeFile(char *fname, char *buffer, int sectors);
void dir();
void printInt(int intToPrint);
int readSector(char *buf, int absSector);
void yield();
void showProcesses();
int kill(int segment);
int restoreFile(char *fname);

#endif
