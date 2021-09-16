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

#ifndef NET_SOCKET_STREAM_TLS_TLSHANDSHAKE_H
#define NET_SOCKET_STREAM_TLS_TLSHANDSHAKE_H

#include "net/ReadEventReceiver.h"
#include "net/WriteEventReceiver.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>
#include <openssl/ossl_typ.h> // for SSL

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::socket::stream::tls {

    class TLSHandshake
        : public ReadEventReceiver
        , public WriteEventReceiver {
    public:
        static void doHandshake(SSL* ssl,
                                const std::function<void(void)>& onSuccess,
                                const std::function<void(void)>& onTimeout,
                                const std::function<void(int)>& onError);

    private:
        TLSHandshake(SSL* ssl,
                     const std::function<void(void)>& onSuccess,
                     const std::function<void(void)>& onTimeout,
                     const std::function<void(int)>& onError);

        void readEvent() override;
        void writeEvent() override;
        void timeoutEvent() override;
        void unobserved() override;

        SSL* ssl = nullptr;
        std::function<void(void)> onSuccess;
        std::function<void(void)> onTimeout;
        std::function<void(int)> onError;

        int fd = -1;
    };

} // namespace net::socket::stream::tls

#endif // NET_SOCKET_STREAM_TLS_TLSHANDSHAKE_H
