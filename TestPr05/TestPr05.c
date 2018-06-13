#include <lpc17xx.h>

#include <stdbool.h>

#include "am_sevensegment.h"

#define PIN(X) (1ul << X)

#define BTN_RST 0

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



int main(int argc, char** argv)
{
	// Setting quad seven segment pins
	struct SevenSegmentPins ssp;
	sevensegment_pins_init(&ssp, 10, 15, 9, 23, 21, 4, 14, 19);
	
	struct QuadSevenSegmentPins qssp;
	quad_sevensegment_pins_init(&qssp, &ssp, DIG1, DIG2, DIG3, DIG4, DEG_HIGH, DEG_LOW, CLK_HIGH, CLK_LOW);
	
	LPC_GPIO1->FIODIR |= quad_sevensegment_pins_total(&qssp); //set pins as output
	
	LPC_GPIO1->FIOSET |= PIN(qssp.deg_anode) | PIN(qssp.clk_anode);
	LPC_GPIO1->FIOCLR |= PIN(qssp.clk_cathode) | PIN(qssp.clk_cathode);
	
	struct SevenSegment ss[4];
	double_value_spilit_4digitSS(88, 88, ss);
	
	
	while(true) {
		multiplex_4digit_sevensegment(&qssp, ss, 1);
	}
	
	//return 0;
}


