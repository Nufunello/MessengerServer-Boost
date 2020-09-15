#pragma once

#include <boost/beast/http/verb.hpp>
#include <boost/beast/http/read.hpp>

namespace HTTP
{
    namespace Requests
    {
        using Response = std::string;
        using Socket = int;

        using Method = boost::beast::http::verb;

        class Request
            : public boost::asio::ip::tcp::socket
        {
        public:
            Request() = delete;

            Request(boost::asio::ip::tcp::socket&& socket)
                : boost::asio::ip::tcp::socket{std::move(socket)}
            {}

            ~Request() = default;

        public:
            inline Method getMethod()
            {
                return Method::post;
            }
            
        };
        
    };
};