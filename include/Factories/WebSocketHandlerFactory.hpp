#pragma once

#include "Factories/AHandlerFactory.hpp"

#include "Handlers/WebSocketHandler.hpp"

namespace Factories
{
    class WebSocketHandlerFactory
        : public AHandlerFactory
    {
    public:
        WebSocketHandlerFactory(Users::Data::UsersData& usersData, Subscribes::ActiveUsersSubscribe& activeUsersSubscribes)
            : _websocketHandler{usersData, activeUsersSubscribes}
        {
            AHandlerFactory::setHandler(&_websocketHandler);
        }

        virtual ~WebSocketHandlerFactory() = default;

    public:
        Handlers::AHandler* getHandler(URI::URIIterator& iterator) const override
        {
            return iterator.isLastSegment() ? AHandlerFactory::getHandler(iterator) : nullptr;
        }

    private:
        Handlers::WebSocketHandler _websocketHandler;
    
    };
}
