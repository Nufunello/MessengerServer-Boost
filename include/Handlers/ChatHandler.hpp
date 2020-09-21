#pragma once

#include "Handlers/AAuthorizedHandler.hpp"
#include "Handlers/WebPageLoader.hpp"

#include "HTTP/Responses/UnauthorizedResponse.hpp"

namespace Handlers
{
    class ChatHandler
        : public AAuthorizedHandler
        , private WebPageLoader
    {
    public:
        ChatHandler()
            : WebPageLoader{"/webpages/chat"}
        {}

        ~ChatHandler() = default;

    protected:
        HTTP::Responses::IResponse::Ptr doGet(HTTP::Requests::Request&& request, URI::Segment target) override
        {
            return WebPageLoader::getResponseWithPage(std::move(request), target);
        }

    };
};
