/*
Driving a servo motor:

* The PWM cycle rate for a standard position servo motor should be 20 mili secounds.
* Which means the servo motor should receive PWM with frequncy of 5 Hz.
* Servo motor will go to its minimum rotational angle (zero) when the PWM cycle is high for
	1 mili secound or in other words when the pulse is high in about its 5 percent of total pulse.
* And the servo motor will go to its maximum rotational angle (which is mostly 180) when the PWM cycle rate	
	is high for 2 mili secounds or in other words when the pulse is high in about its 10 percent of total pulse.
* All the mentioned behaviour means that if we want to be able to control our 180 degree servo in every
	degree we should be able to spilit the 1 mili secound (diffrence betwwen 2 and 1 mili secounds) to 180.
	so we should simply multiply the 20 to 180. (20 * 180 = 3600)
* 3600 is the value the my timer should count in 20 mili second so when I want to set the servo position to 
	minimum rotational angle I simply make the falling edge of my timer to fall in 180 counts and when I want to
	make set my servo position to its maximum rotational angle I simply make the pulse falling edge to fall when the 
	timer counts match 360.
* If I want my timer to count 3600 counts every 20 milisecouds Its frequency should be 18e4 (3600/20e-3).
* The prescaler register should be configured in a way to make my timer act in 18e4 frequncy. The following formula
	will do that:
	Prescaler = SystemCoreClock/(4*18e4)
	* In a 100 MHz CoreClock the Prescaler value will be: 
*/


#include <lpc17xx.h>

#include <stdbool.h>

#define PIN(X) (1ul << X)
#define BIT(X) (1ul << X)

void delay_ms(double tms)
{
	unsigned long i = SystemCoreClock * (tms / 5000);
	while(i--); // this line wastes 5 cpu cycle
}


int main(int argc, char** argv)
{
	// Pin Function Select Register 4
	// Select P2.1 as PWM1.2 [um 8.5.5]
	LPC_PINCON->PINSEL4 |= BIT(2);
	LPC_PINCON->PINSEL4 &= ~BIT(3);
	
	
	// PWM Prescale Register
	// Set prescaler value to
	LPC_PWM1->PR = SystemCoreClock/(4*18e4);
	
	// PWM Control Register [um 24.6.6]
	// PWM2 single edge, output enable
	LPC_PWM1->PCR = BIT(10);
	
	// Match Register 0 [um 24.6]
	// Set cycle rate of PWM (pulse duration)
	LPC_PWM1->MR0 = 3600;
	
	// Match Register 2 [um 24.6]
	// Set falling edge of PWM
	LPC_PWM1->MR2 = 180;
	
	
	// PWM Latch Enable Register
	// The contents of match registers will
	// be transferred to the shadow registers if
	// the corresponding bit in the Latch Enable Register has been set.
	// Enable shadow latch for match registers (MR0 & MR2)
	LPC_PWM1->LER = BIT(0) | BIT(2);
	
	// PWM Match Control Registers [um 24.6.4]
	// Bit 1: The PWM->TC will be reset if PWM->MR0 matches it. (TC == MR0)
	LPC_PWM1->MCR = BIT(1);
	
	// PWM Timer Control Register [um 24.6.2]
	// Bit 1: Reset counter and prescaler
	LPC_PWM1->TCR = BIT(1);
	
	// PWM Timer Control Register [um 24.6.2]
	// Bit 0: Counter Enable, Bit 3: PWM Enable (copy MRx in shadow)
	LPC_PWM1->TCR = BIT(0) | BIT(3);
	
	double dly = 1000;
	while(true)	{
		
		LPC_PWM1->MR2 = 180;
		LPC_PWM1->LER = BIT(2);
		
		delay_ms(dly);
		
		LPC_PWM1->MR2 = 270;
		LPC_PWM1->LER = BIT(2);
		
		delay_ms(dly);
		
		LPC_PWM1->MR2 = 360;
		LPC_PWM1->LER = BIT(2);
		
		delay_ms(dly);
		
		LPC_PWM1->MR2 = 270;
		LPC_PWM1->LER = BIT(2);
		
		delay_ms(dly);
		
	}
	
	//return 0;
}
