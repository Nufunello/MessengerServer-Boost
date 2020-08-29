#pragma once

#include "Data/AccessRights.h"
#include "Exceptions/UsernameAlreadyInUseException.h"

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
        using AccessRightsMap = std::map<UserToken, AccessRights>;

    public:
        AuthorizedUsersMap();
        virtual ~AuthorizedUsersMap()  = default;

    public:
        const AccessRights& getUnathorizedAccessRights() const
        {
            return _unauthorizedAccessRights;
        }
        
        const AccessRights& getAccessRights(const TokenView& token) const;

    private:
        bool isUserWithNameAuthorized(const UserName& username) const
        {
            return _usersAuthorized.find(username) != _usersAuthorized.end();
        }

    public:
        bool isUserWithTokenViewAuthorized(const TokenView& token) const
        {
            const UserToken tok = _stringGenerator(token.begin(), token.end());
            return _accessRights.find(tok) != _accessRights.end();
        }

    private:
        Token addUser(UserName&& username)
        {
            _usersAuthorized.emplace(std::move(username));
            const AccessRightsMap::const_iterator it = _accessRights.emplace(_randomGenerator(), _authorizedAccessRights).first;
            return boost::uuids::to_string(it->first);
        }

    public:
        Token addUser(const UserNameView& username)
        {
            UserName user = username.to_string();

            if (this->isUserWithNameAuthorized(user))
            {
                throw Exceptions::UsernameAlreadyInUseException();
            }
            else
            {
                return this->addUser(std::move(user));
            }
        }

    private:
        const AccessRights _unauthorizedAccessRights;
        const AccessRights _authorizedAccessRights;

        AccessRightsMap    _accessRights;
        std::set<UserName> _usersAuthorized;

        StringGenerator _stringGenerator;
        RandomGenerator _randomGenerator;

    };
}