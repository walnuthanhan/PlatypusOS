#include "userlib.h"

int printString(char *str){
	return interrupt(0x21, 0x00, str, 0, 0);
}

int upgradeReadString(char *buf, int size) {
	return interrupt(0x21, 0x01, buf, size, 0);
}

int readString(char *buf) {
	return interrupt(0x21, 0x01, buf, 0, 0);
}

int readChar() {
	char buf[1];
	interrupt(0x21, 0x11, buf, 0, 0);
	return buf;
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
