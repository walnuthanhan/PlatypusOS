/*
 *  proc.h
 *  
 *  Data structures and functions used for managing memory
 *  and processes.
 */

#ifndef PROC 
#define PROC

#ifndef NULL
#define NULL 0x00
#endif

// Constants that are useful in the memory map entries.
#define FREE 0
#define USED 1
#define NO_FREE_SEGMENTS -1

// Constants for the various states that a process may be in.
#define DEFUNCT 0
#define RUNNING 1
#define READY 2
#define BLOCKED 3
#define STARTING 4

/*
 * The Process Control Block
 */
struct PCB {
	char name[7];  		// 6 chars for filename + 1 for null terminator.
	int state;		// One of the constants above.
	int segment;		// memory segment occupied by the process.
	int stackPointer;	// address of the stack pointer for the process.
	
	/*
	 * Pointers to allow PCB's to be stored in doubly linked lists.
 	 */
	struct PCB *next;
	struct PCB *prev;
};

/*
 * The following structure ensures that the global variables needed for
 * memory and process management are defined only in the class where the
 * label MAIN is defined (e.g. kernel.c).  Everywhere else they are defined
 * as externally defined global variables and will be resolved by the linker.
 */
#ifdef MAIN
  struct PCB *running;		// Pointer to the PCB of the running process
  struct PCB idleProc;		// Pointer to a PCB for the idle process
  struct PCB *readyHead; 	// Pointer to the PCB at the head of the ready queue
  struct PCB *readyTail;	// Pointer to the PCB at the tail of the ready queue
  struct PCB pcbPool[8];	// Pool of PCBs to be used

  int memoryMap[8];		// Memory map with one entry for each segment
				// Entry 0 corresponds to segment 0x2000
				// Entry 1 corresponds to segment 0x3000
				// etc...
				// Entry 7 corresponds to segment 0x9000

#else
  extern struct PCB *running;
  extern struct PCB idleProc;
  extern struct PCB *readyHead; 
  extern struct PCB *readyTail;
  extern struct PCB pcbPool[8];
  extern int memoryMap[8];
#endif

/*---- Function Prototypes ----*/


/*
 * This function initializes all of the global variables and 
 * structures defined above.  All entries in the memory map should be marked as FREE.
 * All of the PCBs in the pcbPool should have the first character of their 
 * names set to null, their state set to DEFUNCT and their segment and stack
 * pointers set to 0x0000. The PCB for the idle process should be initialized 
 * with the name IDLE, the state READY, and both the segment and stack pointers
 * set to 0x0000.  running should refer to the PCB for the idle process and the ready queue
 * should be empty.
 */  
void initializeProcStructures();

/*
 * Returns the index of the first free memory segment or -1 if 
 * all of the memory segments are used.  The returned memory segment
 * should be marked as used.
 */
int getFreeMemorySegment();

/*
 * Release the memory segment indicated by the given index into
 * the memory map.
 */
void releaseMemorySegment(int seg);

/*
 * Return a pointer to an available PCB from the PCB pool.  All of the
 * available PCBs should be marked as DEFUNCT.  The returned PCB should
 * be marked as STARTING. Return NULL if there are no PCBs available.
 */
struct PCB *getFreePCB();

/*
 * Release the provided PCB by setting its state to DEFUNCT, setting
 * its next and prev pointers to NULL and setting the first character
 * of its name to 0x00.
 */
void releasePCB(struct PCB *pcb);

/*
 * Add the provided PCB to the tail of the ready queue.
 */
void addToReady(struct PCB *pcb);

/*
 * Remove the PCB at the head of the ready queue and return a 
 * pointer to it.
 */
struct PCB *removeFromReady();

#endif