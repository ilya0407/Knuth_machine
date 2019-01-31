#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define OP_COUNT 255 
#define stack_segment 49152
#define data_segment 16384
#define pool_segment 32768


short int current_addr;
short int start_addr;
short int addr_in_mem;
short int num_loadbytes;
char flag_const = 0;

typedef union 
{
	long long int octabyte;
    unsigned long long int uoctabyte;

    int tetrabyte[2];
    unsigned int utetrabyte[2];

    short int vyde[4];
    unsigned short int uvyde[4];

    char byte[8];
    unsigned char ubyte[8];

    double foctabyte;
    float ftetrabyte[2];
}mem;

typedef mem* mem1;

mem1 virtual_memory;
mem registers_general[256];

typedef struct
{
	mem num;
	char* name;
}reg_spec;

reg_spec registers_special[32] = {
	{{0},"rB"},{{0},"rD"},{{0},"rE"},{{0},"rH"},{{0},"rJ"},{{0},"rM"},
	{{0},"rR"},{{0},"rBB"},{{0},"rC"},{{0},"rN"},{{0},"rO"},{{0},"rS"},
	{{0},"rI"},{{0},"rT"},{{0},"rTT"},{{0},"rK"},{{0},"rQ"},{{0},"rU"},
	{{0},"rV"},{{32},"rG"},{{2},"rL"},{{0},"rA"},{{0},"rF"},{{0},"rP"},
	{{0},"rW"},{{0},"rX"},{{0},"rY"},{{0},"rZ"},{{0},"rWW"},{{0},"rXX"},
	{{0},"rYY"},{{0},"rZZ"}
}; 

struct operation{
	void (*opr)(int,int,int);
	int number;
};


enum ROUND {ROUND_OFF,ROUND_UP,ROUND_DOWN,ROUND_NEAR};


//data loading(signed) to registers
void ldb (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].byte[0] = virtual_memory[oper2].byte[oper3%8];
	else
		if(oper3<256)
			registers_general[oper1].byte[0] = oper3;
}

void ldw (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].vyde[0] = virtual_memory[oper2].vyde[oper3/2];
	else
		if(oper3<256)
			registers_general[oper1].vyde[0] = oper3;
}

void ldt (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].tetrabyte[0] = virtual_memory[oper2].tetrabyte[oper3/4];
	else
		if(oper3<256)
			registers_general[oper1].tetrabyte[0] = oper3;
}

void ldo (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = virtual_memory[oper2].octabyte;
	else
		if(oper3<256)
			registers_general[oper1].octabyte = oper3;
}

//data loading(unsigned) to registers
void ldbu (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].ubyte[0] = virtual_memory[oper2].ubyte[oper3%8];
	else
		if(oper3<256)
			registers_general[oper1].ubyte[0] = oper3;
}

void ldwu (int oper1, int oper2, int oper3)
{
    if(flag_const == 0)
		registers_general[oper1].uvyde[0] = virtual_memory[oper2].uvyde[oper3/2];
	else
		if(oper3<256)
			registers_general[oper1].uvyde[0] = oper3;
}

void ldtu (int oper1, int oper2, int oper3)
{
    if(flag_const == 0)
		registers_general[oper1].utetrabyte[0] = virtual_memory[oper2].utetrabyte[oper3/4];
	else
		if(oper3<256)
			registers_general[oper1].utetrabyte[0] = oper3;
}

void ldou (int oper1, int oper2, int oper3)
{
    if(flag_const == 0)
		registers_general[oper1].uoctabyte = virtual_memory[oper2].uoctabyte;
	else
		if(oper3<256)
			registers_general[oper1].uoctabyte = oper3;
}

void ldht (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].tetrabyte[1] = virtual_memory[oper2].tetrabyte[oper3/4];
	else 
		if(oper3 < 256)
			registers_general[oper1].tetrabyte[1] = oper3;
}
//loading address
void lda (int oper1, int oper2, int oper3)
{
    registers_general[oper1].octabyte = oper2 + oper3;
}

//data loading(signed) to memory
void stb (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		virtual_memory[oper2].byte[oper3%8] = registers_general[oper1].byte[0];
	else
		virtual_memory[oper2].byte[oper3%8] = registers_general[oper1].byte[0];
}

void stw (int oper1, int oper2, int oper3)
{
    virtual_memory[oper2].vyde[oper3/2] = registers_general[oper1].vyde[0];
}

void stt (int oper1, int oper2, int oper3)
{
    virtual_memory[oper2].tetrabyte[oper3/4] = registers_general[oper1].tetrabyte[0];
}

