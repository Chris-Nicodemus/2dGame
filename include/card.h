#ifndef __CARD_H__
#define __CARD_H__

#include "entity.h"


/**
 * @brief create a card so the player can see their hand
 * @param name the type of card
 * @param player the owner of the card
 * @return reference to the card entity
*/
Entity *card_new(char *name, Entity *player);

/**
 * @brief gives a string associated with a card
 * @return the string
*/
char *card_get_random();

/**
 * @brief converts card type to string for use
 * @param self the card
 * @return the string for use
*/
char* card_toString(Entity *self);

/**
 * @brief generate a new card for the shop
 * @return reference to the card entity
*/
Entity *card_shop_new();
#endif