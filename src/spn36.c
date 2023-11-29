// 30bit inpput 15 bit output SPN implementation
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "spn36.h"

#define S_BOX_INPUT_SIZE 30
#define S_BOX_OUTPUT_SIZE 15

#define FEISTEL_ROUNDS 4


uint16_t* getSubKeys(uint64_t key){
	static uint16_t sub_keys[FEISTEL_ROUNDS];
	uint16_t mask = 0b111111111111111;
	for(int i=0;i<FEISTEL_ROUNDS;i++){
		sub_keys[i] = key & mask;
		key = key >> 15;
	}
	return sub_keys;
}

//Function to get the value of a bit of a short
uint16_t isBitSet(uint32_t valueToCheck,uint32_t indexToCheck){
	unsigned long long mask = 1;
	mask = mask<<indexToCheck;
	if((valueToCheck&mask)!=0) return 1;
	return 0;
}

uint16_t selectBox(uint16_t left, uint16_t key){
	//Concatinate left and key to give to S-BOX as an input
	uint32_t input_to_xor = left;
	input_to_xor = input_to_xor << 16;
	input_to_xor = input_to_xor + key;
	
	uint16_t s_box_output = 0;
	//TO-DO Check the result is correct
	for(int i=S_BOX_OUTPUT_SIZE-1;i>=0;i--){
		uint8_t cur_out = 0;
		s_box_output = s_box_output << 1;
		for(int j=0;j<5;j++){
			//Selected bit for XOR
			if(isBitSet(input_to_xor,S_BOX[i][j])){
				cur_out = cur_out ^ 1;
			}
			else{
				cur_out = cur_out ^ 0;
			}
		}
		s_box_output = s_box_output + cur_out;
	}
	return s_box_output;
}

uint16_t permutationBox(uint16_t s_box_output){
	uint16_t p_box_output = 0;
	
	for(int i=0; i<S_BOX_OUTPUT_SIZE; i++){
		uint16_t permutadedPlace = P_BOX[i];
		uint16_t value = isBitSet(s_box_output,i);
		value = value << permutadedPlace;
		p_box_output+=value;
	}
	return p_box_output;
}

uint16_t roundFunction(uint16_t left,uint16_t key){
	uint16_t p_box_output = selectBox(left,key);
	return permutationBox(p_box_output);
}

md_addr_t encryptCacheLineAddr(md_addr_t cache_line_to_encrypt, uint64_t encryption_key){
	//Do not encrypt the last 6 bits on the cache line
	//since it is the block index
	uint32_t part_to_encrypt = cache_line_to_encrypt >> 6;
	//Save block index to use later
	uint8_t block_index = cache_line_to_encrypt & 0b111111;
	uint16_t left = part_to_encrypt >> 15;
	uint16_t right = part_to_encrypt & 0b111111111111111;
	
	uint16_t* sub_keys = getSubKeys(encryption_key);
	
	for(int round = 0; round<FEISTEL_ROUNDS; round++){
	uint16_t round_fun_out = roundFunction(left,sub_keys[round]);
		uint16_t temp = left;
		left = round_fun_out ^ right;
		right = temp;
	}
	
	md_addr_t encrypted_line = left;
	encrypted_line = encrypted_line << 15;
	encrypted_line = encrypted_line + right;
	encrypted_line = encrypted_line << 6;
	encrypted_line = encrypted_line + block_index;
	
	return encrypted_line;
}

int main(){
	clock_t start, end;
	double cpu_time_used;
	srand(time(NULL));
	uint64_t key = rand();  
	
	md_addr_t line = 0b110011000100001010011001111011110100;
	
	start = clock();
	for(int i=0;i<10000000;i++){
		md_addr_t enc_line = encryptCacheLineAddr(0b110011000100001010011011111011110100,key);
		//printf("Line: %llu\n",line);
		//printf("Encr: %llu\n",enc_line);
		//key = rand();
	}
	end = clock();
	// Calculate the time taken in seconds
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Print the time taken
    printf("Time taken: %f seconds\n", cpu_time_used);
}