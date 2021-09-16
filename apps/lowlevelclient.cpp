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

#include "config.h"                                 // for CLIENTCERTF
#include "log/Logger.h"                             // for Writer, Storage
#include "net/SNodeC.h"                             // for SNodeC
#include "net/socket/ip/address/ipv4/InetAddress.h" // for InetAddress, ip
#include "net/socket/ip/tcp/ipv4/Socket.h"          // for Socket
#include "net/socket/stream/SocketClient.h"         // for SocketClient<>:...
#include "net/socket/stream/SocketContext.h"        // for SocketProtocol
#include "net/socket/stream/SocketContextFactory.h" // for SocketProtocolF...
#include "net/socket/stream/legacy/SocketClient.h"  // for SocketClient
#include "net/socket/stream/tls/SocketClient.h"     // for SocketClient
#include "web/http/client/ResponseParser.h"         // for ResponseParser

namespace net::socket::stream {
    class SocketConnection;
} // namespace net::socket::stream

#include <any> // for any
#include <cstring>
#include <functional>         // for function
#include <map>                // for map, operator==
#include <openssl/asn1.h>     // for ASN1_STRING_get...
#include <openssl/crypto.h>   // for OPENSSL_free
#include <openssl/obj_mac.h>  // for NID_subject_alt...
#include <openssl/ossl_typ.h> // for X509
#include <openssl/ssl3.h>     // for SSL_free, SSL_new
#include <openssl/x509.h>     // for X509_NAME_oneline
#include <openssl/x509v3.h>   // for GENERAL_NAME
#include <ostream>            // for size_t, endl
#include <stdint.h>           // for int32_t
#include <string>             // for allocator, string
#include <type_traits>        // for add_const<>::type
#include <utility>            // for tuple_element<>...

namespace web::http {
    class CookieOptions;
}
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

using namespace net::socket::ip;
using namespace net::socket::ip::address::ipv4;
using namespace net::socket::stream;

static web::http::client::ResponseParser* getResponseParser(SocketContext* socketContext) {
    web::http::client::ResponseParser* responseParser = new web::http::client::ResponseParser(
        socketContext,
        [](void) -> void {
        },
        [](const std::string& httpVersion, const std::string& statusCode, const std::string& reason) -> void {
            VLOG(0) << "++ Response: " << httpVersion << " " << statusCode << " " << reason;
        },
        [](const std::map<std::string, std::string>& headers, const std::map<std::string, web::http::CookieOptions>& cookies) -> void {
            VLOG(0) << "++   Headers:";
            for (const auto& [field, value] : headers) {
                VLOG(0) << "++       " << field + " = " + value;
            }

            VLOG(0) << "++   Cookies:";
            for (const auto& [name, cookie] : cookies) {
                VLOG(0) << "++     " + name + " = " + cookie.getValue();
                for (const auto& [option, value] : cookie.getOptions()) {
                    VLOG(0) << "++       " + option + " = " + value;
                }
            }
        },
        [](char* content, std::size_t contentLength) -> void {
            char* strContent = new char[contentLength + 1];
            memcpy(strContent, content, contentLength);
            strContent[contentLength] = 0;
            VLOG(0) << "++   OnContent: " << contentLength << std::endl << strContent;
            delete[] strContent;
        },
        [](web::http::client::ResponseParser& parser) -> void {
            VLOG(0) << "++   OnParsed";
            parser.reset();
        },
        [](int status, const std::string& reason) -> void {
            VLOG(0) << "++   OnError: " + std::to_string(status) + " - " + reason;
        });

    return responseParser;
}

class SimpleSocketProtocol : public SocketContext {
public:
    explicit SimpleSocketProtocol(net::socket::stream::SocketConnection* socketConnection)
        : SocketContext(socketConnection) {
        responseParser = getResponseParser(this);
    }

    ~SimpleSocketProtocol() override {
        delete responseParser;
    }

    void onReceiveFromPeer() override {
        responseParser->parse();
    }

    void onWriteError(int errnum) override {
        VLOG(0) << "OnWriteError: " << errnum;
    }

    void onReadError(int errnum) override {
        VLOG(0) << "OnReadError: " << errnum;
    }

private:
    web::http::client::ResponseParser* responseParser;
};

class SimpleSocketProtocolFactory : public net::socket::stream::SocketContextFactory {
private:
    SocketContext* create(net::socket::stream::SocketConnection* socketConnection) const override {
        return new SimpleSocketProtocol(socketConnection);
    }
};

