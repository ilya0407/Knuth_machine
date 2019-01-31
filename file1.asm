Func MACRO A,B,C,D,&x

ADD ax,A,2
ADD bx,A,3
ADD ax,C,4
ADD ax,D,C&x
ENDM

num		IS 4
a		IS $1
b		IS $2
c		IS $3
d		IS $4
		LOC #100
Main	LDB c,0,num
		XOR a,a,a	
		LDB b,0,1
m		ADD d,a,0
		ADD d,d,b
		Func a,b,c,d,e
		ADD a,b,0
		ADD b,d,0
		CMP $5,d,c
		BN  $5,m
		TRAP 0
