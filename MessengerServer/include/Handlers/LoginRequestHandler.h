#pragma once

#include "ARequestHandler.h"

#include "Exceptions/RedirectException.h"
#include "Exceptions/AlreadyAuthorizedException.h" 

#include "Handlers/ResourceFileMap.h"

namespace MyRequestHandlers
{
    class LoginRequestHandler :
        public ARequestHandler,
        public ResourceFileMap
    {
    public:
        LoginRequestHandler(Data::AuthorizedUsersMap& authorizedUsers) :
            ARequestHandler{authorizedUsers},
            ResourceFileMap{"/webpages/login"}, 
            _authorizedUsers{authorizedUsers}
        {}

        virtual ~LoginRequestHandler() = default;

    private:
        Data::AuthorizedUsersMap::UserNameView getUserName(const Message& message)
        {
            const Message::const_iterator itName = message.find(Field::authorization);
            if (itName == message.end())
                throw Exceptions::RequestException("Authorization name required", Status::bad_request);
            else
                return itName->value();
        }

    protected:
        virtual Response doGet(boost::string_view currentTarget, const Message& message) override
        {
            if (hasRights(message))
                return ResourceFileMap::getResponseWithPage(currentTarget);
            else
                throw Exceptions::AlreadyAuthorizedException();
        }

        virtual Response doPost(boost::string_view currentTarget, const Message& message) override;
        virtual Response doDelete(boost::string_view currentTarget, const Message& message) override;

    private:
        Data::AuthorizedUsersMap& _authorizedUsers;

    };
}
