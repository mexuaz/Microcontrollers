#include <lpc17xx.h>

int main()
{
	/* set GPIO pin number 1.23 as an output (LED)*/
	LPC_GPIO1->FIODIR |= 1 << 23;
	
	/* set GPIO pin number 2.0 as an input (Push Button)*/
	LPC_GPIO2->FIODIR &= ~(1 << 0); 
	
	while(1) {
		if(LPC_GPIO2->FIOPIN & (1<<0)) {
			LPC_GPIO1->FIOSET |= 1 << 23;
		} else {
			LPC_GPIO1->FIOCLR |= 1 << 23;
		}			
	}
	
	//return 0;
}
