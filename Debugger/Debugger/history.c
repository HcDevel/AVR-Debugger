/*
 * history.c
 *
 * Created: 08.10.2014 22:47:09
 *  Author: Robin
 */ 

#include <string.h>
#include "history.h"
unsigned char history_index = 0;
char history[7][20] = {" ", " ", " ", " ", " ", " ", " "}; //Maximal 7, Leerzeichen erforderlich
unsigned char modified = 0;


void set_index (unsigned char new_index) { //TODO: rename to set_history_index
	history_index = new_index;
}

void index_up (void) {
	
	if (history_index < 6) {
		history_index += 1; //TODO: ++
	} else {
		history_index = 0;
	}
}

void index_down (void) {
	
	if (history_index > 0) {
		history_index -= 1; //TODO --
	} else {
		history_index = 6;
	}
}

void set_history (const char *text) { //TODO: rename to set_history_index
	strcpy(history[history_index], text);
}

unsigned char get_history_index(void) {
	return (history_index);
}

void add_history (const char *text) {
	strcat(history[history_index], text);
	modified = 1;
}

char get_history (unsigned char index) {
	return (history[index]);
}

unsigned char history_modified (void) {
	return (modified);
}

void history_refreshed (void) {
	modified = 0;
}

void update_lcd(void) {
	modified = 1;
}

void history_clear (void) {
	history_index = 0;
	while (history_index < 7) {
		strcpy(history[history_index], "");
		history_index += 1;
	}
	modified = 1;
	
	history_index = 0;
}