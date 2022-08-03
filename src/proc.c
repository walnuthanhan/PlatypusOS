#include "proc.h"

void initializeProcStructures() {
	int i;
	char idleProcName[5];

	idleProcName[0] = 'I';
	idleProcName[1] = 'D';
	idleProcName[2] = 'L';
	idleProcName[3] = 'E';
	idleProcName[4] = '\0';
	//Set all entries in memory map to FREE
	for(i=0; i<8; i++){
		memoryMap[i] = FREE;	
	}

	//Initialize PCB pool
	for(i=0; i<8; i++){
		pcbPool[i].name[0] = NULL;
		pcbPool[i].state = DEFUNCT;
		pcbPool[i].segment = 0x0000;
		pcbPool[i].stackPointer = 0x0000;	
	}

	//Initialize idle PCB
	for(i=0; i<5; i++){
		idleProc.name[i] = idleProcName[i];
	}
	idleProc.state = READY;
	idleProc.segment = 0x0000;
	idleProc.stackPointer = 0x0000;

	running = &idleProc;
	readyHead = NULL;
	readyTail = NULL;
	
}

int getFreeMemorySegment() {
	int i;
	for(i=0; i<8; i++) {
		if (memoryMap[i] == FREE){
			memoryMap[i] = USED;
			return (i+2)*0x1000;	
		}
	}
	return NO_FREE_SEGMENTS;
}

void releaseMemorySegment(int seg) {
	int i = (seg/0x1000)-2;
	if (i >=0 && i <=7){
		memoryMap[i] = FREE;
	}
}

struct PCB *getFreePCB() {
	int i;

	for(i=0; i<8; i++) {
		if(pcbPool[i].state == DEFUNCT) {
			pcbPool[i].state = STARTING;
			return (&pcbPool[i]);
		}
	}
	return NULL;
}

void releasePCB(struct PCB *pcb) {
	pcb->state = DEFUNCT;
	pcb->next = NULL;
	pcb->prev = NULL;
	pcb->name[0] = 0x00;
}

void addToReady(struct PCB *pcb) {
	if(readyHead == NULL) {
		readyHead = pcb;
		readyTail = pcb;
	} else {
		readyTail->next = pcb;
		readyTail = pcb;
	}
}

struct PCB *removeFromReady() {
	struct PCB *pcb;
	pcb = readyHead;
	if (readyHead == readyTail) {
		readyHead = NULL;
		readyTail = NULL;
	} else {
		readyHead = readyHead->next;
	}
	return pcb;
}
