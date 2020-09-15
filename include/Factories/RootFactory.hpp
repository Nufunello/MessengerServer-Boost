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
        RootFactory(AuthorizhedUsers& authorizedUsers)
            : AMappedHandlerFactory{
                    "login", std::make_unique<LoginFactory>(authorizedUsers),
                    "chat", std::make_unique<ChatFactory>(authorizedUsers)
            }
        {
            AHandlerFactory::setHandler(&_rootHandler);
        }

        ~RootFactory() = default;

    private:
        Handlers::RootHandler _rootHandler;
    
    };
};
