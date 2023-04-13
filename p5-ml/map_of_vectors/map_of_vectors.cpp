#include <string>
#include <map>
#include <vector>
#include <utility>

struct my_data final
{
    int count = 0;
    int j = 1;
    std::map<std::string, std::vector<std::string>> content;
};

my_data read(const std::vector<std::pair<std::string, std::string>>& pairs)
{
    my_data retval;

    for (auto&& p : pairs)
    {
        const auto tag = p.first;
        const auto content = p.second;

        retval.content[tag].push_back(content);

        retval.count++;
    }

    return retval;
}


int main()
{
    const std::vector<std::pair<std::string, std::string>> data =
    {
        {"abc", "the quick red fox"},
        {"abc", "jumped over the lazy"},
        {"abc", "brown dogs"},
        {"xyz", "now is the time for"},
        {"xyz", "all good men and"},
        {"xyz", "women to come to"},
        {"xyz", "the aid of their country"},
    };

    const auto result = read(data);
}