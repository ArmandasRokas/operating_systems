

### Review of Adams first-fit

Adam implemented first fit strategy and like expected his first-fit starts looking for a block from the head and looks for next   available block that fits the request. The interesting part here that Adam implemented double linked list as circular, so that means it loops the double linked list until there is returned back to the head. Code in general very good documented. 

 ### Review of Ajs worst-fit

It looks like the worst-fit algorithm does it job. The method loops all the block and searches for a biggest block then the biggest block is returned. Only one thing that is superfluous in my opinion that there is allocating a new empty block with line  `max = malloc(sizeof(struct memoryList))` inside the function. I think this is unnecessary, because the same could be achieved without `malloc`, that means only a pointer variable is enough that can point to the current max. The function could also just return `max` , although it holds still NULL value in case of the block was not found that could fit a request.  But now the method does the double work. It checks if size is not enough, and only then returns NULL. 

### Review of Kristofer

Kristofer implemented the next-fit algorithm. Like expected the algorithm starts always to look for a next available block from where it left off. The only thing that I think is little bit confusing is that the next-fit algorithm is also responsible for joining adjacent free block (call `concatNext` function). I would rather put that responsibility in`myfree` function, where possible adjacent free blocks needs to be joined every time the block is freed, so that the overhead of searching for adjacent free blocks could be eliminated. 

