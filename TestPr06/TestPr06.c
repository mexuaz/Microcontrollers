#include <lpc17xx.h>

#include <stdbool.h>

#include "am_sevensegment.h"

#define PIN(X) (1ul << X)



#define LED_RED 31
#define LED_YELLOW 29
#define LED_GREEN 27

#define DIG1 25
#define DIG2 24
#define DIG3 20
#define DIG4 18

#define DEG_HIGH 1
#define DEG_LOW 0

#define CLK_HIGH 22
#define CLK_LOW 8

#define BTN_RST 4
#define BTN_INC 2
#define BTN_DEC 0


void delay_ms(double tms)
{
	unsigned long i = SystemCoreClock * (tms / 5000);
	while(i--); // this line wastes 5 cpu cycle
}

void multiplex_4digit_sevensegment(const struct QuadSevenSegmentPins* qssp, struct SevenSegment ss[4], unsigned dly)
{
	LPC_GPIO1->FIOSET |= PIN(qssp->digit[0]);
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[1]);
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[2]);
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[3]);
	LPC_GPIO1->FIOSET |= sevensegment_pins_get_set_value(&qssp->ssp, &ss[0], true);
	LPC_GPIO1->FIOCLR |= sevensegment_pins_get_clr_value(&qssp->ssp, &ss[0], true);
	delay_ms(dly);
	
	
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[0]);
	LPC_GPIO1->FIOSET |= PIN(qssp->digit[1]);
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[2]);
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[3]);
	LPC_GPIO1->FIOSET |= sevensegment_pins_get_set_value(&qssp->ssp, &ss[1], true);
	LPC_GPIO1->FIOCLR |= sevensegment_pins_get_clr_value(&qssp->ssp, &ss[1], true);
	delay_ms(dly);
	
	
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[0]);
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[1]);
	LPC_GPIO1->FIOSET |= PIN(qssp->digit[2]);
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[3]);
	LPC_GPIO1->FIOSET |= sevensegment_pins_get_set_value(&qssp->ssp, &ss[2], true);
	LPC_GPIO1->FIOCLR |= sevensegment_pins_get_clr_value(&qssp->ssp, &ss[2], true);
	delay_ms(dly);
	
	
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[0]);
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[1]);
	LPC_GPIO1->FIOCLR |= PIN(qssp->digit[2]);
	LPC_GPIO1->FIOSET |= PIN(qssp->digit[3]);
	LPC_GPIO1->FIOSET |= sevensegment_pins_get_set_value(&qssp->ssp, &ss[3], true);
	LPC_GPIO1->FIOCLR |= sevensegment_pins_get_clr_value(&qssp->ssp, &ss[3], true);
	delay_ms(dly);
}

int dly = 50;
const unsigned int pins = PIN(BTN_INC) | PIN(BTN_DEC) | PIN(BTN_RST);

int main(int argc, char** argv)
{
	//Set as input
	LPC_GPIO2->FIODIR &= ~pins;

	//Set as interrupt pins
	LPC_GPIOINT->IO2IntEnR |= pins;
	NVIC_EnableIRQ(EINT3_IRQn);
	
	// Setting quad seven segment pins
	struct SevenSegmentPins ssp;
	sevensegment_pins_init(&ssp, 10, 15, 9, 23, 21, 4, 14, 19);
	
	struct QuadSevenSegmentPins qssp;
	quad_sevensegment_pins_init(&qssp, &ssp, DIG1, DIG2, DIG3, DIG4, DEG_HIGH, DEG_LOW, CLK_HIGH, CLK_LOW);
	
	LPC_GPIO1->FIODIR |= quad_sevensegment_pins_total(&qssp); //set pins as output
			
	struct SevenSegment ss[4];
		
	while(true) {
		single_value_spilit_4digitSS(dly, ss); 
		multiplex_4digit_sevensegment(&qssp, ss, dly);
	}
	
	//return 0;
}

__irq void EINT3_IRQHandler( void )
{
	unsigned stat = LPC_GPIOINT->IO2IntStatR;
	LPC_GPIOINT->IO2IntClr |= pins;
	LPC_GPIOINT->IO2IntEnR &= ~pins;
	
	if(stat & PIN(BTN_INC)) {
		dly < 10000 ? dly++ : dly;
		while(LPC_GPIO2->FIOPIN & PIN(BTN_INC));
	} else if(stat & PIN(BTN_DEC)) {
		dly > 0 ? dly-- : dly;
		while(LPC_GPIO2->FIOPIN & PIN(BTN_DEC));
	} else if(stat & PIN(BTN_RST)) {
		dly = 50;
		while(LPC_GPIO2->FIOPIN & PIN(BTN_RST));
	}
		
	LPC_GPIOINT->IO2IntEnR |= pins;
}
