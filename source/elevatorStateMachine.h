
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
 * @brief Checks if queue is empty.
 * 
 * @param queue A queue of ElevatorOrder
 * 
 * @return Returns 0 if queue is empty. Returns 1 otherwise.
 */
int checkQueue(ElevatorOrder * queue);

/**
 * @brief Clears the queue for all orders and fills it with empty orders.
 * 
 * @param queue A queue of ElevatorOrder
 * @param length Length of queue
 * 
 * @warning Will only be called when stop button is pushed.
 */
void clearQueue(ElevatorOrder * queue, int length);

/**
 * @brief Sets the elevator to floor 1 as default start.
 * 
 */

void elevatorInit();

/**
 * @brief Elevator functionality when stop button is pressed.
 * 
 */
void elevatorSafetyFunction(); //kan ha et bedre navn
/**
 * @brief Changes the state of the elevator when queue is empty.
 * 
 */
void emptyQueue();

/**
 * @brief Waits a @p seconds seconds time while polling buttons.
 * 
 * @param seconds The desired amout of seconds we wish to stall the elevator.
 * 
 */
void timer(int seconds);

/**
 * @brief Checks if elevator is at floor in the first order.
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
 * @brief The logic for the elevator.
 * 
 * @return Returns 1 if elevator is at ordered floor, and 0 if not.
 */

/**
 * @brief Prints the queue in a nice format
 * 
 * @param queue the queue you want to print.
 * @param length length of the queue that you want to print.
 */

void printQueue();

/**
 * @brief Polls the buttons and checks for orders.
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

