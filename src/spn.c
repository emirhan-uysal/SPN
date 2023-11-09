#include <stdio.h>

#define S_BOX_INPUT 32
#define ROUND 16
#define P_BOX_INPUT 16

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

//Function to get the value of a bit of a short
short int isBitSet(short int valueToCheck,short int indexToCheck){
	short int mask = 1;
	for(int i=0;i<indexToCheck;i++){
		mask = mask<<1;
	}
	if((valueToCheck&mask)!=0) return 1;
	return 0;
}

//T is for the input, K is the key, R for the Round
short int round_function(short int T, short int K, short int R){
	//S-Box part
	short int s_box_output = 0;
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
			if(isBitSet(K,S_BOX_INPUT/2-i-1)){
				s_box_output = s_box_output + 1;
			}
		}
	}
	//P-Box part
	short int p_box_output = 0;
	for(int i = 0;i<P_BOX_INPUT;i++){
		for(int j=0;j<P_BOX_INPUT;j++){
			if(P_Box[i][j]==1){
				if(isBitSet(s_box_output,j)){
					p_box_output = p_box_output + 1;
				}
				p_box_output = p_box_output << 1;
			}
		}
	}
	return p_box_output;
}

//Feistel Network
void spn_encrypt(int line, short int key){
	
}


int main(){	
	short int L = 24890;
	short int K = 10032;
	short int out = round_function(L,K,3);
	printf("L: %d K:%d R_OUT: %d\n",L,K,out);
}
