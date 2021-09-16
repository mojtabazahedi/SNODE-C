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

#ifndef NET_SOCKET_STREAM_TLS_SOCKETWRITER_H
#define NET_SOCKET_STREAM_TLS_SOCKETWRITER_H

#include "log/Logger.h"
#include "net/socket/stream/SocketWriter.h"
#include "net/socket/stream/tls/TLSHandshake.h"
#include "ssl_utils.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef> // for std::size_t
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <sys/types.h> // for ssize_t

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::socket::stream::tls {

    template <typename SocketT>
    class SocketWriter : public stream::SocketWriter<SocketT> {
        using stream::SocketWriter<SocketT>::SocketWriter;

    private:
        ssize_t write(const char* junk, std::size_t junkLen) override {
            sslErr = 0;

            int ret = SSL_write(ssl, junk, static_cast<int>(junkLen));

            if (ret <= 0) {
                sslErr = SSL_get_error(ssl, ret);

                switch (sslErr) {
                    case SSL_ERROR_WANT_READ:
                        LOG(INFO) << "SSL/TLS start renegotiation on write";
                        doSSLHandshake(
                            [](void) -> void {
                                LOG(INFO) << "SSL/TLS renegotiation on write success";
                            },
                            [](void) -> void {
                                LOG(WARNING) << "SSL/TLS renegotiation on write timed out";
                            },
                            [this](int sslErr) -> void {
                                ssl_log("SSL/TLS renegotiation", sslErr);
                                setSSLError(sslErr);
                            });
                        errno = EAGAIN;
                        break;
                    case SSL_ERROR_WANT_WRITE:
                        ret = 0;
                        errno = EAGAIN;
                        break;
                    case SSL_ERROR_ZERO_RETURN:
                        ret = 0;
                        errno = 0;
                        break;
                    case SSL_ERROR_SYSCALL:
                        if (errno != 0) {
                            ssl_log("SSL/TLS write failed", sslErr);
                        }
                        break;
                    default:
                        ssl_log("SSL/TLS write failed", sslErr);
                        break;
                }
            }

            return ret;
        }

        int getError() override {
            int ret = errno;

            switch (sslErr) {
                case SSL_ERROR_NONE:
                    [[fallthrough]];
                case SSL_ERROR_ZERO_RETURN:
                    ret = 0;
                    break;
                case SSL_ERROR_SYSCALL:
                    break;
                default:
                    ret = -sslErr;
                    break;
            };

            return ret;
        }

    protected:
        virtual void doSSLHandshake(const std::function<void()>& onSuccess,
                                    const std::function<void()>& onTimeout,
                                    const std::function<void(int)>& onError) = 0;

        void setSSLError(int sslErr) {
            this->sslErr = sslErr;
        }

        SSL* ssl = nullptr;

        int sslErr = SSL_ERROR_NONE;
    };

} // namespace net::socket::stream::tls

#endif // NET_SOCKET_STREAM_TLS_SOCKETWRITER_H