void sto (int oper1, int oper2, int oper3)
{
    virtual_memory[oper2].octabyte = registers_general[oper1].octabyte;
}

//data loading(unsigned) to memory
void stbu (int oper1, int oper2, int oper3)
{
	virtual_memory[oper2].ubyte[oper3%8] = registers_general[oper1].ubyte[0];
}

void stwu (int oper1, int oper2, int oper3)
{
	virtual_memory[oper2].vyde[oper3/2] = registers_general[oper1].uvyde[0];
}

void sttu (int oper1, int oper2, int oper3)
{
    virtual_memory[oper2].tetrabyte[oper3/4] = registers_general[oper1].tetrabyte[0];
}

void stou (int oper1, int oper2, int oper3)
{
	virtual_memory[oper2].uoctabyte = registers_general[oper1].uoctabyte;
}

void stht(int oper1, int oper2, int oper3)
{
    virtual_memory[oper2].tetrabyte[oper3/4] = registers_general[oper1].tetrabyte[1];
}

void stco(int oper1, int oper2, int oper3)
{
    virtual_memory[oper2].tetrabyte[oper3/4] = oper1;
}

void add(int oper1, int oper2, int oper3)
{
    registers_general[oper1].octabyte = registers_general[oper2].octabyte + registers_general[oper3].octabyte;
}

void sub(int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = registers_general[oper2].octabyte - registers_general[oper3].octabyte;
	else 
		registers_general[oper1].octabyte = registers_general[oper2].octabyte - oper3;
	
}

void mul(int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = registers_general[oper2].octabyte * registers_general[oper3].octabyte;
	else 
		 registers_general[oper1].octabyte = registers_general[oper2].octabyte * oper3;
}

void div_(int oper1, int oper2, int oper3)
{
    if(flag_const == 0)
	{	
		registers_general[oper1].octabyte = registers_general[oper2].octabyte / registers_general[oper3].octabyte;
		registers_special[17].num.octabyte = registers_general[oper2].octabyte % registers_general[oper3].octabyte;//rR = 17
	}
	else
		{
			registers_general[oper1].octabyte = registers_general[oper2].octabyte / oper3;
			registers_special[17].num.octabyte = registers_general[oper2].octabyte % oper3;//rR = 17
		}
	
}

void addu(int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte + registers_general[oper3].uoctabyte;
	else 
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte + (unsigned int)oper3;
}

void subu(int oper1, int oper2, int oper3)
{
    if(flag_const == 0)
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte - registers_general[oper3].uoctabyte;
	else
		if(oper3 < 256)
			registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte - (unsigned int)oper3;
}

void mulu(int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
	{
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte * registers_general[oper3].uoctabyte % 2^64 ;
		registers_special[7].num.uoctabyte = registers_general[oper2].uoctabyte * registers_general[oper3].uoctabyte / 2^64 ;
	}
	else 
		{
			registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte * (unsigned int)oper3 % 2^64 ;
			registers_special[7].num.uoctabyte = registers_general[oper2].uoctabyte * (unsigned int)oper3 / 2^64 ;
		}	
}

void divu(int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
	{
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte / registers_general[oper3].uoctabyte;
		registers_special[17].num.uoctabyte = registers_general[oper2].uoctabyte % registers_general[oper3].uoctabyte;//rR = 17
	}
	else
		{
			registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte / (unsigned int)oper3;
			registers_special[17].num.uoctabyte = registers_general[oper2].uoctabyte % (unsigned int)oper3;//rR = 17
		}	
}

void _2addu (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte * 2 + registers_general[oper3].uoctabyte;
	else
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte * 2 + (unsigned int)oper3;
}

void _4addu (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte * 4 + registers_general[oper3].uoctabyte;
	else 
		{
			registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte * 4 + (unsigned int)oper3;
		}
}

void _8addu (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte * 8 + registers_general[oper3].uoctabyte;
	else 
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte * 8 + (unsigned int)oper3;
}

void _16addu (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte * 16 + registers_general[oper3].uoctabyte;
	else 
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte * 16 + (unsigned int)oper3;
}

void neg (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = oper2 - registers_general[oper3].octabyte;
	else 
		registers_general[oper1].octabyte = oper2 - oper3;
}

void negu (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].uoctabyte = oper2 - registers_general[oper3].uoctabyte;
	else 
		registers_general[oper1].uoctabyte = oper2 - (unsigned int)oper3;
}

