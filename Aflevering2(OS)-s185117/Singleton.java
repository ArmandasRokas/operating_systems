//This code is made by Christoffer Detlef s185117
//To make the code work as it should do, I have gotten help from other class mates.
import java.io.*;

//To make sure that only one person can access the logfile can write to it, I have made it a Singleton.
//This makes only one instance of it.
class Singleton{

    private static final Singleton inst = new Singleton();
        //To make sure that only one thread have access to the logfile, and can write into it, I have made use of synchronized.
        //What it does, is it makes sure that only one thread can have access to file.
        
        synchronized void FileWriter(String str) throws IOException {
            //void FileWriter(String str) throws IOException {

            //Test for mutual exclusion
            //What this test does is that it takes the thread to sleep for 10 seconds. After the 10 seconds it will take the user input and put it into the logfile.
            //However both user can type at the same time, but it will only take one client at the time, and the other one will have to wait.
            System.out.println("The thread is sleeping for 10 seconds, it will come inside the logfile when it wakes up.");
            try {
                Thread.sleep(20000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("Done sleeping");
               
    
            // If the file doesn't exists, create and write to it
            try (FileWriter writer = new FileWriter("logfile", true); //To make sure that the file dosnt get overwritten all the time, I have append true.
                 BufferedWriter bw = new BufferedWriter(writer)) {
                bw.write(str + "\n");
            } catch (IOException e) {
                System.err.format("IOException: %s%n", e);
            }
        }
    Singleton getInstance(){
        return inst;
    }
}
