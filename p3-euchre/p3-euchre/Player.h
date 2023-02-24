#pragma once

/* Player.h
 *
 * Euchre player interface
 * Project UID 1d9f47bfc76643019cfbf037641defe1
 *
 * by Andrew DeOrio
 * awdeorio@umich.edu
 * 2014-12-21
 */

#include <string>
#include <vector>
#include <memory>

#include "../starter-files/Player.h"

enum class Strategy
{
	Simple,
	Human
};

//EFFECTS: Returns a pointer to a player with the given name and strategy
//To create an object that won't go out of scope when the function returns,
//use "return new Simple(name)" or "return new Human(name)"
//Don't forget to call "delete" on each Player* after the game is over
std::unique_ptr<Player> Player_factory(const std::string &name, Strategy);

//EFFECTS: Prints player's name to os
std::ostream & operator<<(std::ostream &os, const Player &p);

