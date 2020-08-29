#include "Data/AuthorizedUsersMap.h"

using namespace Data;
using namespace MyRequestHandlers;

namespace {
    constexpr char ROOT_RESOURCE[]  {""};
    constexpr char LOGIN_RESOURCE[] {"/login"};
    constexpr char CHAT_RESOURCE[] {"/chat"};

    const Data::AccessRights ALL_USERS_ACCESS_RIGHTS (Data::AccessRights::Resource{ROOT_RESOURCE}, {Method::get});
} 

AuthorizedUsersMap::AuthorizedUsersMap() :
    _unauthorizedAccessRights{ ALL_USERS_ACCESS_RIGHTS, LOGIN_RESOURCE, {Method::get, Method::post} },
    _authorizedAccessRights{ ALL_USERS_ACCESS_RIGHTS, CHAT_RESOURCE, {Method::get, Method::post}}
{}

const AccessRights& AuthorizedUsersMap::getAccessRights(const TokenView& token) const
{
    const UserToken convertedToken = _stringGenerator(token.data());
    const AccessRightsMap::const_iterator itAccessRights = _accessRights.find(convertedToken);
    if (itAccessRights != _accessRights.end())
    {
        const AccessRightsMap::value_type& pair = *itAccessRights; 
        return pair.second;
    }
    return _unauthorizedAccessRights;
}
