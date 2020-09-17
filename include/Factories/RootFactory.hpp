#pragma once

#include "Factories/AMappedHandlerFactory.hpp"
#include "Factories/LoginFactory.hpp"
#include "Factories/ChatFactory.hpp"

#include "Handlers/RootHandler.hpp"

namespace Factories
{
    class RootFactory
        : public AMappedHandlerFactory
    {
    public:
        RootFactory(Users::Data::UsersData& usersData)
            : AMappedHandlerFactory{
                    "login", std::make_unique<LoginFactory>(usersData),
                    "chat", std::make_unique<ChatFactory>(usersData)
            }
        {
            AHandlerFactory::setHandler(&_rootHandler);
        }

        ~RootFactory() = default;

    private:
        Handlers::RootHandler _rootHandler;
    
    };
};
