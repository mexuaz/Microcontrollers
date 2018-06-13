#include <lpc17xx.h>

void delay_ms(double tms)
{
	unsigned long i = SystemCoreClock * (tms / 5000);
	while(i--); // this line wastes 5 cpu cycle
}

int main()
{
	/* set GPIO pin number 1.23 as an output */
	LPC_GPIO1->FIODIR |= 1 << 23; 
	
	while(1) {
		LPC_GPIO1->FIOSET |= 1 << 23;
		delay_ms(2000);
		LPC_GPIO1->FIOCLR |= 1 << 23;
		delay_ms(1000);
	}
	
	//return 0;
}
