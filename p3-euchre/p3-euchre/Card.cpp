// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"

#include <cassert>
#include <iostream>
#include <array>
#include <stdexcept>

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
  throw std::invalid_argument("'str': didn't match any rank");
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  std::string str;
  is >> str;
  rank = string_to_rank(str);
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
  throw std::invalid_argument("'str': didn't match any suit");
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  std::string str;
  is >> str;
  suit = string_to_suit(str);
  return is;
}


/////////////// Write your implementation for Card below ///////////////


// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec before implementing
// the following operator overload functions:
//   operator<<
//   operator>>
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=

Card::Card(Rank rank_in, Suit suit_in)
    : rank(rank_in), suit(suit_in)
{
}

// "Initializes Card to the Two of Spades"
Card::Card() : Card(Rank::TWO, Suit::SPADES) {
}

// "returns the next suit, which is the suit of the same color"
Suit Suit_next(Suit suit) {
    switch (suit)
    {
    case Suit::HEARTS: return Suit::DIAMONDS;
    case Suit::DIAMONDS: return Suit::HEARTS;
    case Suit::SPADES: return Suit::CLUBS;
    case Suit::CLUBS: return Suit::SPADES;
    default: throw std::logic_error("Suit_next()");
    }
}

static bool Card_less_bower(const Card& a, const Card& b, Suit trump)
{
    // bowers only for trump
    assert(a.is_trump(trump));
    assert(b.is_trump(trump));

    if (a.is_right_bower(trump))
    {
        return false; // right > anything
    }
    if (b.is_right_bower(trump))
    {
        assert(!a.is_right_bower(trump));
        return true; // anything < right
    }
    if (a.is_left_bower(trump))
    {
        assert(!b.is_right_bower(trump));
        return false; // left > anything except right
    }
    if (b.is_left_bower(trump))
    {
        assert(!a.is_left_bower(trump));
        return true; // anything < right
    }

    // no more bowers
    assert(!a.is_right_bower(trump));
    assert(!b.is_right_bower(trump));
    assert(!a.is_left_bower(trump));
    assert(!b.is_left_bower(trump));
    return a < b;
}
bool Card_less(const Card& a, const Card& b, Suit trump) {
    if (a.is_trump(trump) && !b.is_trump(trump))
    {
        return false; // trump > !trump
    }
    if (!a.is_trump(trump) && b.is_trump(trump))
    {
        return true; // !trump < trump
    }
    if (!a.is_trump(trump) && !b.is_trump(trump))
    {
        return a < b; // no trump -> no bowers
    }

    return Card_less_bower(a, b, trump);
}

static bool Card_less(const Card& a, const Card& b, Suit led, Suit trump)
{
    const auto a_followed_suit = a.get_suit(trump) == led;
    const auto b_followed_suit = b.get_suit(trump) == led;
    if (a_followed_suit && !b_followed_suit)
    {
        return false; // a > b
    }
    if (!a_followed_suit && b_followed_suit)
    {
        return true; // a < b
    }

    assert( (a_followed_suit && b_followed_suit) || (!a_followed_suit && !b_followed_suit) );
    return a < b;
}
bool Card_less(const Card& a, const Card& b, const Card& led_card, Suit trump)
{
    // If there's a trump card, what was led doesn't matter (assume no renege)
    if (a.is_trump(trump) || b.is_trump(trump))
    {
        return Card_less(a, b, trump);
    }

    const auto led_suit = led_card.get_suit(trump);
    return Card_less(a, b, led_suit, trump);
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    // "Prints Card to stream, for example 'Two of Spades'"
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}
std::istream& operator>>(std::istream& is, Card& card)
{
    is >> card.rank;
    std::string of;
    is >> of;
    if (of != "of")
    {
        throw std::runtime_error("format is wrong");
    }
    is >> card.suit;
    return is;
}

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
bool operator==(const Card& lhs, const Card& rhs)
{
    if ((lhs < rhs) || (rhs < lhs))
    {
        return false; // can't be equal
    }
    return true; // must be equal
}
bool operator!=(const Card& lhs, const Card& rhs)
{
    return !(lhs == rhs);
}

Rank Card::get_rank() const
{
    return rank;
}

Suit Card::get_suit() const
{
    return suit;
}
Suit Card::get_suit(Suit trump) const
{
    const auto suit_ = get_suit();
    return is_left_bower(trump) ? Suit_next(suit_) : suit_;
}

//EFFECTS Returns true if card is a face card (Jack, Queen, King or Ace)
bool Card::is_face_or_ace() const
{
    switch (get_rank())
    {
    case Rank::JACK:
    case Rank::QUEEN:
    case Rank::KING:
    case Rank::ACE:
        return true;

    case Rank::TWO:
    case Rank::THREE:
    case Rank::FOUR:
    case Rank::FIVE:
    case Rank::SIX:
    case Rank::SEVEN:
    case Rank::EIGHT:
    case Rank::NINE:
    case Rank::TEN:
        return false;

    default:
        throw std::logic_error("is_face_or_ace()");
    }
}

static inline bool is_bower(Rank rank, Suit suit, Suit trump)
{
    return (rank == Rank::JACK) && (suit == trump);
}
bool Card::is_right_bower(Suit trump) const
{
    return is_bower(get_rank(), get_suit(), trump);
}
bool Card::is_left_bower(Suit trump) const
{
    const auto next_suit = Suit_next(get_suit());
    return is_bower(get_rank(), next_suit, trump);
}

bool Card::is_trump(Suit trump) const
{
    return (get_suit() == trump) || is_left_bower(trump);
}