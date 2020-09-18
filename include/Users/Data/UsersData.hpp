#pragma once

#include "Users/AccessRights/RootAccessRightsNode.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace Users
{
    namespace Data
    {
        constexpr std::initializer_list<Users::AccessRights::MethodWithPointer> UNAUTHUSER_LOGIN_METHODS{HTTP::Requests::Method::get, HTTP::Requests::Method::post};
        constexpr std::initializer_list<Users::AccessRights::MethodWithPointer> AUTH_USER_LOGIN_METHODS{HTTP::Requests::Method::delete_};
        constexpr std::initializer_list<Users::AccessRights::MethodWithPointer> AUTH_USER_CHAT_METHODS{HTTP::Requests::Method::get, HTTP::Requests::Method::post};

        class UsersData
        {
        public:
            enum class LoginStatus
            {
                OK = 0,
                UsernameAlredyInUse,
                NotOK
            };

        private:
            using Token = boost::uuids::uuid; 

        private:
            struct UserData
            {
                UserData(const AccessRights::RootAccessRightsNode& accessRights, std::set<std::string>::const_iterator&& itUserName)
                    : AccessRights{accessRights}
                    , ItUserName{std::move(itUserName)}
                {}

                AccessRights::RootAccessRightsNode AccessRights;
                const std::set<std::string>::const_iterator ItUserName;
            };

        public:
            UsersData()
                : _unauthorizedAllowedMethods{"/login", Users::AccessRights::Methods{UNAUTHUSER_LOGIN_METHODS}}
                , _defaultAuthorizedAllowedMethods{"/chat", Users::AccessRights::Methods{AUTH_USER_CHAT_METHODS}, "/login", Users::AccessRights::Methods{AUTH_USER_LOGIN_METHODS}}
            {}

            ~UsersData() = default;

        private:
            Token generateToken()
            {
                static boost::uuids::random_generator generator;
                return generator.operator()();
            }

        public:
            std::string addUser(const boost::string_view loginName, LoginStatus& loginStatus)
            {
                auto itUser = _userNames.find(loginName);
                if (itUser != std::end(_userNames))
                {
                    loginStatus = LoginStatus::UsernameAlredyInUse;
                    return std::string();
                }

                try
                {
                    auto itUserName = _userNames.emplace_hint(itUser, loginName);

                    Token token = generateToken();
                    std::string tokenStr = boost::uuids::to_string(token);

                    _usersData.emplace(std::move(token), UserData{_defaultAuthorizedAllowedMethods, std::move(itUserName)});
                    loginStatus = LoginStatus::OK;

                    return tokenStr;
                }
                catch(const std::exception& e)
                {
                    loginStatus = LoginStatus::NotOK;
                    return std::string();
                }
            }

            const AccessRights::RootAccessRightsNode& getUsersAllowedMethods(const boost::string_view strToken)
            {
                static boost::uuids::string_generator stringGenerator;
                Token token = stringGenerator.operator()(strToken.begin(), strToken.end());
                const auto itUser = _usersData.find(token);
                return itUser == std::end(_usersData) ? _unauthorizedAllowedMethods : itUser->second.AccessRights;
            }

            const AccessRights::RootAccessRightsNode& getUnauthorizedAllowedMethods() const
            {
                return _unauthorizedAllowedMethods;
            } 

        private:
            std::map<Token, UserData, std::less<>> _usersData;
            std::set<std::string, std::less<>> _userNames;

            const AccessRights::RootAccessRightsNode _unauthorizedAllowedMethods;
            const AccessRights::RootAccessRightsNode _defaultAuthorizedAllowedMethods;
        };
    };
};
