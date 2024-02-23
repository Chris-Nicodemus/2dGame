#ifndef __CARD_H__
#define __CARD_H__

#include "entity.h"


/**
 * @brief create a card so the player can see their hand
 * @param name the type of card
 * @return reference to the card entity
*/
Entity *card_new(char *name);

#endif