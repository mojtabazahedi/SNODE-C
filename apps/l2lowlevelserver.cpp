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

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "log/Logger.h"                                // for Writer, Storage
#include "net/SNodeC.h"                                // for SNodeC
#include "net/socket/bluetooth/address/L2CapAddress.h" // for L2CapAddress
#include "net/socket/bluetooth/l2cap/Socket.h"         // for l2cap
#include "net/socket/bluetooth/l2cap/SocketServer.h"   // for SocketServer
#include "net/socket/stream/SocketContext.h"           // for SocketProtocol
#include "net/socket/stream/SocketContextFactory.h"    // for SocketProtoco...
#include "net/socket/stream/SocketServer.h"            // for SocketServer<...

namespace net::socket::stream {
    class SocketConnection;
} // namespace net::socket::stream

#include <cstddef>    // for size_t
#include <functional> // for function
#include <string>     // for allocator

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

using namespace net::socket::bluetooth::l2cap;

class SimpleSocketProtocol : public net::socket::stream::SocketContext {
public:
    explicit SimpleSocketProtocol(net::socket::stream::SocketConnection* socketConnection)
        : net::socket::stream::SocketContext(socketConnection) {
    }

    void onReceiveFromPeer() override {
        char junk[4096];

        std::size_t ret = readFromPeer(junk, 4096);

        VLOG(0) << "Data to reflect: " << std::string(junk, ret);
        sendToPeer(junk, ret);
    }

    void onWriteError(int errnum) override {
        VLOG(0) << "OnWriteError: " << errnum;
    }

    void onReadError(int errnum) override {
        VLOG(0) << "OnReadError: " << errnum;
    }
};

class SimpleSocketProtocolFactory : public net::socket::stream::SocketContextFactory {
public:
    ~SimpleSocketProtocolFactory() = default;

    net::socket::stream::SocketContext* create(net::socket::stream::SocketConnection* socketConnection) const override {
        return new SimpleSocketProtocol(socketConnection);
    }
};

int main(int argc, char* argv[]) {
    net::SNodeC::init(argc, argv);

    SocketServer<SimpleSocketProtocolFactory> btServer(
        [](const SocketServer<SimpleSocketProtocolFactory>::SocketAddress& localAddress,
           [[maybe_unused]] const SocketServer<SimpleSocketProtocolFactory>::SocketAddress& remoteAddress) -> void { // OnConnect
            VLOG(0) << "OnConnect";

            VLOG(0) << "\tServer: " + localAddress.toString();
            VLOG(0) << "\tClient: " + remoteAddress.toString();
        },
        []([[maybe_unused]] SocketServer<SimpleSocketProtocolFactory>::SocketConnection* socketConnection) -> void { // onConnected
            VLOG(0) << "OnConnected";

        },
        [](SocketServer<SimpleSocketProtocolFactory>::SocketConnection* socketConnection) -> void { // onDisconnect
            VLOG(0) << "OnDisconnect";

            VLOG(0) << "\tServer: " + socketConnection->getLocalAddress().toString();
            VLOG(0) << "\tClient: " + socketConnection->getRemoteAddress().toString();
        });

    btServer.listen(
        SocketServer<SimpleSocketProtocolFactory>::SocketAddress("A4:B1:C1:2C:82:37", 0x1023), 5, [](int errnum) -> void { // titan
            if (errnum != 0) {
                LOG(ERROR) << "BT listen: " << errnum;
            } else {
                LOG(INFO) << "BT listening on psm 0x1023";
            }
        });

    return net::SNodeC::start();
}
