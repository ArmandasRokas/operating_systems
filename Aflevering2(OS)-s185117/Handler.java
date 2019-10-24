//This code is made by Christoffer Detlef s185117.
//The code is inspired by: https://www.geeksforgeeks.org/multi-threaded-chat-application-set-1/
import java.io.*;
import java.net.Socket;
import java.util.StringTokenizer;

class Handler implements Runnable {
    private String name;
    final DataInputStream dis;
    final DataOutputStream dos;
    private Socket socket;
    boolean isloggedin;
    private Singleton writeFile = new Singleton();

    public Handler(Socket socket, String name, DataOutputStream dos, DataInputStream dis) {
        this.dis = dis;
        this.dos = dos;
        this.name = name;
        this.socket = socket;
        this.isloggedin=true;
    }


    @Override
    public void run() {
        String received;
        while (true) {
            try {
                //Here it reads the message that the user have send, and will then print it out in the server console.
                received = dis.readUTF();
                System.out.println(received);

                //Here the code will split up the message. It does that so it can see what client it have to send it to.
                StringTokenizer st = new StringTokenizer(received, "@");
                String MsgToSend = st.nextToken();
                String recipient = st.nextToken();

                //This checks if the client that the other client is trying to type to exists.
                //If it exists will will write the msg, to it.
                for (Handler mc : Server.al) {
                    if (mc.name.equals(recipient) && mc.isloggedin == true) {
                        mc.dos.writeUTF(this.name+" says : "+MsgToSend);
                        mc.writeFile.getInstance().FileWriter(this.name+" says : "+ "'"+MsgToSend+"'" + " to : " + recipient);
                        break;
                    }
                }
            } catch (Exception e) {
                e.getStackTrace();
            }
        }
    }
}