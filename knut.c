#include <stdio.h>
#include <math.h>

long long int registers_general[256];

long long int registers_spec[32];

char memory[2^64-1];


struct operation{
	char* name;
	int number;
};

struct operation operations[] = {
	
	{"TRAP",0x00},	{"FCMP",0x01},	{"FUN",0x02},	{"FEQL",0x03},	{"FADD",0x04},	{"FIX",0x05},	{"FSUB",0x06},	{"FIXU",0x07},
	{"FLOT",0x08},	{"FLOT",0x09},	{"FLOTU",0x0A},	{"FLOTU",0x0B},	{"SFLOT",0x0C},	{"SFLOT",0x0D},	{"SFLOTU",0x0E},{"SFLOTU",0x0F},
																																	
	{"FMUL",0x10},	{"FCMPE",0x11},	{"FUNE",0x12},	{"FEQLE",0x13},	{"FDIV",0x14},	{"FSQRT",0x15},	{"FREM",0x16},	{"FINT",0x17},
	{"MUL",0x18},	{"MUL",0x19},	{"MULU",0x1A},	{"MULU",0x1B},	{"DIV",0x1C},	{"DIV",0x1D},	{"DIVU",0x1E},	{"DIVU",0x1F},
	
	{"ADD",0x20},	{"ADD",0x21},	{"ADDU",0x22},	{"ADDU",0x23},	{"SUB",0x24},	{"SUB",0x25},	{"SUBU",0x26},	{"SUBU",0x27},
	{"2ADDU",0x28},	{"2ADDU",0x29},	{"4ADDU",0x2A},	{"4ADDU",0x2B},	{"8ADDU",0x2C},	{"8ADDU",0x2D},	{"16ADDU",0x2E},{"16ADDU",0x2F},
	
	{"CMP",0x30},	{"CMP",0x31},	{"CMPU",0x32},	{"CMPU",0x33},	{"NEG",0x34},	{"NEG",0x35},	{"NEGU",0x36},	{"NEGU",0x37},
	{"SL",0x38},	{"SL",0x39},	{"SLU",0x3A},	{"SLU",0x3B},	{"SR",0x3C},	{"SR",0x3D},	{"SRU",0x3E},	{"SRU",0x3F},

	{"BN",0x40},	{"BN",0x41},	{"BZ",0x42},	{"BZ",0x43},	{"BP",0x44},	{"BP",0x45},	{"BOD",0x46},	{"BOD",0x47},
	{"BNN",0x48},	{"BNN",0x49},	{"BNZ",0x4A},	{"BNZ",0x4B},	{"BNP",0x4C},	{"BNP",0x4D},	{"BEV",0x4E},	{"BEV",0x4F},
	
	{"PBN",0x50},	{"PBN",0x51},	{"PBZ",0x52},	{"PBZ",0x53},	{"PBP",0x54},	{"PBP",0x55},	{"PBOD",0x56},	{"PBOD",0x57},
	{"PBNN",0x58},	{"PBNN",0x59},	{"PBNZ",0x5A},	{"PBNZ",0x5B},	{"PBNP",0x5C},	{"PBNP",0x5D},	{"PBEV",0x5E},	{"PBEV",0x5F},
	
	{"CSN",0x60},	{"CSN",0x61},	{"CSZ",0x62},	{"CSZ",0x63},	{"CSP",0x64},	{"CSP",0x65},	{"CSOD",0x66},	{"CSOD",0x67},
	{"CSNN",0x68},	{"CSNN",0x69},	{"CSNZ",0x6A},	{"CSNZ",0x6B},	{"CSNP",0x6C},	{"CSNP",0x6D},	{"CSEV",0x6E},	{"CSEV",0x6F},
	
	{"ZSN",0x70},	{"ZSN",0x71},	{"ZSZ",0x72},	{"ZSZ",0x73},	{"ZSP",0x74},	{"ZSP",0x75},	{"ZSOD",0x76},	{"ZSOD",0x77},
	{"ZSNN",0x78},	{"ZSNN",0x79},	{"ZSNZ",0x7A},	{"ZSNZ",0x7B},	{"ZSNP",0x7C},	{"ZSNP",0x7D},	{"ZSEV",0x7E},	{"ZSEV",0x7F},
	
	{"LDB",0x80},	{"LDB",0x81},	{"LDBU",0x82},	{"LDBU",0x83},	{"LDW",0x84},	{"LDW",0x85},	{"LDWU",0x86},	{"LDWU",0x87},
	{"LDT",0x88},	{"LDT",0x89},	{"LDTU",0x8A},	{"LDTU",0x8B},	{"LDO",0x8C},	{"LDO",0x8D},	{"LDOU",0x8E},	{"LDOU",0x8F},
	
	{"LDSF",0x92},	{"LDSF",0x93},	{"LDHT",0x94},	{"LDHT",0x95},	{"CSWAP",0x96},	{"CSWAP",0x97},	{"LDUNC",0x86},	{"LDUNC",0x97},
	{"LDVTS",0x9A},	{"LDVTS",0x9A},	{"PRELD",0x9A},	{"PRELD",0x9B},	{"FSUB",0x9C},	{"FIXU",0x9D},	{"GO",0x9E},	{"GO",0x9F},
	
	{"STB",0xA0},	{"STB",0xA1},	{"STBU",0xA2},	{"STBU",0xA3},	{"STW",0xA4},	{"STW",0xA5},	{"STWU",0xA6},	{"STWU",0xA7},
	{"STT",0xA8},	{"STT",0xA9},	{"STTU",0xAA},	{"STTU",0xAB},	{"STO",0xAC},	{"STO",0xAD},	{"STOU",0xAE},	{"STOU",0xAF},
	
