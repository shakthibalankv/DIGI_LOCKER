void UART0_CONFIG(void)
{
PINSEL0|=0x5;
U0LCR=0x83;
U0DLL=97;
U0DLM=0;
U0LCR=0x03;
}

void UART0_TX(unsigned char c)
{
while(((U0LSR>>5)&1)==0);
U0THR=c;
}

void UART0_STR(unsigned char *s)
{
while(*s)
UART0_TX(*s++);
}

void UART0_INT(int n)
{
unsigned char a[10];
signed char i=0;
if(n==0)
UART0_TX('0');
else
{
if(n<0)
{
UART0_TX('-');
n=-n;
}
}
while(n>0)
{
a[i++]=n%10;
n=n/10;
}
for(--i;i>=0;i--)
UART0_TX(a[i]+48);
}

unsigned char UART0_RX(void)
{
while((U0LSR&0x1)==0);
return U0RBR;
}
