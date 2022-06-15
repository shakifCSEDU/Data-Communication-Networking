import java.net.*;
import java.io.*;
class Client{
    public static void main(String args[])throws Exception{
        Socket s=new Socket("localhost",3333);
        DataOutputStream dout=new DataOutputStream(s.getOutputStream());
        BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
        Parity parityUser = new Parity();
        HDB3LineCoder hdb3User = new HDB3LineCoder();
        int isCorrupt, state;

        String input="", output="";
        while(!input.equals("stop")){
            System.out.println("Please enter the bitstream:");
            input=br.readLine();
            if(!input.equals("stop")) {
                hdb3User.clear();
                parityUser.clear();
                System.out.println("Choose initial voltage\n\t1. +5V\n\t2. -5V");
                hdb3User.setState(2 - Integer.parseInt(br.readLine()));
                System.out.println("Would you like to corrupt the data?\n\t1. Yes\n\t2. No");
                isCorrupt = Integer.parseInt(br.readLine());
                parityUser.setBitstream(input);
                parityUser.generate();
                System.out.println("The bitstream with parity bits added is");
                parityUser.printEncoded();

                hdb3User.setInput(parityUser.getEncodedStream());
                output = hdb3User.encode();

                System.out.println("Sending the voltage sequence:\n" + output);

                if(isCorrupt == 1){
                    parityUser.corrupt();
                    state = hdb3User.getState();
                    hdb3User.clear();
                    hdb3User.setState(state);
                    hdb3User.setInput(parityUser.getEncodedStream());
                    output = hdb3User.encode();
                }
            }
            else output = input;
            dout.writeUTF(output);
            dout.flush();
        }

        dout.close();
        s.close();
    }
}