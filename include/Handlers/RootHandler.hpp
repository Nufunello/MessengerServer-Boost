#pragma once

#include "Handlers/AHandler.hpp"
#include "Handlers/WebPageLoader.hpp"

#include "HTTP/Responses/UserAlreadyAuthorizedResponse.hpp"

using AuthorizhedUsers = int;

namespace Handlers
{
    class RootHandler
        : public AHandler
        , private WebPageLoader
    {
    public:
        RootHandler()
            : WebPageLoader{"/webpages/"}
        {}

        ~RootHandler() = default;

    protected:
        HTTP::Responses::IResponse::Ptr doGet(HTTP::Requests::Request&& request, URI::Segment target) override
        {
            if (target.isEmpty())
            {
                return std::make_unique<HTTP::Responses::RedirectResponse>(std::move(request), "/login", "URL: \"/\" is not allowed");
            }
            else
            {
                return WebPageLoader::getResponseWithPage(std::move(request), target);
            }
        }
    };
}