	{"STSF",0xB2},	{"STSF",0xB3},	{"STHT",0xB4},	{"STHT",0xB5},	{"STCO",0xB6},	{"STCO",0xB7},	{"LDUNC",0xB6},	{"LDUNC",0xB7},
	{"STVTS",0xBA},	{"STVTS",0xBA},	{"PREST",0xBA},	{"PREST",0xBB},	{"SYNCID",0xBC},{"SYNCID",0xBD},{"PUSHGO",0xBE},{"PUSHGO",0xBF},
	
	{"OR",0xC0},	{"OR",0xC1},	{"ORN",0xC2},	{"ORN",0xC3},	{"NOR",0xC4},	{"NOR",0xC5},	{"XOR",0xC6},	{"XOR",0xC7},
	{"AND",0xC8},	{"AND",0xC9},	{"ANDN",0xCA},	{"ANDN",0xCB},	{"NAND",0xCC},	{"NAND",0xCD},	{"NXOR",0xCE},	{"NXOR",0xCF},

	{"BDIF",0xD0},	{"BDIF",0xD1},	{"WDIF",0xD2},	{"WDIF",0xD3},	{"ODIF",0xD4},	{"ODIF",0xD5},	{"TDIF",0xD6},	{"TDIF",0xD7},
	{"MUX",0xD8},	{"MUX",0xD9},	{"SADD",0xDA},	{"SADD",0xDB},	{"MORD",0xDC},	{"MOR",0xDD},	{"MXOR",0xDE},	{"MXOR",0xDF},

	{"SETH",0xE0},	{"SETHM",0xE1},	{"SETML",0xE2},	{"SETL",0xE3},	{"INCH",0xE4},	{"INCMH",0xE5},	{"INCML",0xE6},	{"INCL",0xE7},
	{"ORH",0xE8},	{"ORMH",0xE9},	{"ORML",0xEA},	{"ORL",0xEB},	{"ANDNH",0xEC},	{"ANDNMH",0xED},{"ANDNML",0xEE},{"ANDNL",0xEF},
		
	{"JMP",0xF0},	{"JMP",0xF1},	{"PUSHJ",0xF2},	{"PUSHJ",0xF3},	{"GETA",0xF4},	{"GETA",0xF5},	{"PUT",0xF6},	{"PUT",0xF7},
	{"POP",0xF8},	{"RESUME",0xF9},{"[UN]SAVE",0xFA},{"[UN]SAVE",0xFB},{"SYNC",0xFC},{"SWYM",0xFD},{"GET",0xFE},	{"TRIP",0xFF}
};

int num_current_operation, oper1, oper2, oper3;


void analise(int opcode[]){
	num_current_operation = opcode[0];
	oper1 = opcode[1];
	oper2 = opcode[2];
	oper3 = opcode[3];
}

//data loading(signed) to registers
void ldb (int oper1, int oper2, int oper3)
{
	registers_general[oper1] = memory[oper2 + oper3];
	printf("%.16llx\n",registers_general[oper1]);
}

void ldw (int oper1, int oper2, int oper3)
{
	if(oper3/4 == 1)
	{
		if(oper3%2 == 1)
		{
			registers_general[oper1] = memory[oper2 + oper3] + ((memory[oper2 + oper3 - 1] + 1) << 8);
		}
		else
		{
			registers_general[oper1] = ((memory[oper2 + oper3] + 1) << 8) + memory[oper2 + oper3 + 1];
		}
	}
	else
	{
		if(oper3%2 == 1)
		{
			registers_general[oper1] = memory[oper2 + oper3] + (memory[oper2 + oper3 - 1] << 8);
		}
		else
		{
			registers_general[oper1] = (memory[oper2 + oper3] << 8) + memory[oper2 + oper3 + 1];
		}
	}

	printf("%.16llx\n",registers_general[oper1]);

}

void ldt (int oper1, int oper2, int oper3)
{
	if(oper3 / 4 == 1)
	{
		switch(oper3 % 4)
		{
			case 0: 
			registers_general[oper1] = ((memory[oper2 + oper3] + 1) << 3*8) + 
			((memory[oper2 + oper3 + 1] + 1) << 2*8) + 
			((memory[oper2 + oper3 + 2] + 1) << 8) + 
			memory[oper2 + oper3 + 3];
			break;
			
			case 1:
			registers_general[oper1] = ((memory[oper2 + oper3 - 1] + 1) << 3*8) + 
			((memory[oper2 + oper3] + 1) << 2*8) + 
			((memory[oper2 + oper3 + 1] + 1) << 8) + 
			memory[oper2 + oper3 + 2];
			break;
			
			case 2:
			registers_general[oper1] = ((memory[oper2 + oper3 - 2] + 1) << 3*8) + 
			((memory[oper2 + oper3 - 1] + 1) << 2*8) + 
			((memory[oper2 + oper3] + 1) << 8) + 
			memory[oper2 + oper3 + 1];
			break;
			
			case 3:
			registers_general[oper1] = ((memory[oper2 + oper3 - 3] + 1) << 3*8) + 
			((memory[oper2 + oper3 - 2] + 1) << 2*8) + 
			((memory[oper2 + oper3 - 1] + 1) << 8) + 
			memory[oper2 + oper3];
			break;
		}
	}
	else
	{
		switch(oper3%4)
		{
			case 0: 
			registers_general[oper1] = (memory[oper2 + oper3] << 3*8) + 
			((memory[oper2 + oper3 + 1]) << 2*8) + 
			(memory[oper2 + oper3 + 2] << 8) + 
			memory[oper2 + oper3 + 3];
			break;
			
			case 1:
			registers_general[oper1] = (memory[oper2 + oper3 - 1] << 3*8) + 
			(memory[oper2 + oper3] << 2*8) + 
			(memory[oper2 + oper3 + 1] << 8) + 
			memory[oper2 + oper3 + 2];
			break;
			
			case 2:
			registers_general[oper1] = (memory[oper2 + oper3 - 2] << 3*8) + 
			(memory[oper2 + oper3 - 1] << 2*8) + 
			(memory[oper2 + oper3] << 8) + 
			memory[oper2 + oper3 + 1];
			break;
				
			case 3:
			registers_general[oper1] = (memory[oper2 + oper3 - 3] << 3*8) + 
			(memory[oper2 + oper3 - 2] << 2*8) + 
			(memory[oper2 + oper3 - 1] << 8) + 
			memory[oper2 + oper3];
			break;	
				
		}
	}
	printf("%.16llx\n",registers_general[oper1]);
}

