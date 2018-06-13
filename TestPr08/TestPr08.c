#include <lpc17xx.h>

#include <stdbool.h>

#define PIN(X) (1ul << X)
#define BIT(X) (1ul << X)

#define LED_RED 31

/*
	delay function in secounds
	the value can be expressed 
	in fractional format for mili
	secounds or even smaller values
*/
void delay_s(double s)
{
	/*
		Get Peripheral clock selection for Timer 2 (4.7.3 um)
		The timer 2 peripheral clock is divided by div variable
		decalred below.
	*/
	unsigned pclks = LPC_SC->PCLKSEL0 & (BIT(4) | BIT(5));
	unsigned div;
	switch (pclks) {
		case 0:
			div = 4;
			break;
		case 1:
			div = 1;
			break;
		case 2:
			div = 2;
			break;
		case 3:
			div = 8;
			break;
	}
	
	LPC_TIM1->TCR = 2; // reset and stop timer 1
	LPC_TIM1->MCR = 0; // disable interrupt on match (TC == MRX)
	LPC_TIM1->TC = 0; // set Timer_Counter rigiter to zero
	LPC_TIM1->TCR = 1; // start timer 1
	
	//SystemCoreClock: System Clock Frequency (Core Clock)
	
	double timer_freq;// timer frequncy
	if(LPC_TIM1->PR != 0) {
		timer_freq = SystemCoreClock / (div*LPC_TIM1->PR); 
	} else {
		timer_freq = SystemCoreClock / div;
	}
	unsigned long counter = (unsigned)(s * timer_freq);
	
	while(LPC_TIM1->TC < counter);
	LPC_TIM1->TCR = 2; // reset and stop timer 2
}



int main(int argc, char** argv)
{
	// set led red pin as output
	LPC_GPIO1->FIODIR |= PIN(LED_RED);
			
	while(true)
	{
		if(LPC_GPIO1->FIOPIN & PIN(LED_RED)) {
			LPC_GPIO1->FIOCLR |= PIN(LED_RED);
		} else {
			LPC_GPIO1->FIOSET |= PIN(LED_RED);
		}
		delay_s(0.08);
	}
	
	//return 0;
}