void sl (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = registers_general[oper2].octabyte * pow(2,registers_general[oper3].octabyte);
	else 
		registers_general[oper1].octabyte = registers_general[oper2].octabyte * pow(2,oper3);
}

void slu (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte * pow(2,registers_general[oper3].uoctabyte);
	else 
		registers_general[oper1].octabyte = registers_general[oper2].octabyte * pow(2,(unsigned int)oper3);
}

void sr (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = registers_general[oper2].octabyte / pow(2,registers_general[oper3].octabyte);
	else 
		registers_general[oper1].octabyte = registers_general[oper2].octabyte / pow(2,oper3);
}

void sru (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte / pow(2,registers_general[oper3].uoctabyte);
	else 
		registers_general[oper1].uoctabyte = registers_general[oper2].uoctabyte / pow(2,(unsigned int)oper3);
}

void cmp (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = (registers_general[oper2].octabyte > registers_general[oper3].octabyte) -
											(registers_general[oper2].octabyte < registers_general[oper3].octabyte);
	else 
		{
			registers_general[oper1].octabyte = (registers_general[oper2].octabyte > oper3) -
												(registers_general[oper2].octabyte < oper3);
		}
}

void cmpu (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = (registers_general[oper2].uoctabyte > registers_general[oper3].uoctabyte) -
											(registers_general[oper2].uoctabyte < registers_general[oper3].uoctabyte);
	else 
		{
			registers_general[oper1].octabyte = (registers_general[oper2].uoctabyte > (unsigned int)oper3) -
												(registers_general[oper2].uoctabyte < (unsigned int)oper3);
		}
}

void csn (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte < 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
}

void csz (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte == 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
}

void csp (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte > 0)
		registers_general[oper1].octabyte = registers_general[oper3].octabyte;
}

void csod (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte % 2 == 1)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
}

void csnn (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte >= 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
}

void csnz (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte != 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
}

void csnp (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte <= 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
}

void csev (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte % 2 == 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
}

void zsn (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte < 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			
				registers_general[oper1].octabyte = oper3;
	}
	else
		registers_general[oper1].octabyte = 0;
}

void zsz (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte == 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			
				registers_general[oper1].octabyte = oper3;
	}
	else
		registers_general[oper1].octabyte = 0;
}

void zsp (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte > 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			
				registers_general[oper1].octabyte = oper3;
	}
	else
		registers_general[oper1].octabyte = 0;
}

void zsod (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte % 2 == 1)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
	else
		registers_general[oper1].octabyte = 0;
}

void zsnn (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte >= 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
	else
		registers_general[oper1].octabyte = 0;
}

void zsnz (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte != 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
	else
		registers_general[oper1].octabyte = 0;
}

void zsnp (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte <= 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
	else
		registers_general[oper1].octabyte = 0;
}

void zsev (int oper1, int oper2, int oper3)
{
	if(registers_general[oper2].octabyte % 2 == 0)
	{
		if(flag_const == 0)
			registers_general[oper1].octabyte = registers_general[oper3].octabyte;
		else 
			registers_general[oper1].octabyte = oper3;
	}
	else
		registers_general[oper1].octabyte = 0;
}

void and (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = registers_general[oper2].octabyte & registers_general[oper3].octabyte;
	else 
		registers_general[oper1].octabyte = registers_general[oper2].octabyte & oper3;
}

void or (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = registers_general[oper2].octabyte | registers_general[oper3].octabyte;
	else 
		registers_general[oper1].octabyte = registers_general[oper2].octabyte | oper3;
}

void xor (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = registers_general[oper2].octabyte ^ registers_general[oper3].octabyte;
	else 
		registers_general[oper1].octabyte = registers_general[oper2].octabyte ^ oper3;
}

void andn (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = registers_general[oper2].octabyte & ~(registers_general[oper3].octabyte);
	else 
		registers_general[oper1].octabyte = registers_general[oper2].octabyte & ~oper3;
}

void orn (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = registers_general[oper2].octabyte | ~(registers_general[oper3].octabyte);
	else 
		registers_general[oper1].octabyte = registers_general[oper2].octabyte | ~oper3;
}

void nand (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = ~(registers_general[oper2].octabyte & registers_general[oper3].octabyte);
	else 
		registers_general[oper1].octabyte = ~(registers_general[oper2].octabyte & oper3);
}

void nor (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = ~(registers_general[oper2].octabyte | registers_general[oper3].octabyte);
	else 
		registers_general[oper1].octabyte = ~(registers_general[oper2].octabyte | oper3);
}

