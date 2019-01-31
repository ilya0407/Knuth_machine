#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int number_macro = 0;
const int SIZE_TEXT = 20000;
const int NUMB_OP = 3;

typedef struct{
	char* name;
	int number_operands;
	char* text_macro;
	char** operands;
}macrooperator;

macrooperator* mas_macro;
	

void comment(char* str)
{
	int number = strlen(str);
	for(int i = 0; i<strlen(str); i++)
	{
		if(str[i] == ';')
		{
			number = i;
		}
	}
	
	str[number] = '\0';
}

int search(char* str, char ch)
{
	for(int i = 0; i<strlen(str); i++)
	{
		if(str[i] == ch)
		{
			return i;
		}
	}
	return -1;
}

void analise(char* str)
{
//  Searching for macro and counting
	char* str_cpy = (char*)malloc(strlen(str));
	strcpy(str_cpy,str);
	comment(str_cpy);

	char* p = strtok(str_cpy," ,\n");
	while(p != NULL)
	{
		if(strcmp(p,"MACRO") == 0)
		{
			number_macro++;
		}
		p = strtok(NULL," ,");
	}
	free(str_cpy);
}

void change(int num_macro, char** operands)
{
	char* text = (char*)malloc(strlen(mas_macro[num_macro].text_macro));
	strcpy(text,mas_macro[num_macro].text_macro);
	char* text_new = (char*)malloc(SIZE_TEXT);

	int num_in_str = 0;
	int f = 0;
	char* p = strtok(text," ,\n");
	while(p != NULL)
	{
		f = 0;
		if(num_in_str == NUMB_OP+1)
		{
			strcat(text_new,"\n");
			num_in_str = 0;
		}
		
		for(int i = 0; i<mas_macro[num_macro].number_operands; i++)
		{
			if(strcmp(p,mas_macro[num_macro].operands[i]) == 0)
			{
				if(search(operands[i],'\n')!=-1)
					operands[i][search(operands[i],'\n')] = '\0';
				
				int k = search(mas_macro[num_macro].operands[i],'&');
				
				if(k!=-1)
				{
					char* s = (char*)malloc(strlen(mas_macro[num_macro].operands[i]));
					strcpy(s,mas_macro[num_macro].operands[i]);
					s[k] = '\0';
					strcat(s,operands[i]);
					strcat(text_new,s);
				}
				else
					strcat(text_new,operands[i]);
				
				num_in_str++;
				f = 1;
				break;
			}
		}
		
		if(f==0)
		{
			strcat(text_new,p);
			num_in_str++;
		}
		
		if(num_in_str == 1)
			strcat(text_new," ");
		
		if(num_in_str > 1 && num_in_str < NUMB_OP + 1)
			strcat(text_new,",");
		p = strtok(NULL," ,\n");
	}
	mas_macro[num_macro].text_macro = text_new;
}

int count_macro = 0;
int flag_text = 0;
int flag_endm = 0;
int flag_macro = 0;
char* text;	

