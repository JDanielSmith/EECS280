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


static std::optional<Request> getRequest()
{
    if (!std::cin)
    {
        return std::nullopt;
    }

    std::string line;
    std::getline(std::cin, line);
    std::string method, path;
    if (!get_method_and_path(line, method, path))
    {
        return std::nullopt;
    }

    Request retval(to_HttpMethod(method), path);
   
    // "... your implementation can assume that these things are correct: ..."
    std::string header;
    std::getline(std::cin, header);
    //retval.headers.push_back(header); // Host:
    std::getline(std::cin, header);
    //retval.headers.push_back(header); // Content-Type:
    std::getline(std::cin, header);
    //retval.headers.push_back(header); // Content-Length:
    const auto space = header.find(' ');
    if (space == std::string::npos)
    {
        return std::nullopt;
    }
    const auto length = header.substr(space + 1);
    const auto  content_length = std::stoi(length);

    std::string blank;
    std::getline(std::cin, blank);
    if (!blank.empty())
    {
        return std::nullopt;
    }

    if (content_length > 0)
    {
        std::cin >> retval.body;
    }

    return retval;
}

static int getRequest(std::istream& cin)
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

static void getResponse(HttpStatus status, std::ostream& cout, const std::string& content)
{
    cout << "HTTP/1.1 " << static_cast<int>(status) << " " << to_string(status) << "\n";
    cout << "Content-Type: application/json; charset=utf-8\n";
    cout << "Content-Length: " << content.length() << "\n";
    cout << "\n";
    cout << content;
}

static void writeResponse(const Response& response)
{
    const auto status = response.status;
    std::cout << "HTTP/1.1 " << static_cast<int>(status) << " " << to_string(status) << "\n";
    std::cout << "Content-Type: application/json; charset=utf-8\n";

    std::string content;
    if (!response.body.is_null())
    {
        content = response.body.dump(4) + "\n";
    }
    std::cout << "Content-Length: " << content.length() << "\n";
    std::cout << "\n";
    std::cout << content;
}

Response createResponse_BadRequest()
{
    // "Then, return the following response after reading the entire request. ..."
    Response retval(HttpStatus::BadRequest);
    return retval;
}

static int Get_api(std::istream& cin, std::ostream& cout)
{
    const auto content_length = getRequest(cin);
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
    getResponse(HttpStatus::OK, cout, api);
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
        throw std::logic_error("not implemented");
    }
    if (request.path == "/api/queue/")
    {
        throw std::logic_error("not implemented");
    }

    return createResponse_BadRequest();
}

int Post(const std::string& path, std::istream& cin, std::ostream& cout)
{
    if (path != "/api/queue/tail/")
    {
        return EXIT_FAILURE;
    }

    const auto content_length = getRequest(cin);
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
    getResponse(HttpStatus::Created, cout, response.dump(4) + "\n");

    return EXIT_SUCCESS;
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

    nlohmann::json response;
    response["location"] = s.location;
    response["position"] = queue.size();
    response["uniqname"] = s.uniqname;
    return Response(HttpStatus::Created, response);
}

int Delete(const std::string& path, std::istream& cin)
{
    if (path != "/api/queue/head/")
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

Response createResponse_DELETE(const Request& request)
{
    if (request.path != "/api/queue/head/")
    {
        return createResponse_BadRequest();
    }

    throw std::logic_error("not implemented");
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
    auto request = getRequest();
    while (request)
    {
        const auto response = createResponse(*request);
        writeResponse(response);

        request = getRequest();
    }

    return EXIT_SUCCESS;
}

