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

#ifndef WEB_WS_SUBPROTOCOL_ECHO_SERVER_ECHO_H
#define WEB_WS_SUBPROTOCOL_ECHO_SERVER_ECHO_H

#include "web/websocket/server/SubProtocol.h"

namespace net::timer {
    class Timer;
} // namespace net::timer

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef> // for std::size_t
#include <cstdint> // for uint16_t
#include <string>  // for allocator, string

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#define NAME "echo"

namespace web::websocket::subprotocol::echo::server {

    class Echo : public web::websocket::server::SubProtocol {
    public:
        Echo();

        Echo(const Echo&) = delete;
        Echo& operator=(const Echo&) = delete;

        ~Echo() override;

        void onConnected() override;
        void onMessageStart(int opCode) override;
        void onMessageData(const char* junk, std::size_t junkLen) override;
        void onMessageEnd() override;
        void onMessageError(uint16_t errnum) override;
        void onPongReceived() override;
        void onDisconnected() override;

    private:
        std::string data;

        int flyingPings = 0;

        net::timer::Timer& pingTimer;
    };

} // namespace web::websocket::subprotocol::echo::server

#endif // WEB_WS_SUBPROTOCOL_ECHO_SERVER_ECHO_H
