#include "Handlers/LoginRequestHandler.h"

#include "Exceptions/RedirectException.h"

using namespace MyRequestHandlers;

namespace {
    constexpr Field TOKEN_FIELD = Field::cookie;
    constexpr Field SET_TOKEN_FIELD = Field::set_cookie;
}

Response LoginRequestHandler::doPost(boost::string_view currentTarget, const Message& message)
{
    if (!this->hasRights(message))
    {
        throw Exceptions::AlreadyAuthorizedException();
    }

    const Data::AuthorizedUsersMap::UserNameView view = this->getUserName(message);
    Response response;
    Data::AuthorizedUsersMap::Token token = _authorizedUsers.addUser(view);
    response.insert(SET_TOKEN_FIELD, token);
    response.result(Status::ok);
    return response;
}

Response LoginRequestHandler::doDelete(boost::string_view currentTarget, const Message& message)
{
    if (!this->hasRights(message))
    {
        throw Exceptions::NotAuthorizedException();
    }
    _authorizedUsers.deleteUser(message.at(TOKEN_FIELD));
    
    Response response;
    response.result(Status::ok);
    response.insert(SET_TOKEN_FIELD, "");
    return response;
}
