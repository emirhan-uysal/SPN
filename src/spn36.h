//Select box, has size of 15x30, each line represents the output line
//each column represents the input of the S-box
uint32_t S_BOX[] = {
    0b00001110010110111101011010000001,
	0b00110110101010010010100100011101,
	0b00101000000111101000111010011001,
	0b00101101100000011000110101100110,
	0b00000110111110000010100010110010,
	0b00011010111000111101000010001001,
	0b00010011101011111001101010001001,
	0b00110101101110010100110100100101,
	0b00110110101010110101001000000100,
	0b00100110110000100100010111001001,
	0b00011011110000100110011001110000,
	0b00011111101011010001110100101101,
	0b00101101111100101010100011110010,
	0b00001011000001111110110011100010,
	0b00101011101000110111000111001011
};
//Permutation box
//Each index represents the input line of the P box
//and each value represents where that index value is
//mapped at the output.
uint16_t P_BOX[] = {
	12,
	9,
	13,
	1,
	10,
	2,
	7,
	3,
	5,
	11,
	6,
	4,
	0,
	14,
	8
};

//memory line
typedef unsigned long long md_addr_t;

uint16_t isBitSet(uint32_t valueToCheck,uint32_t indexToCheck);

uint16_t selectBox(uint16_t left, uint16_t key);

uint16_t permutationBox(uint16_t s_box_output);

uint16_t roundFunction(uint16_t left,uint16_t key);

uint16_t* getSubKeys(uint64_t key);

md_addr_t encryptCacheLineAddr(md_addr_t cache_line_to_encrypt, uint64_t encryption_key);