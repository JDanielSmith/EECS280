// api.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include <tuple>

#include "json.hpp"

template<typename T>
using list = std::list<T>;

struct Student final
{
    std::string uniqname;
    std::string location;

};
list<Student> queue;

enum class HttpStatus
{
    OK = 200,
    Created = 201,
    NoContent = 204,
    BadRequest = 400,
};

static std::string to_string(HttpStatus status)
{
    switch (status)
    {
    case HttpStatus::OK: return "OK";
    case HttpStatus::Created: return "Created";
    case HttpStatus::NoContent: return "NoContent";
    case HttpStatus::BadRequest: return "BadRequest";
    default:
        throw std::logic_error("Unknown HttpStatus");
    }
}

static int Request(std::istream& cin)
{
    // "... your implementation can assume that these things are correct: ..."
    std::string host;
    std::getline(cin, host);
    std::string content_type;
    std::getline(cin, content_type);
    std::string content_length;
    std::getline(cin, content_length);

    std::string blank;
    std::getline(cin, blank);
    if (!blank.empty())
    {
        return -1;
    }

    const auto space = content_length.find(' ');
    if (space == std::string::npos)
    {
        return -1;
    }
    const auto length = content_length.substr(space + 1);
    return std::stoi(length);
}

static void Response(HttpStatus status, std::ostream& cout, const std::string& content)
{
    cout << "HTTP/1.1 " << static_cast<int>(status) << " " << to_string(status) << "\n";
    cout << "Content-Type: application/json; charset=utf-8\n";
    cout << "Content-Length: " << content.length() << "\n";
    cout << "\n";
    cout << content;
}

static int Get_api(std::istream& cin, std::ostream& cout)
{
    const auto content_length = Request(cin);
    if (content_length != 0)
    {
        return EXIT_FAILURE;
    }
    std::string blank;
    std::getline(cin, blank);
    if (!blank.empty())
    {
        return EXIT_FAILURE;
    }

    static const std::string api =
        "{\n"
        "    \"queue_head_url\": \"http://localhost/queue/head/\",\n"
        "    \"queue_list_url\": \"http://localhost/queue/\",\n"
        "    \"queue_tail_url\": \"http://localhost/queue/tail/\"\n"
        "}\n";
    Response(HttpStatus::OK, cout, api);
    return EXIT_SUCCESS;
}

int Get(const std::string& path, std::istream& cin, std::ostream& cout)
{
    if (path == "/api/")
    {
        return Get_api(cin, cout);
    }

    if (path == "/api/queue/head/")
    {
        return EXIT_SUCCESS;
    }
    if (path == "/api/queue/")
    {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int Post(const std::string& path, std::istream& cin, std::ostream& cout)
{
    if (path != "/api/queue/tail/")
    {
        return EXIT_FAILURE;
    }

    const auto content_length = Request(cin);
    if (content_length <= 0)
    {
        return EXIT_FAILURE;
    }
    nlohmann::json content;
    cin >> content;
    std::string blank;
    std::getline(cin, blank);
    if (!blank.empty())
    {
        return EXIT_FAILURE;
    }

    const Student s{ content["uniqname"],  content["location"] };
    queue.push_back(s);

    nlohmann::json response;
    response["location"] = s.location;
    response["position"] = queue.size();
    response["uniqname"] = s.uniqname;
    Response(HttpStatus::Created, cout, response.dump(4) + "\n");

    return EXIT_SUCCESS;
}

int Delete(const std::string& path, std::istream& cin)
{
    if (path != "/api/queue/head/")
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int BadRequest(std::istream& cin, std::ostream& cout)
{
    // "... read the remainder of the request, including any headers or body ..."
    auto content_length = Request(cin);
    while ((cin) && (content_length > 0))
    {
        char unused;
        cin >> unused;
        content_length--;
    }

    // "Then, return the following response after reading the entire request. ..."
    Response(HttpStatus::BadRequest, cout, "");

    return EXIT_FAILURE;
}

static bool get_method_and_path(const std::string& line, std::string& method, std::string& path)
{
    auto space = line.find(' ');
    if (space == std::string::npos)
    {
        return false;
    }
    method = line.substr(0, space);
    if (method.empty())
    {
        return false;
    }

    path = line.substr(space+1);
    space = path.find(' ');
    if (space == std::string::npos)
    {
        return false;
    }
    const auto http = path.substr(space+1);
    path = path.substr(0, space);   

    return !(path.empty() || http.empty());
}

static int process(const std::string& line, std::istream& cin, std::ostream& cout)
{
    std::string method, path;
    if (!get_method_and_path(line, method, path))
    {
        return EXIT_FAILURE;
    }

    // "The path must exactly match one of /api/, /api/queue/, /api/queue/head/, or /api/queue/tail/, including the slashes."
    static const std::set<std::string> valid_paths{ "/api/", "/api/queue/", "/api/queue/head/", "/api/queue/tail/" };
    const auto it = valid_paths.find(path);
    if (it == valid_paths.end())
    {
        return EXIT_FAILURE;
    }

    if (method == "GET")
    {
        return Get(path, cin, cout);
    }
    if (method == "POST")
    {
        return Post(path, cin, cout);
    }
    if (method == "DELETE")
    {
        return Delete(path, cin);
    }

    return EXIT_FAILURE;
}

int main()
{
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        const auto retval = process(line, std::cin, std::cout);
        if (retval != EXIT_SUCCESS)
        {
            return BadRequest(std::cin, std::cout);
        }
    }

    return EXIT_SUCCESS;
}

