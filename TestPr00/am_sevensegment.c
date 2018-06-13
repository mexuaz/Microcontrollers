/* file: am_sevensegment.c */

#include "am_sevensegment.h"

#define PIN(X) (1ul << X)

void sevensegment_init(struct SevenSegment* ss,
	bool a, bool b, bool c, bool d,
	bool e, bool f, bool g, bool h)
{
	ss->a = a;
	ss->b = b;
	ss->c = c;
	ss->d = d;
	ss->e = e;
	ss->f = f;
	ss->g = g;
	ss->h = h;
}


void sevensegment_pins_init(struct SevenSegmentPins* ssp,
	unsigned long a, unsigned long b, unsigned long c, unsigned long d,
	unsigned long e, unsigned long f, unsigned long g, unsigned long h)
{
	ssp->a = a;
	ssp->b = b;
	ssp->c = c;
	ssp->d = d;
	ssp->e = e;
	ssp->f = f;
	ssp->g = g;
	ssp->h = h;
}





void sevensegment_init_char(struct SevenSegment* ss, bool dot, char ch)
{
	switch (ch) {
		case '0':
			sevensegment_init(ss, true, true, true, true, true, true, false, dot);
			break;
		case '1':
			sevensegment_init(ss, false, true, true, false, false, false, false, dot);
			break;
		case '2':
			sevensegment_init(ss, true, true, false, true, true, false, true, dot);
			break;
		case '3':
			sevensegment_init(ss, true, true, true, true, false, false, true, dot);
			break;
		case '4':
			sevensegment_init(ss, false, true, true, false, false, true, true, dot);
			break;
		case '5':
			sevensegment_init(ss, true, false, true, true, false, true, true, dot);
			break;
		case '6':
			sevensegment_init(ss, true, false, true, true, true, true, true, dot);
			break;
		case '7':
			sevensegment_init(ss, true, true, true, false, false, false, false, dot);
			break;
		case '8':
			sevensegment_init(ss, true, true, true, true, true, true, true, dot);
			break;
		case '9':
			sevensegment_init(ss, true, true, true, true, false, true, true, dot);
			break;
		case 'a':
		case 'A':
			sevensegment_init(ss, true, true, true, false, true, true, true, dot);
			break;
		case 'b':
		case 'B':
			sevensegment_init(ss, false, false, true, true, true, true, true, dot);
			break;
		case 'c':
		case 'C':
			sevensegment_init(ss, true, false, false, true, true, true, false, dot);
			break;
		case 'd':
		case 'D':
			sevensegment_init(ss, false, true, true, true, true, false, true, dot);
			break;
		case 'e':
		case 'E':
			sevensegment_init(ss, true, false, false, true, true, true, true, dot);
			break;
		case 'f':
		case 'F':
			sevensegment_init(ss, true, false, false, false, true, true, true, dot);
			break;
		case '-':
			sevensegment_init(ss, false, false, false, false, false, false, true, dot);
			break;
		case 'g':
		case 'G':
			sevensegment_init(ss, true, true, true, true, false, true, true, dot);
			break;
		case 'h':
		case 'H':
			sevensegment_init(ss, false, false, true, false, true, true, true, dot);
			break;
		case 'j':
		case 'J':
			sevensegment_init(ss, false, true, true, true, false, false, false, dot);
			break;
		case 'l':
		case 'L':
			sevensegment_init(ss, false, false, false, true, true, true, false, dot);
			break;
		case 'p':
		case 'P':
			sevensegment_init(ss, true, true, false, false, true, true, true, dot);
			break;
		case 'q':
		case 'Q':
			sevensegment_init(ss, true, true, true, false, false, true, true, dot);
			break;
		case 's':
		case 'S':
			sevensegment_init(ss, true, false, true, true, false, true, true, dot);
			break;
		case 'v':
		case 'V':
			sevensegment_init(ss, false, true, true, true, true, true, false, dot);
			break;
		case 'x':
		case 'X':
			sevensegment_init(ss, false, true, true, false, true, true, true, dot);
			break;
		case 'y':
		case 'Y':
			sevensegment_init(ss, false, true, true, true, false, true, true, dot);
			break;
		default:
			sevensegment_init(ss, false, false, false, false, false, false, false, dot);
			break;
	}
}




void sevensegment_init_value(struct SevenSegment* ss, bool dot, int i)
{
	switch (i) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			sevensegment_init_char(ss, dot, '0' + i);
			break;
		
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			sevensegment_init_char(ss, dot, 'A' + i - 10);
			break;
		
		default:
			sevensegment_init_char(ss, dot, 0);
			break;
	}
}

unsigned long sevensegment_pins_total(const struct SevenSegmentPins* ssp)
{
	unsigned long ss_pins =
		PIN(ssp->a) |	PIN(ssp->b) |	PIN(ssp->c) |	PIN(ssp->d) |
		PIN(ssp->e) |	PIN(ssp->f) |	PIN(ssp->g) |	PIN(ssp->h) ;
		
	return ss_pins;
	
}

unsigned long sevensegment_pins_assigned(const struct SevenSegmentPins* ssp, const struct SevenSegment* ss)
{
	unsigned long pins =
		(ss->a ? PIN(ssp->a) : 0) |
		(ss->b ? PIN(ssp->b) : 0) |
		(ss->c ? PIN(ssp->c) : 0) |
		(ss->d ? PIN(ssp->d) : 0) |
		(ss->e ? PIN(ssp->e) : 0) |
		(ss->f ? PIN(ssp->f) : 0) |
		(ss->g ? PIN(ssp->g) : 0) |
		(ss->h ? PIN(ssp->h) : 0);
		
	return pins;
}


unsigned long sevensegment_pins_get_set_value(const struct SevenSegmentPins* ssp, const struct SevenSegment* ss, bool commonAnod)
{
	if(commonAnod) {
		return sevensegment_pins_total(ssp);
	} else {
		return sevensegment_pins_assigned(ssp, ss);
	}
}
	

unsigned long sevensegment_pins_get_clr_value(const struct SevenSegmentPins* ssp, const struct SevenSegment* ss, bool commonAnod)
{
	if(commonAnod) {
		return sevensegment_pins_assigned(ssp, ss);
	} else {
		return sevensegment_pins_total(ssp);
	}
}




