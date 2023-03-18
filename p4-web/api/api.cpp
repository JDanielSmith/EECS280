// api.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma warning(push)
#pragma warning(disable: 4996) // '...': warning STL4036: <ciso646> is removed in C++20. ...
#include <stdlib.h>

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include <tuple>
#include <optional>
#include <sstream>
#include <algorithm>

#include "json.hpp"
#pragma warning(pop)

template<typename T>
using list = std::list<T>;

struct Student final
{
    std::string uniqname;
    std::string location;
};
list<Student> queue;

enum class HttpMethod
{
    GET, POST, DELETE
};
static HttpMethod to_HttpMethod(const std::string& method)
{
    if (method == "GET")
    {
        return HttpMethod::GET;
    }
    if (method == "POST")
    {
        return HttpMethod::POST;
    }
    if (method == "DELETE")
    {
        return HttpMethod::DELETE;
    }

    throw std::invalid_argument("Unknown 'method': " + method);
}

struct Request final
{
    HttpMethod method;
    std::string path;
    //std::vector<std::string> headers;
    nlohmann::json body;

    Request(HttpMethod m, const std::string& p) : method(m), path(p) {}
    Request(const Request&) = default;
    Request& operator=(const Request&) = default;
    Request(Request&&) = default;
    Request& operator=(Request&&) = default;
};

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
    case HttpStatus::NoContent: return "No Content";
    case HttpStatus::BadRequest: return "Bad Request";
    default:
        throw std::logic_error("Unknown HttpStatus");
    }
}

struct Response final
{
    HttpStatus status;
    //std::vector<std::string> headers;
    nlohmann::json body;

    Response(HttpStatus s) : status(s) {}
    Response(HttpStatus s, const nlohmann::json& b) : status(s), body(b) {}
    Response(const Response&) = default;
    Response& operator=(const Response&) = default;
    Response(Response&&) = default;
    Response& operator=(Response&&) = default;
};

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

    path = line.substr(space + 1);
    space = path.find(' ');
    if (space == std::string::npos)
    {
        return false;
    }
    const auto http = path.substr(space + 1);
    path = path.substr(0, space);

    return !(path.empty() || http.empty());
}

static std::optional<Request> getRequest(std::istream& cin)
{
    if (!cin)
    {
        return std::nullopt;
    }

    std::string line;
    std::getline(cin, line);
    std::string method, path;
    if (!get_method_and_path(line, method, path))
    {
        return std::nullopt;
    }

    Request retval(to_HttpMethod(method), path);
   
    // "... your implementation can assume that these things are correct: ..."
    std::string header;
    std::getline(cin, header);
    //retval.headers.push_back(header); // Host:
    std::getline(cin, header);
    //retval.headers.push_back(header); // Content-Type:
    std::getline(cin, header);
    //retval.headers.push_back(header); // Content-Length:
    const auto space = header.find(' ');
    if (space == std::string::npos)
    {
        return std::nullopt;
    }
    const auto length = header.substr(space + 1);
    const auto  content_length = std::stoi(length);

    std::string blank;
    std::getline(cin, blank);
    if (!blank.empty())
    {
        return std::nullopt;
    }

    if (content_length > 0)
    {
        cin >> retval.body;
        std::getline(cin, blank);
        if (!blank.empty())
        {
            return std::nullopt;
        }
    }

    return retval;
}

static void writeResponse(std::ostream& cout, const Response& response)
{
    const auto status = response.status;
    cout << "HTTP/1.1 " << static_cast<int>(status) << " " << to_string(status) << "\n";
    cout << "Content-Type: application/json; charset=utf-8\n";

    std::string content;
    if (!response.body.is_null())
    {
        content = response.body.dump(4) + "\n";
    }
    cout << "Content-Length: " << content.length() << "\n";
    cout << "\n";
    cout << content;
}

Response createResponse_BadRequest()
{
    // "Then, return the following response after reading the entire request. ..."
    Response retval(HttpStatus::BadRequest);
    return retval;
}

static auto to_json(const Student& s, size_t position)
{
    nlohmann::json retval;
    retval["location"] = s.location;
    retval["position"] = position;
    retval["uniqname"] = s.uniqname;
    return retval;
}

Response createResponse_GET(const Request& request)
{
    if (request.path == "/api/")
    {
        static const nlohmann::json content = {
          {"queue_head_url","http://localhost/queue/head/"},
          {"queue_list_url", "http://localhost/queue/"},
          {"queue_tail_url", "http://localhost/queue/tail/"} };
        return Response(HttpStatus::OK, content);
    }

    if (request.path == "/api/queue/head/")
    {
        // "If the queue is empty, return a 400 error."
        if (queue.empty())
        {
            return createResponse_BadRequest();
        }

        const auto& student = queue.front();
        const auto response = to_json(student, 1);
        return Response(HttpStatus::OK, response);
    }

    if (request.path == "/api/queue/")
    {
        nlohmann::json response{ {"count", queue.size()}, {"results",{}} };
        int position = 1;
        for (const auto& item : queue)
        {
            auto content = to_json(item, position++);
            response["results"].push_back(std::move(content));
        }

        return Response(HttpStatus::OK, response);
    }

    return createResponse_BadRequest();
}

Response createResponse_POST(const Request& request)
{
    if (request.path != "/api/queue/tail/")
    {
        return createResponse_BadRequest();
    }

    const auto& body = request.body;
    const Student s{ body["uniqname"],  body["location"] };
    queue.push_back(s);

    const auto response = to_json(s, queue.size());
    return Response(HttpStatus::Created, response);
}

Response createResponse_DELETE(const Request& request)
{
    if (request.path != "/api/queue/head/")
    {
        return createResponse_BadRequest();
    }

    // "If the queue is empty, return a 400 error."
    if (queue.empty())
    {
        return createResponse_BadRequest();
    }

    queue.pop_front();
    return Response(HttpStatus::NoContent);
}

Response createResponse(const Request& request)
{
    switch (request.method)
    {
    case HttpMethod::GET: return createResponse_GET(request);
    case HttpMethod::POST: return createResponse_POST(request);
    case HttpMethod::DELETE: return createResponse_DELETE(request);
    default:
        throw std::logic_error("Unknown HttpMethod");
    }
}

static void copy_stream(std::istream& input, std::ostream& output)
{
    std::istreambuf_iterator<std::istream::char_type> begin(input), end;
    std::ostreambuf_iterator<std::ostream::char_type> out(output);
    std::copy(begin, end, out);
}

int main()
{
    std::stringstream input;
    copy_stream(std::cin, input);

    std::stringstream output;
    while (const auto request = getRequest(input))
    {
        const auto response = createResponse(*request);
        writeResponse(output, response);
    }

    output.seekp(0, std::ios_base::beg);
    copy_stream(output, std::cout);

    return EXIT_SUCCESS;
}

