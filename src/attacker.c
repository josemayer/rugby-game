// Standard headers
#include <stdio.h>

// Internal headers
#include "direction.h"
#include "position.h"
#include "spy.h"

// Main header
#include "attacker.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

/*----------------------------------------------------------------------------*/
/*                              PRIVATE FUNCTIONS                             */
/*----------------------------------------------------------------------------*/

void usualStateDirectionsArray(direction_t array[], int state);
void swapPriorityUpDirection(direction_t array[]);
void swapPriorityDownDirection(direction_t array[]);

void usualStateDirectionsArray(direction_t array[], int state) {
	if (state != 1) {
		array[0] = (direction_t) DIR_RIGHT;
		array[1] = (direction_t) DIR_UP_RIGHT;
		array[2] = (direction_t) DIR_DOWN_RIGHT;
		array[3] = (direction_t) DIR_UP;
		array[4] = (direction_t) DIR_DOWN;
		array[5] = (direction_t) DIR_UP_LEFT;
		array[6] = (direction_t) DIR_DOWN_LEFT;
		array[7] = (direction_t) DIR_LEFT;
	}
}

void turnPriorityUpDirection(direction_t array[]) {
	array[0] = (direction_t) DIR_UP_RIGHT;
	array[1] = (direction_t) DIR_RIGHT;
	array[2] = (direction_t) DIR_DOWN_RIGHT;
}

void turnPriorityDownDirection(direction_t array[]) {
	array[0] = (direction_t) DIR_DOWN_RIGHT;
	array[1] = (direction_t) DIR_RIGHT;
	array[2] = (direction_t) DIR_UP_RIGHT;
}

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_attacker_strategy(
    position_t attacker_position, Spy defender_spy) {
	static int round = 0;
	static int state = 0;
	static position_t lastPosition = INVALID_POSITION;
	static direction_t dirVector[8];
	static int directionPointer = 0;

	usualStateDirectionsArray(dirVector, state);
	round++;
	// A partir do 4º round é possível ter uma "visão geral" do movimento
	// do defensor por meio de sua posição. Até lá, o atacante recua ou
	// permanece na mesma região.
	if (round < 4) 
		return dirVector[7];
	// No round 3, o spy é disparado e ocorre a verificação de se o defensor
	// está acima ou abaixo da posição do atacante. Então, altera-se o vetor
	// de direções para uma direção que seja oposta àquela da posição do
	// defensor no momento do spy.
	if (round == 4) {
		position_t defenderPosition = get_spy_position(defender_spy);
		state = 1;
		if (defenderPosition.i - attacker_position.i > 0) {
			turnPriorityUpDirection(dirVector);
		} else {
			turnPriorityDownDirection(dirVector);
		}

	}
	
	if (!equal_positions(lastPosition, attacker_position)) {
		directionPointer = 0;
	} else {
		directionPointer++;
	}

	lastPosition = attacker_position;
	return dirVector[directionPointer];
}

/*----------------------------------------------------------------------------*/