void ldo (int oper1, int oper2, int oper3)
{
	switch(oper3 % 8)
	{
		case 0:
		registers_general[oper1] = memory[oper2 + oper3]*pow(16,14) + memory[oper2 + oper3 + 1] * pow(16,12) +
		memory[oper2 + oper3 + 2] * pow(16,10) + (memory[oper2 + oper3 + 3]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 + 4]+1) * pow(16,6) + (memory[oper2 + oper3 + 5]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 6]+1) * pow(16,2) + memory[oper2 + oper3 + 7];
		registers_general[oper1] -= 1; 
		break;

		case 1:
		registers_general[oper1] = memory[oper2 + oper3 - 1]*pow(16,14) + memory[oper2 + oper3] * pow(16,12) +
		memory[oper2 + oper3 + 1] * pow(16,10) + (memory[oper2 + oper3 + 2]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 + 3]+1) * pow(16,6) + (memory[oper2 + oper3 + 4]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 5]+1) * pow(16,2) + memory[oper2 + oper3 + 6];
		registers_general[oper1] -= 1; 
		break;

		case 2:
		registers_general[oper1] = memory[oper2 + oper3 - 2]*pow(16,14) + memory[oper2 + oper3 - 1] * pow(16,12) +
		memory[oper2 + oper3] * pow(16,10) + (memory[oper2 + oper3 + 1]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 + 2]+1) * pow(16,6) + (memory[oper2 + oper3 + 3]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 4]+1) * pow(16,2) + memory[oper2 + oper3 + 5];
		registers_general[oper1] -= 1; 
		break;

		case 3:
		registers_general[oper1] = memory[oper2 + oper3 - 3]*pow(16,14) + memory[oper2 + oper3 - 2] * pow(16,12) +
		memory[oper2 + oper3 - 1] * pow(16,10) + (memory[oper2 + oper3]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 + 1]+1) * pow(16,6) + (memory[oper2 + oper3 + 2]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 3]+1) * pow(16,2) + memory[oper2 + oper3 + 4];
		registers_general[oper1] -= 1; 
		break;

		case 4:
		registers_general[oper1] = memory[oper2 + oper3 - 4]*pow(16,14) + memory[oper2 + oper3 - 3] * pow(16,12) +
		memory[oper2 + oper3 - 2] * pow(16,10) + (memory[oper2 + oper3 - 1]+1) * pow(16, 8) + 
		(memory[oper2 + oper3]+1) * pow(16,6) + (memory[oper2 + oper3 + 1]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 2]+1) * pow(16,2) + memory[oper2 + oper3 + 3];
		registers_general[oper1] -= 1; 
		break;

		case 5:
		registers_general[oper1] = memory[oper2 + oper3 - 5]*pow(16,14) + memory[oper2 + oper3 - 4] * pow(16,12) +
		memory[oper2 + oper3 - 3] * pow(16,10) + (memory[oper2 + oper3 - 2]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 - 1]+1) * pow(16,6) + (memory[oper2 + oper3]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 1]+1) * pow(16,2) + memory[oper2 + oper3 + 2];
		registers_general[oper1] -= 1; 
		break;

		case 6:
		registers_general[oper1] = memory[oper2 + oper3 - 6]*pow(16,14) + memory[oper2 + oper3 - 5] * pow(16,12) +
		memory[oper2 + oper3 - 4] * pow(16,10) + (memory[oper2 + oper3 - 3]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 - 2]+1) * pow(16,6) + (memory[oper2 + oper3 - 1]+1) * pow(16,4) +
		(memory[oper2 + oper3]+1) * pow(16,2) + memory[oper2 + oper3 + 1];
		registers_general[oper1] -= 1; 
		break;

		case 7:
		registers_general[oper1] = memory[oper2 + oper3 - 7]*pow(16,14) + memory[oper2 + oper3 - 6] * pow(16,12) +
		memory[oper2 + oper3 - 5] * pow(16,10) + (memory[oper2 + oper3 - 4]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 - 3]+1) * pow(16,6) + (memory[oper2 + oper3 - 2]+1) * pow(16,4) +
		(memory[oper2 + oper3 - 1]+1) * pow(16,2) + memory[oper2 + oper3];
		registers_general[oper1] -= 1; 
		break;

	}
	printf("%.16llx\n",registers_general[oper1]);
}

