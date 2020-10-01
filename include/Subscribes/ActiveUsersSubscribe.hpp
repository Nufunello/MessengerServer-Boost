#pragma once

#include "Subscribes/ISubscribe.hpp"

#include "HTTP/Sockets/NewUserMessage.hpp"

#include "HTTP/Sockets/MessageContainers/UserOnlineWebSocketMessageContainer.hpp"
#include "HTTP/Sockets/MessageContainers/UserClosedConnectionWebSocketMessageContainer.hpp"
#include "HTTP/Sockets/MessageContainers/UserLogoutWebSocketMessageContainer.hpp"

namespace Subscribes
{
    class ActiveUsersSubscribe
        : public ISubscribe
    {
    private:
        class ActiveUser
        {
        public:
            ActiveUser(Subscriber subscriber, std::string subscriberToken, boost::string_view username)
                : _subscriber{subscriber}
                , _subscriberToken{std::move(subscriberToken)}
                , _username{username}
            {}

            ~ActiveUser() = default;

            Subscriber::element_type& getSubscriber()
            {
                return *_subscriber;
            }

            const std::string& getToken() const
            {
                return _subscriberToken;
            }

            boost::string_view getUserName() const
            {
                return _username;
            }

        private:
            Subscriber _subscriber;
            std::string _subscriberToken;
            boost::string_view _username;
        };

    public:
        ActiveUsersSubscribe() = default;
        ~ActiveUsersSubscribe() = default;

    private:
        template <typename Message, typename ...Messages>
        inline void writeToSubscriber(Subscriber::element_type& subscriber, Message&& message, Messages&&... messages)
        {
            if (!message.isEmpty())
            {
                subscriber.write(message);
            }
            if constexpr(sizeof...(messages) > 0)
            {
                writeToSubscriber(subscriber, std::forward<Messages>(messages)...);
            }
        }

        template <typename ...Messages>
        inline void writeToSubscribers(Messages&&... messages)
        {
            std::shared_lock lock(_mutex);
            for (auto& subscriberPair : _activeSubscribers)
            {
                writeToSubscriber(subscriberPair.getSubscriber(), std::forward<Messages>(messages)...);
            }
        }

    protected:
        void emplaceSubscriber(Users::Data::Token subscriberToken, const std::string& username, Subscriber pSubscriber) override
        {
            auto token = Users::Data::TokenToStr(subscriberToken);
            HTTP::Sockets::NewUserMessage message{token, username};
            this->writeToSubscribers(message);

            std::scoped_lock lock(_mutex);
            _activeSubscribers.emplace_back(pSubscriber, std::move(token), username);
        }

    public:
        void updateSubscribers() override
        {
            HTTP::Sockets::MessageContainers::UserOnlineWebSocketMessageContainer online;
            HTTP::Sockets::MessageContainers::UserClosedConnectionWebSocketMessageContainer closed;
            HTTP::Sockets::MessageContainers::UserLogoutWebSocketMessageContainer logouted;

            {
                std::scoped_lock lock(_mutex);
                std::erase_if(_activeSubscribers, [&online, &closed, &logouted](auto& activeUser) {
                    auto& subscriber = activeUser.getSubscriber();
                    const auto [stateChanged, state] = subscriber.getState();
                    if (stateChanged)
                    {
                        const std::string& token = activeUser.getToken();
                        switch (state)
                        {
                            case Subscriber::element_type::State::Open:
                            {
                                online.append(token);
                                break;
                            }
                            case Subscriber::element_type::State::Closed:
                            {
                                closed.append(token);
                                break;
                            }
                            case Subscriber::element_type::State::Logouted:
                            {
                                logouted.append(token);
                                break;
                            }

                            default:
                            {
                            }
                        }
                    }

                    return state == HTTP::Sockets::WebSocket::State::Logouted;
                });
            }

            if (!online.isEmpty() || !closed.isEmpty() || !logouted.isEmpty())
            {
                this->writeToSubscribers(online, closed, logouted);
            }
        }
        
        inline void sendStatus(Subscriber subscriber) override
        {
            std::shared_lock lock(_mutex);
            for (auto& activeUser : _activeSubscribers)
            {
                const auto state = activeUser.getSubscriber().getState().second;
                if (state != Subscriber::element_type::State::Logouted)
                {
                    HTTP::Sockets::NewUserMessage message{activeUser.getToken(), activeUser.getUserName().to_string()};
                    subscriber->write(message);
                };
            }
        }

    private:
        std::shared_mutex _mutex;
        std::vector<ActiveUser> _activeSubscribers;

    };
    
}
