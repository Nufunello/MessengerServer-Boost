#pragma once

#include "Handlers/AStrictedHandler.hpp"
#include "Handlers/WebPageLoader.hpp"

#include "HTTP/Responses/UnauthorizedResponse.hpp"
#include "HTTP/Responses/WebSocketResponse.hpp"

namespace Handlers
{
    class ChatHandler
        : public AStrictedHandler
        , private WebPageLoader
    {
    public:
        ChatHandler(Users::Data::UsersData& usersData)
            : WebPageLoader{"/webpages/chat"}
            , _usersData{usersData}
        {}

        ~ChatHandler() = default;

    protected:
        HTTP::Responses::IResponse::Ptr doGet(HTTP::Requests::Request&& request, URI::Segment target) override
        {
            return WebPageLoader::getResponseWithPage(std::move(request), target);
        }

        HTTP::Responses::IResponse::Ptr doPost(HTTP::Requests::Request&& request, URI::Segment target) override
        {
            boost::beast::websocket::stream<boost::beast::tcp_stream> websocket{std::move(request)};
            //websocket.async_read()
            return std::make_unique<HTTP::Responses::WebSocketResponse>(websocket);
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
        Users::Data::UsersData& _usersData;
        
    };
    
};
