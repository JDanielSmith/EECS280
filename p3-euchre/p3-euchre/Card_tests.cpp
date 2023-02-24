// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>

//using namespace std;


TEST(test_card_ctor) {
    const Card default_;
    ASSERT_EQUAL(Rank::TWO, default_.get_rank());
    ASSERT_EQUAL(Suit::SPADES, default_.get_suit());

    const Card c(Rank::ACE, Suit::HEARTS);
    ASSERT_EQUAL(Rank::ACE, c.get_rank());
    ASSERT_EQUAL(Suit::HEARTS, c.get_suit());
}

// Add more test cases here
TEST(test_card_simple_relations)
{
    const Card ace_diamonds(Rank::ACE, Suit::DIAMONDS);
    ASSERT_EQUAL(ace_diamonds, ace_diamonds);

    // "In the simplest case, cards are ordered by rank (A > K > Q > J > 10 > 9), ..."
    const Card king_diamonds(Rank::KING, Suit::DIAMONDS);
    const Card queen_diamonds(Rank::QUEEN, Suit::DIAMONDS);
    ASSERT_TRUE(ace_diamonds > king_diamonds);
    ASSERT_TRUE(king_diamonds > queen_diamonds);
    ASSERT_TRUE(ace_diamonds > queen_diamonds);

    // "... with ties broken by suit (D > C > H > S)."
    const Card ace_clubs(Rank::ACE, Suit::CLUBS);
    const Card ace_hearts(Rank::ACE, Suit::HEARTS);
    const Card ace_spades(Rank::ACE, Suit::SPADES);
    ASSERT_TRUE(ace_diamonds > ace_clubs);
    ASSERT_TRUE(ace_clubs > ace_hearts);
    ASSERT_TRUE(ace_hearts > ace_spades);
    ASSERT_TRUE(ace_diamonds > ace_hearts);
    ASSERT_TRUE(ace_diamonds > ace_spades);
    ASSERT_TRUE(ace_clubs > ace_spades);
}

TEST(test_card_never_bower)
{
    const Card ace_diamonds(Rank::ACE, Suit::DIAMONDS);
    ASSERT_FALSE(ace_diamonds.is_left_bower(Suit::DIAMONDS));
    ASSERT_FALSE(ace_diamonds.is_right_bower(Suit::DIAMONDS));
    ASSERT_FALSE(ace_diamonds.is_left_bower(Suit::HEARTS));
    ASSERT_FALSE(ace_diamonds.is_right_bower(Suit::HEARTS));
}

TEST(test_card_bowers)
{
    const Card jack_diamonds(Rank::JACK, Suit::DIAMONDS);
    const Card jack_hearts(Rank::JACK, Suit::HEARTS);

    ASSERT_FALSE(jack_diamonds.is_left_bower(Suit::CLUBS));
    ASSERT_FALSE(jack_diamonds.is_right_bower(Suit::CLUBS));
    ASSERT_FALSE(jack_diamonds.is_left_bower(Suit::SPADES));
    ASSERT_FALSE(jack_diamonds.is_right_bower(Suit::SPADES));

    ASSERT_TRUE(jack_diamonds.is_right_bower(Suit::DIAMONDS));
    ASSERT_FALSE(jack_diamonds.is_left_bower(Suit::DIAMONDS));
    ASSERT_TRUE(jack_diamonds.is_left_bower(Suit::HEARTS));
    ASSERT_FALSE(jack_diamonds.is_right_bower(Suit::HEARTS));

    ASSERT_TRUE(jack_hearts.is_right_bower(Suit::HEARTS));
    ASSERT_FALSE(jack_hearts.is_left_bower(Suit::HEARTS));
    ASSERT_TRUE(jack_hearts.is_left_bower(Suit::DIAMONDS));
    ASSERT_FALSE(jack_hearts.is_right_bower(Suit::DIAMONDS));
}

