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
//enum STATES { KPstate };

//unsigned char keypad_input;
/*int KPTick(int state){
	switch(state){
	case KPstate:
		keypad_input = GetKeypadKey();
	break;
	}
}
enum
*/
unsigned short userLoc;
enum userStates{start, up, down} userStates;
int userTick(int state){
	unsigned char userIn;
	userIn = GetKeypadKey();
	switch(state){
		case start:
		state = up;
		LCD_Cursor(1);
		LCD_WriteData('*');
		userLoc = 1;
		break;
		case up:
		if(userIn == '2'){
			LCD_Cursor(1);
			LCD_WriteData(' ');
			LCD_Cursor(17);
			LCD_WriteData('*');
			userLoc = 17;
			state = down;
		}
		else{
			state = up;
			LCD_Cursor(1);
			LCD_WriteData('*');
			userLoc = 1;
			//LCD_Cursor(1);
			//LCD_WriteData(' ');

		}
		break;
		case down:
		if(userIn == '5'){
			LCD_Cursor(17);
			LCD_WriteData(' ');
			LCD_Cursor(1);
			LCD_WriteData('*');
			userLoc = 1;
			state = up;
		}
		else{
			state = down;
			LCD_Cursor(17);
			LCD_WriteData('*');
			userLoc = 17;
		}
		break;
		default:
		state = up;
		break;
	}
	return state;
}
enum enemyState{create}enemyState;
unsigned short topLoc = 0;
unsigned short botLoc = 16;
unsigned char botOK = 0x00;
unsigned char topOK = 0x00;
int enemyTick(int state) {
	switch(state){
		case create:
			if(topLoc == 0  && botLoc == 16){
				topLoc = 16;
				topOK = 0x01;
			}
			if(topLoc == 13){
				botLoc = 32;
				botOK = 0x01;
			}
			if(topOK == 0x01){
				if(topLoc <=15){
				LCD_Cursor(topLoc + 1);
				LCD_WriteData(' ');
				}
				LCD_Cursor(topLoc);
				LCD_WriteData('#');
				topLoc --;
			}
			if(topLoc < 1 && topOK == 0x01){
				topOK = 0x00;
				LCD_Cursor(1);
				LCD_WriteData(' ');
			}
			if(botOK == 0x01){
				if(botLoc <= 31){
				LCD_Cursor(botLoc+1);
				LCD_WriteData(' ');
				}
				LCD_Cursor(botLoc);
				LCD_WriteData('#');
				botLoc--;
			}
			if(botLoc < 17 && botOK == 0x01){
				botOK = 0x00;
				LCD_Cursor(17);
				LCD_WriteData(' ');
			}
			break;
	}
	state = create;
  return state;
}
enum collisionState{noCollide, yesCollide, pause}collisionState;
int collisionTick(int state){
	switch(state){
		unsigned char temp;
	        temp = GetKeypadKey();
		case noCollide:
			if(userLoc == topLoc){
				state = yesCollide;
			}
			else if (userLoc == botLoc){
				state = yesCollide;
			}
			else{
			state = noCollide;
			temp = GetKeypadKey();
			if(temp == '1'){
				state = yesCollide;
			}
			}
			break;
		case yesCollide:
			temp = GetKeypadKey();
			if(temp == '1'){
				userLoc = 1;
				topLoc = 0;
				botLoc = 16;
				LCD_ClearScreen();
				state = noCollide;
			}
			else{
				state = yesCollide;
				LCD_Cursor(1);
				LCD_DisplayString(1, "~~~~~~PRESS ONE TO RESTART~~~~~~");

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
    static task task1, task2,task3;
    task *tasks[] = { &task1, &task2,&task3};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	   // Task 1

	   task1.state = 0;//Task initial state.
	   task1.period = 5;//Task Period.
	   task1.elapsedTime = 5;//Task current elapsed time.
           task1.TickFct = &userTick;//Function pointer for the tick.
	   
	   task2.state = 0;
	   task2.period = 100;
	   task2.elapsedTime = 100;
	   task2.TickFct = &enemyTick;
	
	   task3.state = 0;//Task initial state.
	   task3.period = 5;//Task Period.
	   task3.elapsedTime = 5;//Task current elapsed time.
           task3.TickFct = &collisionTick;//Function pointer for the tick.
	  
	   TimerSet(10);
	   TimerOn();
	   LCD_init();
	   LCD_ClearScreen();
	  // LCD_DisplayString(1,"Congratulations!");
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
