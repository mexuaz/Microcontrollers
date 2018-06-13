#include <lpc17xx.h>

#define PIN(X) (1ul << X)

void delay_ms(double tms)
{
	unsigned long i = SystemCoreClock * (tms / 5000);
	while(i--); // this line wastes 5 cpu cycle
}

int main()
{
	/* set GPIO pin number 1.23 as an output (LED)*/
	LPC_GPIO1->FIODIR |= PIN(20) | PIN(23) | PIN(26);
	
	/* Enable rising edge interrupt for (Push Buttons) pin 2.0
	& 2.4 & 2.8 and disable other pins interrupt */
	LPC_GPIOINT->IO2IntEnR = PIN(0) | PIN(4) | PIN(8);
	
	/* Enable falling edge interrupt for all GPIO2 pins */
	LPC_GPIOINT->IO2IntEnF = 0;
	
	/* Enable external GPIO interrupt and register ISR 0 routine as a handler */
	NVIC_EnableIRQ(EINT3_IRQn);
	
	while(1) {
		LPC_GPIO1->FIOCLR |= PIN(20);
		LPC_GPIO1->FIOCLR |= PIN(23);
		LPC_GPIO1->FIOCLR |= PIN(26);
	}
	
	//return 0;
}


//ISR: Interrupt Service Routine
__irq void EINT3_IRQHandler(void)
{
	unsigned long stat = LPC_GPIOINT->IO2IntStatR;
	
	/* Disable rising edge interrupt
	so we don't have nested interrupts */
	LPC_GPIOINT->IO2IntEnR = 0;
		
	//ISR routine
	if(stat & PIN(0)) {
		LPC_GPIO1->FIOSET |= PIN(26);
	} else if(stat & PIN(4)) {
		LPC_GPIO1->FIOSET |= PIN(23);
	} else if(stat & PIN(8)) {
		LPC_GPIO1->FIOSET |= PIN(20);
	}
		
	delay_ms(1000);
	
	//clear all interuppt flags on GPIO2
	LPC_GPIOINT->IO2IntClr = ~0;
	
	LPC_GPIOINT->IO2IntEnR = PIN(0) | PIN(4) | PIN(8);
}
