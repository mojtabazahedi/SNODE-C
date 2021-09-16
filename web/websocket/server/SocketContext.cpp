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

#include "SocketContext.h"

#include "SubProtocol.h"
#include "SubProtocolFactory.h"
#include "SubProtocolFactorySelector.h"
#include "web/http/server/Request.h"  // for Request
#include "web/http/server/Response.h" // for Response
#include "web/websocket/ws_utils.h"

namespace net::socket::stream {
    class SocketConnection;
} // namespace net::socket::stream

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <memory> // for allocator
#include <string> // for string

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#define CLOSE_SOCKET_TIMEOUT 10

namespace web::websocket::server {

    SocketContext::SocketContext(net::socket::stream::SocketConnection* socketConnection, web::websocket::SubProtocol* subProtocol)
        : web::websocket::SocketContext(socketConnection, subProtocol, Transmitter::Role::SERVER) {
    }

    SocketContext::~SocketContext() {
        SubProtocolFactorySelector::instance()->destroy(subProtocol);
    }

    SocketContext* SocketContext::create(net::socket::stream::SocketConnection* socketConnection,
                                         web::http::server::Request& req,
                                         web::http::server::Response& res) {
        std::string subProtocolName = req.header("sec-websocket-protocol");

        SocketContext* context = nullptr;

        SubProtocolFactory* subProtocolFactory =
            dynamic_cast<SubProtocolFactory*>(SubProtocolFactorySelector::instance()->select(subProtocolName));

        if (subProtocolFactory != nullptr) {
            if (subProtocolFactory->role() == SubProtocolFactory::Role::SERVER) {
                SubProtocol* subProtocol = dynamic_cast<SubProtocol*>(subProtocolFactory->create());

                if (subProtocol != nullptr) {
                    subProtocol->setClients(subProtocolFactory->getClients());

                    context = new SocketContext(socketConnection, subProtocol);

                    if (context != nullptr) {
                        res.set("Upgrade", "websocket");
                        res.set("Connection", "Upgrade");
                        res.set("Sec-WebSocket-Protocol", subProtocolName);

                        web::websocket::serverWebSocketKey(req.header("sec-websocket-key"), [&res](char* key) -> void {
                            res.set("Sec-WebSocket-Accept", key);
                        });

                        res.status(101).end(); // Switch Protocol
                    } else {
                        subProtocolFactory->destroy(subProtocol);
                        res.status(500).end(); // Internal Server Error
                    }
                } else {
                    res.status(404).end(); // Not Found
                }
            } else {
                res.status(500).end(); // Internal Server Error
            }
        } else {
            res.status(404).end(); // Not Found
        }

        return context;
    }

} // namespace web::websocket::server
