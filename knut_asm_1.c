#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 

unsigned char* metka(unsigned char* str)
{
	int i = 0;
	int size = 0;
	
	while(str[i] != '\t')
	{
		size++;
		i++;
	}
	
	unsigned char* metka = (unsigned char*)malloc(size);
	
	if(size != 0)
	{
		for(i = 0; i < size; i++)
		{
			metka[i] = str[i];
			str[i] = ' ';
		}
		return metka;
	}
	else
		return NULL;
}

unsigned char* comment(unsigned char* str)
{
	int place = strlen((char*)str);
	for(int i = 0; i<strlen((char*)str); i++)
	{
		if(str[i]=='/')
			{
				place = i+1;
				str[i] = ' ';
			}
	}
	unsigned char* com = (unsigned char*)malloc(strlen((char*)str));
	if(place != strlen((char*)str))
	{
		for(int i = place; i<strlen((char*)str); i++)
		{
			com[i-place] = str[i];
			str[i] = ' ';
		}
		return com;
	}
	else
		return NULL;
}

unsigned char* operator(unsigned char* str)
{
	unsigned char* oper = (unsigned char*)malloc(strlen((char*)str));
	int k = 0;
	for(int i = 0; i < strlen((char*)str); i++)
	{
		if(str[i] != ' ' && str[i] != '\t')
		{
			oper[k] = str[i]; 
			str[i] = ' ';
			k++;
			if(str[i+1] == ' ' || str[i+1] == '\t' || str[i+1] == '\n')
				break;
		}
	}
	return oper;
}

unsigned char* operand1(unsigned char* str)
{
	unsigned char* opnd = (unsigned char*)malloc(strlen((char*)str));
	int k = 0;
	int flag = 0;
	
	for(int i = 0; i<strlen((char*)str); i++)
	{
		if(str[i] != ' '  && str[i] != '\n' && str[i] != '\t')
		{
			if(str[i] == '"' && flag == 0)
			{
				flag = 1;
				str[i] = ' ';
			}
			else 
				if(str[i] != '"' && flag == 1)
				{
					opnd[k] = str[i];
					str[i] = ' ';
					k++;
				}
				else
					if(str[i] == '"' && flag == 1)
					{
						str[i] = ' ';
						if(i != strlen((char*)str)-1)
							str[i+1] = ' ';
						flag = 0;
						break;
					}
					else
						if(str[i] == ',')
							{
								str[i] = ' ';
								break;
							}
						else
							{
								opnd[k] = str[i];
								str[i] = ' ';
								k++;
							}
		}
	}
	
	
	return opnd;
}

unsigned char* operand2(unsigned char* str)
{
	unsigned char* opnd = (unsigned char*)malloc(strlen((char*)str));
	int k = 0;
	int t = 0;
	for(int i = 0; i<strlen((char*)str); i++)
	{
		if(str[i] != ' '  && str[i] != '\n' && str[i] != '\t' && str[i] != '"')
		{
			t = 1;
			opnd[k] = str[i];
			k++;
		}
	}
	if(t == 0)
		return NULL;
	return opnd;
}

void print(unsigned char* metka,unsigned char* operator,unsigned char* operand1, unsigned char* operand2, unsigned char* operand3, unsigned char* comment)
{
	if(metka == NULL)
		metka = (unsigned char*)(" ");
	if(operator == NULL)
		operator = (unsigned char*)(" ");
	if(operand1 == NULL)
		operand1 = (unsigned char*)(" ");
	if(operand2 == NULL)
		operand2 = (unsigned char*)(" ");
	if(operand3 == NULL)
		operand3 = (unsigned char*)(" ");
	if(comment == NULL)
		comment = (unsigned char*)(" ");
	printf("Метка: %5s Оператор: %5s Операнд_1: %10s Операнд_2: %10s Операнд_3: %10s Комментарий:%s\n",metka,operator,operand1,operand2,operand3,comment);
}





