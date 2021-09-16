/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WEB_WS_CLIENT_SUBSPROTOCOL_H
#define WEB_WS_CLIENT_SUBSPROTOCOL_H

#include "web/websocket/SubProtocol.h"

namespace web::websocket::client {
    class SocketContext;
    class SubProtocolFactorySelector;
} // namespace web::websocket::client

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef>
#include <cstdint>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::websocket::client {

    class SubProtocol : public web::websocket::SubProtocol {
    public:
        enum class Role { SERVER, CLIENT };

    protected:
        SubProtocol(const std::string& name);

        SubProtocol() = delete;
        SubProtocol(const SubProtocol&) = delete;
        SubProtocol& operator=(const SubProtocol&) = delete;

    public:
        ~SubProtocol() override = default;

    public:
        /* Facade (API) to WSServerContext -> WSTransmitter to be used from SubProtocol-Subclasses */
        using web::websocket::SubProtocol::sendMessage;

        using web::websocket::SubProtocol::sendMessageEnd;
        using web::websocket::SubProtocol::sendMessageFrame;
        using web::websocket::SubProtocol::sendMessageStart;

        using web::websocket::SubProtocol::sendClose;
        using web::websocket::SubProtocol::sendPing;

    private:
        /* Callbacks (API) WSReceiver -> SubProtocol-Subclasses */
        void onMessageStart(int opCode) override = 0;
        void onMessageData(const char* junk, std::size_t junkLen) override = 0;
        void onMessageEnd() override = 0;
        void onPongReceived() override = 0;
        void onMessageError(uint16_t errnum) override = 0;

        /* Callbacks (API) socketConnection -> SubProtocol-Subclasses */
        void onConnected() override = 0;
        void onDisconnected() override = 0;

        /* Internal used methods */
        void sendBroadcast(uint8_t opCode, const char* message, std::size_t messageLength);
        void sendBroadcastStart(uint8_t opCode, const char* message, std::size_t messageLength);

        friend class web::websocket::client::SocketContext;
        friend class web::websocket::client::SubProtocolFactorySelector;
    };

} // namespace web::websocket::client

#endif // WEB_WS_CLIENT_SUBSPROTOCOL_H
