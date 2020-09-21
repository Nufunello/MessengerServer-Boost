#pragma once

#include "Factories/AMappedHandlerFactory.hpp"
#include "Factories/ActiveUsersFactory.hpp"

#include "Handlers/ChatHandler.hpp"

namespace Factories
{
    class ChatFactory
        : public AMappedHandlerFactory
    {
    public:
        ChatFactory(Users::Data::UsersData& authorizedUsers)
            : AMappedHandlerFactory{"activeUsers", std::make_unique<ActiveUsersFactory>(authorizedUsers)}
        {
            AHandlerFactory::setHandler(&_chatHandler);
        }

        ~ChatFactory() = default;

    private:
        Handlers::ChatHandler _chatHandler;

    };
};
