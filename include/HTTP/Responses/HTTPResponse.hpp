#pragma once

#include <iostream>

#include "HTTP/Responses/IResponse.hpp"

#include <boost/beast/http/write.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/string_body.hpp>

namespace HTTP
{
    namespace Responses
    {
        class HTTPResponse
            : public IResponse
        {
        public:
            using Ptr = std::unique_ptr<HTTPResponse>;

        protected:
            inline static boost::beast::http::response<boost::beast::http::string_body> makeResponse(const boost::beast::http::status status, const std::string& reason)
            {
                boost::beast::http::response<boost::beast::http::string_body> response;
                response.result(status);
                response.reason(reason);
                return response;
            }
            
        protected:
            inline HTTPResponse(boost::asio::ip::tcp::socket&& socket)
                : _socket{std::move(socket)}
            {}

        public:
            inline HTTPResponse(boost::asio::ip::tcp::socket&& socket, const boost::beast::http::status status, const std::string& reason)
                : _socket{std::move(socket)}
                , _response{this->makeResponse(status, reason)}
            {}

            inline HTTPResponse(boost::asio::ip::tcp::socket&& socket, boost::beast::http::response<boost::beast::http::string_body> response)
                : _socket{std::move(socket)}
                , _response{std::move(response)}
            {}

            virtual ~HTTPResponse() = default;

            virtual void send() override
            {
                std::cout << "Result: " << _response.result() << "\nReason: " << _response.reason() << "\nBody: " << _response.body() << std::endl; 
                boost::beast::http::write(_socket, _response);
	            _socket.close();
            }

        private:
            boost::asio::ip::tcp::socket _socket;
            const boost::beast::http::response<boost::beast::http::string_body> _response;

        };
    };
};
