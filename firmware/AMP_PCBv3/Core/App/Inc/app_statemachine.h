/*
 * app_statemachine.h
 *
 *  Created on: Apr 19, 2024
 *      Author: benjaminowen
 */

#ifndef APP_INC_APP_STATEMACHINE_H_
#define APP_INC_APP_STATEMACHINE_H_

typedef enum
{
	STATE_IDLE,		// idle state
	STATE_CONSOLE,	// receiving data from onboard Nucleo UART interface
	STATE_AUTO,		// receiving data from software stack
	STATE_RC,		// receiving data from remote control
	STATE_EBRAKE,	// after receiving e-brake signal
	STATE_ERROR		// catch-all error state
} State_T;

void App_StateMachine_Init();
void App_StateMachine_Tick();
void App_StateMachine_ChangeState(State_T);

#endif /* APP_INC_APP_STATEMACHINE_H_ */
