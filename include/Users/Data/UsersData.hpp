#pragma once

#include "Users/AccessRights/RootAccessRightsNode.hpp"
#include "HTTP/Sockets/WebSocket.hpp"

#include <shared_mutex>

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
        constexpr std::initializer_list<Users::AccessRights::MethodWithPointer> AUTH_USER_WEBSOCKET_METHODS{HTTP::Requests::Method::get};

        using Token = boost::uuids::uuid;
        using NameSet = std::set<std::string, std::less<>>;
        using WebSocketMap = std::map<Token, std::shared_ptr<HTTP::Sockets::WebSocket>>;

        std::string TokenToStr(const Token& token)
        {
            return boost::uuids::to_string(token);
        }

        struct WebSocketUpdateInfo
        {
            const boost::string_view UserName;
            const HTTP::Sockets::WebSocket::State PrevState;
            const Token& WebSocketToken;
            HTTP::Sockets::WebSocket& WebSocket;
        };

        class UserData
        {
        public:
            inline UserData(AccessRights::RootAccessRightsNode accessRights, NameSet::const_iterator itUserName, WebSocketMap::iterator itWebSocket)
                : _accessRights{std::move(accessRights)}
                , _itUserName{std::move(itUserName)}
                , _itWebSocket{std::move(itWebSocket)}
            {}

            inline ~UserData()
            {
                auto pWS = _itWebSocket->second;
                if (pWS)
                {
                    pWS->setLogouted();
                }
            }

        public:
            inline boost::string_view getUserName() const
            {
                return *_itUserName;
            }

            inline HTTP::Sockets::WebSocket& getWebSocket()
            {
                return *_itWebSocket->second;
            }

        private:
            AccessRights::RootAccessRightsNode _accessRights;
            const NameSet::const_iterator _itUserName;
            const WebSocketMap::iterator  _itWebSocket;

        private:
            friend class UsersData;

        };

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

        public:
            UsersData(boost::asio::io_service& service)
                : _service{service}
                , _usersData{}
                , _usersNames{}
                , _unauthorizedAllowedMethods{"/login", Users::AccessRights::Methods{UNAUTHUSER_LOGIN_METHODS}}
                , _defaultAuthorizedAllowedMethods{"/chat", Users::AccessRights::Methods{AUTH_USER_CHAT_METHODS}
                                                , "/login", Users::AccessRights::Methods{AUTH_USER_LOGIN_METHODS}
                                                , "/websocket", Users::AccessRights::Methods{AUTH_USER_WEBSOCKET_METHODS}
                                                }
            {}

            ~UsersData() = default;

        private:
            inline Token generateToken() const
            {
                static boost::uuids::random_generator generator;
                return generator.operator()();
            }

        private:
            std::string addUser(const NameSet::const_iterator itUser, const boost::string_view loginName, Token&& token)
            {
                std::unique_lock lock{_mutex};

                auto itUserName = _usersNames.emplace_hint(itUser, loginName.to_string());

                auto itWebSocket = _websockets.emplace(std::piecewise_construct
                                , std::forward_as_tuple(generateToken())
                                , std::forward_as_tuple(std::make_shared<HTTP::Sockets::WebSocket>(_service))).first;

                auto tokenStr = TokenToStr(token);
                const auto& addedUser = _usersData.emplace(std::piecewise_construct
                                , std::forward_as_tuple(std::move(token))
                                , std::forward_as_tuple(_defaultAuthorizedAllowedMethods, std::move(itUserName), std::move(itWebSocket))).first->second;
                boost::ignore_unused(addedUser);
                return tokenStr;
            }
            
            inline Token viewToToken(const boost::string_view strToken) const
            {
                static boost::uuids::string_generator stringGenerator;
                return stringGenerator.operator()(strToken.begin(), strToken.end());
            }

            LogoutStatus LogoutUser(const std::map<Token, UserData>::const_iterator& itUser)
            {
                std::unique_lock lock{_mutex};

                _usersNames.erase(itUser->second._itUserName);
                _websockets.erase(itUser->second._itWebSocket);
                _usersData.erase(itUser);
                return LogoutStatus::OK;
            }

        public:
            LogoutStatus LogoutUser(const boost::string_view strToken)
            {
                try
                {
                    Token token = viewToToken(strToken);
                    std::shared_lock lock{_mutex};
                    const auto itUser = _usersData.find(token);

                    if (itUser == std::end(_usersData))
                    {
                        return LogoutStatus::TokenIsNotPresent;
                    }
                    else
                    {
                        lock.unlock();
                        return LogoutUser(itUser);
                    }
                }
                catch(const std::exception& e)
                {
                    return LogoutStatus::NotOK;
                }
            }
            
            std::string addUser(const boost::string_view loginName, LoginStatus& loginStatus)
            {
                std::shared_lock lock{_mutex};
                const auto itUserName = _usersNames.find(loginName);
                if (itUserName != std::end(_usersNames))
                {
                    loginStatus = LoginStatus::UsernameAlredyInUse;
                    return std::string();
                }
                lock.unlock();

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

            const AccessRights::RootAccessRightsNode& getUserAcessRights(const boost::string_view strToken)
            {
                const Token token = viewToToken(strToken);

                std::shared_lock lock{_mutex};
                const auto itUser = _usersData.find(token);
                return itUser == std::end(_usersData) ? _unauthorizedAllowedMethods : itUser->second._accessRights;
            }
 
            inline const AccessRights::RootAccessRightsNode& getUnauthorizedUserAcessRights() const
            {
                return _unauthorizedAllowedMethods;
            }

            inline bool isUserAuthorized(boost::string_view tokenStr) const
            {
                const Token token = viewToToken(tokenStr);
                std::shared_lock lock{_mutex};
                return _usersData.contains(token);
            }

            WebSocketUpdateInfo setSocket(const boost::string_view tokenStr, HTTP::Requests::Request&& request)
            {
                const Token token = viewToToken(tokenStr);

                std::unique_lock lock{_mutex};
                auto& userData = _usersData.at(token);
                auto itWebSocket = userData._itWebSocket;
                auto& ws = itWebSocket->second;
                auto prevState = ws->getState().second;
                ws->setSocket(std::move(request));

                return WebSocketUpdateInfo{userData.getUserName(), prevState, itWebSocket->first, *ws};
            }

        private:
            boost::asio::io_service& _service;

            std::map<Token, UserData> _usersData;
            NameSet _usersNames;
            WebSocketMap _websockets;

            const AccessRights::RootAccessRightsNode _unauthorizedAllowedMethods;
            const AccessRights::RootAccessRightsNode _defaultAuthorizedAllowedMethods;

            mutable std::shared_mutex _mutex;

        private:
            friend class UsersWebSockets;

        };
    }
}
