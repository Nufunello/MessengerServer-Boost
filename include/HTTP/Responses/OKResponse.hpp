#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

#include <boost/beast/http/empty_body.hpp>

namespace HTTP
{
    namespace Responses
    {
        class OKResponse
            : public HTTPResponse
        {
        protected:
            inline static boost::beast::http::response<boost::beast::http::string_body> makeResponse(std::string body)
            {
                auto response = HTTPResponse::makeResponse(boost::beast::http::status::ok, "OK");
                response.body() = std::move(body);
                return response;
            }

        public:
            OKResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), boost::beast::http::status::ok, "OK"}
            {}

            OKResponse(boost::asio::ip::tcp::socket&& socket, std::string body)
                : HTTPResponse{std::move(socket), this->makeResponse(std::move(body))}
            {}

            ~OKResponse() = default;
            
        };
    };
};
