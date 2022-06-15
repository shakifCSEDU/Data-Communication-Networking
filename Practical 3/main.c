#include <stdio.h>
#include <string.h>

char bitstream[1000], voltage[1000], voltageA[1000], voltageB[1000];
int bitC, voltC, state, stateInitial, levelInitial, nonZeroInitial;
FILE *encodeIn;
FILE *encodeOut;
FILE *decodeIn;
FILE *decodeOut;

void readData(){												//create and read data from the files
	encodeIn = (fopen("Encode Input Output/encodeInput.txt", "r"));
	decodeIn = (fopen("Decode Input Output/decodeInput.txt", "r"));
	encodeOut = (fopen("Encode Input Output/encodeOutput.txt", "w"));
	decodeOut = (fopen("Decode Input Output/decodeOutput.txt", "w"));
}

void updateData(){												//close the files and write data
	fclose(encodeIn);
	fclose(decodeIn);
	fclose(encodeOut);
	fclose(decodeOut);
}

void initializeEncoder(){										//clear all values and set the previous voltage
	memset(bitstream, 0, sizeof(bitstream));
	memset(voltage, 0, sizeof(voltage));
	fgets(bitstream, 1000, encodeIn);
	bitC = 0;
	voltC = 0;
	state = stateInitial-1;
}

void initializeDecoder(){										//clear and set all values
	memset(bitstream, 0, sizeof(bitstream));
	memset(voltage, 0, sizeof(voltage));
	memset(voltage, 0, sizeof(voltageA));
	memset(voltage, 0, sizeof(voltageB));
	bitC = 0;
	voltC = 0;
	state = stateInitial-1;
}

void twoB1QEncode(){
	int prevLevel = state;
	if(strlen(bitstream) % 2){
		printf("Invalid bitstream\n");
		return;
	}

	while(bitstream[bitC]){
		if(prevLevel == 0){
			if(bitstream[bitC] == '0' && bitstream[bitC+1] == '0')
				strcat(voltage, "+1V ");
			if(bitstream[bitC] == '0' && bitstream[bitC+1] == '1')
				strcat(voltage, "+3V ");
			if(bitstream[bitC] == '1' && bitstream[bitC+1] == '0'){
				strcat(voltage, "-1V ");
				prevLevel = 1;
			}
			if(bitstream[bitC] == '1' && bitstream[bitC+1] == '1'){
				strcat(voltage, "-3V ");
				prevLevel = 1;
			}
		}
		if(prevLevel == 1){
			if(bitstream[bitC] == '0' && bitstream[bitC+1] == '0')
				strcat(voltage, "-1V ");
			if(bitstream[bitC] == '0' && bitstream[bitC+1] == '1')
				strcat(voltage, "-3V ");
			if(bitstream[bitC] == '1' && bitstream[bitC+1] == '0'){
				strcat(voltage, "+1V ");
				prevLevel = 0;
			}
			if(bitstream[bitC] == '1' && bitstream[bitC+1] == '1'){
				strcat(voltage, "+3V ");
				prevLevel = 0;
			}
		}
		bitC+=2;
	}
	fprintf(encodeOut, "%s", voltage);
}

void twoB1QDecode(){
	int prevSign;
	if(!state) prevSign = '+';
	else prevSign = '-';
	while(fscanf(decodeIn, "%s", voltage) != EOF){
		if(voltage[0] == prevSign){
			if(voltage[1] == '3')
				strcat(bitstream, "01");
			else if(voltage[1] == '1')
				strcat(bitstream, "00");
		}
		if(voltage[0] != prevSign){
			if(voltage[1] == '3')
				strcat(bitstream, "1	1");
			else if(voltage[1] == '1')
				strcat(bitstream, "10");
		}

		prevSign = voltage[0];
	}
	fprintf(decodeOut, "%s", bitstream);
}