//data loading(unsigned) to registers
void ldbu (int oper1, int oper2, int oper3)
{
	registers_general[oper1] = (unsigned char)memory[oper2 + oper3];
	printf("%.16llx\n",registers_general[oper1]);
}

void ldwu (int oper1, int oper2, int oper3)
{
	if(oper3/4 == 1)
	{
		if(oper3%2 == 1)
		{
			registers_general[oper1] = (unsigned char)memory[oper2 + oper3] + (((unsigned char)memory[oper2 + oper3 - 1]) << 8);
		}
		else
		{
			registers_general[oper1] = (((unsigned char)memory[oper2 + oper3]) << 8) + (unsigned char)memory[oper2 + oper3 + 1];
		}
	}
	else
	{
		if(oper3%2 == 1)
		{
			registers_general[oper1] = (unsigned char)memory[oper2 + oper3] + ((unsigned char)memory[oper2 + oper3 - 1] << 8);
		}
		else
		{
			registers_general[oper1] = ((unsigned char)memory[oper2 + oper3] << 8) + (unsigned char)memory[oper2 + oper3 + 1];
		}
	}
	printf("%.16llx\n",registers_general[oper1]);

}

void ldtu (int oper1, int oper2, int oper3)
{
	if(oper3 / 4 == 1)
	{
		switch(oper3 % 4)
		{
			case 0: 
			registers_general[oper1] = ((((unsigned int)memory[oper2 + oper3]+1) << 3*8) + 
			((unsigned int)(memory[oper2 + oper3 + 1]+1) << 2*8) + 
			((unsigned int)(memory[oper2 + oper3 + 2]+1) << 8) + 
			(unsigned int)memory[oper2 + oper3 + 3]);
			break;
			
			case 1:
			registers_general[oper1] = (((unsigned int)memory[oper2 + oper3 - 1]+1) << 3*8) + 
			((unsigned int)(memory[oper2 + oper3]+1) << 2*8) + 
			((unsigned int)(memory[oper2 + oper3 + 1]+1) << 8) + 
			(unsigned int)memory[oper2 + oper3 + 2];
			break;
			
			case 2:
			registers_general[oper1] = (((unsigned int)memory[oper2 + oper3 - 2]+1) << 3*8) + 
			(((unsigned int)memory[oper2 + oper3 - 1]+1) << 2*8) + 
			(((unsigned int)memory[oper2 + oper3]+1) << 8) + 
			(unsigned int)memory[oper2 + oper3 + 1];
			break;
			
			case 3:
			registers_general[oper1] = (((unsigned int)memory[oper2 + oper3 - 3]+1) << 3*8) + 
			(((unsigned int)memory[oper2 + oper3 - 2]+1) << 2*8) + 
			(((unsigned int)memory[oper2 + oper3 - 1]+1) << 8) + 
			(unsigned int)memory[oper2 + oper3];
			break;
		}
	}
	else
	{
		switch(oper3%4)
		{
			case 0: 
			registers_general[oper1] = ((unsigned char)memory[oper2 + oper3] << 3*8) + 
			(((unsigned char)memory[oper2 + oper3 + 1]) << 2*8) + 
			((unsigned char)memory[oper2 + oper3 + 2] << 8) + 
			(unsigned char)memory[oper2 + oper3 + 3];
			break;
			
			case 1:
			registers_general[oper1] = ((unsigned char)memory[oper2 + oper3 - 1] << 3*8) + 
			((unsigned char)memory[oper2 + oper3] << 2*8) + 
			((unsigned char)memory[oper2 + oper3 + 1] << 8) + 
			(unsigned char)memory[oper2 + oper3 + 2];
			break;
			
			case 2:
			registers_general[oper1] = ((unsigned char)memory[oper2 + oper3 - 2] << 3*8) + 
			((unsigned char)memory[oper2 + oper3 - 1] << 2*8) + 
			((unsigned char)memory[oper2 + oper3] << 8) + 
			(unsigned char)memory[oper2 + oper3 + 1];
			break;
				
			case 3:
			registers_general[oper1] = ((unsigned char)memory[oper2 + oper3 - 3] << 3*8) + 
			((unsigned char)memory[oper2 + oper3 - 2] << 2*8) + 
			((unsigned char)memory[oper2 + oper3 - 1] << 8) + 
			(unsigned char)memory[oper2 + oper3];
			break;	
				
		}
	}
	printf("%.16llx\n",registers_general[oper1]);
}
void ldou (int oper1, int oper2, int oper3)
{
	switch(oper3 % 8)
	{
		case 0:
		registers_general[oper1] = (unsigned long long)(memory[oper2 + oper3]*pow(16,14) + memory[oper2 + oper3 + 1] * pow(16,12) +
		memory[oper2 + oper3 + 2] * pow(16,10) + (memory[oper2 + oper3 + 3]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 + 4]+1) * pow(16,6) + (memory[oper2 + oper3 + 5]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 6]+1) * pow(16,2) + memory[oper2 + oper3 + 7]);
		registers_general[oper1] -= 1; 
		break;

		case 1:
		registers_general[oper1] = (unsigned long long)(memory[oper2 + oper3 - 1]*pow(16,14) + memory[oper2 + oper3] * pow(16,12) +
		memory[oper2 + oper3 + 1] * pow(16,10) + (memory[oper2 + oper3 + 2]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 + 3]+1) * pow(16,6) + (memory[oper2 + oper3 + 4]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 5]+1) * pow(16,2) + memory[oper2 + oper3 + 6]);
		registers_general[oper1] -= 1; 
		break;

		case 2:
		registers_general[oper1] = (unsigned long long)(memory[oper2 + oper3 - 2]*pow(16,14) + memory[oper2 + oper3 - 1] * pow(16,12) +
		memory[oper2 + oper3] * pow(16,10) + (memory[oper2 + oper3 + 1]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 + 2]+1) * pow(16,6) + (memory[oper2 + oper3 + 3]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 4]+1) * pow(16,2) + memory[oper2 + oper3 + 5]);
		registers_general[oper1] -= 1; 
		break;

		case 3:
		registers_general[oper1] = (unsigned long long)(memory[oper2 + oper3 - 3]*pow(16,14) + memory[oper2 + oper3 - 2] * pow(16,12) +
		memory[oper2 + oper3 - 1] * pow(16,10) + (memory[oper2 + oper3]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 + 1]+1) * pow(16,6) + (memory[oper2 + oper3 + 2]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 3]+1) * pow(16,2) + memory[oper2 + oper3 + 4]);
		registers_general[oper1] -= 1; 
		break;

		case 4:
		registers_general[oper1] = (unsigned long long)(memory[oper2 + oper3 - 4]*pow(16,14) + memory[oper2 + oper3 - 3] * pow(16,12) +
		memory[oper2 + oper3 - 2] * pow(16,10) + (memory[oper2 + oper3 - 1]+1) * pow(16, 8) + 
		(memory[oper2 + oper3]+1) * pow(16,6) + (memory[oper2 + oper3 + 1]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 2]+1) * pow(16,2) + memory[oper2 + oper3 + 3]);
		registers_general[oper1] -= 1; 
		break;

		case 5:
		registers_general[oper1] = (unsigned long long)(memory[oper2 + oper3 - 5]*pow(16,14) + memory[oper2 + oper3 - 4] * pow(16,12) +
		memory[oper2 + oper3 - 3] * pow(16,10) + (memory[oper2 + oper3 - 2]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 - 1]+1) * pow(16,6) + (memory[oper2 + oper3]+1) * pow(16,4) +
		(memory[oper2 + oper3 + 1]+1) * pow(16,2) + memory[oper2 + oper3 + 2]);
		registers_general[oper1] -= 1; 
		break;

		case 6:
		registers_general[oper1] = (unsigned long long)(memory[oper2 + oper3 - 6]*pow(16,14) + memory[oper2 + oper3 - 5] * pow(16,12) +
		memory[oper2 + oper3 - 4] * pow(16,10) + (memory[oper2 + oper3 - 3]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 - 2]+1) * pow(16,6) + (memory[oper2 + oper3 - 1]+1) * pow(16,4) +
		(memory[oper2 + oper3]+1) * pow(16,2) + memory[oper2 + oper3 + 1]);
		registers_general[oper1] -= 1; 
		break;

		case 7:
		registers_general[oper1] = (unsigned long long)(memory[oper2 + oper3 - 7]*pow(16,14) + memory[oper2 + oper3 - 6] * pow(16,12) +
		memory[oper2 + oper3 - 5] * pow(16,10) + (memory[oper2 + oper3 - 4]+1) * pow(16, 8) + 
		(memory[oper2 + oper3 - 3]+1) * pow(16,6) + (memory[oper2 + oper3 - 2]+1) * pow(16,4) +
		(memory[oper2 + oper3 - 1]+1) * pow(16,2) + memory[oper2 + oper3]);
		registers_general[oper1] -= 1; 
		break;

	}
	printf("%.16llx\n",registers_general[oper1]);
}

