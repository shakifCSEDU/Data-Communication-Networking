#include <stdio.h>
#include <string.h>

char bitstream[1000], voltage[1000], voltageA[1000], voltageB[1000];
int bitC, voltC, state, stateInitial;
FILE *encodeIn;
FILE *encodeOut;
FILE *decodeIn;
FILE *decodeOut;

void readData(){
	encodeIn = (fopen("Encode Input Output/encodeInput.txt", "r"));
	decodeIn = (fopen("Decode Input Output/decodeInput.txt", "r"));
	encodeOut = (fopen("Encode Input Output/encodeOutput.txt", "w"));
	decodeOut = (fopen("Decode Input Output/decodeOutput.txt", "w"));
}

void updateData(){
	fclose(encodeIn);
	fclose(decodeIn);
	fclose(encodeOut);
	fclose(decodeOut);
}

void initializeEncoder(){
	memset(bitstream, 0, sizeof(bitstream));
	memset(voltage, 0, sizeof(voltage));
	fgets(bitstream, 1000, encodeIn);
	bitC = 0;
	voltC = 0;
	state = stateInitial-1;
}

void initializeDecoder(){
	memset(bitstream, 0, sizeof(bitstream));
	memset(voltage, 0, sizeof(voltageA));
	memset(voltage, 0, sizeof(voltageB));
	bitC = 0;
	voltC = 0;
	state = stateInitial-1;
}

void ManchesterEncode(){
	while(bitstream[bitC]){
		switch(bitstream[bitC])
		{
			case '1':
			strcat(voltage, "-5V +5V   ");
			break;
			case '0':
			strcat(voltage, "+5V -5V   ");
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

void ManchesterDecode(){
	while(fscanf(decodeIn, "%s %s", voltageA, voltageB) != EOF){
		if(!strcmp(voltageA, "+5V") && !strcmp(voltageB, "-5V"))
			strcat(bitstream, "0");

		else if(!strcmp(voltageA, "-5V") && !strcmp(voltageB, "+5V"))
			strcat(bitstream, "1");

		else{
			printf("Invalid input");
			return;
		}
	}
	fprintf(decodeOut, "%s", bitstream);
	fclose(decodeOut);
}

void DiffManchesterEncode(){
	while(bitstream[bitC]){
		switch(bitstream[bitC])
		{
			case '1':
			state = (state+1)%2;
			if(state)
				strcat(voltage, "-5V +5V   ");
			else
				strcat(voltage, "+5V -5V   ");
			break; 
			case '0':
			if(state)
				strcat(voltage, "-5V +5V   ");
			else
				strcat(voltage, "+5V -5V   ");
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

void DiffManchesterDecode(){
	while(fscanf(decodeIn, "%s %s", voltageA, voltageB) != EOF){
		if(!strcmp(voltageA, "+5V") && !strcmp(voltageB, "-5V")){
			if(state){
				strcat(bitstream, "1");
				state = (state+1) % 2;
			}
			else
				strcat(bitstream, "0");
		}
		else if(!strcmp(voltageA, "-5V") && !strcmp(voltageB, "+5V")){
			if(state)
				strcat(bitstream, "0");
			else{
				strcat(bitstream, "1");
				state = (state+1) % 2;
			}
		}
		else{
			printf("Invalid input");
			return;
		}
	}
	fprintf(decodeOut, "%s", bitstream);
}

void AMIEncode(){
	while(bitstream[bitC]){
		switch(bitstream[bitC])
		{
			case '1':
			state = (state+1)%2;
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

void AMIDecode(){
	char currentSign;
	if(stateInitial==1) currentSign='-';
	else currentSign='+';

	while(fscanf(decodeIn, "%s", voltage) != EOF){
		if(!strcmp(voltage, "0V"))
			strcat(bitstream, "0");

		else if(!strcmp(voltage, "-5V") || !strcmp(voltage, "+5V")){
			if(voltage[0] != currentSign){
				printf("Invalid input");
				return;
			}

			if(currentSign=='-')
				currentSign = '+';
			else currentSign = '-';

			strcat(bitstream, "1");
		}

		else{
			printf("Invalid input");
			return;
		}
	}
	fprintf(decodeOut, "%s", bitstream);
}

int main(){
	int mode, mech;
	while(1){
		printf("\nEncoding and Decoding in Biphase(Manchester and Differential Manchester) and AMI [0 to exit]\n\t1. Encode\n\t2. Decode\n");
		scanf("%d", &mode);
		if(mode == 0) break;
		if(mode < 1 || mode > 2){
			printf("Invalid command");
			continue;
		}

		printf("\nChoose encoding mechanism\n\t1. Manchester\n\t2. Differential Manchester\n\t3. AMI\n");
		scanf("%d", &mech);

		if(mech < 1 || mech > 3){
			printf("Invalid command");
			continue;
		}

		printf("\nChoose inital voltage\n\t1. +5V\n\t2. -5V\n");
		scanf("%d", &stateInitial);

		if(stateInitial < 1 || stateInitial > 2){
			printf("Invalid command");
			continue;
		}

		readData();
		if(mode==1){
			initializeEncoder();
			switch(mech)
			{
				case 1:
				ManchesterEncode();
				break;
				case 2:
				DiffManchesterEncode();
				break;
				case 3:
				AMIEncode();
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
				ManchesterDecode();
				break;
				case 2:
				DiffManchesterDecode();
				break;
				case 3:
				AMIDecode();
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
