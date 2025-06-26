/*******************************************************************************
* action.h
* GameAction public interface
*******************************************************************************/

#ifndef ACTION_H
#define ACTION_H

extern const void * GameAction;

typedef enum {
    ACTION_SUCCESS,
    ACTION_FAILURE
} GameActionResult;

GameActionResult validate_action(void *action, GameState *state);

#endif