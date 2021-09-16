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

#ifndef WEB_WS_SERVER_SOCKTECONTEXT_H
#define WEB_WS_SERVER_SOCKTECONTEXT_H

#include "web/websocket/SocketContext.h"

namespace net::socket::stream {
    class SocketConnection;
} // namespace net::socket::stream

namespace web::http::server {
    class Request;
    class Response;
} // namespace web::http::server

namespace web::websocket {
    class SubProtocol;
} // namespace web::websocket

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::websocket::server {

    class SocketContext : public web::websocket::SocketContext {
    protected:
        SocketContext(net::socket::stream::SocketConnection* socketConnection, web::websocket::SubProtocol* subProtocol);

        ~SocketContext() override;

    public:
        static SocketContext*
        create(net::socket::stream::SocketConnection* socketConnection, web::http::server::Request& req, web::http::server::Response& res);
    };

} // namespace web::websocket::server

#endif // WEB_WS_SERVER_SOCKTECONTEXT_H
