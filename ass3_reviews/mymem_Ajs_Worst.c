/*Operativesystems, Mandatory assignment 3, Memory Management: Worst-Fit
  19/11/19, Written by: Ajs Ritsmer Stormholt, s174517
  This program simulates the use of worst fit as the memory management strategy for a memory block
  The program is heavely based on this github project: https://github.com/nealian/cse325_project5/blob/master/mymem.c?fbclid=IwAR38Re6rl5MzitSIDUX8HOuQUak7wDHS-uqiAx09P0CQ5QMsKv8xK02YJT8
  However, edited by me to work as the program from the github creates segmentation fault if executed with this assignments test.
  Additionally, every comment in this script is written by me.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>

struct memoryList
{
  // doubly-linked list
  struct memoryList *last; //pointer to the last node in the linked-list
  struct memoryList *next; //pointer to the next node in the linked-list

  int size;   // Size of the memory block
  char alloc; // 1 if this block is allocated,  0 if this block is free.

  void *ptr; // location of block in memory pool.
};

struct memoryList *worst_block(size_t requested); //Prototype for support function

strategies myStrategy = NotSet; // Current Strategy.

size_t mySize;         //Total size of memory
void *myMemory = NULL; //Pointer to total memoryblock.

static struct memoryList *head; //Head node of the linked list. Is the first block of the total memory
static struct memoryList *next; // Points to the next memory block of the last allocated memory block.

//The following function initialises the total memory. and variables used for bookkeeping.
// The function takes the strategy to be used to allocation and the size of the total memory.
//The function does not return anything, and should be called as the first function.
void initmem(strategies strategy, size_t sz)
{
  myStrategy = strategy; // Strategy of memory management set.

  mySize = sz; // Size of total memory set.

  if (myMemory != NULL)
    free(myMemory); // in case this is not the first time initmem2 is called

  /* TODO: release any other memory you were using for bookkeeping when doing a re-initialization! */
  if (head != NULL)
    free(head); // The global variables needs to be freed.
  if (next != NULL)
    free(next);

  /* TODO: Initialize memory management structure. */
  myMemory = malloc(sz);                    //Allocate room for the total memory.
  head = malloc(sizeof(struct memoryList)); //Allocate room for a head memory block.
                                            //As it points to a struct, the size of the struct should be allocated
  next = malloc(sizeof(struct memoryList)); // Same for next.
  head->size = sz;                          //Initialize the struct variables of head. As no memory block are allocated at this point.
  head->alloc = 0;                          //Head and next will be pointing to structs equal to the total memory.
  head->ptr = myMemory;
  next = head;

  head->last = head; //Making the linked list full-circle, inorder to iterate through the list, and know when the bottom is reached
  head->next = head; // See line: xx
}

//The following function creates and allocates a memory block with the size: requested
//If succesfull the function returns a pointer to the memory block.
//If not succesfull the function returns null.
void *mymalloc(size_t requested)
{

  struct memoryList *block = NULL;           // Create memory block pointer
  block = malloc(sizeof(struct memoryList)); // allocate room for the pointer.

  switch (myStrategy) // Only the worst fit case is implemented.
  {
  case First:
    return NULL;
    break;
  case Best:
    return NULL;
    break;
  case Worst:
    block = worst_block(requested); //Worst fit method is called and requested memory size is parsed.
    break;                          // Returned is the pointer to memory block saved as block
  case Next:
    return NULL;
    break;
  default:
    printf(stderr, "myalloc: Invalid memory management strategy!\n"); //Error message
    return NULL;
  }

  if (block == NULL)
  {
    printf("Error in mymalloc: No space big enough to meet requirments"); // Error message if NULL is returned from worst_block function.
    return NULL;
  }

  if (block->size > requested) // if the size of the allocated memory block is bigger than that which is requested.
  {                            // The excess space should be freed.

    struct memoryList *remainder = malloc(sizeof(struct memoryList)); //memory block pointer to point at the excess allocated memory

    //The remainder is inserted between the block and the next memory block, with the following code:
    remainder->next = block->next;
    remainder->next->last = remainder;
    remainder->last = block;
    block->next = remainder;

    //The remainder memory block struct is set:
    remainder->size = block->size - requested; //size should be the excess space.
    remainder->alloc = 0;                      //The excess space should be freed.
    remainder->ptr = block->ptr + requested;   // the pointer should point at the block pointer offset + the requested size offset.
    block->size = requested;                   //Now the block size is accurate, according to what was requested.
    next = remainder;                          // Next is set to the memory block next of the last allocated memory block.
  }
  else // Else if the size of the biggest memory block was accurate to the size requested
  {
    next = block->next; // Next is set to the memory block next of the last allocated memory block.
  }

  block->alloc = 1; // The block is set as allocated.

  return block->ptr; // Returning void pointer to the block.
}

// The following function takes the requested size, finds the biggest avalible space the total memory, compares it to the requested size,
// and if its big enough or bigger the memory block will be returned to be allocated in mymalloc. If the biggest block is not big enough to meet requirements, null is returned.
struct memoryList *worst_block(size_t requested)
{
  //Two memory blocks are created, index is set to the beginning of the linkedlist.
  struct memoryList *index = head, *max = NULL; // max is set to null.
  max = malloc(sizeof(struct memoryList));      // Room for the max memory block is allocated.
  do                                            //This do-while loop iterates thorugh the linked list finding hte biggest free memory block.
  {
    if (!(index->alloc) && (!max || index->size > max->size)) //If the index is not allocated and max is null or the sixe of index is bigger than the size of max.
    {
      max = index; // The the index memory block is saved to max.
    }
    index = index->next;   // index is set as the next node in the linked list.
  } while (index != head); // Runs until index has made it full-circle in the linked list.

