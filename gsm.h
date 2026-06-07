extern char otp[5];

void SEND_OTP_SMS(void);
void SEND_ALERT_SMS(void);

void SEND_OTP_SMS(void)
{
	UART0_STR("AT\r");

	delay_ms(1000);

	UART0_STR("AT+CMGF=1\r");

	delay_ms(1000);

	UART0_STR("AT+CMGS=\"+917305790354\"\r");

	delay_ms(1000);

	UART0_STR("Your OTP is: ");

	UART0_STR(otp);

	delay_ms(1000);

	UART0_TX(0x1A);

	delay_ms(3000);
}

void SEND_ALERT_SMS(void)
{
	UART0_STR("AT\r");

	delay_ms(1000);

	UART0_STR("AT+CMGF=1\r");

	delay_ms(1000);

	UART0_STR("AT+CMGS=\"+911234567890\"\r");

	delay_ms(1000);

	UART0_STR("ALERT! SYSTEM LOCKED");

	delay_ms(1000);

	UART0_TX(0x1A);

	delay_ms(3000);
}
