#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define S_BOX_INPUT 32
#define ROUND 16
#define P_BOX_INPUT 16
#define NUMBER_OF_ROUNDS 16

//Initilization of the S-Box
//Each line has exacly 16 input lines to XOR
int S_Box[ROUND][S_BOX_INPUT] = {
	{1,1,0,1,0,1,0,1,0,1,0,1,1,0,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0,1,1,1},
	{1,1,1,0,0,0,1,0,0,1,0,1,0,0,1,0,1,1,1,1,1,1,1,0,0,0,0,1,0,1,0,0},
	{1,0,0,1,1,1,0,1,1,0,1,1,1,0,0,0,0,0,1,0,1,1,0,0,0,1,1,1,0,0,0,1},
	{1,1,0,1,1,0,0,1,0,0,0,0,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0},
	{0,1,0,0,1,0,0,0,1,1,1,1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,0,1,0},
	{1,0,0,1,1,0,0,1,0,1,0,1,0,0,0,1,1,1,1,1,1,0,1,0,0,0,0,1,0,1,0,1},
	{0,1,0,1,0,1,1,0,1,0,1,0,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1,0,0,0,1,0},
	{0,1,1,0,0,0,0,0,0,1,0,1,1,1,1,1,1,0,1,0,0,0,1,1,0,1,0,1,0,1,0,1},
	{0,1,1,0,0,1,1,0,1,0,1,1,0,0,0,1,1,1,1,0,0,0,1,0,1,1,0,0,1,0,1,0},
	{0,0,0,1,0,0,1,0,1,1,1,1,0,1,0,0,0,1,1,0,1,1,1,0,1,1,1,0,1,0,0,0},
	{1,1,0,1,0,0,1,0,1,0,1,0,0,0,1,1,1,0,1,1,1,0,1,1,0,1,0,0,0,0,1,0},
	{1,0,0,0,0,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,0,1,1,1,1,1,0,0,0,1,0,1},
	{1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0},
	{1,0,0,0,0,1,1,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1,1,0,1,1,0},
	{0,0,1,0,0,0,1,0,1,0,0,1,1,0,0,0,0,1,0,1,1,1,1,1,0,1,1,0,1,1,0,1},
	{0,1,0,1,1,0,0,0,0,1,1,0,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,0,0,0,1,0}
};

//Iniitilization of the P-Box
//Each bit remapped to a different bit uniquely
//Goes through least significant bit to most significant
int P_Box[P_BOX_INPUT][P_BOX_INPUT] = {
	{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
	{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
	{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
	{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
	{0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
	{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0}
};
//For testing purposes
void printBinary(short int num) {
    int bits = sizeof(num) * 8; // Calculate the number of bits in a short int
    for (int i = bits - 1; i >= 0; i--) {
        short int bit = (num >> i) & 1; // Extract each bit using bitwise AND and right shift
        printf("%d", bit);
    }
    printf("\n");
}

//Function to get the value of a bit of a short
unsigned short int isBitSet(unsigned short int valueToCheck,unsigned short int indexToCheck){
	unsigned short int mask = 1;
	mask = mask<<indexToCheck;
	if((valueToCheck&mask)!=0) return 1;
	return 0;
}

//T is for the input, K is the key, R for the Round
unsigned short int round_function(unsigned short int T, unsigned short int K, unsigned short int R){
	//S-Box part
	unsigned short int s_box_output = 0;
	for(int i=0;i<S_BOX_INPUT/2;i++){
		if(S_Box[R][i]==1){
			s_box_output = s_box_output<<1;
			if(isBitSet(T,S_BOX_INPUT/2-i-1)){
				s_box_output = s_box_output + 1;
			}
		}
	}
	for(int i=S_BOX_INPUT/2;i<S_BOX_INPUT;i++){	
		if(S_Box[R][i]==1){
			s_box_output = s_box_output<<1;
			if(isBitSet(K,S_BOX_INPUT-i-1)){
				s_box_output = s_box_output + 1;
			}
		}
	}
	//P-Box part
	unsigned short int p_box_output = 0;
	for(int i = 0;i<P_BOX_INPUT;i++){
		for(int j=0;j<P_BOX_INPUT;j++){
			if(P_Box[i][j]==1){
				p_box_output = p_box_output << 1;
				if(isBitSet(s_box_output,j)){
					p_box_output = p_box_output + 1;
				}
				break;
			}
		}
	}
	return p_box_output;
}

//Feistel Network
void spn_encrypt(unsigned int* line, unsigned short int key){
	//Spliting 32-bit line into a Left and Right to be used in feistel network
	unsigned short int L = *line >> 16;
	unsigned short int R = *line & 0xFFFF;
	
	//printf("L: %d, R: %d\n",L,R);
	for(int round=0;round<NUMBER_OF_ROUNDS;round++){
		//Get F(L)
		L = round_function(L,key,round);
		//printf("Rounded L: %d\n",L);
		//Get F(L) XOR R
		L = L ^ R;
		//printf("L: %d, R: %d\n",L,R);
		//Swap L and R
		unsigned short int temp = R;
		R = L;
		L = temp;
	}
	
	unsigned int lr_concatinated = (unsigned int)(L<<16) + (unsigned int)R;
	*line = lr_concatinated;
}

int main(){	

	unsigned int line = 213893;
	unsigned int temp = line;
	unsigned short int key = 5570;
	unsigned int* ptr = &line;
	for(int i=0;i<10;i++){		
		spn_encrypt(ptr,key);
		printf("-----\nLine: %d\nKey: %d\nEncrypted Lide:%d\n",temp,key,line);
		key++;
		*ptr = temp;	
	}
}
