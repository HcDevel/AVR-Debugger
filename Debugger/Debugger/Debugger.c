#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h> //TODO: remove
#include <string.h>
#include <util/delay.h>
#include "lcd-routines.h"
#include "communication.h"
#include "history.h"

char lcd_buffer[100] = "";

int main(void) {
	//Ausgänge
	DDRB |= (1<<5); //LED Grün
	DDRB |= (1<<4); //LED Gelb
	
	//Eingänge
	DDRD &= ~(1<<5); //Löschen, Rot
	PORTD|=(1<<5);
	
	DDRD &= ~(1<<6); //Hoch scrollen, Grün
	PORTD|=(1<<6);
	
	DDRD &= ~(1<<7); //Runter scrollen, Blau
	PORTD|=(1<<7);
	
	DDRB &= ~(1<<0); //Pause, Violett
	PORTB|=(1<<0);
	
	//LCD Initialisieren
	lcd_init();
	lcd_clear();
	lcd_setcursor(6, 2);
	
	lcd_string("Debugger");
	lcd_setcursor(7, 3);
	lcd_string("bereit");
	
	//Variablen Deklaration
	unsigned char lcd_zeile = 0;
	signed char history_index_cache = 0;
	unsigned char scroll_offset = 0;
	
	unsigned char scroll_up_released = 1;
	unsigned char scroll_down_released = 1;
	unsigned char pause_released = 1;
	
	
	dict_initialization(); //Angepasste ASCII Zeichentabelle laden
	init_communication();
	sei(); //Initialisierung abgeschlossen, Interrupts aktivieren
	
	while(1) {
		//Display Generator
		if (is_breaked() == 1 && scroll_offset > 0) {
			scroll_offset = 0;
			update_lcd();
		}
		
		history_index_cache = get_history_index() - scroll_offset - 4;
		
		if (history_index_cache < 0) {
			history_index_cache += 7; //TODO: ++
		}
		
		while (lcd_zeile < 4 && history_modified() == 1) {
				lcd_setcursor(0, lcd_zeile + 1);
				lcd_string("                    ");
				lcd_setcursor(0, lcd_zeile + 1);
				lcd_string(get_history(history_index_cache));
				
			lcd_zeile += 1;
			
			if (history_index_cache < 6) {
				history_index_cache++;
			} else {
				history_index_cache = 0;
			}
			
		}
		history_refreshed();
		lcd_zeile = 0;
		
		//Buttons
		if (!(PIND & (1<<5))) { //Nachrichten löschen
			history_clear();
			breakpoint_continue();
			reset_communication();
		}
		
		if (!(PIND & (1<<6)) && scroll_up_released == 1) { //Hoch scrollen
			scroll_up_released = 0;
			pause(1);
			if (scroll_offset < 3) { //TODO: Wert anpassen
				scroll_offset++;
				update_lcd();
			}
		} else if ((PIND & (1<<6)) && scroll_up_released == 0) {
			scroll_up_released = 1;
		}
		
		if (!(PIND & (1<<7)) && scroll_down_released == 1) { //Runter scrollen
			scroll_down_released = 0;
			if (scroll_offset > 0) { //TODO: Wert anpassen
				scroll_offset--;
				update_lcd();
			} else {
				pause(0);
			}
		} else if ((PIND & (1<<7)) && scroll_down_released == 0) {
			scroll_down_released = 1;
		}
		
		if (!(PINB & (1<<0)) && pause_released == 1) { //Pause
			pause_released = 0;
			
			if (is_paused() == 0) { //Wenn Pausiert
				pause(1);
			} else {
				pause(0);
				scroll_offset = 0; //Scrolling mit beenden der Pause zurücksetzen
				update_lcd();
			}
			
			if (is_breaked() == 1) { //Wenn warten auf Breakpoint
				breakpoint_continue();
			}
			
		} else if ((PINB & (1<<0)) && pause_released == 0) {
			pause_released = 1;
		}
		
		//Status LEDs
		if (is_paused() == 1 || is_breaked() == 1) { //Paused & Breaked
			PORTB|=(1<<5);
		} else {
			PORTB&=~(1<<5);
		}
		
		if (scroll_offset > 0) { //Scrolling
			PORTB|=(1<<4);
		} else {
			PORTB&=~(1<<4);
		}
		
		
		if (is_breaked() && (PIND & (1<<2))) { //Während umgekehrte Kommunikation läuft, prüfen, ob Debugger im Empfangsmodus ist
			history_clear();
			reset_communication();
		}
		_delay_ms(50);
	}
	return (0) ;
}