void MLT3Encode(){
	int prevLevel, prevNonZero = 2 - nonZeroInitial;					//0 means positive
	if(levelInitial == 1) prevLevel = 1;
	if(levelInitial == 2) prevLevel = -1;
	if(levelInitial == 3) prevLevel = 0;
	while(bitstream[bitC]){
		switch(bitstream[bitC])
		{
			case '1':
				if(prevLevel==0){
					sprintf(voltage+strlen(voltage), "%c5V ", (prevNonZero==0)?'+':'-');
					prevNonZero = (prevNonZero+1) % 2;
					prevLevel = prevNonZero==0?-1:1;
				}
				else{
					strcat(voltage, "0V ");
					prevLevel = 0;
				}
				break; 
			case '0':
				if(prevLevel==0)
					strcat(voltage, "0V ");
				else sprintf(voltage+strlen(voltage), "%c5V ", (prevLevel>0)?'+':'-');
				break;
			default:
				printf("Invalid bitstream\n");
				return;
				break;
		}
		bitC++;
	}
	fprintf(encodeOut, "%s", voltage);
}

void MLT3Decode(){
	int prevLevel, prevNonZero = 2 - nonZeroInitial;
	if(levelInitial == 1) prevLevel = 1;
	if(levelInitial == 2) prevLevel = -1;
	if(levelInitial == 3) prevLevel = 0;
	while(fscanf(decodeIn, "%s", voltage) != EOF){
		if(!strcmp(voltage, "0V")){
			if(prevLevel == 0)
				strcat(bitstream, "0");
			if(prevLevel != 0)
				strcat(bitstream, "1");
			prevLevel = 0;
		}
		else if(!strcmp(voltage, "+5V")){
			if(prevLevel == 1)
				strcat(bitstream, "0");
			else strcat(bitstream, "1");
			prevLevel = 1;
			prevNonZero = 1;
		}
		else if(!strcmp(voltage, "-5V")){
			if(prevLevel == -1)
				strcat(bitstream, "0");
			else strcat(bitstream, "1");
			prevLevel = -1;
			prevNonZero = 0;
		}
		else{
			printf("Invalid input\n");
			return;
		}
	}
	fprintf(decodeOut, "%s", bitstream);
}

void AMIEncode(char bits[]){
	bitC = 0;	
	while(bits[bitC]){
		switch(bits[bitC])
		{
			case '1':
			state = (state+1)%2;
			if(state)
				strcat(voltage, "-5V ");
			else
				strcat(voltage, "+5V ");
			break;
			case 'B':
			state = (state+1)%2;
			if(state)
				strcat(voltage, "-5V ");
			else
				strcat(voltage, "+5V ");
			break;
			case 'V':
			if(state)
				strcat(voltage, "-5V ");
			else
				strcat(voltage, "+5V ");
			break;
			case '0':
			strcat(voltage, "0V ");
			break;
			default:
			printf("Invalid bitstream\n");
			return;
			break;
		}
		bitC++;
	}
	fprintf(encodeOut, "%s", voltage);
}

void B8ZSEncode(){
	int zeroCount, initial;
	char replace[9] = "000VB0VB";
	while(bitstream[bitC]){
		initial = bitC;
		zeroCount = 1;
		while(bitstream[bitC] == '0' && zeroCount != 8 ){
			zeroCount++;
			bitC++;
		}
		if(zeroCount==8){
			for(int i = bitC-zeroCount+1; i <= bitC; i++)
				bitstream[i] = replace[i+zeroCount-bitC-1];
		}
		else{
			bitC = initial;
		}
		if(bitstream[bitC])
			bitC++;
	}
	AMIEncode(bitstream);
}

void B8ZSDecode(){
	char currentSign;
	int vCount = 0, length;
	if(stateInitial==1) currentSign='-';
	else currentSign='+';

	while(fscanf(decodeIn, "%s", voltage) != EOF){
		if(vCount){
			strcat(bitstream, "0");
			vCount--;
			continue;
		}
		if(!strcmp(voltage, "0V"))
			strcat(bitstream, "0");

		else if(!strcmp(voltage, "-5V") || !strcmp(voltage, "+5V")){
			if(voltage[0] != currentSign){
				vCount = 4;
				length = strlen(bitstream);
				for(int i=0; i<3; i++){
					bitstream[length-i] = '0';
				}
			}
			else{
				if(currentSign=='-')
					currentSign = '+';
				else currentSign = '-';

				strcat(bitstream, "1");
			}
		}
		else{
			printf("Invalid input");
			return;
		}
	}
	fprintf(decodeOut, "%s", bitstream);
}