tls::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket> getTlsClient() {
    tls::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket> tlsClient(
        [](const tls::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket>::SocketAddress& localAddress,
           const tls::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket>::SocketAddress& remoteAddress) -> void { // OnConnect
            VLOG(0) << "OnConnect";

            VLOG(0) << "\tServer: " + remoteAddress.toString();
            VLOG(0) << "\tClient: " + localAddress.toString();
        },
        [](tls::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket>::SocketConnection* socketConnection) -> void { // onConnected
            VLOG(0) << "OnConnected";

            socketConnection->sendToPeer("GET /index.html HTTP/1.1\r\nConnection: close\r\n\r\n"); // Connection: close\r\n\r\n");

            X509* server_cert = SSL_get_peer_certificate(socketConnection->getSSL());
            if (server_cert != NULL) {
                long verifyErr = SSL_get_verify_result(socketConnection->getSSL());

                VLOG(0) << "\tServer certificate: " + std::string(X509_verify_cert_error_string(verifyErr));

                char* str = X509_NAME_oneline(X509_get_subject_name(server_cert), 0, 0);
                VLOG(0) << "\t   Subject: " + std::string(str);
                OPENSSL_free(str);

                str = X509_NAME_oneline(X509_get_issuer_name(server_cert), 0, 0);
                VLOG(0) << "\t   Issuer: " + std::string(str);
                OPENSSL_free(str);

                // We could do all sorts of certificate verification stuff here before deallocating the certificate.

                GENERAL_NAMES* subjectAltNames =
                    static_cast<GENERAL_NAMES*>(X509_get_ext_d2i(server_cert, NID_subject_alt_name, NULL, NULL));

                int32_t altNameCount = sk_GENERAL_NAME_num(subjectAltNames);
                VLOG(0) << "\t   Subject alternative name count: " << altNameCount;
                for (int32_t i = 0; i < altNameCount; ++i) {
                    GENERAL_NAME* generalName = sk_GENERAL_NAME_value(subjectAltNames, i);
                    if (generalName->type == GEN_URI) {
                        std::string subjectAltName =
                            std::string(reinterpret_cast<const char*>(ASN1_STRING_get0_data(generalName->d.uniformResourceIdentifier)),
                                        static_cast<std::size_t>(ASN1_STRING_length(generalName->d.uniformResourceIdentifier)));
                        VLOG(0) << "\t      SAN (URI): '" + subjectAltName;
                    } else if (generalName->type == GEN_DNS) {
                        std::string subjectAltName =
                            std::string(reinterpret_cast<const char*>(ASN1_STRING_get0_data(generalName->d.dNSName)),
                                        static_cast<std::size_t>(ASN1_STRING_length(generalName->d.dNSName)));
                        VLOG(0) << "\t      SAN (DNS): '" + subjectAltName;
                    } else {
                        VLOG(0) << "\t      SAN (Type): '" + std::to_string(generalName->type);
                    }
                    //                    sk_GENERAL_NAME_free(generalName);
                }
                sk_GENERAL_NAME_pop_free(subjectAltNames, GENERAL_NAME_free);

                X509_free(server_cert);
            } else {
                VLOG(0) << "\tServer certificate: no certificate";
            }
        },
        [](tls::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket>::SocketConnection* socketConnection) -> void { // onDisconnect
            VLOG(0) << "OnDisSimpleSocketProtocolFactory, connect";

            VLOG(0) << "\tServer: " + socketConnection->getRemoteAddress().toString();
            VLOG(0) << "\tClient: " + socketConnection->getLocalAddress().toString();

        },
        {{"certChain", CLIENTCERTF}, {"keyPEM", CLIENTKEYF}, {"password", KEYFPASS}, {"caFile", SERVERCAFILE}});

    InetAddress remoteAddress("localhost", 8088);

    tlsClient.connect(remoteAddress, [](int err) -> void {
        if (err) {
            PLOG(ERROR) << "Connect: " + std::to_string(err);
        } else {
            VLOG(0) << "Connected";
        }
    });

    return tlsClient;
}

legacy::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket> getLegacyClient() {
    legacy::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket> legacyClient(
        [](const legacy::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket>::SocketAddress& localAddress,
           const legacy::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket>::SocketAddress& remoteAddress) -> void { // OnConnect
            VLOG(0) << "OnConnect";

            VLOG(0) << "\tServer: " + remoteAddress.toString();
            VLOG(0) << "\tClient: " + localAddress.toString();
        },
        [](legacy::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket>::SocketConnection* socketConnection)
            -> void { // onConnected
            VLOG(0) << "OnConnected";

            socketConnection->getSocketContext()->sendToPeer(
                "GET /index.html HTTP/1.1\r\nConnection: close\r\n\r\n"); // Connection: close\r\n\r\n");
        },
        [](legacy::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket>::SocketConnection* socketConnection)
            -> void { // onDisconnect
            VLOG(0) << "OnDisconnect";

            VLOG(0) << "\tServer: " + socketConnection->getRemoteAddress().toString();
            VLOG(0) << "\tClient: " + socketConnection->getLocalAddress().toString();

        },
        {{}});

    InetAddress remoteAddress("localhost", 8080);

    legacyClient.connect(remoteAddress, [](int err) -> void {
        if (err) {
            PLOG(ERROR) << "Connect: " << std::to_string(err);
        } else {
            VLOG(0) << "Connected";
        }
    });

    return legacyClient;
}

int main(int argc, char* argv[]) {
    net::SNodeC::init(argc, argv);

    {
        InetAddress remoteAddress("localhost", 8080);

        legacy::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket> legacyClient = getLegacyClient();

        legacyClient.connect(remoteAddress, [](int err) -> void { // example.com:81 simulate connnect timeout
            if (err) {
                PLOG(ERROR) << "Connect: " << std::to_string(err);
            } else {
                VLOG(0) << "Connected";
            }
        });

        remoteAddress = InetAddress("localhost", 8088);

        tls::SocketClient<SimpleSocketProtocolFactory, tcp::ipv4::Socket> tlsClient = getTlsClient();

        tlsClient.connect(remoteAddress, [](int err) -> void {
            if (err) {
                PLOG(ERROR) << "Connect: " << std::to_string(err);
            } else {
                VLOG(0) << "Connected";
            }
        });
    }

    return net::SNodeC::start();
}
