/*
 *  testproc.c
 
 */

#define MAIN

#include "stdio.h"
#include "assert.h"
#include <string.h>
#include "proc.h"

void testInit() {	
	initializeProcStructures();
	
	assert(running == &idleProc);
	assert(readyHead == NULL);
	assert(readyTail == NULL);
	int i=0;
	for (i=0; i<8; i++) {
		assert(memoryMap[i] == FREE);
		assert(pcbPool[i].name[0] == 0x00);
		assert(pcbPool[i].state == DEFUNCT);
		assert(pcbPool[i].segment == 0x00);
		assert(pcbPool[i].stackPointer == 0x00);
	}
	
	assert(strcmp(idleProc.name, "IDLE\0") == 0);
	assert(idleProc.segment == 0x0000);
	assert(idleProc.stackPointer == 0x0000); 
}

void testGetFreeMemorySegment() {
	initializeProcStructures();

	assert(getFreeMemorySegment() == (0+2)*0x1000);
	assert(getFreeMemorySegment() == (1+2)*0x1000);
	assert(getFreeMemorySegment() == (2+2)*0x1000);
	assert(getFreeMemorySegment() == (3+2)*0x1000);
	assert(getFreeMemorySegment() == (4+2)*0x1000);
	assert(getFreeMemorySegment() == (5+2)*0x1000);
	assert(getFreeMemorySegment() == (6+2)*0x1000);
	assert(getFreeMemorySegment() == (7+2)*0x1000);
	assert(getFreeMemorySegment() == NO_FREE_SEGMENTS);
}

void testReleaseMemorySegment() {
	initializeProcStructures();

	releaseMemorySegment((0+2)*0x1000);
	assert(getFreeMemorySegment() == (0+2)*0x1000);
	getFreeMemorySegment();
	getFreeMemorySegment();
	releaseMemorySegment((1+2)*0x1000);
	assert(getFreeMemorySegment() == (1+2)*0x1000);
	releaseMemorySegment((-1+2)*0x1000);
	releaseMemorySegment((8+2)*0x1000);
	assert(getFreeMemorySegment() == (3+2)*0x1000);
	
}

void testGetFreePCB() {
	initializeProcStructures();
	
	assert(getFreePCB() == &pcbPool[0]);
	assert(pcbPool[0].state == STARTING);
	assert(getFreePCB() == &pcbPool[1]);
	assert(pcbPool[1].state == STARTING);
	assert(getFreePCB() == &pcbPool[2]);
	assert(getFreePCB() == &pcbPool[3]);
	assert(getFreePCB() == &pcbPool[4]);
	assert(getFreePCB() == &pcbPool[5]);
	assert(getFreePCB() == &pcbPool[6]);
	assert(getFreePCB() == &pcbPool[7]);
	assert(getFreePCB() == NULL);
}

void testReleasePCB() {
	struct PCB *pcb;
	initializeProcStructures();

	pcb = getFreePCB();
	releasePCB(pcb);

	assert(pcb->state == DEFUNCT);
	assert(pcb->next == NULL);
	assert(pcb->prev == NULL);
	assert(pcb->name[0] == 0x00);
	
	assert(pcbPool[0].state == DEFUNCT);
	assert(pcbPool[0].next == NULL);
	assert(pcbPool[0].prev == NULL);
	assert(pcbPool[0].name[0] == 0x00);
}

void testAddToReady() {
	initializeProcStructures();

	addToReady(&pcbPool[0]);
	assert(readyHead == &pcbPool[0]);
	assert(readyTail == &pcbPool[0]);	
	addToReady(&pcbPool[1]);
	assert(readyHead == &pcbPool[0]);
	assert(readyTail == &pcbPool[1]);
}

void testRemoveFromReady() {
	initializeProcStructures();

	assert(removeFromReady() == NULL);

	addToReady(&pcbPool[0]);
	addToReady(&pcbPool[1]);
	addToReady(&pcbPool[2]);

	assert(removeFromReady() == &pcbPool[0]);
	assert(readyHead == &pcbPool[1]);
	assert(readyTail == &pcbPool[2]);

	assert(removeFromReady() == &pcbPool[1]);
	assert(removeFromReady() == &pcbPool[2]);
	assert(readyHead == NULL);
	assert(readyTail == NULL);

}

int main() {
	printf("Testing initializeProcStructures\n");
	testInit();

	printf("Testing getFreeMemorySegment\n");
	testGetFreeMemorySegment();

	printf("Testing testReleaseMemorySegment\n");
	testReleaseMemorySegment();

	printf("Testing testGetFreePCB\n");
	testGetFreePCB();

	printf("Testing testReleasePCB\n");
	testReleasePCB();

	printf("Testing testAddToReady\n");
	testAddToReady();

	printf("Testing testRemove\n");
	testRemoveFromReady();

	printf("done\n");

	
}
