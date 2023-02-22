#include <assert.h>
#include <iostream>
#include <vector>
#include <stdexcept>

enum class Rank {TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE};
enum class Suit { SPADES, HEARTS, CLUBS, DIAMONDS };
struct Card final
{
    Card() : Card(Rank::TWO, Suit::SPADES) {}
    Card(Rank rank_in, Suit suit_in) : rank(rank_in), suit(suit_in) {}

    Suit get_suit() const { return suit; }
    Rank get_rank() const { return rank; }

private:
    Rank rank;
    Suit suit;
};
static bool less(const Card& lhs, const Card& rhs)
{
    // "In the simplest case, cards are ordered by rank (A > K > Q > J > 10 > 9), ..."
    if (lhs.get_rank() < rhs.get_rank())
    {
        return true;
    }
    if (lhs.get_rank() > rhs.get_rank())
    {
        return false;
    }
    assert(lhs.get_rank() == rhs.get_rank());

    // "... with ties broken by suit (D > C > H > S)."
    return lhs.get_suit() < rhs.get_suit();
}
bool operator<(const Card& lhs, const Card& rhs) {
    return less(lhs, rhs);
}
bool operator==(const Card& lhs, const Card& rhs)
{
    if ((lhs < rhs) || (rhs < lhs))
    {
        return false; // can't be equal
    }
    return true; // must be equal
}
bool operator<=(const Card& lhs, const Card& rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}
bool operator>(const Card& lhs, const Card& rhs)
{
    return !(lhs <= rhs);
}
bool operator>=(const Card& lhs, const Card& rhs)
{
    return !(lhs < rhs);
}

bool operator!=(const Card& lhs, const Card& rhs)
{
    return !(lhs == rhs);
}

static bool can_follow_suit(const std::vector<Card>& hand, const Card& led_card, Suit trump)
{
    // TODO: too simple for Euchre!
    for (auto&& card : hand)
    {
        if (card.get_suit() == led_card.get_suit())
        {
            return true;
        }
    }

    return false;
}

static Card highest_card(const std::vector<Card>& hand, const Card& led_card, Suit trump)
{
    Card retval;

    // TODO: too simple for Euchre!
    for (auto&& card : hand)
    {
        if (card > retval)
        {
            retval = card;
        }
    }

    return retval;
}

static Card lowest_card(const std::vector<Card>& hand, Suit trump)
{
    Card retval(Rank::ACE, Suit::SPADES); // highest possible card

    // TODO: too simple for Euchre!
    for (auto&& card : hand)
    {
        if (card < retval)
        {
            retval = card;
        }
    }

    return retval;
}

static Card play_card(const std::vector<Card>& hand, const Card& led_card, Suit trump)
{
    // "If a Simple Player can follow suit, ..."
    if (can_follow_suit(hand, led_card, trump))
    {
        // "... they play the highest card that follows suit."
        return highest_card(hand, led_card, trump);
    }

    // "Otherwise, they play the lowest card in their hand."
    return lowest_card(hand, trump);
}

static const std::vector<Card> hand{
    Card(Rank::TEN, Suit::SPADES),
    Card(Rank::JACK, Suit::SPADES),
    Card(Rank::QUEEN, Suit::SPADES),
    Card(Rank::KING, Suit::SPADES),
    Card(Rank::ACE, Suit::SPADES) };
static void test_can_follow_suit()
{
    Card led_card(Rank::NINE, Suit::SPADES);
    assert(can_follow_suit(hand, led_card, Suit::SPADES));
    assert(can_follow_suit(hand, led_card, Suit::CLUBS));

    led_card = Card(Rank::NINE, Suit::CLUBS);
    assert(!can_follow_suit(hand, led_card, Suit::SPADES));
    assert(!can_follow_suit(hand, led_card, Suit::CLUBS));
}

static void test_highest_card()
{
    Card led_card(Rank::NINE, Suit::SPADES);
    auto actual = highest_card(hand, led_card, Suit::SPADES);
    assert(actual == Card(Rank::ACE, Suit::SPADES));
    actual = highest_card(hand, led_card, Suit::CLUBS);
    assert(actual == Card(Rank::ACE, Suit::SPADES));

    led_card = Card(Rank::NINE, Suit::CLUBS);
    actual = highest_card(hand, led_card, Suit::SPADES);
    assert(actual == Card(Rank::ACE, Suit::SPADES));
    actual = highest_card(hand, led_card, Suit::CLUBS);
    assert(actual == Card(Rank::ACE, Suit::SPADES));
}

static void test_lowest_card()
{
    auto actual = lowest_card(hand,  Suit::SPADES);
    assert(actual == Card(Rank::TEN, Suit::SPADES));
    actual = lowest_card(hand,  Suit::CLUBS);
    assert(actual == Card(Rank::TEN, Suit::SPADES));

    actual = lowest_card(hand,  Suit::SPADES);
    assert(actual == Card(Rank::TEN, Suit::SPADES));
    actual = lowest_card(hand,  Suit::CLUBS);
    assert(actual == Card(Rank::TEN, Suit::SPADES));
}

static void test_play_card()
{
    Card led_card(Rank::NINE, Suit::SPADES);
    auto actual = play_card(hand, led_card, Suit::SPADES);
    assert(actual == Card(Rank::ACE, Suit::SPADES));
    actual = play_card(hand, led_card, Suit::CLUBS);
    assert(actual == Card(Rank::ACE, Suit::SPADES));

    led_card = Card(Rank::NINE, Suit::CLUBS);
    actual = play_card(hand, led_card, Suit::SPADES);
    assert(actual == Card(Rank::TEN, Suit::SPADES));
    actual = play_card(hand, led_card, Suit::CLUBS);
    assert(actual == Card(Rank::TEN, Suit::SPADES));
}

int main()
{
    test_can_follow_suit();
    test_highest_card();
    test_lowest_card();
    test_play_card();
    std::cout << "Hello World!\n";
}
