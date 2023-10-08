#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
unsigned char countmin =0;
unsigned char countsec =0;
unsigned char counth =0;
void TM1_int(void){
	TCNT1 = 0;
	OCR1A = 1000;
	TIMSK |= (1<<OCIE1A);
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<<CS10)| (1<<CS12);
}
int main(void)
{	DDRC |= 0x0f;
PORTC &= 0xf0;
DDRA |= 0x3f;
PORTA |= 0x3f;
SREG |= (1<<7);
DDRB   &= (~(1<<PB2));
DDRD   &= (~(1<<PD2));
DDRD   &= (~(1<<PD3));
PORTB|=(1<<PB2);
PORTD|=(1<<PD2);
MCUCSR |= (1<<ISC10)|(1<<ISC11)|(1<<ISC01);
GICR   |= (1<<INT2)|(1<<INT1)|(1<<INT0);
TM1_int();

while(1)
{
	PORTA = (PORTA & 0xC0) |  (1<<PA0);
	PORTC = (PORTC & 0xF0) | (countsec%10);
	_delay_ms(2);
	PORTA = (PORTA & 0xC0) |  (1<<PA1);
	PORTC = (PORTC & 0xF0) | (countsec/10);
	_delay_ms(2);
	PORTA = (PORTA & 0xC0) |  (1<<PA2);
	PORTC = (PORTC & 0xF0) | (countmin%10);
	_delay_ms(2);
	PORTA = (PORTA & 0xC0) |  (1<<PA3);
	PORTC = (PORTC & 0xF0) | (countmin/10);
	_delay_ms(2);
	PORTA = (PORTA & 0xC0) |  (1<<PA4);
	PORTC = (PORTC & 0xF0) | (counth%10);
	_delay_ms(2);
	PORTA = (PORTA & 0xC0) |  (1<<PA5);
	PORTC = (PORTC & 0xF0) | (counth/10);
	_delay_ms(2);
}
}
ISR(TIMER1_COMPA_vect)
{
	if(counth==23&&countmin==59&&countsec==59){
		counth=0;
		countmin=0;
		countsec=0;
	}
	if(countmin==59&&countsec==59){
		countmin=0;
		countsec=0;
		counth++;
	}
	if(countsec==59){
		countsec=0;
		countmin++;
	}
	else
		countsec++;


}
ISR(INT0_vect){
	countsec=0;
	countmin=0;
	counth=0;
}
ISR(INT1_vect)
{TCCR1B &=~(1<<CS12)&~  (1<<CS10);
}
ISR(INT2_vect)
{TCCR1B |=(1<<CS10)| (1<<CS12);
}
