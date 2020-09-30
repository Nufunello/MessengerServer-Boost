#pragma once

#include "HTTP/Cookies.hpp"

#include <boost/asio/streambuf.hpp>

#include <boost/beast/http/verb.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/websocket.hpp>

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
                , _stream{1024}
                , _ec{}
                , _parser{}
            {
                boost::beast::http::read(*this, _stream, _parser, _ec);
                _cookie = Cookie{this->message()[Field::cookie]};
            }

            ~Request() = default;

        public:
            inline bool isValid() const
            {
                return !_ec.failed();
            }

            inline std::string error() const
            {
                return _ec.message();
            }

            inline const Message& message() const
            {
                return _parser.get();
            }

            inline bool isWebsocketRequest() const
            {
                return boost::beast::websocket::is_upgrade(this->message());
            }

            inline std::optional<boost::string_view> getToken() const
            {
                return _cookie.getAttribute("Token");
            }

        private:
	        boost::asio::streambuf _stream;
	        boost::beast::error_code _ec;
            boost::beast::http::request_parser<boost::beast::http::string_body> _parser;
            Cookie _cookie;
            
        };
    }
}
