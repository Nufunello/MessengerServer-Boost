#pragma once

#include <iostream>

#include "HTTP/Responses/IResponse.hpp"

#include <boost/beast/http/write.hpp>

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
            inline static Response makeResponse(const Status status, const std::string& reason)
            {
                Response response;
                response.result(status);
                response.reason(reason);
                return response;
            }
            
        protected:
            inline HTTPResponse(boost::asio::ip::tcp::socket&& socket)
                : _socket{std::move(socket)}
            {}

        public:
            inline HTTPResponse(boost::asio::ip::tcp::socket&& socket, const Status status, const std::string& reason)
                : _socket{std::move(socket)}
                , _response{this->makeResponse(status, reason)}
            {}

            inline HTTPResponse(boost::asio::ip::tcp::socket&& socket, Response response)
                : _socket{std::move(socket)}
                , _response{std::move(response)}
            {}

            virtual ~HTTPResponse() = default;

            virtual void send() override
            {
                boost::beast::http::write(_socket, _response);
	            _socket.close();
            }

        private:
            boost::asio::ip::tcp::socket _socket;
            const Response _response;

        };
    };
};
