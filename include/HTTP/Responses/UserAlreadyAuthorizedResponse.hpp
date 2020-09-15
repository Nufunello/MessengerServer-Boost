#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

#include <boost/beast/http/empty_body.hpp>

namespace HTTP
{
    namespace Responses
    {
        class UserAlreadyAuthorizedResponse
            : public HTTPResponse
        {
        public:
            UserAlreadyAuthorizedResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), boost::beast::http::status::bad_request, "User is alredy authorized"}
            {}

            ~UserAlreadyAuthorizedResponse() = default;
            
        };
    };
};
