/*
 * Rob.c
 *
 * Created: 6/19/2022 
 * Author : Ahmed Ayman
 */ 

#define F_CPU 1000000UL

#define SET_BIT(ADDRESS, BIT) ADDRESS |= (1 << BIT)
#define RESET_BIT(ADDRESS, BIT) ADDRESS &= ~(1 << BIT)
#define TOGGLE_BIT(ADDRESS, BIT) ADDRESS ^= (1 << BIT)
#define READ_BIT(ADDRESS, BIT) ((ADDRESS >> BIT) & 1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// ---------------------------- Motor Directions ---------------------------- //

void motors_init()
{
	DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);
}

void mov_forward()
{
	PORTB &= ~((1 << PB3) | (1 << PB5));
	PORTB |= (1 << PB2) | (1 << PB4);
}

void mov_backward()
{
	PORTB &= ~((1 << PB2) | (1 << PB4));
	PORTB |= (1 << PB3) | (1 << PB5);
}

void mov_left()
{
	PORTB &= ~((1 << PB2) | (1 << PB5));
	PORTB |= (1 << PB3) | (1 << PB4);
}

void mov_right()
{
	PORTB &= ~((1 << PB3) | (1 << PB4));
	PORTB |= (1 << PB2) | (1 << PB5);
}

void mov_stop()
{
	PORTB &= ~((1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));
}

// ---------------------------- Sensor Intializatoin & Calculation ---------------------------- //

void sensor_init()
{
	RESET_BIT(DDRB, PB0);
	SET_BIT(DDRB, PB1);
}

double sensor_distance()
{
	SET_BIT(PORTB, PB1);
	_delay_us(10);
	RESET_BIT(PORTB, PB1);

	int count = 0;
	while (READ_BIT(PINB, PB0) == 0)
		;
	while (READ_BIT(PINB, PB0) == 1)
		count++;

	_delay_ms(10);
	return count / 8.0;
}

// ---------------------------- Main Function---------------------------- //
int main()
{
	motors_init();
	sensor_init();
	DDRD = 0xFF;
	while (1)
	{
		PORTD ^= (1<<0);
		if (sensor_distance() > 30)
		{
			PORTD |= (1<<1);
			PORTD &= ~(1<<2);
			mov_forward();
		}
		else
		{
			PORTD |= (1<<2);
			PORTD &= ~(1<<1);
			mov_right();
			_delay_ms(500);
		}
	}
}