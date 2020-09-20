#pragma once

#include "HTTP/Responses/RedirectResponse.hpp"

namespace HTTP
{
    namespace Responses
    {
        class UserAlreadyAuthorizedResponse
            : public RedirectResponse
        {
        public:
            UserAlreadyAuthorizedResponse(boost::asio::ip::tcp::socket&& socket)
                : RedirectResponse{std::move(socket), "/chat", "User is alredy authorized"}
            {}

            ~UserAlreadyAuthorizedResponse() = default;
            
        };
    };
};
