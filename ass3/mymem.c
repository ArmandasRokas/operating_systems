#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>
#include <limits.h>
/**
 * Returns -1 if request can not be allocated
 *
*/
void * firstFit(size_t requested);
void * bestFit(size_t requested);

/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

struct memoryList
{
  // doubly-linked list
  struct memoryList *last;
  struct memoryList *next;

  int size;            // How many bytes in this block?
  char alloc;          // 1 if this block is allocated,
                       // 0 if this block is free.
  void *ptr;           // location of block in memory pool.
};

strategies myStrategy = NotSet;    // Current strategy


size_t mySize;
void *myMemory = NULL;

static struct memoryList *head; // static only used within compilation unit
static struct memoryList *next; // Should store value for next fit, where the memoryList er slapt sidst


/* initmem must be called prior to mymalloc and myfree.

   initmem may be called more than once in a given exeuction;
   when this occurs, all memory you previously malloc'ed  *must* be freed,
   including any existing bookkeeping data.

   strategy must be one of the following:
		- "best" (best-fit)
		- "worst" (worst-fit)
		- "first" (first-fit)
		- "next" (next-fit)
   sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
*/

void initmem(strategies strategy, size_t sz)
{	
	myStrategy = strategy;

	/* all implementations will need an actual block of memory to use */
	mySize = sz;

	if (myMemory != NULL) free(myMemory); /* in case this is not the first time initmem2 is called */

	/* TODO: release any other memory you were using for bookkeeping when doing a re-initialization! */
    struct memoryList* currBlock;
    struct memoryList* nextCurrBlock = head;
  
	while(nextCurrBlock != NULL){
        currBlock = nextCurrBlock;
		nextCurrBlock = nextCurrBlock->next;
        free(currBlock);
	}


	myMemory = malloc(sz);
	
	/* TODO: Initialize memory management structure. */
	head = (struct memoryList*) malloc(sizeof (struct memoryList));
    head->last = NULL;
	head->next = NULL;
	head->size = sz;
	head->alloc = 0;
	head->ptr = myMemory;	

}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1 
 */

void *mymalloc(size_t requested)
{
	
	assert((int)myStrategy > 0);
	
	
	switch (myStrategy)
	  {
	  case NotSet: 
	            return NULL;
	  case First:
	            return firstFit(requested);
	  case Best:
	            return bestFit(requested);
	  case Worst:
	            return NULL;
	  case Next:
	            return NULL;
	  }
	return NULL;
}

void * firstFit(size_t requested){
    struct memoryList* currBlock = head;
    struct memoryList* allocatedBlock = NULL;
    int found = -1;
    while(found != 1 && currBlock != NULL){

        if(currBlock->size == requested && currBlock->alloc == 0){
            currBlock->alloc = 1;
            allocatedBlock = currBlock;            
            found = 1;
        } else if(currBlock->size > requested && currBlock->alloc == 0){

			int remainingSize = currBlock->size - requested;
			currBlock->size = requested;
			currBlock->alloc = 1;
			struct memoryList* newBlock = (struct memoryList*) malloc(sizeof (struct memoryList));

			newBlock->next = currBlock->next;

            if(newBlock->next != NULL){
                newBlock->next->last = newBlock;
            }
			currBlock->next = newBlock;
			newBlock->last = currBlock;
			newBlock->size = remainingSize;
			newBlock->alloc = 0;
			newBlock->ptr = (char *)currBlock->ptr + requested;			
            allocatedBlock = currBlock;
			found = 1;
		} else{

              
 
			currBlock = currBlock->next;
		}
	}
    if(allocatedBlock == NULL){
        return NULL;
    }
    return allocatedBlock->ptr;		
   
}

void * bestFit(size_t requested){
    int remainingSize = INT_MAX;
    struct memoryList* currBestFit = NULL;
    // Searching for best fit
    struct memoryList* currBlock = head;
	while(currBlock != NULL){
		if(currBlock->alloc == 0 && (currBlock->size-requested) < remainingSize){
                        
            remainingSize = currBlock->size-requested; 			
            currBestFit = currBlock;
		}
		currBlock = currBlock->next;
	}
    // Allocating best fit
    if(currBestFit == NULL){
        return NULL;
    } 
    if(currBestFit->size == requested){

        currBestFit->alloc = 1;         
        return  currBestFit->ptr;
    } else{

        // making new block and linking together
		int remainingSize = currBestFit->size - requested;
		currBestFit->size = requested;
		currBestFit->alloc = 1;
		struct memoryList* newBlock = (struct memoryList*) malloc(sizeof (struct memoryList));
		newBlock->next = currBestFit->next;
                
        
        if(newBlock->next != NULL){
              newBlock->next->last = newBlock;
        }
		currBestFit->next = newBlock;
		newBlock->last = currBestFit;
		newBlock->size = remainingSize;
		newBlock->alloc = 0;
		newBlock->ptr = (char *)currBestFit->ptr + requested;

        return currBestFit->ptr;
    }
    
}


