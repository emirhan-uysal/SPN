//Select box, has size of 15x30, each line represents the output line
//each column represents the input of the S-box
uint32_t S_BOX[15][5] = {
    {15,11,8,4,0},
	{13,12,9,3,1},
	{14,10,9,5,4},
	{11,8,7,4,0},
	{13,11,5,3,2},
	{15,9,4,2,0},
	{14,8,6,3,1},
	{11,8,5,3,0},
	{15,13,11,9,7},
	{14,12,8,6,4},
	{13,7,5,4,1},
	{15,10,7,6,0},
	{13,12,7,4,0},
	{13,11,9,7,5},
	{11,7,4,2,1}
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