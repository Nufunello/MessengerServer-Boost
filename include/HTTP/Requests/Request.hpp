#pragma once

#include <boost/asio/streambuf.hpp>

#include <boost/beast/http/verb.hpp>
#include <boost/beast/http/read.hpp>

namespace HTTP
{
    namespace Requests
    {
        using Method = boost::beast::http::verb;
	    using Message = boost::beast::http::message<true, boost::beast::http::string_body>;
        using Field = boost::beast::http::field;

        class Request
            : public boost::asio::ip::tcp::socket
        {
        public:
            Request() = delete;

            Request(boost::asio::ip::tcp::socket&& socket)
                : boost::asio::ip::tcp::socket{std::move(socket)}
            {
                boost::beast::http::read_header(*this, stream, parser, ec);
            }

            ~Request() = default;

        public:
            inline bool isValid() const
            {
                return !ec.failed();
            }

            inline std::string error() const
            {
                return ec.message();
            }

            inline const Message& message() const
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