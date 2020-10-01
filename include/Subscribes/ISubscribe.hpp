#pragma once

#include "HTTP/Sockets/WebSocket.hpp"

#include "Users/Data/UsersData.hpp"

#include <memory>

namespace Subscribes
{
    class ISubscribe
    {
    protected:
        using Subscriber = std::shared_ptr<HTTP::Sockets::WebSocket>;
    
    protected:
        ISubscribe() = default;

    public:
        virtual ~ISubscribe() = default;
    
    protected:
        virtual void emplaceSubscriber(Users::Data::Token subscriberToken, const std::string& username, Subscriber pSubscriber) = 0;

    public:
        virtual void updateSubscribers() = 0;
        virtual void sendStatus(Subscriber subscriber) = 0;
        void addSubscribe(Users::Data::Token subscriberToken, const std::string& username, Subscriber pSubscriber)
        {
            sendStatus(pSubscriber);
            emplaceSubscriber(std::move(subscriberToken), username, pSubscriber);
        }

    };
    
}
