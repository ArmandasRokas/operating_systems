Chat application with a client and server - Christoffer Detlef s185117

First I will tell you how to use the program, and then I will tell you some more information about the program.
So first of you will have to open up 1 terminal to set the hole thing up. So you will first find the directory where
the folder with all the java files are. When you have done that, you will now have to compile these files. If you have
not already done that you will have to type "sudo apt-get update" after that you wanna type in "sudo apt install default-jdk"
when that is done and you are in the directory where all the files are, you will than type in javac filename.java. So
you will have to do that with all the java files. When that is done, you can now run the different classes. The first
class you want to run is the server class, you open that by typing java Server. Now the server is running. Now you want
to open 1 or more clients. You do that by open a new terminal, and go to the same directory, and then type java Client.
If you want to open more clients, you will have open more terminals, and do the same thing. If the files are already
compilet, than you do not have to compile them again.


The idea of this application is a chat system between two or more clients. These clients should than be connected to
the same server, to make sure that they can speak together. Beside just talking to each other, the program should
save all the messages between them in a logfile.

The reason why the program has multithreading is so there can be more than just one client. Becasue one client uses one
thread. To handle the clients using multithreading I have a client handler class named Handler. The Handler takes care
of the request and response. Where the server makes the new thread. Mutual exclusion is when you only want one client
to type at the time. So in this program to make sure that this can happen I have used synchronized. This makes sure 
that only one client can access the logfile at the time. 

The program is written in Java, and the program used to write it is IntelliJ. The reason why I have used that is 
because you can run the server and multiple clients at the same time, and you can also type and read messages in the
console. This does so I did not need to open multiple terminals for each client and the server. This is why I would
recommend the person testing this to run it there. However it can also be runned inside the terminal of the OS.

Like I said earlier, this application needs multithreading because else you would not be avaliable to be write to more
than the server. The multithreading does so the server can hold more clients at the same time, which is what a chat
application needs. The reason why it also needs mutual exclusion is for when a message is send to the logfile
If more than one person can do that, than if two clients write at the same time, it could mess up the text inside the
logfile.

To test if the server can have more than one client active, I simple start up the server, and then I start up three
clients. When the threeclients opens, it will print out in the server console that there is three sockets connectet.
I then try to write from client 1 to client 3 by writing "Hello@client 3". This will than print out a message on 
client 2 console. This than means that one client have send a message to that other client. If I go into client 2, I
can see that this client did not get the message.

To test the mutual exclusion I have made a test inside the Singleton class. Inside the synchronized there is a try and
catch, which will make the thread sleep for 10 seconds before it than will wake up again. After the 10 seconds are over
the logfile will be updated with the message. If there is two clients write at the same time, one of the clients will
have to wait the 10 seconds for the first one, and then again 10 seconds for the last one. What this test shows is that
there is mutual exclusion where only one thread is allowed to type into the logfile at a time. 
To test this yourself you will have to open up the Singleton class and remove the comments in the code. When this is done
You will use the program as you normally would. If you go and check the logfile after the first thread is done, you can
see that only the first message was printed to the logfile.
