
## Idea

Random access file. (Database)


The program should manage user id's like Mysql do with `AUTO_INCREMENT` feature. That means the program should receive only an username and determine the next id to put the username by using global veriable that holds the last inserted id. When an username is inserted into the user tabel (Random access file) the global variable that holds the last inserted id should be increased by one, so it's ready for next call.
The idea is that threads is going to be used to boost the performance by creating a new thread for every call to put an username to Random-Access-file. 

## Race condition
First The program is implemented withuot threads. And the result after putting 10 users in the table is in the figure 1. Like it is excpected 10 users with respective id's and no id's is skipped. 
Next the program is implemented with threads to execute `putUser()` function parallel and tested with the same 10 users. And as you can see in the figure the race condition occured.  
The race condition occured when the global variable was used for multiple threads to deremine the next id to put into Random Access file. As you can see in the picture below, there was trying to put 100 users at once. Instead of 100 user in table only 11 was inserted. The problem that many threads started at once by taking the same id and they overwrite the same id with many usernames in the Random Access file. So mostly username above 90 exists in the table, because these threads with these username was sheduled as last. 
Expected . Actual
the result is unpredictable and changes every time. 

- it can also be that it makes fseek and another thread makes fwrite in another threads fseek. fseek fseek - sets or moves the position of the read / write cursor for the indicated stream [^tomasz]
because it uses the global file pointer. 

**Mutual exclusion** - it states that no other process is allowed to execute in the critical section if a process is exectuing in critical section .  [^geeks1]

- SEEK and WRITE skal være atomic

### Solution

as you can see in the figure 1. After mutex implementation all users has been inserted to the file. However some usernames was mixed up. For ex. user with id 2 has a username4, but it only means that thread that received an user with username4 got change to execute the code earlier than thread that got an user with id X by taking next availlbe id, that was in this case 2. So it only confirms that the program using threads and procted critical sections with mutex locks. 



## Why?

the basic idea to you threads here to boost performance.

if 100 needs to be inserted toi the table and one user wait for read operation, so it receive the to the user that asked for read 



All writing operation is executes in one thread, while read is executes in another thread



## Mutex lock
Locks the resources but not code segment, because each thread has it own code, but the same file pointer 

So only one thread at the time can access File pointer, if it read or write threads, so it does not interleaves each other. 

- dont touch fp until another thread is done with that

 https://stackoverflow.com/questions/40479316/what-does-it-mean-to-lock-a-resource-using-pthread-mutex-lock 







 Lock & Unlock technique.Locking part is done in the Entry Section, so that only one process is allowed to enter into the Critical Section, after it complete its execution, the process is moved to the Exit Section, where Unlock Operation is done so that another process in the Lock Section can repeat this process of Execution.This process is designed in such a way that all the three conditions of the Critical Sections are satisfied. 



And it fixed again by adding mutex

- 

-  https://www.webopedia.com/TERM/M/mutex.html 
-  [https://geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/](https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/) 



[^tomasz]: 62409 C programming for Java programmers. Lab Manual V0.1 April 2019. By Tomasz Blaszczyk
[^geeks1]:  https://www.geeksforgeeks.org/process-synchronization-set-2/ 

