							num IS 4
							a IS $1
							b IS $2
							c IS $3
							d IS $4
							LOC #100
#100: 81 03 00 04	Main	LDB c,0,num
#104: c6 01 01 01			XOR a,a,a	
#108: 81 02 00 01			LDB b,0,1	
#10C: 21 04	01 00	m		ADD d,a,0 			
#110: 20 04	04 02			ADD d,d,b		
#114: 21 01	02 00			ADD a,b,0
#118: 21 02 04 00			ADD b,d,0
#11C: 30 05 04 03			CMP $5,d,c
#120: 40 05 ff fb			BN  $5,m 					
#124: 00 00	00 00			TRAP 0	
	
