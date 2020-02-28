
/**
 * @file
 * @brief Elevator functionality
 *
 * 
 */

#ifndef ELEVATORSTATEMACHINE_H
#define ELEVATORSTATEMACHINE_H
#include <stdio.h>
#include "queue.h"


/**
 * @brief Defines states used in ElevatorFSM
 */
typedef enum {
	MOVE_UP,
	MOVE_DOWN, 
	STANDING_STILL, 
	DOOR_OPEN
} State;

extern State g_state;
extern State g_prev_state;
extern int g_queue_length;
extern int g_floor;
extern ElevatorOrder g_queue[10];

/**
 * @brief Functionality for when desired floor is reached
 * 
 * @param floor The desired floor the elevator has reached
 */
void floorReached(int floor);


/**
 * @brief Sets the elevator to floor 1 as default start.
 * 
 */

void elevatorInit();

/**
 * @brief Elevator functionality when stop button is pressed or the obstuction switch is active.
 * 
 */
void elevatorSafetyFunction(); 

/**
 * @brief Waits a @p seconds seconds time while polling buttons.
 * 
 * @param seconds The desired amout of seconds we wish to stall the elevator.
 * 
 */
void timer(int seconds);

/**
 * @brief Checks if elevator is at floor in the first element of the queue.
 * 
 * 
 */
int checkDestination();

/**
 * @brief Sets the order lights and floor indicator lights based on where the elevator is.
 * 
 */
void checkAndSetLights();

/**
 * @brief Turns off all order lights.
 * 
 */
void clearAllOrderLights();


/**
 * @brief Polls the buttons and checks for orders. If there is any it calls addOrder() with that order.
 * 
 *
 */

void checkAndAddOrders();

/**
 * @brief Polls floor sensors and updates g_floor.
 * 
 *
 */

void updateCurrentFloor();

/**
 * @brief Finite state machine for the elevator. Contains logic for switching states.
 * 
 *
 */

int FSM();


#endif

