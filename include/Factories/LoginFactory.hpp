#pragma once

#include "Factories/AHandlerFactory.hpp"
#include "Handlers/LoginHandler.hpp"

namespace Factories
{
    class LoginFactory
        : public AHandlerFactory
    {
    public:
        LoginFactory(AuthorizhedUsers& authorizedUsers)
            : _loginHandler{authorizedUsers}
        {
            AHandlerFactory::setHandler(&_loginHandler);
        }

        ~LoginFactory() = default;

    private:
        Handlers::LoginHandler _loginHandler;
    
    };
};
