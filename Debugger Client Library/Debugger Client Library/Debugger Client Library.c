/*
 * Debugger_Client_Library.c
 *
 * Created: 23.08.2014 23:34:58
 *  Author: Robin
 */ 

#define F_CPU 8000000

#include <util/delay.h>
#include "debugger.h"
#include <stdlib.h>
#include <avr/io.h>


int main(void) {
	_delay_ms(500);
	debug_init();
	/*send(1);
	send_string("~If => Baum == True");
	send(2);
	
	send(1);
	send_string("~PWM Start");
	send(2);
	
	debug_string("Test");
	
	

	// convert 123 to string [buf]
	
	debug_int("PWM [Hz]", 230);*/
	breakpoint("Test");
	unsigned long test = 0;
	while (1) {
		debug_int("Msg", test);
		_delay_ms(1000);
		test++;
	}
}