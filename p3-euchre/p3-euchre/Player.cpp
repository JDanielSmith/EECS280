// Project UID 1d9f47bfc76643019cfbf037641defe1
#include "Player.h"

#include <assert.h>

#include <memory>
#include<stdexcept>
#include <vector>
#include <set>
#include <algorithm>
#include <optional>

static Card find_lowest(const std::set<Card>& hand, Suit trump)
{
	assert(!hand.empty());
	Card retval(Rank::JACK, trump); // highest card
	for (const auto& c : hand)
	{
		if (Card_less(c, retval, trump))
		{
			retval = c;
		}
	}
	assert(hand.contains(retval));
	return retval;
}

static std::optional<Card> follow_suit(const std::set<Card>& hand, const Card& led_card, Suit trump)
{
	assert(!hand.empty());
	std::optional<Card> retval;
	for (const auto& c : hand)
	{
		if ((c.get_suit() == led_card.get_suit()) || c.is_left_bower(trump))
		{
			if (!retval.has_value())
			{
				retval = c;
			}
			else
			{
				if (Card_less(*retval, c, led_card, trump))
				{
					*retval = c;
				}
			}
		}
	}
	return retval;
}

class SimplePlayer final : public Player
{
	std::string name_;
	std::set<Card> hand;

public:
	explicit SimplePlayer(const std::string& name) : name_(name) {}

	const std::string& get_name() const override
	{
		return name_;
	}

	void add_card(const Card& c) override
	{
		if (hand.size() >= MAX_HAND_SIZE)
		{
			throw std::logic_error("add_card(): hand.size() >= MAX_HAND_SIZE");
		}
		hand.insert(c);
	}

	bool make_trump(const Card& upcard, bool is_dealer,
		int round, Suit& order_up_suit) const override
	{
		if ((round != 1) && (round != 2))
		{
			throw std::invalid_argument("make_trump(): round");
		}

		Suit trump = upcard.get_suit();
		if (round == 1)
		{
			// "They will order up if that would mean they have two or more cards that are either face or ace cards of the
			// trump suit (the right and left bowers, and Q, K, A of the trump suit, which is the suit proposed by the upcard)."
			int good_count = 0;
			for (const auto& c : hand)
			{
				if (c.is_face_or_ace() && c.is_trump(trump))
				{
					good_count++;
				}
			}
			if (good_count >= 2)
			{
				// "If Player wishes to order up a trump suit then return true and change order_up_suit to desired suit.
				// If Player wishes to pass, then do not modify order_up_suit and return false."
				order_up_suit = trump;
				return true;
			}
			return false;
		}

		assert(round == 2);
		
		// "During round two, a Simple Player considers ordering up the suit with the same color as the upcard, which would
		// make that suit trump."
		trump = Suit_next(trump);

		// "They will order up if that would mean they have one or more cards that are either face or ace cards of the trump
		// suit in their hand (the right and left bowers, and Q, K, A of the order-up suit). For example, if the upcard is a Heart
		// and the player has the King of Diamonds in their hand, they will order up Diamonds."
		int good_count = 0;
		for (const auto& c : hand)
		{
			if (c.is_face_or_ace() && c.is_trump(trump))
			{
				good_count++;
			}
		}
		if (good_count >= 1)
		{
			order_up_suit = trump;
			return true;
		}

		// "If making reaches the dealer during the second round, we invoke screw the dealer, where the dealer is forced to
		// order up. In the case of screw the dealer, the dealer will always order up the suit with the same color as the upcard."
		if (is_dealer)
		{
			order_up_suit = trump;
			return true;
		}

		// "The Simple Player will not order up any other suit."
		return false;		
	}

	void add_and_discard(const Card& upcard) override
	{
		assert(hand.size() == MAX_HAND_SIZE);

		// "If the trump suit is ordered up during round one, the dealer picks up the upcard.
		// The dealer then discards the lowest card in their hand, even if this is the upcard, ..."
		const auto trump = upcard.get_suit();
		assert(upcard.is_trump(trump));

		hand.insert(upcard);
		assert(hand.size() == MAX_HAND_SIZE + 1);
		const auto discard = find_lowest(hand, trump);
		hand.erase(discard);
	}

	Card lead_card(Suit trump) override
	{
		// "... they play the highest non-trump card in their hand. If they have only trump cards,
		// they play the highest trump card in their hand."
		std::optional<Card> non_trump;
		for (const auto& c : hand)
		{
			if (!c.is_trump(trump))
			{
				if (!non_trump.has_value())
				{
					non_trump = c;
				}
				else
				{
					if (Card_less(*non_trump, c, trump))
					{
						*non_trump = c;
					}
				}
			}
		}

		Card played;
		if (non_trump.has_value())
		{
			played = *non_trump;
		}
		else
		{
			played = Card(Rank::NINE, trump); // lowest card
			for (const auto& c : hand)
			{
				if (Card_less(played, c, trump))
				{
					played = c;
				}
			}
		}

		assert(hand.contains(played));
		hand.erase(played);
		return played;
	}

	Card play_card(const Card& led_card, Suit trump) override
	{
		// "If a Simple Player can follow suit, they play the highest card that follows suit.
		// Otherwise, they play the lowest card in their hand."
		Card played{};
		auto followed_suit = follow_suit(hand, led_card, trump);
		if (followed_suit.has_value())
		{
			played = *followed_suit;
		}
		else
		{
			played = find_lowest(hand, trump);
		}

		assert(hand.contains(played));
		hand.erase(played);
		return played;
	}

	~SimplePlayer() = default;
};

class HumanPlayer final : public Player
{
	std::string name_;
	std::set<Card> hand;

public:
	explicit HumanPlayer(const std::string& name) : name_(name) {}

	const std::string& get_name() const override
	{
		return name_;
	}

	void add_card(const Card& c) override
	{
		if (hand.size() >= MAX_HAND_SIZE)
		{
			throw std::logic_error("add_card(): hand.size() >= MAX_HAND_SIZE");
		}
		hand.insert(c);
	}

	bool make_trump(const Card& /*upcard*/, bool /*is_dealer*/,
		int /*round*/, Suit& /*order_up_suit*/) const override
	{
		throw std::logic_error("make_trump()");
	}

	void add_and_discard(const Card& /*upcard*/) override
	{
		throw std::logic_error("add_and_discard()");
	}

	Card lead_card(Suit /*trump*/) override
	{
		throw std::logic_error("lead_card()");
	}

	Card play_card(const Card& /*led_card*/, Suit /*trump*/) override
	{
		throw std::logic_error("play_card()");
	}

	~HumanPlayer() = default;
};


std::unique_ptr<Player> Player_factory(const std::string& name, Strategy strategy) {
	// We need to check the value of strategy and return 
	// the corresponding player type.
	if (strategy == Strategy::Simple) {
		return std::make_unique<SimplePlayer>(name);
	}
	if (strategy == Strategy::Human) {
		return std::make_unique<HumanPlayer>(name);
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
	if (strategy == "Human") {
		return Player_factory(name, Strategy::Human).release();
	}
	// Repeat for each other type of Player

	// Invalid strategy if we get here
	throw std::invalid_argument("strategy");
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
	os << p.get_name();
	return os;
}