void ldht (int oper1, int oper2, int oper3)
{
	if(oper3/4==0)
	{
		switch(oper3%4)
		{
			case 0: 
			registers_general[oper1] = (memory[oper2 + oper3]*pow(16,14)) + 
			(memory[oper2 + oper3 + 1]*pow(16,12)) + 
			(memory[oper2 + oper3 + 2]*pow(16,10)) + 
			(memory[oper2 + oper3 + 3]*pow(16,8)) ;
			break;
			
			case 1:
			registers_general[oper1] = (memory[oper2 + oper3 - 1]*pow(16,14)) + 
			(memory[oper2 + oper3]*pow(16,12)) + 
			(memory[oper2 + oper3 + 1]*pow(16,10)) + 
			memory[oper2 + oper3 + 2]*pow(16,8);
			break;
			
			case 2:
			registers_general[oper1] = (memory[oper2 + oper3 - 2]*pow(16,14)) + 
			(memory[oper2 + oper3 - 1]*pow(16,12)) + 
			(memory[oper2 + oper3]*pow(16,10)) + 
			memory[oper2 + oper3 + 1]*pow(16,8);
			break;
				
			case 3:
			registers_general[oper1] = (memory[oper2 + oper3 - 3]*pow(16,14)) + 
			(memory[oper2 + oper3 - 2]*pow(16,12)) + 
			(memory[oper2 + oper3 - 1]*pow(16,10)) + 
			memory[oper2 + oper3]*pow(16,8);
			break;	
		}
	}
	else
	{
		switch(oper3%4)
		{
			case 0: 
			registers_general[oper1] = (memory[oper2 + oper3]+1)*pow(16,14) + 
			(memory[oper2 + oper3 + 1]+1)*pow(16,12) + 
			(memory[oper2 + oper3 + 2]+1)*pow(16,10) + 
			(memory[oper2 + oper3 + 3]*pow(16,8)) ;
			break;
			
			case 1:
			registers_general[oper1] = (memory[oper2 + oper3 - 1]+1)*pow(16,14) + 
			(memory[oper2 + oper3]+1)*pow(16,12) + 
			(memory[oper2 + oper3 + 1]+1)*pow(16,10) + 
			memory[oper2 + oper3 + 2]*pow(16,8);
			break;
			
			case 2:
			registers_general[oper1] = (memory[oper2 + oper3 - 2]+1)*pow(16,14) + 
			(memory[oper2 + oper3 - 1]+1)*pow(16,12) + 
			(memory[oper2 + oper3]+1)*pow(16,10) + 
			memory[oper2 + oper3 + 1]*pow(16,8);
			break;
				
			case 3:
			registers_general[oper1] = (memory[oper2 + oper3 - 3]+1)*pow(16,14) + 
			(memory[oper2 + oper3 - 2]+1)*pow(16,12) + 
			(memory[oper2 + oper3 - 1]+1)*pow(16,10) + 
			memory[oper2 + oper3]*pow(16,8);
			break;	
		}
	}
	printf("%.16llx\n",registers_general[oper1]);
}
//loading address
void lda (int oper1, int oper2, int oper3)
{
	registers_general[oper1] = oper2 + oper3;
}

