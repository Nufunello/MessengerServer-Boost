#pragma once

#include "HTTP/Responses/RedirectResponse.hpp"

namespace HTTP
{
    namespace Responses
    {
        class UnauthorizedResponse
            : public RedirectResponse
        {
        public:
            UnauthorizedResponse(boost::asio::ip::tcp::socket&& socket)
                : RedirectResponse{std::move(socket), "/login", "User is not authorized"}
            {}

            ~UnauthorizedResponse() = default;
            
        };
    }
}
