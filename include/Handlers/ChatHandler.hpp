#pragma once

#include "Handlers/AStrictedHandler.hpp"
#include "Handlers/WebPageLoader.hpp"

#include "HTTP/Responses/UserAlreadyAuthorizedResponse.hpp"
#include "HTTP/Responses/UnauthorizedResponse.hpp"

using AuthorizhedUsers = int;

namespace Handlers
{
    class ChatHandler
        : public AStrictedHandler
        , public WebPageLoader
    {
    public:
        ChatHandler(AuthorizhedUsers& authorizedUsers)
            : _authorizedUsers{authorizedUsers}
            , WebPageLoader{"/webpages/chat"}
        {}

        ~ChatHandler() = default;

    protected:
        HTTP::Responses::IResponse::Ptr doGet(HTTP::Requests::Request&& request, URI::Segment target) override
        {
            return WebPageLoader::getResponseWithPage(std::move(request), target);
        }

    protected:
        virtual HTTP::Responses::IResponse::Ptr findAppropiateResponse(HTTP::Requests::Request&& request, const HTTP::Requests::Method method) override
        {
            switch (method)
            {
            case HTTP::Requests::Method::post:
            case HTTP::Requests::Method::get:
                return std::make_unique<HTTP::Responses::UnauthorizedResponse>(std::move(request));
            
            default:
                return AStrictedHandler::findAppropiateResponse(std::move(request), method);
            }
        }

    private:
        AuthorizhedUsers& _authorizedUsers;
        
    };
    
};
