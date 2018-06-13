#include <lpc17xx.h>

#include <stdbool.h>

#define PIN(X) (1ul << X)
#define BIT(X) (1ul << X)


#define LED_RED 31


int main(int argc, char** argv)
{
	// set led red pin as output
	LPC_GPIO1->FIODIR |= PIN(LED_RED);
			
	
	
	// Power Control for Peripherals register (PCONP)
	// (4.8.9 um)
	// Setting up TIMER2
	LPC_SC->PCONP |= BIT(22); // timer2 by default is off so power-on timer 2
	LPC_TIM2->TCR = 2; // reset and stop timer 2
	LPC_TIM2->PR = 500; // set Prescaler value to 500
	LPC_TIM2->MCR = 0; // disable interrupt on match (TC == MRX)
	LPC_TIM2->TC = 0; // set Timer_Counter rigiter to zero
	LPC_TIM2->TCR = 1; // start timer 2
	
	/*
		Peripheral clock selection for Timer 2 peripheral is defined 
		in bit 12 & 13 of PCLKSEL1 (4.7.3 um) and since by default 
		their values are zero and it means that the core clock for timer 2 peripheral 
		is divided by 4: SystemCoreClock / 4.
		to find the timer counter clock the the prescaler register value
		should be considered too. therfore the
		timer counter register clock is: SystemCoreClock / (4*LPC_TIM2->PR)
	*/
	
	double delay = 0.1; // delay value in secounds
	double timer2_freq = SystemCoreClock / (4*LPC_TIM2->PR); // timer 2 frequncy
			
	while(true)
	{
		if(LPC_TIM2->TC >= delay * timer2_freq) {
			LPC_TIM2->TC = 0;
			if(LPC_GPIO1->FIOPIN & PIN(LED_RED)) {
				LPC_GPIO1->FIOCLR |= PIN(LED_RED);
			} else {
				LPC_GPIO1->FIOSET |= PIN(LED_RED);
			}
		}
	}
	
	//return 0;
}