//data loading(signed) to memory 
void stb (int oper1, int oper2, int oper3)
{
	memory[oper2+oper3] = (char)registers_general[oper1];
}

void stw (int oper1, int oper2, int oper3)
{
	short int k = (short int)registers_general[oper1];
	
	if(oper3%2==0)
	{
		memory[oper2+oper3] = k/(16*16);
		memory[oper2+oper3+1] = (char)k; 
	}
	else
	{
		memory[oper2+oper3-1] = k/(16*16);
		memory[oper2+oper3] = (char)k; 
	}
}

void stt (int oper1, int oper2, int oper3)
{
	int k = (int)registers_general[oper1];
	printf("%x\n",k);
	switch(oper3%4)
	{
		case 0:
		memory[oper2+oper3] = k /(16*16*16*16*16*16)-1;		
		memory[oper2+oper3+1] = k /(16*16*16*16);
		memory[oper2+oper3+2] = k /(16*16);
		memory[oper2+oper3+3] = (char)k;
		break;
		
		case 1:
		memory[oper2+oper3-1] = k /(16*16*16*16*16*16)-1;
		memory[oper2+oper3] = k /(16*16*16*16);
		memory[oper2+oper3+1] = k /(16*16);
		memory[oper2+oper3+2] = (char)k;
		break;
		
		case 2:
		memory[oper2+oper3-2] = k /(16*16*16*16*16*16)-1;
		memory[oper2+oper3-1] = k /(16*16*16*16);
		memory[oper2+oper3] = k /(16*16);
		memory[oper2+oper3+1] = (char)k;;
		break;
		
		case 3:
		memory[oper2+oper3-3] = k /(16*16*16*16*16*16)-1;
		memory[oper2+oper3-2] = k /(16*16*16*16);
		memory[oper2+oper3-1] = k /(16*16);
		memory[oper2+oper3] = (char)k;
		break;
	}
}

