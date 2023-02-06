// Project UID 1d9f47bfc76643019cfbf037641defe1
#include "Player.h"

#include <assert.h>

Player* Player_factory(const std::string& /*name*/, const std::string& /*strategy*/) {
	assert(false);
	return nullptr;
}

std::ostream& operator<<(std::ostream& os, const Player& /*p*/) {
	assert(false);
	return os;
}
