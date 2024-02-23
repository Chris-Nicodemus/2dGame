#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"


/**
 * @brief spawn a player
 * @return pointer to player, or NULL on error
 * @param  pos position that the player will spawn
*/
Entity *player_new(Vector2D pos);

/**
 * @brief shuffles the player's deck
 * @param self player in question
*/
//void player_shuffle(Entity *self);

/*player custom data notes
data is turned into a list
0 - deck            sacred decklist of the player during a run
1 - currentDeck     list of cards in-combat decklist for the player
2 - hand            list of cards in player's current hand
3 - discard         list of cards used by the player
*/
#endif