void HDB3Encode(){
	int zeroCount, oneCount = 0, initial;
	char replaceA[5] = "000V";
	char replaceB[5] = "B00V";
	while(bitstream[bitC]){
		initial = bitC;
		zeroCount = 1;
		if(bitstream[bitC] == '1')
			oneCount++;
		while(bitstream[bitC] == '0' && zeroCount != 4){
			zeroCount++;
			bitC++;
		}
		if(zeroCount==4){
			for(int i = bitC-zeroCount+1; i <= bitC; i++){
				if(oneCount%2)
					bitstream[i] = replaceA[i+zeroCount-bitC-1];
				else bitstream[i] = replaceB[i+zeroCount-bitC-1];
			}
			oneCount = 0;
		}
		else{
			bitC = initial;
		}
		if(bitstream[bitC])
			bitC++;
	}
	AMIEncode(bitstream);
}

void HDB3Decode(){
	char currentSign;
	int length;
	if(stateInitial==1) currentSign='-';
	else currentSign='+';

	while(fscanf(decodeIn, "%s", voltage) != EOF){
		if(!strcmp(voltage, "0V"))
			strcat(bitstream, "0");

		else if(!strcmp(voltage, "-5V") || !strcmp(voltage, "+5V")){
			if(voltage[0] != currentSign){
				length = strlen(bitstream);
				for(int i=0; i<4; i++){
					bitstream[length-i] = '0';
				}
			}
			else{
				if(currentSign=='-')
					currentSign = '+';
				else currentSign = '-';

				strcat(bitstream, "1");
			}
		}
		else{
			printf("Invalid input");
			return;
		}
	}
	printf("%s\n", bitstream);
	fprintf(decodeOut, "%s", bitstream);
}





int main(){
	int mode, mech;
	while(1){
		printf("\nEncoding and Decoding [0 to exit]\n\t1. Encode\n\t2. Decode\n");
		scanf("%d", &mode);
		if(mode == 0) break;
		if(mode < 1 || mode > 2){
			printf("Invalid command");
			continue;
		}

		printf("\nChoose encoding mechanism\n\t1. 2B1Q\n\t2. MLT-3\n\t3. B8ZS\n\t4. HDB3\n");
		scanf("%d", &mech);

		if(mech < 1 || mech > 4){
			printf("Invalid command");
			continue;
		}

		if(mech != 2){
			printf("\nChoose inital voltage\n\t1. +5V\n\t2. -5V\n");
			scanf("%d", &stateInitial);
			if(stateInitial < 1 || stateInitial > 2){
				printf("Invalid command");
				continue;
			}
		}
		else{
			printf("\nChoose inital voltage\n\t1. +5V\n\t2. -5V\n\t3. 0V\n");
			scanf("%d", &levelInitial);
			if(levelInitial < 1 || levelInitial > 3){
				printf("Invalid command");
				continue;
			}
			printf("\nChoose last non-zero voltage\n\t1. +5V\n\t2. -5V\n");
			scanf("%d", &nonZeroInitial);
			if(nonZeroInitial < 1 || nonZeroInitial > 2){
				printf("Invalid command");
				continue;
			}
		}

		

		readData();
		if(mode==1){
			initializeEncoder();
			switch(mech)
			{
				case 1:
				twoB1QEncode();
				break;
				case 2:
				MLT3Encode();
				break;
				case 3:
				B8ZSEncode();
				break;
				case 4:
				HDB3Encode();
				break;
				default:
				printf("Invalid command");
				break;
			}
		}
		else if(mode==2){
			readData();
			initializeDecoder();
			switch(mech)
			{
				case 1:
				twoB1QDecode();
				break;
				case 2:
				MLT3Decode();
				break;
				case 3:
				B8ZSDecode();
				break;
				case 4:
				HDB3Decode();
				break;
				default:
				printf("Invalid command");
				break;
			}
		}

		updateData();
	}
	return 0;
}
