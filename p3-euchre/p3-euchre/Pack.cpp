// Project UID 1d9f47bfc76643019cfbf037641defe1
#include "Pack.h"

#include <assert.h>

#include <stdexcept>

Pack::Pack()
{

}

Pack::Pack(std::istream& /*pack_input*/)
{

}

void Pack::reset() {
    assert(false);
    assert(next); // DELETEME: avoid error "private field is not used"
}

Card Pack::deal_one()
{
    throw std::logic_error("deal_one()");
}

void Pack::shuffle()
{
    throw std::logic_error("shuffle()");
}

bool Pack::empty() const
{
    throw std::logic_error("shuffle()");
}