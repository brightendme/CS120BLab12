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
#include "io.c"
#endif
unsigned short index_count = 1;
enum STATES { KPstate };

unsigned char keypad_input;
/*int KPTick(int state){
	switch(state){
	case KPstate:
		keypad_input = GetKeypadKey();
	break;
	}
}
enum
*/
int tick(int state) {
  //unsigned char keypad_input;
   switch(state) {
    case KPstate:
	    keypad_input = GetKeypadKey();
	    //LCD_Cursor(2);
	    if(keypad_input != '\0'){
		//LCD_ClearScreen();
    		
	        LCD_Cursor(index_count);		
	    	LCD_WriteData(keypad_input);
		if(index_count <=15){
			index_count ++;
		}
		else{
			index_count = 1;
		}
	    }
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
	   task1.period = 50;//Task Period.
	   task1.elapsedTime = 50;//Task current elapsed time.
           task1.TickFct = &tick;//Function pointer for the tick.

	   TimerSet(10);
	   TimerOn();
	   LCD_init();
	   LCD_DisplayString(1,"Congratulations!");
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
