#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

#include "HTTP/Requests/Request.hpp"

namespace HTTP
{
    namespace Responses
    {
        class RedirectResponse
            : public HTTPResponse
        {
        protected:
            inline static Response makeResponse(const boost::string_view location, const boost::string_view message)
            {
                auto response = HTTPResponse::makeResponse(Status::see_other, message);
                response.insert(HTTP::Requests::Field::location, location);
                return response;
            }

        public:
            RedirectResponse(boost::asio::ip::tcp::socket&& socket, const boost::string_view location, const boost::string_view message)
                : HTTPResponse{std::move(socket), makeResponse(location, message)}
            {}

            ~RedirectResponse() = default;
            
        };
    }
}
