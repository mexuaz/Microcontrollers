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


void assign_4digit_sevensegment(unsigned a, struct SevenSegment ss[4])
{
	sevensegment_init_value(&ss[0], false, (a / 1000)%10);
	sevensegment_init_value(&ss[1], false, (a / 100)%10);
	sevensegment_init_value(&ss[2], false, (a / 10)%10);
	sevensegment_init_value(&ss[3], false, (a / 1)%10);
}
void multiplex_4digit_sevensegment(const struct SevenSegmentPins* ssp, struct SevenSegment ss[4], unsigned dly)
{
	LPC_GPIO1->FIOSET |= PIN(DIG1);
	LPC_GPIO1->FIOCLR |= PIN(DIG2);
	LPC_GPIO1->FIOCLR |= PIN(DIG3);
	LPC_GPIO1->FIOCLR |= PIN(DIG4);
	LPC_GPIO1->FIOSET |= sevensegment_pins_get_set_value(ssp, &ss[0], true);
	LPC_GPIO1->FIOCLR |= sevensegment_pins_get_clr_value(ssp, &ss[0], true);
	delay_ms(dly);
	
	
	LPC_GPIO1->FIOCLR |= PIN(DIG1);
	LPC_GPIO1->FIOSET |= PIN(DIG2);
	LPC_GPIO1->FIOCLR |= PIN(DIG3);
	LPC_GPIO1->FIOCLR |= PIN(DIG4);
	LPC_GPIO1->FIOSET |= sevensegment_pins_get_set_value(ssp, &ss[1], true);
	LPC_GPIO1->FIOCLR |= sevensegment_pins_get_clr_value(ssp, &ss[1], true);
	delay_ms(dly);
	
	
	LPC_GPIO1->FIOCLR |= PIN(DIG1);
	LPC_GPIO1->FIOCLR |= PIN(DIG2);
	LPC_GPIO1->FIOSET |= PIN(DIG3);
	LPC_GPIO1->FIOCLR |= PIN(DIG4);
	LPC_GPIO1->FIOSET |= sevensegment_pins_get_set_value(ssp, &ss[2], true);
	LPC_GPIO1->FIOCLR |= sevensegment_pins_get_clr_value(ssp, &ss[2], true);
	delay_ms(dly);
	
	
	LPC_GPIO1->FIOCLR |= PIN(DIG1);
	LPC_GPIO1->FIOCLR |= PIN(DIG2);
	LPC_GPIO1->FIOCLR |= PIN(DIG3);
	LPC_GPIO1->FIOSET |= PIN(DIG4);
	LPC_GPIO1->FIOSET |= sevensegment_pins_get_set_value(ssp, &ss[3], true);
	LPC_GPIO1->FIOCLR |= sevensegment_pins_get_clr_value(ssp, &ss[3], true);
	delay_ms(dly);
}



int main(int argc, char** argv)
{

	struct SevenSegmentPins ssp;
	sevensegment_pins_init(&ssp, 10, 15, 9, 23, 21, 4, 14, 19);
	LPC_GPIO1->FIODIR |= ~0;//sevensegment_pins_total(&ssp); //set pins as output
		
	struct SevenSegment ss[4];
	
	unsigned long t = SystemCoreClock / 100000;
	assign_4digit_sevensegment(t, ss); 
	
	while(true) {
		
		multiplex_4digit_sevensegment(&ssp, ss, 2);
	}
	
	//return 0;
}


