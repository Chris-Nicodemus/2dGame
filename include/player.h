#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

typedef enum
{
    PlayerGroundIdle,
    PlayerGroundAttack,
    PlayerAirIdle,
    PlayerAirAttack,
    PlayerTransform
} PlayerState;

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
 * @brief delete the current list of entities showing the deck
*/
void player_show_deck_close();

/**
 * @brief do the action based on the card (only necessary when card requires a target(s))
 * @param self the player
 * @param card the card to use
*/
void player_play_card(Entity *self, Entity *card);

/**
 * @brief start the combat. Also checks to see if currentdeck is the same as the sacred deck at the start of the combat. If not, then remake the curent deck
 * @param self the player
*/
void player_combat_start(Entity *self);
/**
 * @brief end the player's turn
 * @param self the player in question
*/
void player_end_turn(Entity *self);

/**
 * @brief discard a card from the player's hand
 * @param self the player
 * @param card the card to discard
*/
void player_discard(Entity *self, Entity *card);


/**
 * @brief discard the player's hand
 * @param self the player
*/
void player_discard_hand(Entity *self);
/**
 * @brief do damage to the player
 * @param player the player to recieve damage
 * @param dealer the entity doing damage
 * @param damage the damage to do
 * @param damageType the type of damage being done
*/
void player_damage(Entity *player, Entity *dealer, int damage, DamageType damageType);

/**
 * @brief reset the player's health and deck when they lose and then open the main menu
 * @param self the player to reset
*/
void player_reset(Entity *self);

/**
 * @brief start the combat. Also checks to see if currentdeck is the same as the sacred deck at the start of the combat. If not, then remake the curent deck
 * @param player1 the first player
 * @param player2 the second player
*/
void player_multi_combat_start(Entity *player1, Entity *player2);

/**
 * @brief ends the current player's turn in a commbat
*/
void player_multi_end_turn();

/*player custom data notes
data is turned into a list
0 - deck            sacred decklist of the player during a run
1 - currentDeck     list of cards in-combat decklist for the player
2 - discard         list of cards used by the player
3 - hand            list of cards in player's current hand
*/
#endif