  if (max->size >= requested) // If the size of max, which is the biggest free memory space at this point, is bigger or equal the the requested memory size
  {
    return max; // The max is returned
  }
  else // Otherwise null is returned.
  {
    return NULL;
  }
}

// This function frees the space of the total memory of which the block pointer is pointer towards.
// The function does not return anything.
void myfree(void *block)
{
  struct memoryList *container = head; // A memory block is created set to the first node of the linked list, head.
  do
  { // This do-while loop iterates through the linkedlist finding the memory block corresponding to that which block is pointing to.
    if (container->ptr == block)
    {
      break;
    }
    container = container->next;
  } while (container != head);

  // The containger now holds the requested memory block and frees it.
  container->alloc = 0;

  //If the previous memory block also is free, the requested block is brought together with the previous.
  if (container != head && !(container->last->alloc))
  {
    struct memoryList *prev = container->last; //A memory block pointer is created to point at the previous node in the linked list.
    prev->next = container->next;              // The container is removed from the linked is.
    prev->next->last = prev;
    prev->size += container->size; // The prev memory block absorbs the containers space in the total memory block.

    if (next == container) // If the next pointer was set to the container it is now set to the prev.
    {
      next = prev;
    }

    free(container);  // the container is freed from the actual memory
    container = prev; // And set to the new prev pointer. To be used in the next if statement.
  }

  // If the next node is/also is free is the brought together to one free memory block. Same procedure as above.
  if (container->next != head && !(container->next->alloc))
  {
    struct memoryList *second = container->next;
    container->next = second->next;
    container->next->last = container;
    container->size += second->size;

    if (next == second)
    {
      next = container;
    }

    free(second);
  }
}

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when refered to "memory" here, it is meant that the 
 * memory pool this module manages via initmem/mymalloc/myfree. 
 */

//This function finds the number of free memory blocks in the total memory.
int mem_holes()
{
  return mem_small_free(mySize + 1); //Returns the function call of mem_small_free to the size of the total memory block, +1 incase the entire memory block is free.
}

//This function return the number of allocated bytes
int mem_allocated()
{
  return mySize - mem_free(); //Which is the total size of the memory block minus the number of free bytes.
}

//This function returns the amount of bytes free.
int mem_free()
{
  int count = 0; // A counter set to 0 is created

  struct memoryList *index = head; //Memory block pointer index is created, set to head inorder to iterate thourgh the linked list.
  do                               //This do-while loop iterates through the memory blocks
  {
    if (!(index->alloc)) // Adding to the counter if the memory block is free.
    {
      count += index->size; // Adds the size of the block inorder to get the total amount of bytes.
    }
    index = index->next;
  } while (index != head);

  return count;
}

//This function returns the size of the biggest free memory block.
int mem_largest_free()
{
  int max_size = 0;

  // The function iterates thourgh the linked is list using the index pointer
  struct memoryList *index = head;
  do
  {
    if (!(index->alloc) && index->size > max_size) // If the memory block is free and its size is bigger than max_size
    {
      max_size = index->size; // the bigger size is saved to max_size.
    }
    index = index->next;
  } while (index != head);

  return max_size; // max_size is returned.
}

//This function returns the number of free memory blocks smaller or equal to the input size.
int mem_small_free(int size)
{
  int count = 0; // counter of memory blocks.

  // The function iterates through the memory blocks adding 1 to the count if the size of index is free and smaller or equl to input size.
  struct memoryList *index = head;
  do
  {
    if (!(index->alloc) && (index->size <= size))
    {
      count++;
    }
    index = index->next;
  } while (index != head);

  return count;
}

//This function returns a character 1 or 0 corresponding to whether or not the byte of which the input ptr is pointing to.
char mem_is_alloc(void *ptr)
{ // The function iterates through the memory blocks
  struct memoryList *index = head;
  while (index->next != head)
  {
    if (ptr < index->next->ptr) // If the ptr address is smaller than the ptr of the next node of the linked list the ptr adress most be in the index
    {
      return index->alloc; // the status of the index memory block is returned
    }
    index = index->next;
  }

  //If the iteration does not find the address in the blocks is it assumed the ptr must be in the last node, and its status is returned.
  return index->alloc;
}

//The code below was given with the assignment, and comments where not made by me.

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
strategies strategyFromString(char *strategy)
{
  if (!strcmp(strategy, "best"))
  {
    return Best;
  }
  else if (!strcmp(strategy, "worst"))
  {
    return Worst;
  }
  else if (!strcmp(strategy, "first"))
  {
    return First;
  }
  else if (!strcmp(strategy, "next"))
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
  printf("Memory List {\n");
  /* Iterate over memory list */
  struct memoryList *index = head;
  do
  {
    printf("\tBlock %p,\tsize %d,\t%s\n",
           index->ptr,
           index->size,
           (index->alloc ? "[ALLOCATED]" : "[FREE]"));
  } while ((index = index->next) != head);
  printf("}\n");
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */
void print_memory_status()
{
  printf("%d out of %d bytes allocated.\n", mem_allocated(), mem_total());
  printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n", mem_free(), mem_holes(), mem_largest_free());
  printf("Average hole size is %f.\n\n", ((float)mem_free()) / mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv)
{
  strategies strat;
  void *a, *b, *c, *d, *e;
  if (argc > 1)
    strat = strategyFromString(argv[1]);
  else
    strat = Worst;

  /* A simple example.  
	   Each algorithm should produce a different layout. */

  initmem(strat, 500);

  a = mymalloc(100);
  b = mymalloc(100);
  c = mymalloc(100);
  myfree(b);
  d = mymalloc(50);
  myfree(a);
  e = mymalloc(25);

  print_memory();
  print_memory_status();
}
