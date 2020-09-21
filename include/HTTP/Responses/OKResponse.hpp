#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

namespace HTTP
{
    namespace Responses
    {
        class OKResponse
            : public HTTPResponse
        {
        public:
            inline static Response makeResponse(std::string body)
            {
                auto response = HTTPResponse::makeResponse(Status::ok, "OK");
                response.body() = std::move(body);
                return response;
            }

        public:
            OKResponse(boost::asio::ip::tcp::socket&& socket, std::string body = "")
                : HTTPResponse{std::move(socket), this->makeResponse(std::move(body))}
            {}

            ~OKResponse() = default;
            
        };
    };
};
