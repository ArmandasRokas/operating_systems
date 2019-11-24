// my code is heavely based on an example https://github.com/nealian/cse325_project5

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>

/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

//protorype
struct memoryList* firstFit(size_t requested);

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

static struct memoryList *head;
static struct memoryList *next;


//init memory
void initmem(strategies strategy, size_t sz)
{
	myStrategy = strategy;

	//change the size of the first block of the system
	mySize = sz;

	if (myMemory != NULL) free(myMemory); 

	myMemory = malloc(sz);

	//allocate the memory
	head = malloc(sizeof(struct memoryList));

	head->size = sz; //set size of head
	head->alloc = 0; //set to no allocated
	head->ptr = myMemory; //set heads pointer to point at the beginning of the myMemory block
	next = head;

	//make the head point to itself, this is used to make it a circular memory
	head->last = head;
    head->next = head;

}

//allocate if requestedSize is met and there is more space left create reminder
void *mymalloc(size_t requestedSize)
{
	assert((int)myStrategy > 0);
	
	struct memoryList* memBlock =NULL;// create pointer for the soon to be allocated memoryblock

	switch (myStrategy)
	  {
	  case NotSet: 
	            return NULL;
				break;
	  case First:
		
     	memBlock = firstFit(requestedSize); //return the memory block advadeble

	  break;

	            return NULL;
				break;
	  case Best:
	            return NULL;
				break;
	  case Worst:
	            return NULL;
				break;
	  case Next:
	            return NULL;
				break;
	  }

	if(memBlock==NULL) { //if no free block exist
    	fprintf(stderr, "Memory error: myalloc: No available blocks of size %lu!\n", requestedSize);
    	return NULL;
  	}

	if(memBlock->size > requestedSize) { //if there is more space make a new empty node as remainder
		struct memoryList* 	remainder = malloc(sizeof(struct memoryList)); //lav en pointer til ramainder som har allokeret størrelsen
		
		//insert element
		remainder->next = memBlock->next; //make the remainders next pointer point to the element after the memBlocks
		remainder->next->last = remainder; //make the remainders next pointer point to the memBlocks last block
		remainder->last = memBlock; //make the remainders last pointer back at the memBlock
		memBlock->next = remainder; //make the memBlock's next pointer point to the remainder
		
		//split the memory and create the reminder
		remainder->size = memBlock->size - requestedSize; // make the remiainders size the differens between the size of the block vs requestedSize
		remainder->alloc = 0; //remainder is free
		remainder->ptr = memBlock->ptr + requestedSize; //remainders pointer will begin at the location of memBlock plus requestedSize
		memBlock->size = requestedSize; //make the size of what the remainder is the requestedSize
		next = remainder;
	
	} else {

		next = memBlock->next; //if there is no remainder just point to the next element

  	}
	
    memBlock->alloc=1; //make the memBlock allocated

	//remainder->next
	//printf("%d",->size - memBlock->size );


	return memBlock->ptr; //return the memory pointer to the memBlock
	//return NULL;
}

//find first. fFnds the first block
struct memoryList* firstFit(size_t requested) {
	
  struct memoryList* index = head; //make pointer and make it point to head

  do {
	  
    if( index->alloc==0 && index->size >= requested) { //if valid return the block
		
      return index;

    }

	index = index->next;

  } while(index != head);

//error
  return NULL;
}


// Frees the block 
void myfree(void* block) {

  // searching for the block pointer in the memory
  struct memoryList* container = head;
  do {
    if(container->ptr == block) { //if pointer equals to the block
      break; //stop, container is now block
    }
  } while((container = container->next) != head);

  /*container->ptr = block;
   printf("[ %d ]",container->size);
   */

  container->alloc = 0; //set as deallocated

  //combine blocks if the prev block is free
  if(container != head && !(container->last->alloc)) {
    struct memoryList* prev = container->last;

    prev->next = container->next; //make the ealier elemnts next pointer point to at the next
    prev->next->last = prev;
    prev->size += container->size;
    
    if(next == container) {
      next = prev;
    }
    
    free(container); //free the mem
    container = prev;
  }

  
  //combine blocks if more are free
  if(container->next != head && !(container->next->alloc)) {

    struct memoryList* second = container->next;

    container->next = second->next;
    container->next->last = container;
    container->size += second->size;
    
    if(next == second) {
      next = container;
    }
    
    free(second);
  }
}

