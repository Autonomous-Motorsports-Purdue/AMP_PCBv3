/*
 * app_statemachine.c
 *
 *  Created on: Apr 19, 2024
 *      Author: benjaminowen
 */

#include "app_statemachine.h"
#include "driver_fan.h"
#include "driver_ebrake.h"
#include "driver_status_led.h"

State_T current_state;

// function to be called before tick() function
void App_StateMachine_Init()
{
	// set current state to idle
	current_state = STATE_IDLE;
	// turn all fans off (should be default state anyway)
	Driver_Fan_All_Off();
	// initialize e-brake
	Driver_EBrake_Init();
}

// function to be called on timer interrupt
void App_StateMachine_Tick()
{
	// statements to be called regardless of state
	//
	// run state-specific code
	switch (current_state)
	{
		case (STATE_IDLE):
		{
			Driver_Status_LED_SetHex(0x1);
			App_StateMachine_ChangeState(STATE_AUTO);
			break;
		}

		case (STATE_AUTO):
		{
			Driver_Status_LED_SetHex(0x2);
			App_StateMachine_ChangeState(STATE_RC);
			break;
		}

		case (STATE_RC):
		{
			Driver_Status_LED_SetHex(0x3);
			App_StateMachine_ChangeState(STATE_EBRAKE);
			break;
		}

		case (STATE_EBRAKE):
		{
			Driver_Status_LED_SetHex(0x4);
			App_StateMachine_ChangeState(STATE_ERROR);
			break;
		}

		case (STATE_ERROR):
		{
			Driver_Status_LED_SetHex(0x8);
			App_StateMachine_ChangeState(STATE_IDLE);
			break;
		}
	}
}

void App_StateMachine_ChangeState(State_T new_state)
{
	// statements to be called regardless of state transition
	//
	// change state
	current_state = new_state;
}
