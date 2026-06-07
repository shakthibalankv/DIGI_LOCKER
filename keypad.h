#include<lpc21xx.h>
#define c0 (IOPIN1 &(1 << 20))
#define c1 (IOPIN1 &(1 << 21))
#define c2 (IOPIN1 &(1 << 22))
#define c3 (IOPIN1 &(1 << 23))
#define r0 1<<16
#define r1 1<<17
#define r2 1<<18
#define r3 1<<19
unsigned int key_lut[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
unsigned int keyscan(void){
	unsigned char row_val,col_val;
	IODIR1=r0|r1|r2|r3;// ROW AS OUTPUT
	while(1){
  IOCLR1=r0|r1|r2|r3;
	IOSET1=c0|c1|c2|c3;
	while((c0&&c1&&c2&&c3)==1);
	//testing for row-0
	IOCLR1=r0;
	IOSET1=r1|r2|r3;
	if((c0&&c1&&c2&&c3)==0){
		row_val=0;
		break;
	}
	//testing for row-1
	IOCLR1=r1;
	IOSET1=r0|r2|r3;
	if((c0&&c1&&c2&&c3)==0){
		row_val=1;
		break;
	}
	//testing for row-2
	IOCLR1=r2;
	IOSET1=r1|r0|r3;
	if((c0&&c1&&c2&&c3)==0){
		row_val=2;
		break;
	}
	//testing for row-3
	IOCLR1=r3;
	IOSET1=r1|r2|r0;
	if((c0&&c1&&c2&&c3)==0){
		row_val=3;
		break;
	}
}
if(c0==0)
	col_val=0;
else if(c1==0)
	col_val=1;
else if(c2==0)
	col_val=2;
else
	col_val=3;
delay_ms(150);
while((c0&&c1&&c2&&c3)==0);
return key_lut[row_val][col_val];
}

	
int KEYPAD(void)
{
    int res;
    res = keyscan();            // Returns 1–16 from LUT

    if(res <= 9)
        return (res + '0');     // '1' to '9'
    else
        return (res + '0');     // For your project keys 10-16 as needed
}

