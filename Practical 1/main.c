#include <stdio.h>
#include <string.h>

int main(){
	char bitstream[1000];
	char voltage[1000];
	int mode, mech, bitC, voltC, state;
	
	FILE *encodeIn = NULL;
	FILE *encodeOut = NULL;
	FILE *decodeIn = NULL;
	FILE *decodeOut = NULL;

	encodeIn = (fopen("Encode Input Output/encodeInput.txt", "r"));
	decodeIn = (fopen("Decode Input Output/decodeInput.txt", "r"));
	encodeOut = (fopen("Encode Input Output/encodeOutput.txt", "w"));
	decodeOut = (fopen("Decode Input Output/decodeOutput.txt", "w"));
	
	printf("\nChoose what to do\n\t1. Encode\n\t2. Decode\n");
	scanf("%d", &mode);

	printf("\nChoose encoding mechanism\n\t1. NRZ-L\n\t2. NRZ-I\n");
        scanf("%d", &mech);
	


	if(mode==1){
		memset(bitstream, 0, 1000*sizeof(bitstream[0]));
		memset(voltage, 0, 1000*sizeof(voltage[0]));
		fgets(bitstream, 1000, encodeIn);
		bitC = 0;
		voltC = 0;
		state = 1;
		if(mech==1){
			while(bitstream[bitC]){
				if(bitstream[bitC] == '1'){
					strcat(voltage, "-5V ");
				}
				else if(bitstream[bitC] == '0'){
					strcat(voltage, "+5V ");
				}
				else{
					printf("Invalid bitstream");
					return 0;
				}
				bitC++;
			}
			fprintf(encodeOut, "%s", voltage);
		}
		else if(mech==2){
			while(bitstream[bitC]){
                                if(bitstream[bitC] == '1'){
                                        state = (state+1)%2;
					if(state)
						strcat(voltage, "-5V ");
					else
						strcat(voltage, "+5V ");
                                }
                                else if(bitstream[bitC] == '0'){
                                        if(state)
                                                strcat(voltage, "-5V ");
                                        else
                                                strcat(voltage, "+5V ");
                                }
                                else{
                                        printf("Invalid bitstream");
                                        return 0;
                                }
				bitC++;
                        }
			fprintf(encodeOut, "%s", voltage);
		}
		else{
			printf("Invalid command");
			return 0;
		}
	}

	else if(mode==2){
		memset(bitstream, 0, 1000*sizeof(bitstream[0]));
                memset(voltage, 0, 1000*sizeof(voltage[0]));
                bitC = 0;
                voltC = 0;
                state = 1;
		if(mech==1){
			while(fscanf(decodeIn, "%s", voltage) != EOF){
				if(!strcmp(voltage, "+5V")){
					strcat(bitstream, "0");
				}
				else if(!strcmp(voltage, "-5V")){
					strcat(bitstream, "1");
				}
				else{
					printf("Invalid input");
					return 0;
				}
			}
			fprintf(decodeOut, "%s", bitstream);
                }
                else if(mech==2){
			while(fscanf(decodeIn, "%s", voltage) != EOF){
                                if(!strcmp(voltage, "+5V")){
                                        if(state){
						strcat(bitstream, "1");
						state = (state+1) % 2;
					}
					else
						strcat(bitstream, "0");
				}
                                else if(!strcmp(voltage, "-5V")){
                                        if(state)
						strcat(bitstream, "0");
					else{
						strcat(bitstream, "1");
						state = (state+1) % 2;
					}
                                }
                                else{
                                        printf("Invalid input");
                                        return 0;
                                }
                        }
			fprintf(decodeOut, "%s", bitstream);
                }
                else{
			printf("Invalid command");
                        return 0;

                }
	}

	else{
		printf("Invalid command");
                return 0;
	}
	return 0;
}
