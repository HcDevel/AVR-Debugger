/*
 * debugger.c
 *
 * Created: 24.08.2014 00:07:58
 *  Author: Robin
 */ 
#include "debugger.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

unsigned char receive = 0;

void debug_init() {
	CLOCK_DDR |= (1<<CLOCK_PIN); //Clock Ausgang
	DATA_DDR |= (1<<DATA_PIN); //Data Ausgang
}

void send (uint8_t letter_value) { //char wird direkt in ASCII Dezimal umgewandelt
	if (receive == 0) {
		uint8_t count = 0;
	
		while (count < 7) { //Es müssen 7 Bit übertragen werden
			_delay_us(20); //Wartezeit zum lesen am Debugger
		
			if (letter_value%2 == 1) { //Umrechnung in eine Dualzahl
				DATA_PORT |= (1<<DATA_PIN); //Durch Pullup am Debugger ist logisch 1 low
			} else {
				DATA_PORT &= ~(1<<DATA_PIN);
			}
			letter_value = letter_value / 2;
		
			CLOCK_PORT &= ~(1<<CLOCK_PIN); //Übertragungstakt toggeln
			CLOCK_PORT |= (1<<CLOCK_PIN);
		
			count += 1; //Hochsetzen des Bitzählers
		}
	}
}

void send_string(const char *data ){
	uint8_t sent = 0;
	while( *data != '\0' && sent < 20) { //Sendet maximal 20 Zeichen, egal wie lang die Eingabe ist
		send( *data++ );
		sent = sent + 1;
	}
}

void debug_string(const char *text) {
	send(1);
	send_string(text);
	send(2);
}

void debug_int(const char *name, signed long integer) {
	send(1);
	send_string(name);
	send_string(": ");
	char snum[6];
	send_string(itoa(integer, snum, 10));
	send(2);
}

void breakpoint(const char *name) {
	DDRC |= (1<<4); //Debug LED TODO: remove
	
	send(3);
	send_string(name);
	send(2);
	
	_delay_ms(10); //Auf Empfang umschalten, Wartezeit für Pegelwechsel abwarten
	receive = 1;
	DATA_DDR &= ~(1<<DATA_PIN);
	DATA_PORT|=(1<<DATA_PIN);
	CLOCK_PORT &= ~(1<<CLOCK_PIN); //Clock Pin ist HIGH während receive
	_delay_ms(10);
	
	while ((DATA_INPUT_PIN & (1<<DATA_PIN))) { //Auf Pegeländerung warten zum Fortfahren
		PORTC |= (1<<4);
		_delay_ms(10);
	}
	
	PORTC &= ~(1<<4);
	_delay_ms(10);
	receive = 0;
	
	DATA_DDR |= (1<<DATA_PIN);
	DATA_PORT|=(1<<DATA_PIN);
	CLOCK_PORT |= (1<<CLOCK_PIN);
	
	_delay_ms(500);
	send(4);
}