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

#ifndef NET_SOCKET_SOCKETADDRESS_H
#define NET_SOCKET_SOCKETADDRESS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstring>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::socket {

    template <typename SockAddrT>
    class SocketAddress {
    public:
        using SockAddr = SockAddrT;

        SocketAddress()
            : sockAddr{} {
        }

        SocketAddress(const SocketAddress& socketAddress) {
            *this = socketAddress;
        }

        explicit SocketAddress(const SockAddr& sockAddr) {
            *this = sockAddr;
        }

        SocketAddress& operator=(const SocketAddress& socketAddress) {
            if (this != &socketAddress) {
                *this = socketAddress.sockAddr;
            }

            return *this;
        }

        SocketAddress& operator=(const SockAddr& sockAddr) {
            memcpy(&this->sockAddr, &sockAddr, sizeof(SockAddr));

            return *this;
        }

        const sockaddr& getSockAddr() const {
            return reinterpret_cast<const sockaddr&>(sockAddr);
        }

        socklen_t getSockAddrLen() const {
            return sizeof(SockAddr);
        }

        virtual std::string toString() const = 0;

    protected:
        SockAddr sockAddr;
    };

} // namespace net::socket

#endif // NET_SOCKET_SOCKETADDRESS_H
