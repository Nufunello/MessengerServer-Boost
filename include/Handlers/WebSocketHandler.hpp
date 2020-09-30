#pragma once

#include "Handlers/AAuthorizedHandler.hpp"

#include "HTTP/Responses/ResourceNotFoundResponse.hpp"
#include "HTTP/Responses/BadRequestResponse.hpp"
#include "HTTP/Responses/WebSocketResponse.hpp"

#include "HTTP/Sockets/IdentifiyingWebSocketMessage.hpp"

#include "Users/Data/UsersData.hpp"

namespace Handlers
{
    class WebSocketHandler
        : public AAuthorizedHandler
    {
    public:
        WebSocketHandler(Users::Data::UsersData& usersData)
            : _usersData{usersData}
        {
        }

        ~WebSocketHandler() = default;

    protected:
        HTTP::Responses::IResponse::Ptr doGet(HTTP::Requests::Request&& request, URI::Segment target) override
        {
            if (!target.isEmpty())
            {
                return std::make_unique<HTTP::Responses::ResourceNotFoundResponse>(std::move(request));
            }

            if (!request.isWebsocketRequest())
            {
                return std::make_unique<HTTP::Responses::BadRequestResponse>(std::move(request));
            }

            const auto reqToken = request.getToken();
            Users::Data::WebSocketUpdateInfo info = _usersData.setSocket(reqToken.value(), std::move(request));

            if (info.PrevState == HTTP::Sockets::WebSocket::State::FirstConnection)
            {
                HTTP::Sockets::IdentifiyingWebSocketMessage message{Users::Data::TokenToStr(info.WebSocketToken), info.UserName};
                info.WebSocket.write(message);
            }

            return std::make_unique<HTTP::Responses::WebSocketResponse>();
        }

    private:
        Users::Data::UsersData& _usersData;

    };
}
