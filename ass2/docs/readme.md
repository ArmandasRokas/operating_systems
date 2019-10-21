## Users storage using random access file

Author: Armandas Rokas (s185144)
21/9 2019


### The idea of the application

The idea of the application is to simulate a database by using random-access-file in C program. The random access file should contain the user table with `id` and `username` fields. 

It assumes that the database receives two requests one after another. The first request is asks to write 1 million users to the table and the second is to print out only the first 20 users to console. 

### Quick guide

- run `make`
- run `./main [argument]` .  `[argument]` can be `1` , `2` or `3`:
  - `1` - run the program sequentially
  - `2` - run the program using multithreading without mutex protection
  - `3` - run the program using multithreading with mutex protection

### Multithreading

As described above, the database gets two requests. The first it is very huge and will take some time to execute it and the second is where small, so the users expects to receive the result immediately. However if the program runs sequentially the second request waits until the first huge request is done. That gives very bad experience for the second user. If you run a program by given `1` as an argument, you can see in the result that the read request is finished later than the write request. 

By using multithreading the program achieves that the second request can be executed parallel with the first request, that means the second request is done almost immediately(after dummy data is created), while the first request is executed in the background. If you run a program by giving `2` as an argument, you can see in the result how many seconds the read request is finished earlier than the write request. 

However the total time of the program execution is exceeded slightly, but the main purposes with multithreading is achieved that smaller request does not hang until huge requests finishes their job.

### Race condition

So multithreading gave better performance to the program, however it started returning unpredictable results every time, e.g. instead of printing out the first 30 users (users with id's from 1 to 30), the program prints out users with unpredictable id's.

The problem is that these two threads uses the same shared resource i.e. file pointer and `fseek` followed by `fwrite`/`fread` functions must be atomic.  

That means that a race condition is happened as follows: the second thread that prints the users makes `fseek` to the user with id `1`, but meanwhile the first thread that puts users makes `fseek` to the place, where the next user needs to be inserted. The program gets back to the second thread and makes `fread` in the wrong place and the printed result is not what it is expected. 


### Mutual exclusion

There is used mutex lock in order to fix the race condition described above. 

The critical section in this case is the code segments where the file pointer is used, particularly in `putUser()` and `printUsers()` functions. So mutex lock is used before `fseek` and it unlocked again after `fwrite()` /`fread()` depend on if it's `putUser()` or `printUsers` function. It means that if one thread entered critical section and executed `fseek`, another thread is not allowed to enter the section that uses the file pointer until the first thread is finished using file pointer. 



### Conclusion

In this assignment three version of the database application that stores the users table are made. The first version executes the program sequentially, which gives bad performance for smaller requests. The second version uses multithreading that improves performance, but gives unpredictable results. The last third version uses multithreading and moreover is protected with mutex locks in order to avoid race conditions.  