#pragma once

#include "Handlers/ALoadablePageRequestHandler.h"
#include "Data/AuthorizedUsersMap.h"

namespace MyRequestHandlers
{
    class LoginRequestHandler :
        public ALoadablePageRequestHandler
    {
    public:
        LoginRequestHandler(Data::AuthorizedUsersMap& authorizedUsers) :
            ALoadablePageRequestHandler{"/webpages/login"}, 
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
        virtual Response doPost(boost::string_view currentTarget, const Message& message) override;

    private:
        Data::AuthorizedUsersMap& _authorizedUsers;

    };
}
