// Project UID 1d9f47bfc76643019cfbf037641defe1
#include "Pack.h"

#include <assert.h>

#include <stdexcept>
#include <vector>
#include <type_traits>

inline static void increment_rank(Rank& rank)
{
    using Rank_underlying_t = std::underlying_type_t<Rank>;
    auto rank_ = static_cast<Rank_underlying_t>(rank);
    rank_++;
    rank = static_cast<Rank>(rank_);
}
static void order_suit(std::vector<Card>& pack, Suit suit, int cards)
{
    if (cards > 13)
    {
        throw std::invalid_argument("There at most 13 cards in a suit.");
    }

    // The rank of the cards is from 2 to Ace; we want the highest ones
    Rank rank = Rank::TWO;
    for (auto low_cards_to_skip = 13 - cards; low_cards_to_skip > 0; low_cards_to_skip--)
    {
        increment_rank(rank);
    }
    for (int count = 0; count < cards; count++)
    {
        pack.emplace_back(rank, suit);
        increment_rank(rank);
    }
}

std::vector<Card> create_standard_order_pack(int size)
{
    if ((size % 4) != 0)
    {
        throw std::invalid_argument("'size' must be a multiple of 4.");
    }
    const auto cards_per_suit = size / 4;

    std::vector<Card> retval;

    // We know there are four suits numbered 0, 1, 2, 3
    for (int suit_ = 0; suit_ < 4; suit_++)
    {
        const auto suit = static_cast<Suit>(suit_);
        order_suit(retval, suit, cards_per_suit);
    }
    return retval;
}

Pack::Pack() : next(0)
{
    const auto pack = create_standard_order_pack(PACK_SIZE);
    size_t i = 0;
    for (const auto& card : pack)
    {
        cards[i++] = card;
    }
}

Pack::Pack(std::istream& pack_input) : next(0)
{
    size_t i = 0;
    while (pack_input)
    {
        if (i >= cards.size())
        {
            break; // blank line after last card
        }
        pack_input >> cards[i++];
    }
}

void Pack::reset() {
    next = 0;
}

Card Pack::deal_one()
{
    if (empty())
    {
        throw std::logic_error("deal_one(): pack is empty.");
    }

    auto retval = cards[static_cast<size_t>(next)];
    next++;
    return retval;
}

void Pack::shuffle()
{
    reset();
    assert(!empty());
    throw std::logic_error("shuffle()");
}

bool Pack::empty() const
{
    return next >= PACK_SIZE;
}