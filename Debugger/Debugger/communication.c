/*
 * communication.c
 *
 * Created: 24.08.2014 00:43:54
 *  Author: Robin
 */ 

#define F_CPU 8000000

#include <stdint.h>
#include "history.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

char *ascii[123]; //TODO: Unsigned!
unsigned char breaked = 0;
uint8_t bit_count = 0;
uint8_t bit_value[7] = {1, 2, 4, 8, 16, 32, 64};
uint8_t bit_buffer = 0;
unsigned char paused = 0;

void init_communication(void) { //TODO: rename (zeile 31)
	//Datenpins initialisieren
	DDRD &= ~(1<<2); //Clock Pin
	PORTD|=(1<<2);
	
	DDRD &= ~(1<<3); //Data Pin
	PORTD|=(1<<3);
	MCUCR |= (1<<ISC01) | (1<<ISC00); //Steigende Flanke, Interrupt (INT0) für Datentakt
	GICR  |= (1<<INT0);
}

void dict_initialization() {
		//Index von Befehlen ist kleiner als 32
	
		ascii[32] = " "; //Sonderzeichen
		ascii[33] = "!";
		ascii[34] = "\"";
		ascii[35] = "#";
		ascii[36] = "$";
		ascii[37] = "%";
		ascii[38] = "&";
		ascii[39] = "'";
		ascii[40] = "(";
		ascii[41] = ")";
		ascii[42] = "*";
		ascii[43] = "+";
		ascii[44] = ",";
		ascii[45] = "-";
		ascii[46] = ".";
		ascii[47] = "/";
		
		ascii[58] = ":";
		ascii[59] = ";";
		ascii[60] = "<";
		ascii[61] = "=";
		ascii[62] = ">";
		ascii[63] = "?";
		ascii[64] = "@";
		
		ascii[91] = "[";
		ascii[92] = "\"";
		ascii[93] = "]";
		ascii[94] = "^";
		ascii[95] = "_";
		ascii[96] = "`";
		
		ascii[123] = "{";
		ascii[124] = "|";
		ascii[125] = "}";
		ascii[126] = "~";
		
		ascii[48] = "0"; //Zahlen
		ascii[49] = "1";
		ascii[50] = "2";
		ascii[51] = "3";
		ascii[52] = "4";
		ascii[53] = "5";
		ascii[54] = "6";
		ascii[55] = "7";
		ascii[56] = "8";
		ascii[57] = "9";
		
		ascii[65] = "A"; //Großbuchstaben
		ascii[66] = "B";
		ascii[67] = "C";
		ascii[68] = "D";
		ascii[69] = "E";
		ascii[70] = "F";
		ascii[71] = "G";
		ascii[72] = "H";
		ascii[73] = "I";
		ascii[74] = "J";
		ascii[75] = "K";
		ascii[76] = "L";
		ascii[77] = "M";
		ascii[78] = "N";
		ascii[79] = "O";
		ascii[80] = "P";
		ascii[81] = "Q";
		ascii[82] = "R";
		ascii[83] = "S";
		ascii[84] = "T";
		ascii[85] = "U";
		ascii[86] = "V";
		ascii[87] = "W";
		ascii[88] = "X";
		ascii[89] = "Y";
		ascii[90] = "Z";
		
		ascii[97] = "a"; //Kleinbuchstaben
		ascii[98] = "b";
		ascii[99] = "c";
		ascii[100] = "d";
		ascii[101] = "e";
		ascii[102] = "f";
		ascii[103] = "g";
		ascii[104] = "h";
		ascii[105] = "i";
		ascii[106] = "j";
		ascii[107] = "k";
		ascii[108] = "l";
		ascii[109] = "m";
		ascii[110] = "n";
		ascii[111] = "o";
		ascii[112] = "p";
		ascii[113] = "q";
		ascii[114] = "r";
		ascii[115] = "s";
		ascii[116] = "t";
		ascii[117] = "u";
		ascii[118] = "v";
		ascii[119] = "w";
		ascii[120] = "x";
		ascii[121] = "y";
		ascii[122] = "z";
		
		
		
	}
	
void command (uint8_t id) { //Führt Steuerzeichen aus
	if (id == 1 && paused == 0) { //Neue Nachricht
		set_history("~");
	} else if (id == 2 && paused == 0) { //Nachricht ende
		index_up();
	} else if (id == 3) { //Breakpoint
		set_history("!Break: ");
		breaked = 1;
		paused = 0;
	} else if (id == 4) {
		breaked = 0;
		paused = 0;
	}
}

void process_transfer (uint8_t id) { //Verarbeitet empfangende Daten
	if (id < 32) {
		command(id);
	} else if (paused == 0 || breaked == 1) {
		const char *x = ascii[id];
		add_history(x);
	} else {
		bit_buffer = 0;
		bit_count = 0;
	}
}

void reset_communication(void) {
	bit_buffer = 0; //Setzt Kommunikationsvariablen zurück
	bit_count = 0;
	paused = 0;
	breaked = 0;
	DDRD |= (1<<3); //Pin Eingänge wiederherstellen
	PORTD &= ~(1<<3);
	DDRD &= ~(1<<3);
	PORTD|=(1<<3);
}

void breakpoint_continue(void) { //Sendet den Impuls zum Fortfahren und dreht die Datenleitung anschließend wieder um
	if (breaked == 1) {
		DDRD |= (1<<3);
		PORTD &= ~(1<<3);
		_delay_ms(100);
		DDRD &= ~(1<<3);
		PORTD|=(1<<3);
		reset_communication();
	}
}

unsigned char is_breaked(void) {
	return (breaked);
}

void pause (unsigned char set) {
	if (set == 1 || set == 0) {
		paused = set;
	}
}

unsigned char is_paused (void) {
	return (paused);
}

ISR(INT0_vect) { //Interrupt bei Datentakt
	if (PIND & (1<<3)) { //Logisch 1, Addition des Bit Values
		bit_buffer = bit_buffer + bit_value[bit_count];
	}
		
	bit_count += 1;

	if (bit_count == 7) {
		process_transfer(bit_buffer);
		bit_count = 0;
		bit_buffer = 0;
	}
}