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

#ifndef NET_SOCKET_STREAM_SOCKETCLIENT_H
#define NET_SOCKET_STREAM_SOCKETCLIENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <any>
#include <cerrno>
#include <cstddef>
#include <functional>
#include <map>
#include <memory>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::socket::stream {

    template <typename SocketContextFactoryT, typename SocketConnectorT>
    class SocketClient {
        SocketClient() = delete;

    public:
        using SocketContextFactory = SocketContextFactoryT;
        using SocketConnector = SocketConnectorT;
        using SocketConnection = typename SocketConnector::SocketConnection;
        using SocketAddress = typename SocketConnection::Socket::SocketAddress;

        SocketClient(const std::function<void(const SocketAddress&, const SocketAddress&)>& onConnect,
                     const std::function<void(SocketConnection*)>& onConnected,
                     const std::function<void(SocketConnection*)>& onDisconnect,
                     const std::map<std::string, std::any>& options = {{}})
            : socketContextFactory(std::make_shared<SocketContextFactory>())
            , _onConnect(onConnect)
            , _onConnected(onConnected)
            , _onDisconnect(onDisconnect)
            , options(options) {
        }

        virtual ~SocketClient() = default;

        void connect(const SocketAddress& remoteAddress, const SocketAddress& bindAddress, const std::function<void(int)>& onError) const {
            SocketConnector* socketConnector = new SocketConnector(socketContextFactory, _onConnect, _onConnected, _onDisconnect, options);

            socketConnector->connect(remoteAddress, bindAddress, onError);
        }

        void connect(const SocketAddress& remoteAddress, const std::function<void(int)>& onError) const {
            connect(remoteAddress, SocketAddress(), onError);
        }

        void onConnect(const std::function<void(const SocketAddress&, const SocketAddress&)>& onConnect) {
            _onConnect = onConnect;
        }

        void onConnected(const std::function<void(SocketConnection*)>& onConnected) {
            _onConnected = onConnected;
        }

        void onDisconnect(const std::function<void(SocketConnection*)>& onDisconnect) {
            _onDisconnect = onDisconnect;
        }

        std::shared_ptr<SocketContextFactory> getSocketContextFactory() {
            return socketContextFactory;
        }

    protected:
        std::shared_ptr<SocketContextFactory> socketContextFactory;

    private:
        std::function<void(const SocketAddress&, const SocketAddress&)> _onConnect;
        std::function<void(SocketConnection*)> _onConnected;
        std::function<void(SocketConnection*)> _onDisconnect;

        std::map<std::string, std::any> options;
    };

} // namespace net::socket::stream

#endif // NET_SOCKET_STREAM_SOCKETCLIENT_H
