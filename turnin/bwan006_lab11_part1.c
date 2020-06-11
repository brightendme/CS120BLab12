/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "scheduler.h"
#include "keypad.h"
#endif

enum STATES { KPstate };

unsigned char KPout = 0x00;

int tick(int state) {
  unsigned char keypad_input;
  keypad_input = GetKeypadKey();
  switch(state) {
    case KPstate:
    switch(keypad_input) {
      			case '\0':
      			KPout = 0x1F;
      			break;
      			case '1':
      			KPout = 0x01;
      			break;
      			case '2':
      			KPout = 0x02;
      			break;
      			case '3':
      			KPout = 0x03;
      			break;
      			case '4':
      			KPout = 0x04;
      			break;
      			case '5':
      			KPout = 0x05;
      			break;
      			case '6':
      			KPout = 0x06;
      			break;
      			case '7':
      			KPout = 0x07;
      			break;
      			case '8':
      			KPout = 0x08;
      			break;
      			case '9':
      			KPout = 0x09;
      			break;
      			case 'A':
      			KPout = 0x0A;
      			break;
      			case 'B':
      			KPout = 0x0B;
      			break;
      			case 'C':
      			KPout = 0x0C;
      			break;
      			case 'D':
      			KPout = 0x0D;
      			break;
      			case '*':
      			KPout = 0x0E;
      			break;
      			case '0':
      			KPout = 0x00;
      			break;
      			case '#':
      			KPout = 0x0F;
      			break;
      			default:
      			KPout = 0xFF;
      			break;
    }
    state = KPstate;
    PORTA = KPout;
    break;
  }
  return state;
}
int main(void) {
    /* Insert DDR and PORT initializations */
     DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0xFF; PORTB= 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    DDRD = 0xFF; PORTD = 0x00;

    // unsigned long int
    static task task1;
    task *tasks[] = { &task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	   // Task 1
	   task1.state = 0;//Task initial state.
	   task1.period = 2;//Task Period.
	   task1.elapsedTime = 2;//Task current elapsed time.
           task1.TickFct = &tick;//Function pointer for the tick.

	   TimerSet(10);
	   TimerOn();

     unsigned short i; // Scheduler for-loop iterator
   	while(1) {
   		for ( i = 0; i < numTasks; i++ ) {
   			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
   				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
   				tasks[i]->elapsedTime = 0;
   			}
   			tasks[i]->elapsedTime += 1;
   		}
   		while(!TimerFlag);
   		TimerFlag = 0;
   	}
    return 0;
}
