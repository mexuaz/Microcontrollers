#include <lpc17xx.h>

#include <stdbool.h>

#define PIN(X) (1ul << X)
#define BIT(X) (1ul << X)

// Maximum frequency is 12.5 MHz
#define FREQ 12.5

int main(int argc, char** argv)
{

	// Pin Function Select Register 4
	// Select P2.1 as PWM1.2 [um 8.5.5]
	LPC_PINCON->PINSEL4 |= BIT(2);
	LPC_PINCON->PINSEL4 &= ~BIT(3);
	
	
	// PWM Prescale Register
	// Set prescaler value to zero
	LPC_PWM1->PR = 1024; // Do not set it to values lower than 50
	
	// PWM Control Register [um 24.6.6]
	// PWM2 single edge, output enable
	LPC_PWM1->PCR = BIT(10);
	
	// Match Register 0 [um 24.6]
	// Set cycle rate of PWM (pulse duration)
	LPC_PWM1->MR0 = SystemCoreClock/(4.*FREQ*LPC_PWM1->PR);
	
	// Match Register 2 [um 24.6]
	// Set falling edge of PWM
	LPC_PWM1->MR2 =  SystemCoreClock/(4.*FREQ*LPC_PWM1->PR*2);
	
	
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
	
	
	while(true);
}
