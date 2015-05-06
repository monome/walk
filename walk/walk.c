// http://en.wikipedia.org/wiki/Linear_feedback_shift_register

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define P_MOM_A	0
#define P_TOG_A	1
#define P_MOM_B	2
#define P_TOG_B	3

#define P_XOR	1
#define P_AND	0

#define P_IN_A  (1<<5)
#define P_IN_B  (1<<6)
#define P_IN_I  (1<<7)


#define FILTER_TIME 10;


int main(void) {
	uint8_t in_a = 0, in_b = 0, count_a = 0, count_b = 0;

	uint8_t mom_a, tog_a, mom_b, tog_b, xor, and, invert;

	DDRA = P_MOM_A | P_TOG_A | P_MOM_B | P_TOG_B;		// outputs
	PORTA = P_IN_A | P_IN_B | P_IN_I;					// clear, pull-ups on
	DDRB = P_XOR | P_AND;								// outputs
	PORTB = 0;											// clear

	// MCUCR |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
    // GIMSK |= (1 << PCIE0);     // Turns on INT0
    // PCMSK0 = P_IN_A | P_IN_B;

    invert = (PINA & P_IN_I) != 0;

	sei();

	while(1) {
		if((PINA & P_IN_A) != in_a) {
			in_a = PINA & P_IN_A;
			count_a = FILTER_TIME;
		}

		if((PINA & P_IN_B) != in_b) {
			in_b = PINA & P_IN_B;
			count_b = FILTER_TIME;
		}

		if(count_a) {
			if(count_a == 1) {
				if(!in_a == !invert) {
					PORTA ^= (1 << P_TOG_A);
					PORTA &= ~(1 << P_MOM_A);
					mom_a = 1;
				}
				else {
					PORTA ^= (1 << P_MOM_A);
					mom_a = 0;
				}
			}

			count_a--;
		}

		if(count_b) {
			if(count_b == 1) {
				if(!in_b == !invert) {
					PORTA ^= (1 << P_TOG_B);
					PORTA &= ~(1 << P_MOM_B);
					mom_b = 1;
				}
				else {
					PORTA ^= (1 << P_MOM_B);
					mom_b = 0;
				}
			}

			count_b--;
		}

		xor = !(mom_a ^ mom_b);
		and = !(mom_a & mom_b);

		PORTB = (xor << P_XOR) | (and << P_AND);

		_delay_ms(1);
	}

	return 0;
}

