import java.net.*;
import java.io.*;
class Server{
    public static void main(String args[])throws Exception{
        ServerSocket ss = new ServerSocket(3333);
        Socket s = ss.accept();
        DataInputStream din=new DataInputStream(s.getInputStream());
        BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
        Parity parityUser = new Parity();
        HDB3LineCoder hdb3User = new HDB3LineCoder();

        String input="";
        while(!input.equals("stop")){
            input=din.readUTF();
            if(!input.equals("stop")) {
                hdb3User.clear();
                parityUser.clear();
                System.out.println("The voltage sequence received:\n" + input);
                System.out.println("Choose initial voltage\n\t1. +5V\n\t2. -5V");
                hdb3User.setState(2 - Integer.parseInt(br.readLine()));
                hdb3User.setInput(input);
                hdb3User.decode();
                System.out.println("The bitstream along with parity bits:\n" + hdb3User.getBitstream());
                parityUser.setBitstream(hdb3User.getBitstream());
                System.out.println("The original bitstream: " + parityUser.check());
            }
        }
        din.close();
        s.close();
        ss.close();
    }
}