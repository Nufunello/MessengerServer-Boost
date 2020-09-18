#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

namespace HTTP
{
    namespace Responses
    {
        class UserAlreadyAuthorizedResponse
            : public HTTPResponse
        {
        public:
            UserAlreadyAuthorizedResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), Status::bad_request, "User is alredy authorized"}
            {}

            ~UserAlreadyAuthorizedResponse() = default;
            
        };
    };
};
