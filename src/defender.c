// Standard headers
#include <stdio.h>

// Internal headers
#include "direction.h"
#include "position.h"
#include "spy.h"

// Main header
#include "defender.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

/*----------------------------------------------------------------------------*/
/*                             PRIVATE FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

void usualOrderDirections(direction_t array[], int state);
void switchUpDownPriority(direction_t array[]);

void usualOrderDirections(direction_t array[], int state) {
	if (state != 1) {
		array[0] = (direction_t) DIR_UP;
		array[1] = (direction_t) DIR_RIGHT;
		array[2] = (direction_t) DIR_DOWN;
		array[3] = (direction_t) DIR_LEFT;
	}
}

void switchUpDownPriority(direction_t array[]) {
	direction_t temp = array[0];
	array[0] = array[2];
	array[2] = temp;
}

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_defender_strategy(
    position_t defender_position, Spy attacker_spy) {
	static direction_t orderDirections[4];
	static position_t lastPosition = INVALID_POSITION;
	static int horizontalOffset = 0;
	static int state = 0;
	static int round = 0;
	static int pointerOrderArray = 0;
	int horizontalOpening = 3;

	usualOrderDirections(orderDirections, state);
	round++;

	// Nos três primeiros rounds, anda "cegamente" para frente.
	if (round < 4) {
		horizontalOffset++;
		return orderDirections[3];
	}
	// Após isso, dispara o spy e vai para a região vertical em que o
	// atacante se encontra.
	if (round == 4) {
		position_t attackerPosition = get_spy_position(attacker_spy);
		if (attackerPosition.i - defender_position.i > 0) {
			switchUpDownPriority(orderDirections);
			state = 1;
		}
	}
	// Utilizando-se de uma estratégia de "goleiro", o defensor fica "cercando"
	// uma região no tabuleiro, onde é provável que o atacante se encontre.
	if (horizontalOffset > horizontalOpening || 
	    !equal_positions(lastPosition, defender_position)) {
		pointerOrderArray = 0;
	} else {
		pointerOrderArray++;
	}

	if (pointerOrderArray == 3) {
		horizontalOffset++;
	} else if (pointerOrderArray == 1) {
		horizontalOffset--;
	}

	lastPosition = defender_position;
	return orderDirections[pointerOrderArray];
}

/*----------------------------------------------------------------------------*/
