#include "Data/AuthorizedUsersMap.h"

#include <iostream>

using namespace Data;
using namespace MyRequestHandlers;

namespace {
    constexpr char LOGIN_RESOURCE[] {"/login"};
    constexpr char CHAT_RESOURCE[] {"/chat"};

    const Data::AccessRights ALL_USERS_ACCESS_RIGHTS{};

    std::pair<Data::AccessRights::Resource, std::initializer_list<Method>> getResourceRightPair(std::string&& resource, std::initializer_list<Method> methods)
    {
        return std::make_pair<Data::AccessRights::Resource, std::initializer_list<Method>>(std::move(resource), std::move(methods));
    }
} 

AuthorizedUsersMap::AuthorizedUsersMap() :
    _unauthorizedAccessRights{ ALL_USERS_ACCESS_RIGHTS, LOGIN_RESOURCE, {Method::get, Method::post} },
    _authorizedAccessRights{ ALL_USERS_ACCESS_RIGHTS,
            { 
                getResourceRightPair(CHAT_RESOURCE, {Method::get, Method::post}), 
                getResourceRightPair(LOGIN_RESOURCE, {Method::delete_})
            }
        }
{}

const AccessRights& AuthorizedUsersMap::getAccessRights(const TokenView& token) const
{
    const UserToken convertedToken = _stringGenerator(token.begin(), token.end());
    const AccessRightsMap::const_iterator itAccessRights = _accessRights.find(convertedToken);
    if (itAccessRights != _accessRights.end())
    {
        const AccessRightsMap::value_type& pair = *itAccessRights; 
        return pair.second.first;
    }
    return _unauthorizedAccessRights;
}

AccessRights AuthorizedUsersMap::deleteUser(const TokenView& token)
{
    const UserToken tok = _stringGenerator(token.begin(), token.end());
    const AccessRightsMap::const_iterator itToken = _accessRights.find(tok);
    if (itToken == _accessRights.end())
    {
        throw Exceptions::NotAuthorizedException();
    }
    else
    {
        std::pair<AccessRights, UsersSet::const_iterator> pair = std::move(itToken->second);
        _accessRights.erase(itToken);
        _usersAuthorized.erase(pair.second);
        return pair.first;
    }
}

AuthorizedUsersMap::Token AuthorizedUsersMap::addUser(const UsersSet::const_iterator& itUser, UserName&& username)
{
    UsersSet::const_iterator tempIt = _usersAuthorized.emplace_hint(itUser, std::move(username));
    std::pair<AccessRights, UsersSet::const_iterator> pair{_authorizedAccessRights, tempIt};
    const AccessRightsMap::const_iterator it = _accessRights.emplace(_randomGenerator(), std::move(pair)).first;
    return boost::uuids::to_string(it->first);
}

AuthorizedUsersMap::Token AuthorizedUsersMap::addUser(const UserNameView& username)
{
    UserName user = username.to_string();

    const UsersSet::const_iterator itUser = _usersAuthorized.find(user);
    if (itUser != _usersAuthorized.end())
    {
        throw Exceptions::UsernameAlreadyInUseException();
    }
    else
    {
        return this->addUser(itUser, std::move(user));
    }
}
