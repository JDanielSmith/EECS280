#include <stdint.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <tuple>
#include <map>
#include <cmath>
#include <algorithm>

// https://en.cppreference.com/w/cpp/string/basic_string/replace
static std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with)
{
    std::size_t count{};
    for (std::string::size_type pos{};
        inout.npos != (pos = inout.find(what.data(), pos, what.length()));
        pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}


/***********************************************************************/

struct PlanetName final
{
    int64_t ID;
    std::string Name;
};
std::istream& operator>>(std::istream& is, PlanetName& result)
{
    is >> result.ID;
    is >> result.Name;

    std::ignore = replace_all(result.Name, "_", " ");
    std::ignore = replace_all(result.Name, "XX", "");

    return is;
}

/***********************************************************************/

struct Location final
{
    int64_t Row = -1;
    int64_t Column = -1;
};
std::ostream& operator<<(std::ostream& os, const Location& location)
{
    os << location.Row << " " << location.Column;
    return os;
}

struct PlanetLocation final
{
    Location Location;
    char Symbol;
    int64_t ID;
};
std::istream& operator>>(std::istream& is, PlanetLocation& result)
{
    is >> result.Location.Row;
    is >> result.Location.Column;
    is >> result.Symbol;
    is >> result.ID;
    return is;
}
bool operator<(const PlanetLocation& lhs, const PlanetLocation& rhs)
{
    return lhs.ID < rhs.ID;
}
bool operator==(const PlanetLocation& lhs, const PlanetLocation& rhs)
{
    return lhs.ID == rhs.ID;
}

struct Locations final
{
    int64_t GridRows, GridColumns;
    Location Starting;
    Location Ending;
    std::vector<PlanetLocation>  Planets;
};
std::istream& operator>>(std::istream& is, Locations& result)
{
    is >> result.GridRows; is >> result.GridColumns;
    is >> result.Starting.Row; is >> result.Starting.Column;
    is >> result.Ending.Row; is >> result.Ending.Column;

    // https://en.cppreference.com/w/cpp/io/basic_ios/eof
    for (PlanetLocation pl; is >> pl; )
    {
        result.Planets.push_back(pl);
    }

    return is;
}

/***********************************************************************/

auto filter_planets(const Locations& l, std::ostream& os)
{
    std::vector<PlanetLocation> retval;

    for (auto& planet : l.Planets)
    {
        auto& location = planet.Location;
        if (((location.Row > 0) && (location.Column > 0))
            && ((location.Row <= l.GridRows) && (location.Column <= l.GridColumns)))
        {
            retval.push_back(planet);
        }
        else
        {
            os << planet.ID << " out of range - ignoring\n";
        }
    }

    return retval;
}

PlanetLocation find_nearest(const Location& current, const std::vector<PlanetLocation>& planets)
{
    std::vector<PlanetLocation> nearest;

    auto current_distance = std::numeric_limits<double>::max();
    for (auto& planet : planets)
    {
        const auto x = current.Row - planet.Location.Row;
        const auto y = current.Column - planet.Location.Column;
        
        // https://en.cppreference.com/w/cpp/numeric/math/hypot
        const auto distance = std::hypot(x, y);
        if (distance < current_distance)
        {
            current_distance = distance;
            nearest.clear();
            nearest.push_back(planet);
        }
        else if (!(distance > current_distance)) // i.e., ==
        {
            nearest.push_back(planet);
        }
    }

    std::sort(nearest.begin(), nearest.end());
    return nearest.front();
}

auto plan_vist(Location starting, std::vector<PlanetLocation> planets)
{
    Location current = starting;
    std::vector<PlanetLocation> retval;
    while (!planets.empty())
    {
        auto nearest = find_nearest(current, planets);
        current = nearest.Location;
        retval.push_back(nearest);

        const auto it = std::remove(planets.begin(), planets.end(), nearest);
        planets.erase(it);
    }

    return retval;
}

void set_char(std::vector<std::vector<char>>& grid, const Location& location, char ch)
{
    grid[location.Row - 1][location.Column - 1] = ch;
}
void print_map(std::ostream& os, const Locations& locations)
{
    std::vector<std::vector<char>> grid(locations.GridRows, std::vector<char>(locations.GridColumns, '.'));

    set_char(grid, locations.Starting, 's');
    for (auto& planet : locations.Planets)
    {
        set_char(grid, planet.Location, planet.Symbol);
    }
    set_char(grid, locations.Ending, 'e');

    for (const auto& row : grid)
    {
        for (const auto& col : row)
        {
            os << col;
        }
        os << '\n';
    }
}

void print_route(std::ostream& os, const std::map<int64_t, std::string>& names, const Locations& locations)
{
    os << "Start at " << locations.Starting << '\n';
    for (auto& planet : locations.Planets)
    {
        os << "Go to " << names.at(planet.ID) << " at " << planet.Location << '\n';
    }
    os << "End at " << locations.Ending << '\n';
}

int main_()
{
    //std::cout << "Enter Locations Filename: ";
    std::filesystem::path fn = "locations_2.txt";
    //std::cin >> fn;

    // https://en.cppreference.com/w/cpp/io/basic_ifstream
    std::ifstream ifs_locations(fn);
    if (!ifs_locations.is_open())
    {
        std::cerr << "Input file could not be opened\n";
        return EXIT_FAILURE;
    }
    Locations locations;
    ifs_locations >> locations;

    //std::cout << "Enter Names  Filename: ";
    fn = "names_2.txt";
    //std::cin >> fn;

    // https://en.cppreference.com/w/cpp/io/basic_ifstream
    std::ifstream ifs_names(fn);
    if (!ifs_names.is_open())
    {
        std::cerr << "Input file could not be opened\n";
        return EXIT_FAILURE;
    }
    std::map<int64_t, std::string> names;
    // https://en.cppreference.com/w/cpp/io/basic_ios/eof
    for (PlanetName pn; ifs_names >> pn; )
    {
        names[pn.ID] = pn.Name;
    }

    /********************************************************/

    auto planets = filter_planets(locations, std::clog);
    locations.Planets = plan_vist(locations.Starting, planets);
    print_map(std::cout, locations);
    print_route(std::cout, names, locations);

    return EXIT_SUCCESS;
}
int main()
{
    try
    {
        return main_();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown exception." << std::endl;
    }
    return EXIT_FAILURE;
}