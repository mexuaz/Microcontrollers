#include <lpc17xx.h>

#define PIN(X) (1ul << X)

int main()
{
	/* set GPIO pin number 1.23 as an output (LED)*/
	LPC_GPIO1->FIODIR |= PIN(23);
	
	/* set GPIO pin number 2.0 as an input (Push Button)*/
	LPC_GPIO2->FIODIR &= ~PIN(0); 
	
	while(1) {
		if(LPC_GPIO2->FIOPIN & PIN(0)) {
			LPC_GPIO1->FIOSET |= PIN(23);
		} else {
			LPC_GPIO1->FIOCLR |= PIN(23);
		}			
	}
	
	//return 0;
}
