/*
 * app_statemachine.c
 *
 *  Created on: Apr 19, 2024
 *      Author: benjaminowen
 */

#include "app_statemachine.h"
#include "driver_fan.h"
#include "driver_ebrake.h"

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
			break;
		}

		case (STATE_AUTO):
		{
			break;
		}

		case (STATE_RC):
		{
			break;
		}

		case (STATE_EBRAKE):
		{
			break;
		}

		case (STATE_ERROR):
		{
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
