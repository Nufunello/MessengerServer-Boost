#pragma once

#include "Factories/AHandlerFactory.hpp"
#include "Handlers/ChatHandler.hpp"

namespace Factories
{
    class ChatFactory
        : public AHandlerFactory
    {
    public:
        ChatFactory(Users::Data::UsersData& authorizedUsers)
            : _chatHandler{authorizedUsers}
        {
            AHandlerFactory::setHandler(&_chatHandler);
        }

        ~ChatFactory() = default;

    private:
        Handlers::ChatHandler _chatHandler;
    
    };
};
