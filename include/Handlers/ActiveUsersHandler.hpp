#pragma once

#include "Handlers/AAuthorizedHandler.hpp"

#include "HTTP/Responses/WebSocketResponse.hpp"

#include "Users/Data/UsersData.hpp"

#include <forward_list>
#include <thread>
#include <chrono>
#include <mutex>

namespace Handlers
{
    class ActiveUsersHandlers
        : public AAuthorizedHandler
    {
    private:
        static constexpr size_t C_AVG_USERS {100};

    private:
        class WebSocket
        {
        public:
            WebSocket(HTTP::Requests::Request&& request)
                : _buffer{128}
                , _websocket{std::move(request)}
                , _name{""}
                , _isDisconnected{false}
            {
                boost::system::error_code ec;
                _websocket.accept(request.message(), ec);
                _isDisconnected = ec.failed();

                _websocket.async_read(_buffer, [&](const boost::system::error_code& errorCode, std::size_t bytes_transferred){
                    boost::ignore_unused(bytes_transferred);
                    _isDisconnected = errorCode.failed();
                });
            }

            ~WebSocket()
            {
                _websocket.next_layer().close();
            }

        public:
            void setName(boost::string_view name)
            {
                _name = name;
            }

            boost::string_view getName() const
            {
                return _name;
            }

            void write(const std::string& message)
            {
                _websocket.write_some(true, boost::asio::buffer(message));
            }
            
            bool isDisconnected() const
            {
                return _isDisconnected;
            }

        private:
            boost::beast::flat_buffer _buffer;
            boost::beast::websocket::stream<boost::asio::ip::tcp::socket> _websocket;
            boost::string_view _name;
            bool _isDisconnected;

        };

    public:
        ActiveUsersHandlers(Users::Data::UsersData& usersData)
            : _websockets{}
            , _usersData{usersData}
            , _updateWebSockets{std::thread([&](){
                while (true)
                {
                    std::unique_lock<std::mutex> lock(_mutex, std::defer_lock);
                    if (lock.try_lock())
                    {
                        this->updateWebSockets();
                        lock.unlock();
                        std::this_thread::sleep_for(std::chrono::seconds{1});
                    }
                }
            })}
            , _mutex{}
        {
            _websockets.reserve(C_AVG_USERS);
        }

        virtual ~ActiveUsersHandlers() = default;

    private:
        void updateWebSockets()
        {
            const auto itEnd = std::end(_websockets);
            const auto itClosed = std::remove_if(std::begin(_websockets), itEnd, [](const std::unique_ptr<WebSocket>& websocket){
                return websocket->isDisconnected();
            });

            if (itClosed != itEnd)
            {
                std::string closed;
                
                for (auto itWebSocket = std::begin(_websockets); itWebSocket != itClosed; ++itWebSocket)
                {
                    std::unique_ptr<WebSocket>& ptr = *itWebSocket;
                    closed.append("-d" + ptr->getName().to_string());
                }

                _websockets.erase(itClosed, itEnd);

                this->writeAll(closed);
            }
        }

        void writeAll(const std::string& message)
        {
            auto itEnd = std::end(_websockets);
            for (auto itWebSocket = std::begin(_websockets); itWebSocket != itEnd; ++itWebSocket)
            {
                std::unique_ptr<WebSocket>& ptr = *itWebSocket;
                ptr->write(message);
            }
        }

        void notifyAboutNewUser(const boost::string_view name)
        {
            const std::string message = "-a" + name.to_string();
            this->writeAll(message);
        }

    protected:
        HTTP::Responses::IResponse::Ptr doGet(HTTP::Requests::Request&& request, URI::Segment target) override
        {
            if (!target.isEmpty())
            {
                return std::make_unique<HTTP::Responses::ResourceNotFoundResponse>(std::move(request));
            }

            auto ws = std::make_unique<WebSocket>(std::move(request));

            if (!ws->isDisconnected())
            {
                std::lock_guard<std::mutex> lock(_mutex, std::adopt_lock);

                boost::string_view token = request.getToken().value();
                boost::string_view userName = _usersData.getUserName(token);
                ws->setName(userName);
                notifyAboutNewUser(userName);
                _websockets.emplace_back(std::move(ws));
            }

            return std::make_unique<HTTP::Responses::WebSocketResponse>();
        }

    private:
        std::vector<std::unique_ptr<WebSocket>> _websockets; 
        Users::Data::UsersData& _usersData;
        std::thread _updateWebSockets;
        std::mutex _mutex;

    };
}
