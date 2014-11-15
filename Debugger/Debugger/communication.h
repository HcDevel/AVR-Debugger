/*
 * communication.h
 *
 * Created: 24.08.2014 00:43:38
 *  Author: Robin
 */ 


#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

void dict_initialization (void);
void command (uint8_t id);
void process_transfer (uint8_t id);
unsigned char is_breaked(void);
void init_communication(void);
void pause (unsigned char set);
unsigned char is_paused (void);
void reset_communication(void);
void breakpoint_continue(void);

#endif /* COMMUNICATION_H_ */