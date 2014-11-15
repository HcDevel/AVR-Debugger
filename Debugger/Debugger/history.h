/*
 * history.h
 *
 * Created: 08.10.2014 22:47:24
 *  Author: Robin
 */ 


#ifndef HISTORY_H_
#define HISTORY_H_

void add_history (const char *text);
char get_history(unsigned char index);
void set_history(const char *text);
unsigned char get_history_index(void);
void index_up(void);
void index_down (void);
unsigned char history_modified (void);
void history_refreshed (void);
void update_lcd(void);
void history_clear (void);

#endif /* HISTORY_H_ */