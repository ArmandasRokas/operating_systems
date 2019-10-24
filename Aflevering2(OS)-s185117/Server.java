//This code is made by Christoffer Detlef s185117.
//The code is inspired by: https://www.geeksforgeeks.org/multi-threaded-chat-application-set-1/
import java.io.*;
import java.util.*;
import java.net.*;

public class Server {
    static ArrayList<Handler> al = new ArrayList<>();

    static int i = 1;

    public static void main(String[] args) throws IOException {

        ServerSocket serverSocket = new ServerSocket(3520);
        Socket socket;
        //file name only


        //This loop keeps running to check if there is coming any client request.
        while (true) {
            //This is used to accept the socket request. It does that by checking the addr, localport and port.
            socket = serverSocket.accept();

            System.out.println("Client wants to join with addr, port and localport : " + socket);

            DataInputStream dis = new DataInputStream(socket.getInputStream());
            DataOutputStream dos = new DataOutputStream(socket.getOutputStream());

            System.out.println("Creating a new handler for this client " + i);
            //This is creating a handler object for the request there just got accepted.
            Handler ch = new Handler(socket,"client " + i, dos, dis);
            //This than makes the thread and takes the handler object
            Thread t = new Thread(ch);

            System.out.println("Adding this client to active client list");
            //Here it adds the client to the arraylist. It does that so there always is a list of all the users.
            al.add(ch);
            //This starts the thread
            t.start();
            //Every client gets a new name by first getting "client" and then a new number. The first client starts with 1, and then the next one will get 2.
            i++;
        }
    }
}