//***** Memory status/property functions ******

//find holes in the block
int mem_holes()
{
	int sum = 0; //contains the sum of bytes
	struct memoryList* container = head; //make pointer

	do{

		if(container->alloc==0&&container->next->alloc!=0){ //cheks if hole, then checks if next next is a allocated hole
			sum+=1;
		}
		container=container->next;

	}while(container!=head);


	return sum; //return the sum of bytes
	//eturn 0;
}

//count allocated amount of bytes
int mem_allocated()
{
	int sum = 0; //contains the sum of bytes
	struct memoryList* container = head; //make pointer

	do{

		if(container->alloc==1){ //check if allocated
			sum+=container->size;
		}
		container=container->next; //next element

	}while(container!=head);


	return sum; //return the sum of bytes
}

//find free bytes
int mem_free()
{
	int sum = 0; //contains the sum of bytes
	struct memoryList* container = head; //make pointer

	do{

		if(container->alloc==0){
			sum+=container->size;
		}
		container=container->next;

	}while(container!=head);


	return sum; //return the sum of bytes
}

//biggest block free
int mem_largest_free()
{
	
	int biggestBlock = 0; //contains the nubmer of biggest block in bytes
	struct memoryList* container = head; //make pointer

	do{

		if(container->alloc==0 && container->size > biggestBlock){ //search through biggest

			biggestBlock=container->size;
		}
		container=container->next;

	}while(container!=head);


	return biggestBlock; //return the sum of bytes
}

//return number of blocks smaller than size
int mem_small_free(int size)
{
	int smallerThan = 0;
	struct memoryList* container = head; //make pointer

	do{

		if( container->alloc==0 && container->size <= size){
			smallerThan++;
		}
		container=container->next;

	}while(container!=head);


	return smallerThan;
}       

//chek for block has allocated memory
char mem_is_alloc(void *ptr)
{
	struct memoryList* container = head; //make pointer

	do{

		if( container->ptr > ptr ){ //check if the adress is bigger than the ptr. this must return the block
			return container->last->alloc; 
		}
		container=container->next;

	}while(container!=head);

    return NULL;
}



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


void print_memory_status() {
  printf("%d out of %d bytes allocated.\n",mem_allocated(),mem_total());
  printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",mem_free(),mem_holes(),mem_largest_free());
  printf("Average hole size is %f.\n\n",((float)mem_free())/mem_holes());
}


void try_mymem(int argc, char **argv) { //the simple test code
        strategies strat;
	void *a, *b, *c, *d, *e,*f;

	if(argc > 1){
	  strat = strategyFromString(argv[1]);
	}else{
	  strat = First;
	}


	initmem(strat,500); //init memory
	
	a = mymalloc(100); //allocate
	b = mymalloc(100); 
	c = mymalloc(200); 

	myfree(b); //free up block b

	d = mymalloc(10);

	myfree(a);

	e = mymalloc(10);

	f = mymalloc(10);

	
	print_memory_status();

	//printf("\n- %d -\n",mem_small_free(100));

	//struct memoryList* index=NULL;
	//index=head;
	
	/*do{
		printf("yoo\n");
		index=index->next;
	}while (index->next!=head);


	struct memoryList* node = head; 
    struct memoryList* last; 
    printf("\nTraversal in forward direction \n"); 
    while (node != NULL) { 
        printf(" %d ", node->size); 
        last = node; 
        node = node->next; 
    } 
*/

	
}
