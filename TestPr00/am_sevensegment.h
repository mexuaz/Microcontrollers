/* file: am_sevensegment.h */

#include <stdbool.h>


struct SevenSegment {
	bool a;
	bool b;
	bool c;
	bool d;
	bool e;
	bool f;
	bool g;
	bool h;
};

void sevensegment_init(struct SevenSegment* ss,
	bool a, bool b, bool c, bool d,
	bool e, bool f, bool g, bool h);

struct SevenSegmentPins {
	unsigned long a;
	unsigned long b;
	unsigned long c;
	unsigned long d;
	unsigned long e;
	unsigned long f;
	unsigned long g;
	unsigned long h;
};

/* Initialize SevenSegmentPins structure */ 
void sevensegment_pins_init(struct SevenSegmentPins* ssp,
	unsigned long a, unsigned long b, unsigned long c, unsigned long d,
	unsigned long e, unsigned long f, unsigned long g, unsigned long h);


/* Initialize the ss structures to complaince the given char */ 
void sevensegment_init_char(struct SevenSegment* ss, bool dot, char ch);

/* Initialize the ss structures to complaince the given value */ 
void sevensegment_init_value(struct SevenSegment* ss, bool dot, int i);

/* return the total pin set of seven segment in a unsigned long with eight digits of one */
unsigned long sevensegment_pins_total(const struct SevenSegmentPins* ssp);
	
/* return the SevenSegment assigned pin set in ss */
unsigned long sevensegment_pins_assigned(const struct SevenSegmentPins* ssp, const struct SevenSegment* ss);

/* Get the value to be assigned to FIOSET */
unsigned long sevensegment_pins_get_set_value(const struct SevenSegmentPins* ssp, const struct SevenSegment* ss, bool commonAnod);
	
/* Get the value to be assigned to FIOCLR */
unsigned long sevensegment_pins_get_clr_value(const struct SevenSegmentPins* ssp, const struct SevenSegment* ss, bool commonAnod);
