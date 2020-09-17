#pragma once

#include <boost/asio/streambuf.hpp>

#include <boost/beast/http/verb.hpp>
#include <boost/beast/http/read.hpp>

namespace HTTP
{
    namespace Requests
    {
        using Response = std::string;
        using Socket = int;

        using Method = boost::beast::http::verb;
	    using Message = boost::beast::http::message<true, boost::beast::http::string_body>;

        class Request
            : public boost::asio::ip::tcp::socket
        {
        public:
            Request() = delete;

            Request(boost::asio::ip::tcp::socket&& socket)
                : boost::asio::ip::tcp::socket{std::move(socket)}
            {
                boost::beast::http::read_header(socket, stream, parser, ec);
            }

            ~Request() = default;

        public:
            inline bool isValid() const
            {
                return !ec.failed();
            }

            inline Method getMethod()
            {
                return Method::get;
            }

            inline const Message& fields() const
            {
                return parser.get();
            }

        private:
	        boost::asio::streambuf stream;
	        boost::beast::error_code ec;
            boost::beast::http::request_parser<boost::beast::http::string_body> parser;
            
        };
        
    };
};