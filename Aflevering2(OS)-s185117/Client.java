//This code is made by Christoffer Detlef s185117.
//The code is inspired by: https://www.geeksforgeeks.org/multi-threaded-chat-application-set-2/
import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Client {
    public static void toServer(String ip, int port) throws UnknownHostException, IOException {

        //A normal scanner for the user input
        Scanner scan = new Scanner(System.in);

        //Here the socket is taking the IP and port
        Socket socket = new Socket(ip, port);

        //Getting the input and output stream for when the clients wants to send and receive messages
        DataInputStream dis = new DataInputStream(socket.getInputStream());
        DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
        //This is for one the client wants to see whats inside the logfile.
        File file = new File("logfile");
        //This makes sure that the logfile will be made.
        if (file.createNewFile()) {
            System.out.println("File is created, so go ahead and write to other clients");
        } else {
            System.out.println("Logfile exist, so go ahead and write to other clients");
        }
        FileInputStream fis = new FileInputStream(file);

        //This is the thread for when you want to send a message.
        Thread sendMessage = new Thread(() -> {
            while (true) {
                //It checks what the user have put in.
                String msg = scan.nextLine();
                //If the client wants to leave, the user just have to type "bye" and it will leave.
                if (msg.equals("bye")) {
                    try {
                        socket.close();
                        System.exit(0);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                //Here it checks if the input is logfile, if it is, it will print out the content of the logfile to that client
                if (msg.equals("logfile")) {
                    int oneByte = 0;
                    while (true) {
                        try {
                            if ((oneByte = fis.read()) == -1) break;
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        System.out.write(oneByte);
                    }
                }
                try {
                    //Here it will take the input that
                    dos.writeUTF(msg);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        //This thread is for when you want to read a message
        Thread readMessage = new Thread(() -> {
            while (true) {
                try {
                    //It sets a string to the input that comes from the input stream. And then it will print it in the console.
                    String msg = dis.readUTF();
                    System.out.println(msg);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        //This two starts the two threads. This means that if these does not exist the client will just stop.
        sendMessage.start();
        readMessage.start();
    }

    //This is used to check what IP and port it should connect to. This is used for the Socket.
    public static void main(String[] args) throws UnknownHostException, IOException{
        Client.toServer("localhost", 3520);
    }
}