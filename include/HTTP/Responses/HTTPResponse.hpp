#pragma once

#include "HTTP/Responses/IResponse.hpp"
#include "HTTP/Requests/Request.hpp"

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
            inline static Response makeResponse(const Status status, const boost::string_view reason)
            {
                Response response;
                response.result(status);
                response.reason(reason);
                return response;
            }

        public:
            inline HTTPResponse(boost::asio::ip::tcp::socket&& socket, Response response)
                : _socket{std::move(socket)}
                , _response{std::move(response)}
            {}

            inline HTTPResponse(boost::asio::ip::tcp::socket&& socket, const Status status, const boost::string_view reason)
                : _socket{std::move(socket)}
                , _response{this->makeResponse(status, reason)}
            {}

            virtual ~HTTPResponse() = default;

            void setCookie(HTTP::Cookie cookie)
            {
                _response.set(HTTP::Requests::Field::set_cookie, cookie.asString());
            }

            virtual void send() override
            {
                boost::beast::http::write(_socket, _response);
            }

            virtual boost::asio::ip::tcp::socket getSocket()
            {
                return std::move(_socket);
            }

        private:
            boost::asio::ip::tcp::socket _socket;
            Response _response;

        };
    }
}
