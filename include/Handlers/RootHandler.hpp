#pragma once

#include "Handlers/AHandler.hpp"
#include "Handlers/WebPageLoader.hpp"

#include "HTTP/Responses/UserAlreadyAuthorizedResponse.hpp"

using AuthorizhedUsers = int;

namespace Handlers
{
    class RootHandler
        : public AHandler
        , public WebPageLoader
    {
    public:
        RootHandler()
            : WebPageLoader{""}
        {}

        ~RootHandler() = default;

    protected:
        HTTP::Responses::IResponse::Ptr doGet(HTTP::Requests::Request&& request, URI::Segment target) override
        {
            return WebPageLoader::getResponseWithPage(std::move(request), target);
        }
        
    };
    
};
