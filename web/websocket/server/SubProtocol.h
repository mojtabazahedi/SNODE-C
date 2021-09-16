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

#ifndef WEB_WS_SERVER_SUBSPROTOCOL_H
#define WEB_WS_SERVER_SUBSPROTOCOL_H

#include "web/websocket/SubProtocol.h" // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::websocket::server {

    class SubProtocol : public web::websocket::SubProtocol {
    protected:
        SubProtocol(const std::string& name);

        SubProtocol() = delete;
        SubProtocol(const SubProtocol&) = delete;
        SubProtocol& operator=(const SubProtocol&) = delete;

        ~SubProtocol() override;

    public:
        /* Facade (API) to WSServerContext -> WSTransmitter to be used from SubProtocol-Subclasses */
        using web::websocket::SubProtocol::sendMessage;

        using web::websocket::SubProtocol::sendMessageEnd;
        using web::websocket::SubProtocol::sendMessageFrame;
        using web::websocket::SubProtocol::sendMessageStart;

        using web::websocket::SubProtocol::sendClose;
        using web::websocket::SubProtocol::sendPing;

        void sendBroadcast(const char* message, std::size_t messageLength);
        void sendBroadcast(const std::string& message);

        void sendBroadcastStart(const char* message, std::size_t messageLength);
        void sendBroadcastStart(const std::string& message);

        void sendBroadcastFrame(const char* message, std::size_t messageLength);
        void sendBroadcastFrame(const std::string& message);

        void sendBroadcastEnd(const char* message, std::size_t messageLength);
        void sendBroadcastEnd(const std::string& message);

        void forEachClient(const std::function<void(SubProtocol*)>& forEachClient);

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

        void setClients(std::shared_ptr<std::list<SubProtocol*>> clients);

        std::shared_ptr<std::list<SubProtocol*>> clients;

        friend class SocketContext;
    };

} // namespace web::websocket::server

#endif // WEB_WS_SERVER_SUBSPROTOCOL_H
