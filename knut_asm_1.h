#ifndef _knut_asm_1_H_
#define _knut_asm_1_H_

unsigned char* metka(unsigned char* str);
unsigned char* comment(unsigned char* str);
unsigned char* operator(unsigned char* str);
unsigned char* operand1(unsigned char* str);
unsigned char* operand2(unsigned char* str);
void print(unsigned char* metka,unsigned char* operator,unsigned char* operand1, unsigned char* operand2, unsigned char* operand3, unsigned char* comment);

#endif
