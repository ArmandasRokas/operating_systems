## Memory management

Author: Armandas Rokas (s185144)
08/11 2019

### Intro

In this assignment there are implemented the best-fit and first-fit strategies for selecting in which block to place a new requested memory block. 

### Guide

In order to run at tests run these commands:

- `./mem -test -f0 all first` for first-fit
- `./mem -test -f0 all best` for best-fit

### First-fit 

The purpose with first-fit is to select the first possible memory block that is large enough to allocate the requested size.

It basically loops the double-linked list and check is size bigger or equal to requested. If yes, so it allocate a request in the node and the remaining size is used to create a new node that is inserted between the newly allocated node and its next node. 

### Best-fit

The purpose with best-fit is to select the block that is smallest possible to fit the request.

The difference from first-fit that it loops all nodes by updating the better fit every time there is found one. When the searching is done, the block is allocated by using the same technique as it explained above. 

### myfree

The purpose with `myfree` is to free the memory block. In order to avoid adjacent free blocks the function checks every time adjacent block and if they are free so they are coalesced. 

### Answer to the questions

##### 1) Why is it so important that adjacent free blocks not be left as such?  What would happen if they were permitted?

It could cause that the free blocks will become too small to satisfy the requests to allocate some memory, although there is a sufficient amount of free memory available. 

##### 2) Which function(s) need to be concerned about adjacent free blocks?

- `myfree()`, because when the memory manager free ups a block, it should coalesce adjacent free blocks. 

##### 3) Name one advantage of each strategy.
- First-fit
  -  Advantage: Fastest algorithm because it searches as little as possible. 
  - Disadvantage:  The remaining unused memory areas left after allocation become waste if it is too smaller. Thus request for larger memory requirement cannot be accomplished. 
- Next-fit 
  - Memory blocks is more evenly distributed in the memory pool, because it does not start looking for a free block every time from the beginning, but from where it left off. 
- Best-fit:
	- Advantage: guarantees that the fragment left behind is as small as possible, so minimizes the wastage space
	- Disadvantage: it consumes a lot of processor time for searching the block which is close to the required size.
	- Disadvantage: the result is that main memory is quickly littered by blocks too small to satisfy memory allocation requests. Thus,
	- Disadvantage: Best-fit may perform poorer than other algorithms in some cases
- Worst-fit
  - Advantage:  Reduces the rate of production of small gaps. 
  - Disadvantage:  If a process requiring larger memory arrives at a later stage then it cannot be accommodated as the largest hole is already split and occupied. 
- References:
  -  https://www.tutorialspoint.com/operating_system/os_memory_allocation_qa2.htm 
  - Stallings William, Operating Systems Internals and Design Principles 9e. ISBN : 0134670957
##### 4) Run the stress test on all strategies, and look at the results (tests.out). What is the significance of "Average largest free block"?  Which strategy generally has the best performance in this metric?  Why do you think this is?

- Stress test no working

##### 5) In the stress test results (see Question 4), what is the significance of "Average number of small blocks"?  Which strategy generally has the best performance in this metric?  Why do you think this is?

- Stress test no working

##### 6) Eventually, the many mallocs and frees produces many small blocks scattered across the memory pool.  There may be enough space to allocate a new block, but not in one place.  It is possible to compact the memory, so all the free blocks are moved to one large free block.  How would you implement this in the system you have built?
There could occur two different cases of free small blocks. The first case is when many small free block is placed contiguous. This case is solved in my implementation of memory management and it works as follows. Every time a block needs to be made free, the `myfree()`  function checks, if the block before or after is free. If it is a case, so joins these blocks to one bigger block.  



The second case is when there are some allocated blocks between the free small blocks. It is more difficult to handle and it is not enough to check block neighbors in the `myfree()` function. So in order to compact the memory there should be a separate functions that looks for the free block and moves to the larger free block.  I would implement this like so. If there are no space to allocate a new block, so the memory manager would try to compact free block and try again to allocate. 
```
****The second case of free small blocks****
|###|
|   |		error no space	
|###|     <--- 	|###|
|   |			|###|
|###|
|   |
|###|
```

##### 7) If you did implement memory compaction, what changes would you need to make in how such a system is invoked (i.e. from a user's perspective)?

- The addresses of the memories should be updated. 
- Dynamic relocation capabilities. 

##### 8) How would you use the system you have built to implement realloc?  (Brief explanation; no code)

- The main goal with `realloc` is to change the size of the block.  So `realloc` firstly needs to check if the size of the request is available.  Next it will allocate a new block with `mymalloc`, copies the content from the previously allocated block to the new block and lastly removes the previously allocated block with `myfree`.

##### 9) Which function(s) need to know which strategy is being used?  Briefly explain why this/these and not others.

-  `mymalloc` because it allocates memory differently depend on the type used. 

##### 10) Give one advantage of implementing memory management using a linked list over a bit array, where every bit tells whether its corresponding byte is allocated.

- Easier to determine the size of the block. Thus, if memory management was implemented by using a bit array, so there should be implemented an extra algorithm, that defines the length of block, when it is needed. 