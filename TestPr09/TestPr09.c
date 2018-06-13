#include <lpc17xx.h>

#include <stdbool.h>

#define PIN(X) (1ul << X)
#define BIT(X) (1ul << X)

#define LED_RED 31


int main(int argc, char** argv)
{
	//set output led 
	LPC_GPIO1->FIODIR |= PIN(LED_RED);
			
	/*
		Peripheral clock selection for Timer 2 peripheral is defined 
		in bit 12 & 13 of PCLKSEL1 (4.7.3 um) and since by default 
		their values are zero and it means that the core clock for timer 2 peripheral 
		is divided by 4: SystemCoreClock / 4.
		to find the timer counter clock the the prescaler register value
		should be considered too. therfore the
		timer counter register clock is: SystemCoreClock / (4*LPC_TIM2->PR)
		if SystemCoreClock is 100 MHz the interrupt will trigger every 2 secounds
	*/
	
	// Power Control for Peripherals register (PCONP)
	// (4.8.9 um)
	// Setting up TIMER2
	LPC_SC->PCONP |= BIT(22); // // timer2 by default is off so power-on timer 2
	LPC_TIM2->TCR = 2; // reset and stop timer 2
	LPC_TIM2->PR = 5000; // set prescaler 5000
	LPC_TIM2->MR0 = 10000; // set match value for compare (each 2 secounds)
	LPC_TIM2->MCR = 1; // interrupt on match (TC == MR0) 
	LPC_TIM2->TC = 0; // timer counter = 0
	NVIC_EnableIRQ(TIMER2_IRQn); // Activate timer 2 on NVIC
	LPC_TIM2->TCR = 1; // start timer 2
	
		
	while(true);
	
	//return 0;
}


//ISR: Interrupt Service Routine
__irq void TIMER2_IRQHandler(void)
{
	LPC_TIM2->IR = 1; // clear timer 2 interrupt flag
	LPC_TIM2->TC = 0; // reset timer counter
	
	if(LPC_GPIO1->FIOPIN & PIN(LED_RED)) {
		LPC_GPIO1->FIOCLR |= PIN(LED_RED);
	} else {
		LPC_GPIO1->FIOSET |= PIN(LED_RED);
	}
	
	
	LPC_TIM2->TC = 0; // reset timer counter
}
