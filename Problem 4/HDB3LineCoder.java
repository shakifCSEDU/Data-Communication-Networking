public class HDB3LineCoder {
    private String bitstream, voltage, input;
    private int state;
    private boolean isValid;

    HDB3LineCoder(){
        this.bitstream = new String();
        this.voltage = new String();
        this.input = new String();
        this.state = 0;
        this.isValid = true;
    }

    public void clear(){
        this.bitstream = "";
        this.voltage = "";
        this.input = "";
        this.state = 0;
        this.isValid = true;
    }

    private void printNonZero(){
        if(state == 1) voltage = voltage.concat("-5V ");
        else voltage = voltage.concat("+5V ");
    }
    private void printZero(){
        voltage = voltage.concat("0V ");
    }
    private void invalidInput(){
        System.out.println("Invalid input");
        voltage = "Invalid input";
        isValid = false;
    }
    private void errorDetected(){
        System.out.println("Error detected");
        bitstream = "Error detected";
        isValid = false;
    }

    private void AMIEncode(char[] bitCharacters){
        int bitC = 0;

        for(bitC = 0; bitC < bitCharacters.length; bitC++){
            if(bitCharacters[bitC] == '1' || bitCharacters[bitC] == 'B'){
                state = (state+1)%2;
                printNonZero();
            }
            else if(bitCharacters[bitC] == 'V') printNonZero();
            else if(bitCharacters[bitC] == '0') printZero();
            else invalidInput();
        }
    }

    public void setBitstream(String bitstream) {
        this.bitstream = bitstream;
    }
    public void setVoltage(String voltage) {
        this.voltage = voltage;
    }
    public void setInput(String input) {
        this.input = input;
    }
    public void setState(int state) {
        this.state = state;
    }
    public void setValid(boolean valid) {
        isValid = valid;
    }

    public String getBitstream() {
        return bitstream;
    }
    public String getVoltage() {
        return voltage;
    }
    public String getInput(){
        return input;
    }
    public int getState() {
        return state;
    }
    public boolean isValid() {
        return isValid;
    }

    public String encode(){
        int zeroCount, nonZeroCount = 0, initial, bitC;
        char[] replace0 = "000V".toCharArray();
        char[] replaceB = "B00V".toCharArray();
        char[] bitsCharacter = input.toCharArray();

        for(bitC = 0; bitC < bitsCharacter.length; bitC++){
            initial = bitC;
            zeroCount = 0;

            if(bitsCharacter[bitC] == '1') nonZeroCount++;

            while(bitC < bitsCharacter.length && bitsCharacter[bitC] == '0' && zeroCount != 4){
                zeroCount++;
                bitC++;
            }

            if(zeroCount == 4){
                bitC--;
                for (int i = bitC-zeroCount+1; i <= bitC; i++){
                    if(nonZeroCount %2 != 0)
                        bitsCharacter[i] = replace0[i+zeroCount-bitC-1];
                    else
                        bitsCharacter[i] = replaceB[i+zeroCount-bitC-1];
                }
                if(nonZeroCount %2 != 0)
                    nonZeroCount++;
                else
                    nonZeroCount+=2;
            }
            else bitC = initial;
        }

        AMIEncode(bitsCharacter);

        return voltage;
    }

    public String decode(){
        char currentSign;
        int nonZeroCount, zeroCount;

        if(state==1) currentSign = '+';
        else currentSign = '-';

        String[] voltages = input.trim().split("\\s+");

        for(int i = 0; i < voltages.length; i++){
            if(voltages[i].equalsIgnoreCase("0V")) bitstream = bitstream.concat("0");

            else if(voltages[i].equalsIgnoreCase("-5V") || voltages[i].equalsIgnoreCase("+5V")){
                if(voltages[i].toCharArray()[0] != currentSign) bitstream = bitstream.concat("V");
                else{
                    if(currentSign == '-') currentSign = '+';
                    else currentSign = '-';

                    bitstream = bitstream.concat("B");
                }
            }
            else{
                errorDetected();
                return bitstream;
            }
        }


        nonZeroCount = 1;
        char[] bitCharacters = bitstream.toCharArray();


        for(int i = 0; i < bitCharacters.length; i++){
            zeroCount = 0;
            while(i < bitCharacters.length && bitCharacters[i] == '0'){
                zeroCount++;
                i++;

                if(zeroCount == 4){
                    errorDetected();
                    return "a";
                }
            }
            if(bitCharacters[i] == 'B') nonZeroCount++;
            if(bitCharacters[i] == 'V'){
                nonZeroCount++;
                if(i-3 < 0){
                    errorDetected();
                    return "b";
                }
                if(bitCharacters[i-3] == 'B') nonZeroCount--;

                if(zeroCount != 2 && zeroCount != 3){
                    errorDetected();
                    return "c";
                }

                if(nonZeroCount%2 == 0 && zeroCount != 2){
                    errorDetected();
                    return "d";
                }

                if(nonZeroCount%2 != 0 && zeroCount != 3){
                    errorDetected();
                    return "e";
                }

                if(nonZeroCount%2 == 0 && zeroCount == 2 && bitCharacters[i-3]!='B'){
                    errorDetected();
                    return "f";
                }
                if(bitCharacters[i-3] == 'B') nonZeroCount++;
            }
        }

        for(int i = 0; i < bitCharacters.length; i++){
            if(bitCharacters[i] == 'B')
                bitCharacters[i] = '1';
            if(bitCharacters[i] == 'V'){
                bitCharacters[i] = '0';
                bitCharacters[i-3] = '0';
            }
        }


        bitstream = new String(bitCharacters);

        return bitstream;
    }

}
