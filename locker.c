#include<lpc21xx.h>
#include"lcdheader.h"
#include"uartheader.h"
#include"keypad.h"
#include"gsm.h"
#include<string.h>

#define m1 1<<20
#define m2 1<<21
#define buzzer 1<<19

char password[]="1612";
char password_enter[5];
char otp_enter[5];
char otp[5];

int attempts=0;
int otp_num;

void get_password(char *);
void get_otp(char *);
void generate_otp(void);
void buzzer_on(void);
void buzzer_off(void);
void locker_open(void);
void locker_close(void);

int main()
{
	IODIR0 |= m1 | m2 | buzzer;
	LCD_INIT();
	UART0_CONFIG();
	scroll_left_to_right("Forti : A secure Two-factor authentication based bank locker access control system");
	/*otp generation using timer*/
  T1PR=0;
	T1TCR=0X01;
	while(1)
	{
		LCD_COMMAND(0x01);
		LCD_STR("ENTER PASSWORD");
		LCD_COMMAND(0xc0);
		get_password(password_enter);
		if(strcmp(password_enter,password)==0)
		{
			generate_otp();
			LCD_COMMAND(0x01);
			LCD_STR("OTP SENDING");
			SEND_OTP_SMS();
			delay_ms(3000);
			
			LCD_COMMAND(0x01);
			LCD_STR("ENTER OTP");
			LCD_COMMAND(0xc0);
			
			get_otp(otp_enter);
			if(strcmp(otp_enter,otp)==0)
			{
				LCD_COMMAND(0x01);
				LCD_STR("ACCESS GRANTED");
				locker_open();
				LCD_COMMAND(0x01);
				LCD_STR("LOCKER OPENED");
				delay_ms(3000);
				locker_close();
				LCD_COMMAND(0x01);
				LCD_STR("LOCKER CLOSED");
				delay_ms(2000);
				attempts=0;
			}
			else
			{
				LCD_COMMAND(0x01);
				LCD_STR("INVALID OTP");
				buzzer_on();
				delay_ms(3000);
				buzzer_off();
			}
		}
		else
		{
			attempts++;
			LCD_COMMAND(0x01);
			LCD_STR("WRONG PASSWORD");
			LCD_COMMAND(0xc0);
			LCD_STR("ACCESS DENIED");
			buzzer_on();
			delay_ms(3000);
			buzzer_off();
			if(attempts>=3)
			{
				LCD_COMMAND(0x01);
				LCD_STR("SYSTEM LOCKED");
				buzzer_on();
				SEND_ALERT_SMS();
				
				delay_ms(3000);   
				buzzer_off();
				while(1);
			}
		}
		delay_ms(2000);
	}
}
				
/*password enter*/
void get_password(char *s)
{
	int i;
	for(i=0;i<4;i++)
	{
		s[i]=KEYPAD();
		UART0_TX(s[i]);
		LCD_DATA('*');
		delay_ms(200);
	}
	s[4]='\0';
}
/*otp enter*/
void get_otp(char *p)
{
	int i;
	for(i=0;i<4;i++)
	{
		p[i]=KEYPAD();
		UART0_TX(p[i]);
		LCD_DATA('*');
		delay_ms(200);
	}
	p[4]='\0';
}

/*otp generation*/
void generate_otp(void)
{
    int i, temp;
    unsigned int seed = T1TC;   
    otp_num = ((seed >> 3) ^ (seed >> 7) ^ seed) % 9000 + 1000;
    temp = otp_num;
    for(i = 3; i >= 0; i--)
    {
        otp[i] = (temp % 10) + '0';
        temp = temp / 10;
    }
    otp[4] = '\0';
}

/*---------------- BUZZER ----------------*/

void buzzer_on(void)
{
	IOSET0 = buzzer;
}

void buzzer_off(void)
{
	IOCLR0 = buzzer;
}

/*---------------- MOTOR ----------------*/

void locker_open(void)
{
	IOSET0 = m1;

	IOCLR0 = m2;

	delay_ms(3000);

	IOCLR0 = m1;
}

void locker_close(void)
{
	IOSET0 = m2;

	IOCLR0 = m1;

	delay_ms(3000);

	IOCLR0 = m2;
}
