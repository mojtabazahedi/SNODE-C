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

#include "Echo.h"

#include "log/Logger.h"
#include "net/timer/IntervalTimer.h" // for IntervalTimer

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional> // for function

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#define MAX_FLYING_PINGS 3
#define PING_DELAY 5

namespace web::websocket::subprotocol::echo::server {

    Echo::Echo()
        : web::websocket::server::SubProtocol(NAME)
        , pingTimer(net::timer::Timer::intervalTimer(
              [this]([[maybe_unused]] const void* arg, [[maybe_unused]] const std::function<void()>& stop) -> void {
                  this->sendPing();
                  this->flyingPings++;
                  if (this->flyingPings >= MAX_FLYING_PINGS) {
                      this->sendClose();
                  }
              },
              {PING_DELAY, 0},
              nullptr)) {
    }

    Echo::~Echo() {
        pingTimer.cancel();
    }

    void Echo::onConnected() {
        VLOG(0) << "On protocol connected:";

        sendMessage("Welcome to SimpleChat");
        sendMessage("=====================");

        VLOG(0) << "\tServer: " + getLocalAddressAsString();
        VLOG(0) << "\tClient: " + getRemoteAddressAsString();
    }

    void Echo::onMessageStart(int opCode) {
        VLOG(0) << "Message Start - OpCode: " << opCode;
    }

    void Echo::onMessageData(const char* junk, std::size_t junkLen) {
        data += std::string(junk, junkLen);

        VLOG(0) << "Message Fragment: " << std::string(junk, junkLen);
    }

    void Echo::onMessageEnd() {
        VLOG(0) << "Message Full Data: " << data;
        VLOG(0) << "Message End";
        /*
                forEachClient([&data = this->data](SubProtocol* client) {
                    client->sendMessage(data);
                });
        */
        sendBroadcast(data);

        data.clear();
    }

    void Echo::onMessageError(uint16_t errnum) {
        VLOG(0) << "Message error: " << errnum;
    }

    void Echo::onPongReceived() {
        VLOG(0) << "Pong received";
        flyingPings = 0;
    }
    void Echo::onDisconnected() {
        VLOG(0) << "On protocol disconnected:";

        VLOG(0) << "\tServer: " + getLocalAddressAsString();
        VLOG(0) << "\tClient: " + getRemoteAddressAsString();
    }

} // namespace web::websocket::subprotocol::echo::server
