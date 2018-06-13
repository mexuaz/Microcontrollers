#include <lpc17xx.h>

#include <stdbool.h>

void delay_ms(double tms)
{
	unsigned long i = SystemCoreClock * (tms / 5000);
	while(i--); // this line wastes 5 cpu cycle
}


#define PIN(X) (1ul << X)
#define BIT(X) (1ul << X)



#define BTN_INC 0
#define BTN_DEC 2
#define BTN_OFF 4

#define CYCLE_RATE 4e4
#define FALLING_EDGE 2e4

#define CHANGE 100

int main(int argc, char** argv)
{
	//set input buttons direction
	LPC_GPIO2->FIODIR &= ~(PIN(BTN_INC) | PIN(BTN_DEC) | PIN(BTN_OFF) );
	
	
	// Pin Function Select Register 4
	// Select P2.1 as PWM1.2 [um 8.5.5]
	LPC_PINCON->PINSEL4 |= BIT(2);
	LPC_PINCON->PINSEL4 &= ~BIT(3);
	
	
	// PWM Prescale Register
	// Set prescaler value t0 2
	LPC_PWM1->PR = 2;
	
	// PWM Control Register [um 24.6.6]
	// PWM2 single edge, output enable
	LPC_PWM1->PCR = BIT(10);
	
	// Match Register 0 [um 24.6]
	// Set cycle rate of PWM (pulse duration)
	LPC_PWM1->MR0 = CYCLE_RATE;
	
	// Match Register 2 [um 24.6]
	// Set falling edge of PWM
	LPC_PWM1->MR2 = FALLING_EDGE;
	
	
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

	while(true) {
	
		if(LPC_GPIO2->FIOPIN & PIN(BTN_INC)) {
			if(LPC_PWM1->MR2 + CHANGE < CYCLE_RATE) {
				LPC_PWM1->MR2 += CHANGE;
			}
			LPC_PWM1->LER = BIT(2);
		}
		
		if(LPC_GPIO2->FIOPIN & PIN(BTN_DEC)) {
			//since MR2 is an unsigned value the
			// substracting it from a value may result in
			// bigger number (becuase an unsigned can not be negative)
			if((LPC_PWM1->MR2 - CHANGE < LPC_PWM1->MR2) && (LPC_PWM1->MR2 - CHANGE != 0)) {
				LPC_PWM1->MR2 -= CHANGE;
			}
			LPC_PWM1->LER = BIT(2);
		}
		
		if(LPC_GPIO2->FIOPIN & PIN(BTN_OFF)) {
			
			if(LPC_PWM1->TCR == 0) {
				LPC_PWM1->TCR = BIT(0) | BIT(3);
			} else {
				// Disable timer when the pwm is low
				while(LPC_PWM1->TC < LPC_PWM1->MR2);
				LPC_PWM1->TCR = 0;
			}
			// Perform single push operation for Off/On Button
			while(LPC_GPIO2->FIOPIN & PIN(BTN_OFF));
		}
		
		delay_ms(20);
		
	} // end of main loop
	
	//return 0;
}