void nxor (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = ~(registers_general[oper2].octabyte ^ registers_general[oper3].octabyte);
	else 
		registers_general[oper1].octabyte = ~(registers_general[oper2].octabyte ^ oper3);
}

void mux (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = (registers_general[oper2].octabyte & registers_special[13].num.octabyte) |
											(registers_general[oper3].octabyte & ~registers_special[13].num.octabyte);
	else 
		registers_general[oper1].octabyte = (registers_general[oper2].octabyte & registers_special[13].num.octabyte) |
											(oper3 & ~registers_special[13].num.octabyte);
}

void sadd (int oper1, int oper2, int oper3)
{
	long long int x;
	if(flag_const == 0)
		x = registers_general[oper2].octabyte & ~registers_general[oper3].octabyte;
	else 
		x = registers_general[oper2].octabyte & ~oper3;
	
		long long int b;
		for (b = 0; x != 0; x >>= 1)
			if (x & 01)
				b++;
		registers_general[oper1].octabyte = b;
}

char max_char (char a, char b)
{
	if(a > b)
		return a;
	else
		return b;
}

void bdif (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
	{
		for(int i = 0; i<8; i++)
		{
			registers_general[oper1].byte[i] = max_char(0, registers_general[oper2].byte[i] - registers_general[oper3].byte[i]);
		}
	}
	else 
		{
			for(int i = 0; i<8; i++)
			{
				registers_general[oper1].byte[i] = max_char(0, registers_general[oper2].byte[i] - oper3);
			}
		}

}

short int max_shortint (short int a, short int b)
{
	if(a > b)
		return a;
	else
		return b;
}

void wdif (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
	{
		for(int i = 0; i<4; i++)
		{
			registers_general[oper1].vyde[i] = max_shortint(0, registers_general[oper2].vyde[i] - registers_general[oper3].vyde[i]);
		}
	}
	else 
		{
			for(int i = 0; i<4; i++)
			{
				registers_general[oper1].vyde[i] = max_shortint(0, registers_general[oper2].vyde[i] - oper3);
			}
		}
}

int max_int (int a, int b)
{
	if(a > b)
		return a;
	else
		return b;
}

void tdif (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
	{
		for(int i = 0; i<2; i++)
		{
			registers_general[oper1].tetrabyte[i] = max_int(0, registers_general[oper2].tetrabyte[i] - registers_general[oper3].tetrabyte[i]);
		}
	}
	else 
		{
			for(int i = 0; i<2; i++)
			{
				registers_general[oper1].tetrabyte[i] = max_int(0, registers_general[oper2].tetrabyte[i] - oper3);
			}
		}
}

long long int max_longlongint (long long int a, long long int b)
{
	if(a > b)
		return a;
	else
		return b;
}

void odif (int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].octabyte = max_int(0, registers_general[oper2].octabyte - registers_general[oper3].octabyte);
	else 
		registers_general[oper1].octabyte = max_int(0, registers_general[oper2].octabyte - oper3);
}

int get_bit(char x, int number)
{
	return (x >> number) & 1;
}

int powint(int x, int i)
{
	int n = 1;
	for(int k = 0; k < i; k++)
	{
		n *= x;
	}
	return n;
}


void mor (int oper1, int oper2, int oper3)
{
	int i = 0, j = 0;
	char Xij;
	if(flag_const == 0)
	{
		for(i = 0; i < 8; i++)
		{
			for(j = 0; j < 8; j++)
			{
				Xij = 0;
				for(int k = 0; k < 8; k++)
				{
					Xij |= get_bit(registers_general[oper3].byte[k],i) & get_bit(registers_general[oper2].byte[j],k);
				}
				registers_general[oper1].byte[j] += Xij * (char)powint(2,i);
			}
		}
	}
	else 
	{
		for(i = 0; i < 8; i++)
		{
			for(j = 0; j < 8; j++)
			{
				Xij = 0;
				for(int k = 0; k < 8; k++)
				{
					Xij |= get_bit(oper3,i) & get_bit(registers_general[oper2].byte[j],k);
				}
				registers_general[oper1].byte[j] += Xij * (char)powint(2,i);
			}
		}
	}
}

void mxor (int oper1, int oper2, int oper3)
{
	int i = 0, j = 0;
	char Xij;

	if(flag_const == 0)
	{
		for(i = 0; i < 8; i++)
		{
			for(j = 0; j < 8; j++)
			{
				Xij = 0;
				for(int k = 0; k < 8; k++)
				{
					Xij ^= get_bit(registers_general[oper3].byte[k],i) & get_bit(registers_general[oper2].byte[j],k);
				}
				registers_general[oper1].byte[j] += Xij * (char)powint(2,i);
			}
		}
	}
	else 
	{
		for(i = 0; i < 8; i++)
		{
			for(j = 0; j < 8; j++)
			{
				Xij = 0;
				for(int k = 0; k < 8; k++)
				{
					Xij ^= get_bit(oper3,i) & get_bit(registers_general[oper2].byte[j],k);
				}
				registers_general[oper1].byte[j] += Xij * (char)powint(2,i);
			}
		}
	}
}

