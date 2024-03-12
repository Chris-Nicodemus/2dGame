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
void player_shuffle(Entity *self);

/**
 * @brief draws certain number of cards from deck. stops drawing if too many cards in hand. if there are no more cards in currentDeck, reshuffles.
 * @param self player in question
 * @param num number of cards to draw
*/
void player_draw(Entity *self, Uint8 num);

/**
 * @brief arranges cards on the screen
 * @param self player in question
*/
void player_arrange_hand(Entity *self);

/**
 * @brief make a new deck for player
 * @param self player in question
*/
void player_new_deck(Entity *self);

/**
 * @brief save the player's deck to a json file
 * @param self the player in question
*/
void player_save_deck(Entity *self);

/**
 * @brief load the player's deck from config
 * @param self the player in question
*/
void player_load_deck(Entity *self);

/**
 * @brief draw the entire deck for the player to view what their deck is
*/
void player_show_deck(Entity *self);

/**
 * @delete the current list of entities showing the deck
*/
void player_show_deck_close();

/*player custom data notes
data is turned into a list
0 - deck            sacred decklist of the player during a run
1 - currentDeck     list of cards in-combat decklist for the player
2 - discard         list of cards used by the player
3 - hand            list of cards in player's current hand
*/
#endif