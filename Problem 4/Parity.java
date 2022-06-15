import java.util.Random;
import java.lang.Math;

public class Parity {
    private String bitstream, encodedStream, decodedStream;
    private boolean isValid;

    private void invalidInput(){
        System.out.println("Invalid input\n");
        encodedStream = "Invalid input";
        isValid = false;
    }

    private void errorDetected(){
        System.out.println("Error detected\n");
        decodedStream = "Error detected";
        isValid = false;
    }

    public Parity() {
        this.bitstream = new String();
        this.encodedStream = new String();
        this.decodedStream = new String();
    }
    public Parity(String bitstream) {
        this.bitstream = bitstream;
        this.encodedStream = new String("");
        this.decodedStream = new String("");
    }

    public void clear(){
        this.bitstream = "";
        this.encodedStream = "";
        this.decodedStream = "";
    }

    public String getBitstream() {
        return bitstream;
    }
    public String getEncodedStream() {
        return encodedStream;
    }
    public String getDecodedStream() {
        return decodedStream;
    }

    public void setBitstream(String bitstream) {
        this.bitstream = bitstream;
    }
    public void setEncodedStream(String encodedStream) {
        this.encodedStream = encodedStream;
    }

    public void setDecodedStream(String decodedStream) {
        this.decodedStream = decodedStream;
    }

    public boolean isValid() {
        isValid = true;
        generate();
        return isValid;
    }

    public String generate(){
        int oneCount = 0, lastRowOneCount = 0;
        int[] lastRow = new int[7];

        for(int i = 0; i < 7; i++){
            lastRow[i] = 0;
        }

        if(bitstream.length()%7 != 0){
            invalidInput();
            return encodedStream;
        }

        for (int i = 0; i < bitstream.length(); i++){
            if(bitstream.charAt(i) == '1'){
                oneCount++;
                lastRow[i%7] = (lastRow[i%7]+1) % 2;
                encodedStream = encodedStream.concat("1");
            }
            else if(bitstream.charAt(i) == '0'){
                encodedStream = encodedStream.concat("0");
            }
            else{
                invalidInput();
                return encodedStream;
            }

            if((i+1)%7 == 0 && i != 0){
                if(oneCount%2 != 0)
                    encodedStream = encodedStream.concat("1");
                else encodedStream = encodedStream.concat("0");
                oneCount=0;
            }
        }

        for(int i = 0; i < 7; i++){
            if(lastRow[i] == 1){
                lastRowOneCount++;
                encodedStream = encodedStream.concat("1");
            }
            else{
                encodedStream = encodedStream.concat("0");
            }
        }

        if(lastRowOneCount%2 != 0) encodedStream = encodedStream.concat("1");
        else encodedStream = encodedStream.concat("0");

        return encodedStream;
    }

    public String check(){
        int rowOneCount = 0;
        int[] columnOneCount = new int[8];

        for(int i = 0; i < 8; i++){
            columnOneCount[i] = 0;
        }

        if(bitstream.length()%8 != 0){
            errorDetected();
            return decodedStream;
        }

        for (int i = 0; i < bitstream.length(); i++){
            if(bitstream.charAt(i) == '1'){
                rowOneCount++;
                columnOneCount[i%8] = (columnOneCount[i%8]+1) % 2;
            }
            else if(bitstream.charAt(i) != '0'){
                errorDetected();
                return decodedStream;
            }

            if((i+1)%8 == 0 && i != 0){
                if(rowOneCount%2 != 0) {
                    errorDetected();
                    return decodedStream;
                }
                rowOneCount=0;
            }
            else if(i < bitstream.length()-8){
                if(bitstream.charAt(i) == '1') decodedStream = decodedStream.concat("1");
                else decodedStream = decodedStream.concat("0");
            }
        }

        for(int i = 0; i < 8; i++){
            if(columnOneCount[i] == 1){
                errorDetected();
                return decodedStream;
            }
        }

        return decodedStream;
    }

    public void corrupt(){
        Random rand = new Random();
        int numberOfChanges = Math.abs(rand.nextInt() % encodedStream.length());
        char[] encodedStreamCharacters = encodedStream.toCharArray();
        int currentIndex;
        for(int i = 0; i < numberOfChanges; i++){
            currentIndex = Math.abs(rand.nextInt() % encodedStream.length());
            if(encodedStreamCharacters[currentIndex] == '1') encodedStreamCharacters[currentIndex] = '0';
            else encodedStreamCharacters[currentIndex] = '1';
        }

        encodedStream = new String(encodedStreamCharacters);
    }

    public void printEncoded(){
        for (int i = 0; i < encodedStream.length(); i++) {
            System.out.print(encodedStream.charAt(i));
            if((i+1)%8 == 0 && i != 0){
                System.out.println("");
            }
        }
    }
    public void printDecoded(){
        for (int i = 0; i < encodedStream.length(); i++) {
            System.out.print(encodedStream.charAt(i));
            if((i+1)%7 == 0 && i != 0){
                System.out.println("");
            }
        }
    }


}