void fadd(int oper1, int oper2, int oper3)
{
	registers_general[oper1].foctabyte = registers_general[oper2].foctabyte + registers_general[oper3].foctabyte;
}

void fsub(int oper1, int oper2, int oper3)
{
    registers_general[oper1].foctabyte = registers_general[oper2].foctabyte - registers_general[oper3].foctabyte;
}

void fmul(int oper1, int oper2, int oper3)
{
    registers_general[oper1].foctabyte = registers_general[oper2].foctabyte * registers_general[oper3].foctabyte;
}

void fdiv(int oper1, int oper2, int oper3)
{
    registers_general[oper1].foctabyte = registers_general[oper2].foctabyte / registers_general[oper3].foctabyte;
}

void frem(int oper1, int oper2, int oper3)
{
    registers_general[oper1].foctabyte = fmod(registers_general[oper2].foctabyte, registers_general[oper3].foctabyte);
}

void fsqrt(int oper1, int oper2, int oper3)
{
    registers_general[oper1].foctabyte = pow(registers_general[oper3].foctabyte,1/2);
}

void fint(int oper1, int oper2, int oper3)
{
    registers_general[oper1].foctabyte = abs((long long int)registers_general[oper3].foctabyte);
}

void fcmp(int oper1, int oper2, int oper3)
{
    registers_general[oper1].foctabyte = (registers_general[oper2].foctabyte > registers_general[oper2].foctabyte) - 
											(registers_general[oper2].foctabyte < registers_general[oper2].foctabyte);
}

void feql(int oper1, int oper2, int oper3)
{
	registers_general[oper1].foctabyte = (registers_general[oper2].foctabyte = registers_general[oper3].foctabyte);
}

void fun(int oper1, int oper2, int oper3)
{
	if (isnan(registers_general[oper2].foctabyte) && isinf(registers_general[oper2].foctabyte) &&
	isnan(registers_general[oper3].foctabyte) && isinf(registers_general[oper3].foctabyte))
			registers_general[oper1].foctabyte = 1;
	else
		registers_general[oper1].foctabyte = 0;
}

void fcmpe(int oper1, int oper2, int oper3)
{
	registers_general[oper3].octabyte = ((registers_general[oper2].foctabyte - registers_general[oper3].foctabyte) > registers_special[3].num.foctabyte) - 
										((registers_general[oper3].foctabyte - registers_general[oper2].foctabyte) > registers_special[3].num.foctabyte);
}

void feqle(int oper1, int oper2, int oper3)
{
	registers_general[oper3].octabyte = (registers_general[oper2].foctabyte - registers_general[oper3].foctabyte) < registers_special[3].num.foctabyte;
}

void fune(int oper1, int oper2, int oper3)
{
	if(isnan(registers_general[oper2].foctabyte - registers_special[3].num.foctabyte) && isinf(registers_general[oper2].foctabyte - registers_special[3].num.foctabyte) &&
		isnan(registers_general[oper3].foctabyte - registers_special[3].num.foctabyte) && isinf(registers_general[oper3].foctabyte - registers_special[3].num.foctabyte))
	registers_general[oper1].foctabyte = 1;
	
	else
		registers_general[oper1].foctabyte = 0;
	
}

void fix(int oper1, int oper2, int oper3)
{
	registers_general[oper1].foctabyte = (long long int)registers_general[oper3].foctabyte;
}

void fixu(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uoctabyte = abs((long long int)registers_general[oper3].foctabyte);
}

void flot(int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].foctabyte = registers_general[oper3].octabyte;
	else
		registers_general[oper1].foctabyte = (double)oper3; 
}

void flotu(int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].foctabyte = registers_general[oper3].uoctabyte;
	else 
		registers_general[oper1].foctabyte = (double)oper3;
}

void sflot(int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].foctabyte = registers_general[oper3].tetrabyte[0];
	else 
		registers_general[oper1].foctabyte = (double)oper3;
}

void sflotu(int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].foctabyte = registers_general[oper3].utetrabyte[0];
	else
		registers_general[oper1].foctabyte = (double)oper3;
}