void sto (int oper1, int oper2, int oper3)
{
	long long int k = registers_general[oper1];
	printf("%llx\n",k);
	switch(oper3%8)
	{
		case 0:
		memory[oper2+oper3] = k / pow(16,14)-1;		
		memory[oper2+oper3+1] = k /pow(16,12)-1;		
		memory[oper2+oper3+2] = k /pow(16,10)-1;		
		memory[oper2+oper3+3] = k /pow(16,8)-1;		
		memory[oper2+oper3+4] = k /pow(16,6)-1;		
		memory[oper2+oper3+5] = (k /pow(16,4))-1;
		memory[oper2+oper3+6] = k /(16*16)-1;
		memory[oper2+oper3+7] = (char)k;
		break;
		
		case 1:
		memory[oper2+oper3-1] = k / pow(16,14)-1;		
		memory[oper2+oper3] = k /pow(16,12)-1;		
		memory[oper2+oper3+1] = k /pow(16,10)-1;		
		memory[oper2+oper3+2] = k /pow(16,8)-1;		
		memory[oper2+oper3+3] = k /pow(16,6)-1;		
		memory[oper2+oper3+4] = k /pow(16,4)-1;
		memory[oper2+oper3+5] = k /(16*16)-1;
		memory[oper2+oper3+6] = (char)k;
		break;
		
		case 2:
		memory[oper2+oper3-2] = k / pow(16,14)-1;		
		memory[oper2+oper3-1] = k /pow(16,12)-1;		
		memory[oper2+oper3] = k /pow(16,10)-1;		
		memory[oper2+oper3+1] = k /pow(16,8)-1;		
		memory[oper2+oper3+2] = k /pow(16,6)-1;		
		memory[oper2+oper3+3] = k /pow(16,4)-1;
		memory[oper2+oper3+4] = k /(16*16)-1;
		memory[oper2+oper3+5] = (char)k;
		break;
		
		case 3:
		memory[oper2+oper3-3] = k / pow(16,14)-1;		
		memory[oper2+oper3-2] = k /pow(16,12)-1;		
		memory[oper2+oper3-1] = k /pow(16,10)-1;		
		memory[oper2+oper3] = k /pow(16,8)-1;		
		memory[oper2+oper3+1] = k /pow(16,6)-1;		
		memory[oper2+oper3+2] = k /pow(16,4)-1;
		memory[oper2+oper3+3] = k /(16*16)-1;
		memory[oper2+oper3+4] = (char)k;
		break;
		
		case 4:
		memory[oper2+oper3-4] = k / pow(16,14)-1;		
		memory[oper2+oper3-3] = k /pow(16,12)-1;		
		memory[oper2+oper3-2] = k /pow(16,10)-1;		
		memory[oper2+oper3-1] = k /pow(16,8)-1;		
		memory[oper2+oper3] = k /pow(16,6)-1;		
		memory[oper2+oper3+1] = k /pow(16,4)-1;
		memory[oper2+oper3+2] = k /(16*16)-1;
		memory[oper2+oper3+3] = (char)k;
		break;
		
		case 5:
		memory[oper2+oper3-5] = k / pow(16,14)-1;		
		memory[oper2+oper3-4] = k /pow(16,12)-1;		
		memory[oper2+oper3-3] = k /pow(16,10)-1;		
		memory[oper2+oper3-2] = k /pow(16,8)-1;		
		memory[oper2+oper3-1] = k /pow(16,6)-1;		
		memory[oper2+oper3] = k /pow(16,4)-1;
		memory[oper2+oper3+1] = k /(16*16)-1;
		memory[oper2+oper3+2] = (char)k;
		break;
		
		case 6:
		memory[oper2+oper3-6] = k / pow(16,14)-1;		
		memory[oper2+oper3-5] = k /pow(16,12)-1;		
		memory[oper2+oper3-4] = k /pow(16,10)-1;		
		memory[oper2+oper3-3] = k /pow(16,8)-1;		
		memory[oper2+oper3-2] = k /pow(16,6)-1;		
		memory[oper2+oper3-1] = k /pow(16,4)-1;
		memory[oper2+oper3] = k /(16*16)-1;
		memory[oper2+oper3+1] = (char)k;
		break;

		case 7:
		memory[oper2+oper3-7] = k / pow(16,14)-1;		
		memory[oper2+oper3-6] = k /pow(16,12)-1;		
		memory[oper2+oper3-5] = k /pow(16,10)-1;		
		memory[oper2+oper3-4] = k /pow(16,8)-1;		
		memory[oper2+oper3-3] = k /pow(16,6)-1;		
		memory[oper2+oper3-2] = k /pow(16,4)-1;
		memory[oper2+oper3-1] = k /(16*16)-1;
		memory[oper2+oper3] = (char)k;
		break;
	}
}

//data loading(unsigned) to memory
void stbu (int oper1, int oper2, int oper3)
{
	memory[oper2+oper3] = (unsigned char)registers_general[oper1];
}

void stwu (int oper1, int oper2, int oper3)
{
	unsigned short int k = (unsigned short int)registers_general[oper1];
	
	if(oper3%2==0)
	{
		memory[oper2+oper3] = k/(16*16);
		memory[oper2+oper3+1] = (unsigned char)k; 
	}
	else
	{
		memory[oper2+oper3-1] = k/(16*16);
		memory[oper2+oper3] = (unsigned char)k; 
	}
}

void sttu (int oper1, int oper2, int oper3)
{
	unsigned int k = (unsigned int)registers_general[oper1];
	printf("%x\n",k);
	
	switch(oper3%4)
	{
		case 0:
		memory[oper2+oper3] = k /(16*16*16*16*16*16);		
		memory[oper2+oper3+1] = k /(16*16*16*16);
		memory[oper2+oper3+2] = k /(16*16);
		memory[oper2+oper3+3] = (unsigned char)k;
		break;
		
		case 1:
		memory[oper2+oper3-1] = k /(16*16*16*16*16*16);
		memory[oper2+oper3] = k /(16*16*16*16);
		memory[oper2+oper3+1] = k /(16*16);
		memory[oper2+oper3+2] = (unsigned char)k;
		break;
		
		case 2:
		memory[oper2+oper3-2] = k /(16*16*16*16*16*16);
		memory[oper2+oper3-1] = k /(16*16*16*16);
		memory[oper2+oper3] = k /(16*16);
		memory[oper2+oper3+1] = (unsigned char)k;;
		break;
		
		case 3:
		memory[oper2+oper3-3] = k /(16*16*16*16*16*16);
		memory[oper2+oper3-2] = k /(16*16*16*16);
		memory[oper2+oper3-1] = k /(16*16);
		memory[oper2+oper3] = (unsigned char)k;
		break;
	}
}