void transform(char* str, FILE* transformed)
{
//  Searching for macro and counting
	char* str_cpy = (char*)malloc(strlen(str));
	strcpy(str_cpy,str);
	comment(str_cpy);
	flag_endm = 0;
	flag_macro = 0;
	char* p = strtok(str_cpy," ,\n");
	while(p != NULL)
	{
		if(strcmp(p,"MACRO") == 0)
		{
			//Signing name of macro
			for(int i = 0; i<search(str,' '); i++)
			{
				mas_macro[count_macro].name[i] = str[i];
			}
			mas_macro[count_macro].name[search(str,' ')] = '\0';
			//Counting number of operands
			mas_macro[count_macro].number_operands = 1;
			for(int i = search(str,' ') + strlen("MACRO"); i<strlen(str); i++)
			{
				if(str[i] == ',')
				{
					mas_macro[count_macro].number_operands++;
				}
			}
			//Memory allocation for operands
			mas_macro[count_macro].operands = (char**)malloc(mas_macro[count_macro].number_operands*sizeof(char*));
			for(int i = 0; i<mas_macro[count_macro].number_operands; i++)
			{
				mas_macro[count_macro].operands[i] = (char*)malloc(256);
			}
			
			//Signing operands
			int k = 0;
			int j = 0;
			for(int i = search(str,' ') + strlen("MACRO")+2; i<strlen(str); i++)
			{
				if(str[i]!=',' && str[i]!='\n')
				{
					mas_macro[count_macro].operands[k][j] = str[i];
					j++;
				}
				else
				{
					mas_macro[count_macro].operands[k][j] = '\0';
					k++;
					j = 0;
				}
				
			}
			flag_text = 1;
			text = (char*)malloc(SIZE_TEXT);
			flag_macro = 1;
			break;
		}
		else
			if(strcmp(p,"ENDM") == 0)
			{
				flag_text = 0;
				flag_endm = 1;
				flag_macro = 0;
				mas_macro[count_macro].text_macro = text;
				count_macro++;
				text = NULL;
				break;
			}
			else
				if(flag_text == 1)
				{
					comment(str);
					strcat(text,str);
					break;
				}
		p = strtok(NULL," ,");
	}
	
	if(flag_macro == 0 && flag_text == 0)
	{
		char* macro_name = (char*)malloc(256);
		for(int i=0; i < search(str,' '); i++)
		{
			macro_name[i] = str[i];
		}
		macro_name[search(str,' ')] = '\0';
		macro_name[search(str,'\n')] = '\0';
			
		int flag = 0;
		for(int i=0; i < number_macro; i++)
		{
				
			if(strcmp(mas_macro[i].name, macro_name) == 0)
			{
				char** operands = (char**)malloc(mas_macro[i].number_operands*sizeof(char*));
				for(int t = 0; t<mas_macro[i].number_operands; t++)
				{
					operands[t] = (char*)malloc(256);
				}
				
				int k = 0;
				int j = 0;
				
				for(int t = search(str,' ')+1; t<strlen(str); t++)
				{
					if(str[t]!=',' && str[t]!='\n')
					{
						operands[k][j] = str[t];
						j++;
					}
					else
					if(str[t]!='\n')
					{
						operands[k][j] = '\0';
						k++;
						j = 0;
					}
					else
					{
						operands[k][j] = '\n';
						operands[k][j+1] = '\0';
						k++;
						j = 0;
					}
				}
				
				if(k != 0 && k == mas_macro[i].number_operands)
					change(i,operands);
				
				fputs(mas_macro[i].text_macro,transformed);
				flag = 1;
				break;
			}
		}
		free(macro_name);
		if(flag == 0 && flag_endm == 0)
		{
			fputs(str,transformed);
		}
	}
	free(str_cpy);
}

int main(int argc,char** argv)
{
	FILE *source_code, *transformed;
	char name_tr[] = "file.t";
	char str[256];
	
	if((source_code = fopen(argv[1],"r")) != NULL)
	{
		while(1)
		{
			fgets((char*)str,256,source_code);
			if(feof(source_code))
				break;
			analise(str);
		}
		
	} else printf("error with opening file\n");
	
	
	fclose(source_code);
	
	source_code = fopen(argv[1],"r");
	
	mas_macro = (macrooperator*)malloc(number_macro);
	
	for(int i = 0; i<number_macro; i++)
	{
		mas_macro[i].text_macro = (char*)malloc(SIZE_TEXT);
		mas_macro[i].name = (char*)malloc(256);
	}
	
	transformed = fopen(name_tr,"w");
	
	while(1)
	{
		fgets((char*)str,256,source_code);
		if(feof(source_code))
			break;
		transform(str,transformed);
	}
	
	
	free(text);
	for(int i = 0; i<number_macro; i++)
	{
		free(mas_macro[i].text_macro);
		free(mas_macro[i].name);
	}
	return 0;
}
