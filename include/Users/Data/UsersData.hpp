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
        constexpr std::initializer_list<Users::AccessRights::MethodWithPointer> AUTH_USER_CHAT_ACTIVE_USERS_METHODS{HTTP::Requests::Method::get};

        class UsersData
        {
        public:
            enum class LoginStatus
            {
                OK = 0,
                UsernameAlredyInUse,
                NotOK
            };

            enum class LogoutStatus
            {
                OK = 0,
                TokenIsNotPresent,
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
                : _usersData{}
                , _usersNames{}
                , _unauthorizedAllowedMethods{"/login", Users::AccessRights::Methods{UNAUTHUSER_LOGIN_METHODS}}
                , _defaultAuthorizedAllowedMethods{"/chat", Users::AccessRights::Methods{AUTH_USER_CHAT_METHODS}
                                                , "/login", Users::AccessRights::Methods{AUTH_USER_LOGIN_METHODS}
                                                , "/chat/activeUsers", Users::AccessRights::Methods{AUTH_USER_CHAT_ACTIVE_USERS_METHODS}
                                                }
            {}

            ~UsersData() = default;

        private:
            Token generateToken()
            {
                static boost::uuids::random_generator generator;
                return generator.operator()();
            }

        private:
            std::string addUser(const std::set<std::string, std::less<>>::const_iterator itUser, const boost::string_view loginName, Token&& token)
            {
                auto itUserName = _usersNames.emplace_hint(itUser, loginName);
                std::string tokenStr = boost::uuids::to_string(token);
                _usersData.emplace(std::move(token), UserData{_defaultAuthorizedAllowedMethods, std::move(itUserName)});
                return tokenStr;
            }
            
            Token viewToToken(const boost::string_view strToken)
            {
                static boost::uuids::string_generator stringGenerator;
                return stringGenerator.operator()(strToken.begin(), strToken.end());
            }

        public:
            LogoutStatus LogoutUser(const boost::string_view strToken)
            {
                try
                {
                    Token token = viewToToken(strToken);
                    auto itUser = _usersData.find(token);

                    if (itUser == std::end(_usersData))
                    {
                        return LogoutStatus::TokenIsNotPresent;
                    }
                    else
                    {
                        _usersNames.erase(itUser->second.ItUserName);
                        _usersData.erase(itUser);
                        return LogoutStatus::OK;
                    }
                }
                catch(const std::exception& e)
                {
                    return LogoutStatus::NotOK;
                }
            }
            
            std::string addUser(const boost::string_view loginName, LoginStatus& loginStatus)
            {
                auto itUserName = _usersNames.find(loginName);
                if (itUserName != std::end(_usersNames))
                {
                    loginStatus = LoginStatus::UsernameAlredyInUse;
                    return std::string();
                }

                try
                {
                    loginStatus = LoginStatus::OK;
                    return addUser(itUserName, loginName, generateToken());
                }
                catch(const std::exception& e)
                {
                    loginStatus = LoginStatus::NotOK;
                    return std::string();
                }
            }

            const AccessRights::RootAccessRightsNode& getUsersAllowedMethods(const boost::string_view strToken)
            {
                Token token = viewToToken(strToken);;
                const auto itUser = _usersData.find(token);
                return itUser == std::end(_usersData) ? _unauthorizedAllowedMethods : itUser->second.AccessRights;
            }

            const AccessRights::RootAccessRightsNode& getUnauthorizedAllowedMethods() const
            {
                return _unauthorizedAllowedMethods;
            }

            boost::string_view getUserName(const boost::string_view strToken)
            {
                Token token = viewToToken(strToken);
                const UserData& userData = _usersData.at(token);
                return *(userData.ItUserName);
            }

        private:
            std::map<Token, UserData> _usersData;
            std::set<std::string, std::less<>> _usersNames;

            const AccessRights::RootAccessRightsNode _unauthorizedAllowedMethods;
            const AccessRights::RootAccessRightsNode _defaultAuthorizedAllowedMethods;
        };
    }
}