/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void* block)
{
            

    
    struct memoryList *  blockToFree = NULL;

    // Search for block in memory management data structure
    struct memoryList* currBlock = head;
        if(currBlock == NULL){
        printf("Head is null\n");
        return;
    }      
	while(currBlock != NULL){
		if(currBlock->ptr == block){
			blockToFree = currBlock;
            break;
		}
		currBlock = currBlock->next;
	}
    
    if(blockToFree == NULL){
        printf("Could not found block to free\n");
        return;
    }
    
    blockToFree->alloc = 0;

   
    if(blockToFree->last != NULL && blockToFree->last->alloc == 0 ){
        blockToFree->last->size = blockToFree->last->size + blockToFree->size;
        blockToFree->last->next = blockToFree->next;
        if(blockToFree->next != NULL){
            blockToFree->next->last = blockToFree->last;
        }
        struct memoryList * currBlock = blockToFree->last;        
        free(blockToFree);
        blockToFree = currBlock;
        
    }    

    if(blockToFree->next != NULL && blockToFree->next->alloc ==0 ){


        blockToFree->next->size = blockToFree->next->size + blockToFree->size;

        blockToFree->next->last = blockToFree->last;
        if(blockToFree->last != NULL){  
            blockToFree->last->next = blockToFree->next;    
        } else {
            head = blockToFree->next;
        }

        free(blockToFree); 
    }

	return;
}

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when refered to "memory" here, it is meant that the 
 * memory pool this module manages via initmem/mymalloc/myfree. 
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes()
{
    int num_holes = 0;
	struct memoryList* currBlock = head;
	while(currBlock != NULL){
		if(currBlock->alloc == 0){
			num_holes++;
		}
		currBlock = currBlock->next;
	}
	return num_holes;
}



/* Number of non-allocated bytes */
int mem_free()
{
	return mySize - mem_allocated();
}

/* Get the number of bytes allocated */
int mem_allocated()
{
	int size = 0;
	struct memoryList* currBlock = head;
	while(currBlock != NULL){
		if(currBlock->alloc == 1){
			size += currBlock->size;
		}
		currBlock = currBlock->next;
        
	}
	return size;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
    int currLargest = 0;
    struct memoryList* currBlock = head;
	while(currBlock != NULL){
		if(currBlock->alloc == 0 && currBlock->size > currLargest){
			currLargest = currBlock->size;
		}
		currBlock = currBlock->next;
	}
	return currLargest;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
    int numSmallFree = 0;
    struct memoryList* currBlock = head;
	while(currBlock != NULL){
		if(currBlock->alloc == 0 && currBlock->size <= size){
			numSmallFree++;
		}
		currBlock = currBlock->next;
	}
	return numSmallFree;
}       

char mem_is_alloc(void *ptr)
{
    char isLocated = 0;
    struct memoryList* currBlock = head;
	while(currBlock != NULL){
		if(currBlock->alloc == 1){
            for(int i = 0; i < currBlock->size; i++){                
                if(currBlock->ptr + i == ptr){
                    isLocated = 1;
                }
            }
		}
		currBlock = currBlock->next;
	}
    return isLocated;
}

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may find them useful.
 */


//Returns a pointer to the memory pool.
void *mem_pool()
{
	return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total()
{
	return mySize;
}


// Get string name for a strategy. 
char *strategy_name(strategies strategy)
{
	switch (strategy)
	{
		case Best:
			return "best";
		case Worst:
			return "worst";
		case First:
			return "first";
		case Next:
			return "next";
		default:
			return "unknown";
	}
}

// Get strategy from name.
strategies strategyFromString(char * strategy)
{
	if (!strcmp(strategy,"best"))
	{
		return Best;
	}
	else if (!strcmp(strategy,"worst"))
	{
		return Worst;
	}
	else if (!strcmp(strategy,"first"))
	{
		return First;
	}
	else if (!strcmp(strategy,"next"))
	{
		return Next;
	}
	else
	{
		return 0;
	}
}


/* 
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory()
{
    struct memoryList * currMemory = head;
    while(currMemory != NULL){
        printf("Size: %d, Alloc: %d\n", currMemory->size, currMemory->alloc);
        currMemory = currMemory->next;    
    }
    /*
    printf("\nPrinting backwards\n");
    currMemory = head;
    while(currMemory != NULL){
        if(currMemory->next==NULL){
            struct memoryList * backList = currMemory;            
            while(backList != NULL){
                printf("Size: %d, Alloc: %d\n", backList->size, backList->alloc);
                backList = backList->last;
            }
        }
        currMemory = currMemory->next;    
    }*/
	return;
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */ 
void print_memory_status()
{
	printf("%d out of %d bytes allocated.\n",mem_allocated(),mem_total());
	printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",mem_free(),mem_holes(),mem_largest_free());
	printf("Average hole size is %f.\n\n",((float)mem_free())/mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv) {
        strategies strat;
	void *a, *b, *c, *d, *e;
	if(argc > 1)
	  strat = strategyFromString(argv[1]);
	else
	  strat = First;
	
	
	/* A simple example.  
	   Each algorithm should produce a different layout. */
	
	initmem(strat,290);
	/*
	a = mymalloc(100);
	b = mymalloc(100);
	c = mymalloc(100);
	myfree(b);
	d = mymalloc(50);
	myfree(a);
	e = mymalloc(25);
	myfree(b);*/

	a = mymalloc(20);
	b = mymalloc(100);
	c = mymalloc(40);
	d = mymalloc(100);
	e = mymalloc(30);
    myfree(a);
    myfree(c);
    myfree(e);
    a = mymalloc(25);  


	print_memory();
	print_memory_status();
	
}
