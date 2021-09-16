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

#ifndef WEB_HTTP_CLIENT_SOCKETCONTEXT_H
#define WEB_HTTP_CLIENT_SOCKETCONTEXT_H

#include "net/socket/stream/SocketContext.h"
#include "web/http/client/ResponseParser.h"

namespace web::http::client {
    class Request;
} // namespace web::http::client

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef>
#include <functional>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::socket::stream {
    class SocketConnection;
} // namespace net::socket::stream

namespace web::http::client {

    class SocketContext : public net::socket::stream::SocketContext {
    public:
        explicit SocketContext(net::socket::stream::SocketConnection* socketConnection)
            : net::socket::stream::SocketContext(socketConnection) {
        }

        virtual ~SocketContext() = default;

        virtual Request& getRequest() = 0;

        virtual void sendToPeerCompleted() = 0;
        virtual void terminateConnection() = 0;
    };

    template <typename RequestT, typename ResponseT>
    class SocketContextT : public SocketContext {
    public:
        using SocketConnection = net::socket::stream::SocketConnection;
        using Request = RequestT;
        using Response = ResponseT;

        SocketContextT(net::socket::stream::SocketConnection* socketConnection,
                       const std::function<void(RequestT&, Response&)>& onResponse,
                       const std::function<void(int, const std::string&)>& onError);

        ~SocketContextT() override = default;

    private:
        void onReceiveFromPeer() override;

        void onWriteError(int errnum) override;
        void onReadError(int errnum) override;

        Request& getRequest() override;

        void terminateConnection() override;

        void sendToPeerCompleted() override;

        Request request;
        Response response;

        ResponseParser parser;
    };

} // namespace web::http::client

#endif // WEB_HTTP_CLIENT_SOCKETCONTEXT_H