void stou (int oper1, int oper2, int oper3)
{
	unsigned long long int k = registers_general[oper1];
	printf("%llx\n",k);
	
	switch(oper3%8)
	{
		case 0:
		memory[oper2+oper3] = k / (unsigned long long)pow(16,14);		
		memory[oper2+oper3+1] = k /(unsigned long long)pow(16,12);		
		memory[oper2+oper3+2] = k /(unsigned long long)pow(16,10);		
		memory[oper2+oper3+3] = (k /(unsigned long long)pow(16,8));		
		memory[oper2+oper3+4] = k /(unsigned long long)pow(16,6);		
		memory[oper2+oper3+5] = k /(unsigned long long)pow(16,4);
		memory[oper2+oper3+6] = k /(16*16);
		memory[oper2+oper3+7] = (unsigned char)k;
		break;
		
		case 1:
		memory[oper2+oper3-1] = k / (unsigned long long)pow(16,14);		
		memory[oper2+oper3] = k /(unsigned long long)pow(16,12);		
		memory[oper2+oper3+1] = k /(unsigned long long)pow(16,10);		
		memory[oper2+oper3+2] = k /(unsigned long long)pow(16,8);		
		memory[oper2+oper3+3] = k /(unsigned long long)pow(16,6);		
		memory[oper2+oper3+4] = k /(unsigned long long)pow(16,4);
		memory[oper2+oper3+5] = k /(16*16);
		memory[oper2+oper3+6] = (unsigned char)k;
		break;
		
		case 2:
		memory[oper2+oper3-2] = k / (unsigned long long)pow(16,14);		
		memory[oper2+oper3-1] = k /(unsigned long long)pow(16,12);		
		memory[oper2+oper3] = k /(unsigned long long)pow(16,10);		
		memory[oper2+oper3+1] = k /(unsigned long long)pow(16,8);		
		memory[oper2+oper3+2] = k /(unsigned long long)pow(16,6);		
		memory[oper2+oper3+3] = k /(unsigned long long)pow(16,4);
		memory[oper2+oper3+4] = k /(16*16);
		memory[oper2+oper3+5] = (unsigned char)k;
		break;
		
		case 3:
		memory[oper2+oper3-3] = k / (unsigned long long)pow(16,14);		
		memory[oper2+oper3-2] = k /(unsigned long long)pow(16,12);		
		memory[oper2+oper3-1] = k /(unsigned long long)pow(16,10);		
		memory[oper2+oper3] = k /(unsigned long long)pow(16,8);		
		memory[oper2+oper3+1] = k /(unsigned long long)pow(16,6);		
		memory[oper2+oper3+2] = k /(unsigned long long)pow(16,4);
		memory[oper2+oper3+3] = k /(16*16);
		memory[oper2+oper3+4] = (unsigned char)k;
		break;
		
		case 4:
		memory[oper2+oper3-4] = k / (unsigned long long)pow(16,14);		
		memory[oper2+oper3-3] = k /(unsigned long long)pow(16,12);		
		memory[oper2+oper3-2] = k /(unsigned long long)pow(16,10);		
		memory[oper2+oper3-1] = k /(unsigned long long)pow(16,8);		
		memory[oper2+oper3] = k /(unsigned long long)pow(16,6);		
		memory[oper2+oper3+1] = k /(unsigned long long)pow(16,4);
		memory[oper2+oper3+2] = k /(16*16);
		memory[oper2+oper3+3] = (unsigned char)k;
		break;
		
		case 5:
		memory[oper2+oper3-5] = k / (unsigned long long)pow(16,14);		
		memory[oper2+oper3-4] = k /(unsigned long long)pow(16,12);		
		memory[oper2+oper3-3] = k /(unsigned long long)pow(16,10);		
		memory[oper2+oper3-2] = k /(unsigned long long)pow(16,8);		
		memory[oper2+oper3-1] = k /(unsigned long long)pow(16,6);		
		memory[oper2+oper3] = k /(unsigned long long)pow(16,4);
		memory[oper2+oper3+1] = k /(16*16);
		memory[oper2+oper3+2] = (unsigned char)k;
		break;
		
		case 6:
		memory[oper2+oper3-6] = k / (unsigned long long)pow(16,14);		
		memory[oper2+oper3-5] = k /(unsigned long long)pow(16,12);		
		memory[oper2+oper3-4] = k /(unsigned long long)pow(16,10);		
		memory[oper2+oper3-3] = k /(unsigned long long)pow(16,8);		
		memory[oper2+oper3-2] = k /(unsigned long long)pow(16,6);		
		memory[oper2+oper3-1] = k /(unsigned long long)pow(16,4);
		memory[oper2+oper3] = k /(16*16);
		memory[oper2+oper3+1] = (unsigned char)k;
		break;

		case 7:
		memory[oper2+oper3-7] = k / (unsigned long long)pow(16,14);		
		memory[oper2+oper3-6] = k /(unsigned long long)pow(16,12);		
		memory[oper2+oper3-5] = k /(unsigned long long)pow(16,10);		
		memory[oper2+oper3-4] = k /(unsigned long long)pow(16,8);		
		memory[oper2+oper3-3] = k /(unsigned long long)pow(16,6);		
		memory[oper2+oper3-2] = k /(unsigned long long)pow(16,4);
		memory[oper2+oper3-1] = k /(16*16);
		memory[oper2+oper3] = (unsigned char)k;
		break;
	}
}

int main(){
	int oper2 = 0x0000;
	int oper3;
	scanf("%d",&oper3);
	
	memory[0x0000] = 0x01;
	memory[0x0001] = 0x23;
	memory[0x0002] = 0x45;
	memory[0x0003] = 0x67;
	memory[0x0004] = 0x89;
	memory[0x0005] = 0xab;
	memory[0x0006] = 0xcd;
	memory[0x0007] = 0xef;
	
	registers_general[2]=0xffcffffaffff000a;
	printf("%.16llx\n",registers_general[2]);
	
	sto(2,oper2,oper3);
	
	printf("\n");
	for(int i=0;i<8;i++)
	{
		printf("%.2x\n",memory[oper2+i]);
	}
	
	printf("\n");
	return 0;
}
/*	int opcode[4];
	FILE* f;
	char fname[] = "file.o";
	
	
	if((f = fopen(fname,"r")) != NULL)
	{
		while(1)
		{	
			for(int i = 0; i<4; i++)
			{
				fscanf(f,"%x",&opcode[i]);
				printf("%x ",opcode[i]);
				analise(opcode);
			}
			if(feof(f)) break;
		}
	}*/
