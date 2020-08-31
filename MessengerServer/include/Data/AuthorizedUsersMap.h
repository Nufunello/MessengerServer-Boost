#pragma once

#include "Data/AccessRights.h"
#include "Exceptions/UsernameAlreadyInUseException.h"
#include "Exceptions/NotAuthorizedException.h"

#include <set>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/random_generator.hpp>

namespace Data
{
    class AuthorizedUsersMap
    {
    public:
        using Token        = std::string;
        using TokenView    = boost::string_view;
        using UserNameView = boost::string_view;

    private:
        using UserName     = std::string;

        using StringGenerator = boost::uuids::string_generator;
        using RandomGenerator = boost::uuids::random_generator;
        using UserToken = boost::uuids::uuid;
        using UsersSet = std::set<UserName>;
        using AccessRightsMap = std::map<UserToken, std::pair<AccessRights, UsersSet::const_iterator>>;

    public:
        AuthorizedUsersMap();
        virtual ~AuthorizedUsersMap()  = default;

    public:
        const AccessRights& getUnathorizedAccessRights() const
        {
            return _unauthorizedAccessRights;
        }
        
        const AccessRights& getAccessRights(const TokenView& token) const;

    public:
        bool isUserWithTokenViewAuthorized(const TokenView& token) const
        {
            const UserToken tok = _stringGenerator(token.begin(), token.end());
            return _accessRights.find(tok) != _accessRights.end();
        }

    private:
        Token addUser(const UsersSet::const_iterator& itUser, UserName&& username);

    public:
        Token addUser(const UserNameView& username);

        AccessRights deleteUser(const TokenView& token);

    private:
        const AccessRights _unauthorizedAccessRights;
        const AccessRights _authorizedAccessRights;

        AccessRightsMap _accessRights;
        UsersSet        _usersAuthorized;

        StringGenerator _stringGenerator;
        RandomGenerator _randomGenerator;

    };
}