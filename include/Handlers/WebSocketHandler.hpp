#pragma once

#include "Handlers/AAuthorizedHandler.hpp"

#include "HTTP/Responses/ResourceNotFoundResponse.hpp"
#include "HTTP/Responses/BadRequestResponse.hpp"
#include "HTTP/Responses/WebSocketResponse.hpp"

#include "HTTP/Sockets/IdentifiyingWebSocketMessage.hpp"

#include "Subscribes/ActiveUsersSubscribe.hpp"

#include "Users/Data/UsersData.hpp"

namespace Handlers
{
    class WebSocketHandler
        : public AAuthorizedHandler
    {
    public:
        WebSocketHandler(Users::Data::UsersData& usersData, Subscribes::ActiveUsersSubscribe& activeUsersSubscribes)
            : _usersData{usersData}
            , _activeUsersSubscribes{activeUsersSubscribes}
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
            auto [username, prevState, websocketToken, websocket] = _usersData.setSocket(reqToken.value(), std::move(request));

            HTTP::Sockets::IdentifiyingWebSocketMessage message{Users::Data::TokenToStr(websocketToken), username};
            websocket->write(message);
            
            if (prevState == HTTP::Sockets::WebSocket::State::FirstConnection)
            {
                _activeUsersSubscribes.addSubscribe(websocketToken, username, websocket);
            }
            else
            {
                _activeUsersSubscribes.sendStatus(websocket);
            }

            return std::make_unique<HTTP::Responses::WebSocketResponse>();
        }

    private:
        Users::Data::UsersData& _usersData;
        Subscribes::ActiveUsersSubscribe& _activeUsersSubscribes;

    };
}