void ldsf(int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		registers_general[oper1].foctabyte = virtual_memory[oper2].ftetrabyte[oper3];
	else 
		registers_general[oper1].foctabyte = (double)oper3;
}

void stsf(int oper1, int oper2, int oper3)
{
	if(flag_const == 0)
		virtual_memory[oper2].ftetrabyte[oper3] = registers_general[oper1].foctabyte ;
	else 
		registers_general[oper1].foctabyte = (double)oper3;
}

void seth(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[3] = (unsigned short int)oper2;
}

void setmh(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[2] = (unsigned short int)oper2;
}

void setml(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[1] = (unsigned short int)oper2;
}

void setl(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[0] = (unsigned short int)oper2;
}

void inch(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[3] += (unsigned short int)oper2;
}

void incmh(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[2] += (unsigned short int)oper2;
}

void incml(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[1] += (unsigned short int)oper2;
}

void incl(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[0] += (unsigned short int)oper2;
}

void orh(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[3] |= (unsigned short int)oper2;
}

void ormh(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[2] |= (unsigned short int)oper2;
}

void orml(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[1] |= (unsigned short int)oper2;
}

void orl(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[0] |= (unsigned short int)oper2;
}

void andnh(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[3] &= ~(unsigned short int)oper2;
}

void andnmh(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[2] &= ~(unsigned short int)oper2;
}

void andnml(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[1] &= ~(unsigned short int)oper2;
}

void andnl(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uvyde[0] &= ~(unsigned short int)oper2;
}

//transitions 

void jmp(int oper1, int oper2, int oper3)
{
	current_addr += oper1*4;
}

void go(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uoctabyte = current_addr + 4;
	if(flag_const == 0)
		current_addr = registers_general[oper2].uoctabyte + registers_general[oper3].uoctabyte;
	else 
		
			current_addr = registers_general[oper2].uoctabyte + (unsigned int)oper3;
}

