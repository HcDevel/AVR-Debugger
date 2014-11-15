/*
 * debugger.h
 *
 * Created: 24.08.2014 00:07:47
 *  Author: Robin
 */ 


#ifndef DEBUGGER_H_
#define DEBUGGER_H_

//Frequenz des Controllers. Kann auch Global definiert werden. Wichting: Muss korrekt sein, sonst können Daten nicht korrekt übertragen werden!!!
#ifndef F_CPU
#define F_CPU 8000000
#endif

//Clock Pin
#define CLOCK_PORT          PORTC
#define CLOCK_DDR           DDRC
#define CLOCK_PIN           PC3

//Data Pin
#define DATA_PORT           PORTC
#define DATA_DDR            DDRC
#define DATA_PIN            PC5
#define DATA_INPUT_PIN		PINC

void debug_init();
void send(unsigned char letter_value);
void send_string(const char *data);
void debug_string(const char *text);
void debug_int(const char *name, signed long integer);
void breakpoint(const char *name);

#endif /* DEBUGGER_H_ */