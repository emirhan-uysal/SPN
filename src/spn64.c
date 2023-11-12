#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define S_BOX_INPUT 64
#define ROUND 16
#define P_BOX_INPUT 32
#define NUMBER_OF_ROUNDS 16

//Initilization of the S-Box
//Each line has exacly 16 input lines to XOR
int S_Box[ROUND][S_BOX_INPUT] = {
	{0,1,0,0,0,0,0,1,0,1,0,1,0,0,1,1,1,0,1,1,1,0,1,1,0,0,1,1,1,1,0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,1,1,0,0,},
	{0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,1,0,1,1,1,1,1,0,0,1,0,1,1,1,1,1,1,0,0,1,1,1,0,1,0,1,0,1,1,0,0,0,0,},
	{1,0,1,0,1,1,0,1,1,1,0,0,1,0,1,0,0,1,1,0,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,1,0,0,0,0,1,1,0,1,0,1,1,1,1,1,1,0,1,},
	{1,1,1,0,1,1,1,1,1,1,0,0,0,0,1,0,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,0,0,0,1,1,1,1,0,0,1,0,0,1,1,0,0,0,1,1,1,0,0,1,0,1,0,1,0,0,0,0,1,1,},
	{1,0,1,0,1,0,0,1,0,1,1,1,0,1,1,0,0,0,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,},
	{1,0,1,0,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,1,0,1,1,1,0,0,1,1,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,1,},
	{0,0,0,1,1,0,1,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,1,0,1,0,1,1,0,1,1,0,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,1,0,0,1,0,1,0,1,1,1,0,1,1,1,},
	{1,1,1,0,0,1,1,0,0,0,0,0,1,0,1,1,0,1,1,0,0,0,1,0,1,0,1,1,1,0,0,1,0,0,1,1,1,0,1,0,1,0,1,1,0,0,1,1,0,0,0,1,1,1,0,1,0,0,0,1,0,1,1,0,},
	{1,1,1,0,0,1,0,1,0,0,1,0,1,1,0,1,1,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,1,1,0,1,1,0,1,1,1,0,0,1,0,0,0,0,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,},
	{1,0,1,1,1,0,0,1,0,0,0,1,0,0,1,1,0,1,1,1,0,0,1,0,0,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,0,0,1,1,0,1,0,1,0,0,1,1,0,1,1,0,0,0,0,0,0,},
	{1,0,1,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,0,0,0,1,0,1,1,0,0,1,0,1,1,0,1,0,1,1,0,0,0,1,0,1,1,0,0,1,0,0,1,0,0,1,1,1,1,1,},
	{1,1,1,0,0,0,0,0,1,1,0,1,1,1,0,0,0,1,1,1,0,1,0,1,1,0,0,0,0,0,1,1,0,0,0,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,1,0,1,1,1,},
	{1,0,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,1,1,1,0,1,1,0,0,0,1,1,0,1,1,1,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,0,0,1,},
	{1,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,},
	{1,1,0,1,0,1,0,1,0,0,0,1,0,0,1,1,0,0,0,1,1,1,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,0,0,1,1,0,1,0,0,0,1,1,1,1,1,0,0,1,},
	{1,0,0,0,0,1,1,1,1,0,0,1,0,1,0,0,1,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,}
};
//Iniitilization of the P-Box
//Each bit remapped to a different bit uniquely
//Goes through least significant bit to most significant
int P_Box[P_BOX_INPUT][P_BOX_INPUT] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,},
	{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,},
	{0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,},
	{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,}
};

//For testing purposes
/*
void printBinary(long long num) {
    int bits = sizeof(num) * 8; // Calculate the number of bits in a long long
    for (int i = bits - 1; i >= 0; i--) {
        long long bit = (num >> i) & 1; // Extract each bit using bitwise AND and right shift
        printf("%d", bit);
    }
    printf("\n");
}
*/

//Function to get the value of a bit of a short
unsigned long long isBitSet(unsigned long long valueToCheck,unsigned long long indexToCheck){
	unsigned long long mask = 1;
	mask = mask<<indexToCheck;
	if((valueToCheck&mask)!=0) return 1;
	return 0;
}

//T is for the input, K is the key, R for the Round
unsigned long long round_function(unsigned long long T, unsigned long long K, unsigned long long R){
	//S-Box part
	unsigned long long s_box_output = 0;
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
	unsigned long long p_box_output = 0;
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
void spn_encrypt(unsigned long long* line, unsigned int key){
	//Spliting 32-bit line into a Left and Right to be used in feistel network
	unsigned int L = *line >> 32;
	unsigned int R = *line & 0xFFFFFFFF;
	
	//printf("L: %d, R: %d\n",L,R);
	for(int round=0;round<NUMBER_OF_ROUNDS;round++){
		//Get F(L)
		L = round_function(L,key,round);
		//printf("Rounded L: %d\n",L);
		//Get F(L) XOR R
		L = L ^ R;
		//printf("L: %d, R: %d\n",L,R);
		//Swap L and R
		unsigned int temp = R;
		R = L;
		L = temp;
	}
    unsigned long long lr_concatinated = L;
    lr_concatinated = lr_concatinated << 32;
    lr_concatinated = lr_concatinated + R;
	//unsigned long long lr_concatinated = (unsigned long long)(L<<32) + (unsigned long long)R;
	*line = lr_concatinated;
}

int main(){	

	unsigned long long line = 1844674407370955161;
	unsigned long long temp = line;
	unsigned int key = 1239820;
	unsigned long long* ptr = &line;
	for(int i=0;i<10;i++){		
		spn_encrypt(ptr,key);
		printf("-----\nLine: %llu\nKey: %d\nEncrypted Lide:%llu\n",temp,key,line);
		key++;
		*ptr = temp;	
	}
}
