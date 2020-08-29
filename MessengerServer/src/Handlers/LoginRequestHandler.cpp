#include "Handlers/LoginRequestHandler.h"

#include "Exceptions/RedirectException.h"

using namespace MyRequestHandlers; 

Response LoginRequestHandler::doPost(boost::string_view currentTarget, const Message& message)
{
    const Message::const_iterator itCookie = message.find(Field::cookie);
    if (itCookie != message.end() && _authorizedUsers.isUserWithTokenViewAuthorized(itCookie->value()))
    {
        throw Exceptions::RedirectException("Already logined", "/chat", Status::found);
    }

    const Data::AuthorizedUsersMap::UserNameView view = this->getUserName(message);
    Response response;
    response.insert(Field::cookie, _authorizedUsers.addUser(view));
    response.result(Status::ok);
    return response;
}
