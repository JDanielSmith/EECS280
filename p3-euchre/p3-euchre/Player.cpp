// Project UID 1d9f47bfc76643019cfbf037641defe1
#include "Player.h"

#include <assert.h>

#include <memory>
#include<stdexcept>

class SimplePlayer final : public Player
{
	std::string name_;

public:
	explicit SimplePlayer(const std::string& name) : name_(name) {}

	const std::string& get_name() const override
	{
		return name_;
	}

	void add_card(const Card& /*c*/) override
	{

	}

	bool make_trump(const Card& /*upcard*/, bool /*is_dealer*/,
		int /*round*/, Suit& /*order_up_suit*/) const override
	{
		throw std::logic_error("make_trump()");
	}

	void add_and_discard(const Card& /*upcard*/) override
	{

	}

	Card lead_card(Suit /*trump*/) override
	{
		throw std::logic_error("lead_card()");
	}

	Card play_card(const Card& /*led_card*/, Suit /*trump*/) override
	{
		throw std::logic_error("play_card()");
	}

	~SimplePlayer() = default;
};


std::unique_ptr<Player> Player_factory(const std::string& name, Strategy strategy) {
	// We need to check the value of strategy and return 
	// the corresponding player type.
	if (strategy == Strategy::Simple) {
		return std::make_unique<SimplePlayer>(name);
	}
	// Repeat for each other type of Player

	// Invalid strategy if we get here
	throw std::invalid_argument("strategy");
}
Player* Player_factory(const std::string& name, const std::string& strategy)
{
	// We need to check the value of strategy and return 
	// the corresponding player type.
	if (strategy == "Simple") {
		return Player_factory(name, Strategy::Simple).release();
	}
	// Repeat for each other type of Player

	// Invalid strategy if we get here
	throw std::invalid_argument("strategy");
}


std::ostream& operator<<(std::ostream& os, const Player& /*p*/) {
	assert(false);
	return os;
}
