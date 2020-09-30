#pragma once

#include "Factories/AMappedHandlerFactory.hpp"
#include "Factories/WebSocketHandlerFactory.hpp"

#include "Handlers/ChatHandler.hpp"

namespace Factories
{
    class ChatFactory
        : public AHandlerFactory
    {
    public:
        ChatFactory()
            : _chatHandler{}
        {
            AHandlerFactory::setHandler(&_chatHandler);
        }

        ~ChatFactory() = default;

    private:
        Handlers::ChatHandler _chatHandler;

    };
}
