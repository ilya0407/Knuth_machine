#include "stdio.h"
#include "knut_asm_1.c"
#include "knut_asm_2.c"
#include <stdlib.h>
#include <string.h>

int atoi_hex(char s[])
{
	int n = 0;
	int mnoz = 1;
	if(s[0]=='$' || s[0]=='#')
	{
		for(int i = strlen(s)-1; i>=1; i--)
		{
			n += mnoz * (s[i]-48);
			mnoz *= 16;
		}
	}
	else
	{
		for(int i = strlen(s)-1; i>=0; i--)
		{
			n += mnoz * (s[i]-48);
			mnoz *= 16;
		}
	}
	return n;
}

int atoi_(char* s)
{
	char* str = s;
	int k;
	if (str[0] == '$')
		str[0] = ' ';
	
	k = atoi(str);
	return k;
}

typedef struct 
{
	unsigned char* metka;
	int addr_or_eq;
}label;


typedef struct
{
	char op1op2op3[3];
	unsigned char code_oper;
}instruction;

int line_number = 0;

char* psoperators[] = {"LOC","IS","GREG","BYTE","WYDE","TETRA","OCTA"};

int flag_loc = 0;

int counter_label = 0;

label mas_label[20];

void row_count(unsigned char* oper, unsigned char* op1)
{
	if(strcmp((char*)oper,psoperators[0])==0)
	{
		line_number = atoi_hex((char*)op1);
		flag_loc = 1;
	}
	else 
		if(flag_loc == 1) line_number += 4;
}

void print_num_row()
{
	if(flag_loc==1)
		printf("%x\t",line_number);
	else
		printf("\t");
}

void sign_label(unsigned char* met, unsigned char* oper, unsigned char* op1)
{
	if(met != NULL)
	{
		mas_label[counter_label].metka = met;
		if(strcmp((char*)oper,psoperators[1]) == 0)
		{
			mas_label[counter_label].addr_or_eq = atoi_((char*)op1);
		}
		else mas_label[counter_label].addr_or_eq = line_number;
		counter_label++;
	}
}

void make_instruction(instruction instr, unsigned char* oper, unsigned char* op1, unsigned char* op2, unsigned char* op3)
{
		instr.code_oper = find_in_hash(oper);
		int flag1 = 0, flag2 = 0, flag3 = 0;
		
		for(int j = 0; j<counter_label; j++)
		{
			if(op1 != NULL)
			{	
				if(strcmp((char*)op1,(char*)mas_label[j].metka) == 0)
				{
					instr.op1op2op3[0] = mas_label[j].addr_or_eq;
					flag1 = 1;
				}
				else 
					if(flag1 == 0)
						instr.op1op2op3[0] = atoi_((char*)op1);
			}
		
			if(op2 != NULL)
			{
				if(strcmp((char*)op2,(char*)mas_label[j].metka) == 0)
				{
					if(instr.code_oper >= 0x40 && instr.code_oper<= 0x5f) //fixing conditional transitions
					{
						instr.op1op2op3[2] = (mas_label[j].addr_or_eq - line_number)/4;
						if(instr.op1op2op3[2] < 0) instr.op1op2op3[1] = -1;
						else instr.op1op2op3[1] = 0;
					}
					else
						instr.op1op2op3[1] = mas_label[j].addr_or_eq;
					flag2 = 1;
				}
				else 
					if(flag2 == 0)
						instr.op1op2op3[1] = atoi_((char*)op2);
			}
			
			if(op3 != NULL)
			{
				if(strcmp((char*)op3,(char*)mas_label[j].metka) == 0)
				{
					instr.op1op2op3[2] = mas_label[j].addr_or_eq;
					flag3 = 1;
				}
				else 
					if(flag3 == 0)
					{
						instr.op1op2op3[2] = atoi_((char*)op3);
					//	instr.code_oper++;
					}
			}
		}
		printf("%x\t%.2x %.2x %.2x %.2x\t\t%s %s %s %s\n",line_number,instr.code_oper, instr.op1op2op3[0], instr.op1op2op3[1], instr.op1op2op3[2],oper,op1,op2,op3);
}

int main(int argc,char** argv)
{
	hash(hashtab,W,lst);
	
	FILE* f;
	unsigned char* str = (unsigned char*)malloc(256);
	char* name = argv[1];
	name = strtok(name,".");
	strcat(name,".mmo");
	instruction instr;
			
	if((f = fopen(argv[1], "r")) != NULL) 
	{	
		while(1)
		{
			fgets((char*)str,300,f);
			str[strlen((char*)str)-1] = ' ';
			if(feof(f)==1)
				break;
			print_num_row();
			unsigned char* met = metka(str);
			unsigned char* oper = operator(str);
			unsigned char* op1 = operand1(str);
			unsigned char* op2 = operand1(str);
			unsigned char* op3 = operand2(str);
			unsigned char* com = comment(str);
			print(met,oper,op1,op2,op3,com);
			sign_label(met,oper,op1);
			row_count(oper,op1);
		}
		
		flag_loc = 0;
		line_number = 0;
		
		freopen(name,"r",f);
		while(1)
		{
			fgets((char*)str,300,f);
			str[strlen((char*)str)-1] = ' ';
			if(feof(f)==1)
				break;
			unsigned char* met = metka(str);
			unsigned char* oper = operator(str);
			unsigned char* op1 = operand1(str);
			unsigned char* op2 = operand1(str);
			unsigned char* op3 = operand2(str);
			unsigned char* com = comment(str);
			
			if(flag_loc == 1)
				make_instruction(instr,oper,op1,op2,op3);
		
			if(strcmp((char*)oper,psoperators[0]) == 0)
			{
				flag_loc = 1;
			}
			row_count(oper,op1);
		}
	}
	return 0;
}
