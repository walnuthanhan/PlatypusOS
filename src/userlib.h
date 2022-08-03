#ifndef USERLIB_H_
#define USERLIB_H_

int printString(char *str);
int upgradeReadString(char *buf, int size);
int readString(char *buf);
int readChar();
int readfile(char *filename, char *buf);
int executeProgram(char* name, int segment);
void terminate();

#endif