TEST(test_card_less_trump)
{
    const Card ace_clubs(Rank::ACE, Suit::CLUBS);
    const Card ace_diamonds(Rank::ACE, Suit::DIAMONDS);
    ASSERT_TRUE(ace_clubs < ace_diamonds);
    ASSERT_TRUE(Card_less(ace_clubs, ace_diamonds, Suit::HEARTS));
    ASSERT_TRUE(Card_less(ace_clubs, ace_diamonds, Suit::DIAMONDS));
    ASSERT_TRUE(Card_less(ace_clubs, ace_diamonds, Suit::SPADES));
    ASSERT_FALSE(Card_less(ace_clubs, ace_diamonds, Suit::CLUBS));

    const Card jack_diamonds(Rank::JACK, Suit::DIAMONDS);
    ASSERT_TRUE(jack_diamonds < ace_diamonds);
    ASSERT_TRUE(Card_less(jack_diamonds, ace_diamonds, Suit::CLUBS));
    ASSERT_TRUE(Card_less(jack_diamonds, ace_diamonds, Suit::SPADES));
    ASSERT_FALSE(Card_less(jack_diamonds, ace_diamonds, Suit::HEARTS));
    ASSERT_FALSE(Card_less(jack_diamonds, ace_diamonds, Suit::DIAMONDS));
    ASSERT_FALSE(Card_less(ace_diamonds, jack_diamonds, Suit::CLUBS));
    ASSERT_FALSE(Card_less(ace_diamonds, jack_diamonds, Suit::SPADES));
    ASSERT_TRUE(Card_less(ace_diamonds, jack_diamonds, Suit::HEARTS));
    ASSERT_TRUE(Card_less(ace_diamonds, jack_diamonds, Suit::DIAMONDS));

    const Card jack_hearts(Rank::JACK, Suit::HEARTS);
    const Card nine_diamonds(Rank::NINE, Suit::DIAMONDS);

    ASSERT_TRUE(Card_less(ace_diamonds, jack_hearts, Suit::DIAMONDS));
    ASSERT_FALSE(Card_less(jack_hearts, ace_diamonds, Suit::DIAMONDS));
    ASSERT_TRUE(Card_less(nine_diamonds, jack_diamonds, Suit::DIAMONDS));
    ASSERT_FALSE(Card_less(jack_diamonds, nine_diamonds, Suit::DIAMONDS));
    ASSERT_TRUE(Card_less(nine_diamonds, jack_hearts, Suit::DIAMONDS));
    ASSERT_FALSE(Card_less(jack_hearts, nine_diamonds, Suit::DIAMONDS));

    ASSERT_TRUE(Card_less(nine_diamonds, ace_clubs, Suit::HEARTS));
    ASSERT_TRUE(Card_less(nine_diamonds, ace_clubs, Suit::CLUBS));
    ASSERT_TRUE(Card_less(nine_diamonds, ace_clubs, Suit::SPADES));
    ASSERT_FALSE(Card_less(nine_diamonds, ace_clubs, Suit::DIAMONDS));
}

TEST(test_card_less_led)
{
    const Card queen_clubs(Rank::QUEEN, Suit::CLUBS);
    const Card ten_clubs(Rank::TEN, Suit::CLUBS);
    const Card king_clubs(Rank::KING, Suit::CLUBS);
    ASSERT_TRUE(Card_less(ten_clubs, king_clubs, queen_clubs, Suit::DIAMONDS));
    ASSERT_FALSE(Card_less(king_clubs, ten_clubs, queen_clubs, Suit::DIAMONDS));
    ASSERT_TRUE(Card_less(ten_clubs, king_clubs, queen_clubs, Suit::HEARTS));
    ASSERT_FALSE(Card_less(king_clubs, ten_clubs, queen_clubs, Suit::HEARTS));
    ASSERT_TRUE(Card_less(ten_clubs, king_clubs, queen_clubs, Suit::SPADES));
    ASSERT_FALSE(Card_less(king_clubs, ten_clubs, queen_clubs, Suit::SPADES));
    ASSERT_TRUE(Card_less(ten_clubs, king_clubs, queen_clubs, Suit::CLUBS));
    ASSERT_FALSE(Card_less(king_clubs, ten_clubs, queen_clubs, Suit::CLUBS));

    const Card ten_spades(Rank::TEN, Suit::SPADES);
    ASSERT_TRUE(Card_less(ten_spades, king_clubs, queen_clubs, Suit::DIAMONDS));
    ASSERT_FALSE(Card_less(king_clubs, ten_spades, queen_clubs, Suit::DIAMONDS));
    ASSERT_TRUE(Card_less(ten_spades, king_clubs, queen_clubs, Suit::HEARTS));
    ASSERT_FALSE(Card_less(king_clubs, ten_spades, queen_clubs, Suit::HEARTS));
    ASSERT_TRUE(Card_less(ten_spades, king_clubs, queen_clubs, Suit::CLUBS));
    ASSERT_FALSE(Card_less(king_clubs, ten_spades, queen_clubs, Suit::CLUBS));
    ASSERT_FALSE(Card_less(ten_spades, king_clubs, queen_clubs, Suit::SPADES));
    ASSERT_TRUE(Card_less(king_clubs, ten_spades, queen_clubs, Suit::SPADES));

    const Card king_spades(Rank::KING, Suit::SPADES);
    ASSERT_TRUE(Card_less(ten_spades, king_spades, queen_clubs, Suit::DIAMONDS));
    ASSERT_FALSE(Card_less(king_spades, ten_spades, queen_clubs, Suit::DIAMONDS));
    ASSERT_TRUE(Card_less(ten_spades, king_spades, queen_clubs, Suit::HEARTS));
    ASSERT_FALSE(Card_less(king_spades, ten_spades, queen_clubs, Suit::HEARTS));
    ASSERT_TRUE(Card_less(ten_spades, king_spades, queen_clubs, Suit::CLUBS));
    ASSERT_FALSE(Card_less(king_spades, ten_spades, queen_clubs, Suit::CLUBS));
    ASSERT_TRUE(Card_less(ten_spades, king_spades, queen_clubs, Suit::SPADES));
    ASSERT_FALSE(Card_less(king_spades, ten_spades, queen_clubs, Suit::SPADES));

}


TEST_MAIN()
