#pragma once

#include "Factories/AHandlerFactory.hpp"
#include "Handlers/LoginHandler.hpp"

namespace Factories
{
    class LoginFactory
        : public AHandlerFactory
    {
    public:
        LoginFactory(Users::Data::UsersData& usersData)
            : _loginHandler{usersData}
        {
            AHandlerFactory::setHandler(&_loginHandler);
        }

        ~LoginFactory() = default;

    private:
        Handlers::LoginHandler _loginHandler;
    
    };
};
