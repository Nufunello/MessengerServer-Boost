#pragma once

#include "HTTP/Requests/Request.hpp"

#include "HTTP/Sockets/AWebSocketMessage.hpp"

#include <boost/asio/io_service.hpp>

namespace Users
{
    namespace Data
    {
        class UserData;
    }
}

namespace HTTP
{
    namespace Sockets
    {
        class WebSocket
        {
        private:
            using WebSocketStream = boost::beast::websocket::stream<boost::asio::ip::tcp::socket>;

        public:
            enum class State
            {
                FirstConnection,
                Open,
                Closed,
                Logouted
            };

        public:
            WebSocket(boost::asio::io_service& service)
                : _buffer{1024}
                , _websocket{service}
                , _state{State::FirstConnection}
                , _stateChanged{true}
            {
            }

            ~WebSocket()
            {
                _websocket.next_layer().close();
            }

        private:
            inline void setLogouted()
            {
                _state = State::Logouted;
            }

            inline void updateState(const boost::system::error_code& errorCode)
            {
                if (_state != State::Logouted)
                {
                    const State _prevState = _state;
                    _state = errorCode == boost::beast::websocket::error::closed || errorCode == boost::asio::error::operation_aborted ? State::Closed : State::Open;
                    _stateChanged = _prevState != _state || _stateChanged;
                }
            }

            inline void acceptSocket(const Requests::Message& message)
            {
                boost::system::error_code ec;
                _websocket.accept(message, ec);
                updateState(ec);
            }

        public:
            void setSocket(Requests::Request&& request)
            {
                _websocket.next_layer() = std::move(request);
                acceptSocket(request.message());
                
                _websocket.async_read(_buffer, [&](const boost::system::error_code& errorCode, std::size_t bytes_transferred){
                    boost::ignore_unused(bytes_transferred);
                    updateState(errorCode);
                });
            }

        public:
            inline std::pair<bool, State> getState()
            {
                auto state = std::make_pair(_stateChanged, _state);
                _stateChanged = false;
                return state;
            }

        public:
            inline void write(const AWebSocketMessage& message)
            {
                if (_state == State::Open)
                {
                    _websocket.write_some(true, boost::asio::buffer(message.asString()));
                }
            }

        private:
            boost::beast::flat_buffer _buffer;
            WebSocketStream _websocket;

            State _state;
            bool _stateChanged;

        private:
            friend Users::Data::UserData;

        };
    }
}
