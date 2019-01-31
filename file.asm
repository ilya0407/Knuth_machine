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
		ADD a,b,0
		ADD b,d,0
		CMP $5,d,c
		BN  $5,m 					
		TRAP 0	
