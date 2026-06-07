#include<lpc21xx.h>
#include"delayheader.h"

#define LCD 0xFF<<4
#define RS 1<<12
#define E 1<<13

void LCD_INIT(void);
void LCD_COMMAND(unsigned char );
void LCD_DATA(unsigned char );
void LCD_STR(unsigned char *);
void LCD_INT(int);
void scroll_left_to_right(const char *);

void LCD_INIT(void)
{
	IODIR0|=LCD|RS|E;
	LCD_COMMAND(0x01);
	LCD_COMMAND(0x02);
	LCD_COMMAND(0x0c);
	LCD_COMMAND(0x38);
}

void LCD_COMMAND(unsigned char cmd)
{
	IOCLR0=LCD;
	IOSET0=cmd<<4;
	IOCLR0=RS;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
}

void LCD_DATA(unsigned char d)
{
	IOCLR0=LCD;
	IOSET0=d<<4;
	IOSET0=RS;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
}

void LCD_STR(unsigned char *s)
{
while(*s)
   {
	 LCD_DATA(*s++);
   }
}

void LCD_INT(int n)
{
unsigned char a[10];
signed char i=0;
if(n==0)
LCD_DATA('0');
else
{
if(n<0)
{
LCD_DATA('-');
n=-n;
}
}
while(n>0)
{
a[i++]=n%10;
n=n/10;
}
for(--i;i>=0;i--)
LCD_DATA(a[i]+48);
}

void scroll_left_to_right(const char *str)
{
    int len = 0, start_col, col, i;
    const char *p = str;

    // Get string length
    while(*p++) len++;

    // start_col: from -(len) [fully off left] to +16 [fully off right]
    for(start_col = 16; start_col >= -len; start_col--)
    {
        LCD_COMMAND(0x01);      // Clear display
        delay_ms(2);
        LCD_COMMAND(0x80);      // Cursor to Row 1, Col 0

        for(col = 0; col < 16; col++)
        {
            i = col - start_col;      // Map display column to string index
            if(i >= 0 && i < len)
                LCD_DATA(str[i]);     // Visible character
            else
                LCD_DATA(' ');        // Blank padding
        }

        delay_ms(300);      // Scroll speed — adjust as needed
    }

    LCD_COMMAND(0x01);          // Clear after scroll completes
    delay_ms(2);
}