void bn(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte < 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void bz(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte == 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void bp(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte > 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void bod(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte%2 == 1)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void bnn(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte%2 >= 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void bnz(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte%2 != 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void bnp(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte%2 <= 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void bev(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte%2 == 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void pbn(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte < 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void pbz(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte == 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void pbp(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte > 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void pbod(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte%2 == 1)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void pbnn(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte%2 >= 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void pbnz(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte%2 != 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void pbnp(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte%2 <= 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

void pbev(int oper1, int oper2, int oper3)
{
	if(registers_general[oper1].octabyte%2 == 0)
	{
		current_addr += (short int)oper2 * 4;
	}
}

//subprogram call

void push(int x)
{
	for(int i = 0; i<x; i++)
	{
		if(i < registers_special[19].num.octabyte)
		{
			registers_general[stack_segment+i].octabyte = registers_general[i].octabyte;
			registers_general[i].octabyte = registers_general[i+x+1].octabyte;
		}
		else 
			break;
	}
}

void pushj(int oper1, int oper2, int oper3)
{
	registers_special[4].num.uoctabyte = current_addr + 4;
	push(oper1);
	current_addr += 4 * (short int)oper2;
}

void pushgo(int oper1, int oper2, int oper3)
{
	registers_special[4].num.uoctabyte = current_addr + 4;
	push(oper1);
	if(flag_const==0)
		current_addr += 4*(registers_general[oper2].uoctabyte + registers_general[oper3].uoctabyte);
	else
		current_addr += 4*(registers_general[oper2].uoctabyte + (unsigned int)oper3);
}

void pop(int oper1, int oper2, int oper3)
{
	current_addr += 4*(registers_special[4].num.uoctabyte + 4 * (short int)oper2);
	for(int i = 0; i<oper1; i++)
	{
		registers_general[i + oper1 + 1].octabyte = registers_general[i].octabyte;
		
		if(i < registers_special[19].num.octabyte && i >= registers_general[20].octabyte)
			registers_general[i].octabyte = 0;
		else 
			if(i < registers_special[19].num.octabyte)
				registers_general[i].octabyte = registers_general[stack_segment+i].octabyte;
	}
}

void get(int oper1, int oper2, int oper3)
{
	registers_general[oper1].uoctabyte = registers_special[oper2].num.uoctabyte;
}

void put(int oper1, int oper2, int oper3)
{
	if(oper2 >= 8 && oper2<= 18)
	 registers_special[oper2].num.uoctabyte = registers_general[oper1].uoctabyte;
}

void geta(int oper1, int oper2, int oper3)
{
	registers_general[oper1].octabyte = oper2;
}

void trap(int oper1, int oper2, int oper3)
{
	for(int i = 0; i < 4; i++)
	{
		registers_special[i+24].num.octabyte = registers_special[i+28].num.octabyte;
	}
	registers_special[0].num.octabyte = registers_special[7].num.octabyte;
}

long long int str_to_int(char s[])
{
	long long int num = 0;
	for(int i = 0; i<=strlen(s); i++)
	{
		if(s[i]>=48 && s[i]<=57)
			num += (s[i] - 48)*pow(16,strlen(s)-i-1);
		else
		if(s[i]>=97 && s[i]<103)
		{
			num += (s[i] - 87)*pow(16,strlen(s)-i-1);
		}
	}
	return num;
}

struct operation operations[OP_COUNT-23] = {
	
	{trap,0x00},{fcmp,0x01},{fun,0x02},{feql,0x03},{fadd,0x04},{fix,0x05},{fsub,0x06},{fixu,0x07},
	{flot,0x08},{flot,0x09},{flotu,0x0A},{flotu,0x0B},{sflot,0x0C},{sflot,0x0D},{sflotu,0x0E},{sflotu,0x0F},
																																	
	{fmul,0x10},{fcmpe,0x11},{fune,0x12},{feql,0x13},{fdiv,0x14},{fsqrt,0x15},{frem,0x16},{fint,0x17},
	{mul,0x18},{mul,0x19},{mulu,0x1A},{mulu,0x1B},{div_,0x1C},{div_,0x1D},{divu,0x1E},{divu,0x1F},
	
	{add,0x20},{add,0x21},{addu,0x22},{addu,0x23},{sub,0x24},{sub,0x25},{subu,0x26},{subu,0x27},
	{_2addu,0x28},{_2addu,0x29},{_4addu,0x2A},{_4addu,0x2B},{_8addu,0x2C},{_8addu,0x2D},{_16addu,0x2E},{_16addu,0x2F},
	
	{cmp,0x30},{cmp,0x31},{cmpu,0x32},{cmpu,0x33},{neg,0x34},{neg,0x35},{negu,0x36},{negu,0x37},
	{sl,0x38},{sl,0x39},{slu,0x3A},{slu,0x3B},{sr,0x3C},{sr,0x3D},{sru,0x3E},{sru,0x3F},

	{bn,0x40},{bn,0x41},{bz,0x42},{bz,0x43},{bp,0x44},{bp,0x45},{bod,0x46},{bod,0x47},
	{bnn,0x48},{bnn,0x49},{bnz,0x4A},{bnz,0x4B},{bnp,0x4C},{bnp,0x4D},{bev,0x4E},{bev,0x4F},
	
	{pbn,0x50},{pbn,0x51},{pbz,0x52},{pbz,0x53},{pbp,0x54},{pbp,0x55},{pbod,0x56},{pbod,0x57},
	{pbnn,0x58},{pbnn,0x59},{pbnz,0x5A},{pbnz,0x5B},{pbnp,0x5C},{pbnp,0x5D},{pbev,0x5E},{pbev,0x5F},
	
	{csn,0x60},{csn,0x61},{csz,0x62},{csz,0x63},{csp,0x64},{csp,0x65},{csod,0x66},{csod,0x67},
	{csnn,0x68},{csnn,0x69},{csnz,0x6A},{csnz,0x6B},{csnp,0x6C},{csnp,0x6D},{csev,0x6E},{csev,0x6F},
	
	{zsn,0x70},{zsn,0x71},{zsz,0x72},{zsz,0x73},{zsp,0x74},{zsp,0x75},{zsod,0x76},{zsod,0x77},
	{zsnn,0x78},{zsnn,0x79},{zsnz,0x7A},{zsnz,0x7B},{zsnp,0x7C},{zsnp,0x7D},{zsev,0x7E},{zsev,0x7F},
	
	{ldb,0x80},{ldb,0x81},{ldbu,0x82},{ldbu,0x83},{ldw,0x84},{ldw,0x85},{ldwu,0x86},{ldwu,0x87},
	{ldt,0x88},{ldt,0x89},{ldtu,0x8A},{ldtu,0x8B},{ldo,0x8C},{ldo,0x8D},{ldou,0x8E},{ldou,0x8F},
	
	{ldsf,0x92},{ldsf,0x93},{ldht,0x94},{ldht,0x95},{go,0x9E},{go,0x9F},
	
	{stb,0xA0},{stb,0xA1},{stbu,0xA2},{stbu,0xA3},{stw,0xA4},{stw,0xA5},{stwu,0xA6},{stwu,0xA7},
	{stt,0xA8},{stt,0xA9},{sttu,0xAA},{sttu,0xAB},{sto,0xAC},{sto,0xAD},{stou,0xAE},{stou,0xAF},
	
	{stsf,0xB2},{stsf,0xB3},{stht,0xB4},{stht,0xB5},{stco,0xB6},{stco,0xB7},{pushgo,0xBE},{pushgo,0xBF},
	
	{or,0xC0},{or,0xC1},{orn,0xC2},{orn,0xC3},{nor,0xC4},{nor,0xC5},{xor,0xC6},{xor,0xC7},
	{and,0xC8},{and,0xC9},{andn,0xCA},{andn,0xCB},{nand,0xCC},{nand,0xCD},{nxor,0xCE},{nxor,0xCF},

	{bdif,0xD0},{bdif,0xD1},{wdif,0xD2},{wdif,0xD3},{odif,0xD4},{odif,0xD5},{tdif,0xD6},{tdif,0xD7},
	{mux,0xD8},{mux,0xD9},{sadd,0xDA},{sadd,0xDB},{mor,0xDC},{mor,0xDD},{mxor,0xDE},{mxor,0xDF},

	{seth,0xE0},{setmh,0xE1},{setml,0xE2},{setl,0xE3},{inch,0xE4},{incmh,0xE5},{incml,0xE6},{incl,0xE7},
	{orh,0xE8},{ormh,0xE9},{orml,0xEA},{orl,0xEB},{andnh,0xEC},{andnmh,0xED},{andnml,0xEE},{andnl,0xEF},
		
	{jmp,0xF0},{jmp,0xF1},{pushj,0xF2},{pushj,0xF3},{geta,0xF4},{geta,0xF5},{put,0xF6},{put,0xF7},
	{pop,0xF8},{get,0xFE}
};

int find_in_mas(int code)
{
	for(int i = 0; i<OP_COUNT-23; i++)
	{
		if(code == operations[i].number)
			return i;
	}
	return -1;
}

void output()
{
	for(int i = 0; i<=5; i++)
	{
		printf("$%d\t%x\n",i,registers_general[i].byte[0]);
	}
}


int main(int argc,char** argv){
	virtual_memory = (mem1)malloc(pow(2,13));
	FILE* f;
	char str[8];
	char str1[4];
	int info;
	
	if ((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Не удалось открыть файл");
		getchar();
		return EXIT_FAILURE;
	}
	
	if(!feof(f))
	{
		fgets(str1,5,f);
		addr_in_mem = str_to_int(str1);
		fgets(str1,5,f);
		start_addr = str_to_int(str1);
		fgets(str1,5,f);
		num_loadbytes = str_to_int(str1);
	}

	int j = 0;
	int num_bytes = 0;
	
	while(1)
	{
		fgets(str,9,f);
		if(feof(f)) break;
		if(str != NULL)
		{
			info = str_to_int(str);
			virtual_memory[(num_bytes + addr_in_mem + 8)/8].utetrabyte[j] = info;
			if(j==0)
				j = 1;
			else
			{
				j = 0;
				num_bytes += 8;
			}
		}
	}
	
	current_addr = start_addr;
	int n1,n2;
	
	for(int i = 0; i<num_bytes; i++)
	{
		if(i%8==0)
		{
			n1 = find_in_mas(virtual_memory[i/8].byte[3]);
			n2 = find_in_mas(virtual_memory[i/8].byte[7]);
			
			if(virtual_memory[i].byte[3]%2 == 1)
			{
				flag_const = 1;
			}
			else flag_const = 0;
			
			if(n1!=-1)
			{
				(*operations[n1].opr)(virtual_memory[i/8].byte[2],virtual_memory[i/8].byte[1],virtual_memory[i/8].byte[0]);
				if(n1==0)
				{
					if(virtual_memory[i/8].byte[2] == 0)
					{	
						output();
						return EXIT_SUCCESS;
					}
				}
			}
			
			if(virtual_memory[i].byte[7]%2 == 1)
			{
				flag_const = 1;
			}
			else flag_const = 0;
			
			
			if(n2!=-1)
			{
				(*operations[n2].opr)(virtual_memory[i/8].byte[6],virtual_memory[i/8].byte[5],virtual_memory[i/8].byte[4]);
				if(n2==0)
				{
					if(virtual_memory[i/8].byte[6] == 0)
					{	
						output();
						return EXIT_SUCCESS;
					}
				}	
			}
		}
	}
	return EXIT_FAILURE;
}
