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

#include "config.h" // just for this example app
#include "express/middleware/StaticMiddleware.h"
#include "express/tls/WebApp.h"
#include "log/Logger.h"
#include "net/socket/ip/address/ipv6/InetAddress.h" // for InetAddress
#include "net/socket/stream/SocketContext.h"        // for SocketContext

#include <any>                // for any
#include <cstddef>            // for size_t, NULL
#include <openssl/asn1.h>     // for ASN1_STRING_get0...
#include <openssl/crypto.h>   // for OPENSSL_free
#include <openssl/obj_mac.h>  // for NID_subject_alt_...
#include <openssl/ossl_typ.h> // for X509
#include <openssl/ssl3.h>     // for SSL_get_peer_cer...
#include <openssl/x509.h>     // for X509_NAME_oneline
#include <openssl/x509v3.h>
#include <stdint.h> // for int32_t
#include <string>   // for string, allocator

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

using namespace express;

Router getRouter() {
    Router router;

    router.use(middleware::StaticMiddleware(SERVERROOT));

    return router;
}

int main(int argc, char* argv[]) {
    Logger::setVerboseLevel(2);

    WebApp::init(argc, argv);

    {
        tls::WebApp6 tlsApp(getRouter(),
                            {{"certChain", SERVERCERTF}, {"keyPEM", SERVERKEYF}, {"password", KEYFPASS}, {"caFile", CLIENTCAFILE}});

        tlsApp.onConnect([](const tls::WebApp6::SocketAddress& localAddress, const tls::WebApp6::SocketAddress& remoteAddress) -> void {
            VLOG(0) << "OnConnect:";

            VLOG(0) << "\tServer: " + localAddress.toString();
            VLOG(0) << "\tClient: " + remoteAddress.toString();
        });

        tlsApp.onConnected([](tls::WebApp6::SocketConnection* socketConnection) -> void {
            VLOG(0) << "OnConnected:";

            X509* client_cert = SSL_get_peer_certificate(socketConnection->getSSL());

            if (client_cert != NULL) {
                long verifyErr = SSL_get_verify_result(socketConnection->getSSL());

                VLOG(2) << "\tClient certificate: " + std::string(X509_verify_cert_error_string(verifyErr));

                char* str = X509_NAME_oneline(X509_get_subject_name(client_cert), 0, 0);
                VLOG(2) << "\t   Subject: " + std::string(str);
                OPENSSL_free(str);

                str = X509_NAME_oneline(X509_get_issuer_name(client_cert), 0, 0);
                VLOG(2) << "\t   Issuer: " + std::string(str);
                OPENSSL_free(str);

                // We could do all sorts of certificate verification stuff here before deallocating the certificate.

                GENERAL_NAMES* subjectAltNames =
                    static_cast<GENERAL_NAMES*>(X509_get_ext_d2i(client_cert, NID_subject_alt_name, nullptr, nullptr));

                int32_t altNameCount = sk_GENERAL_NAME_num(subjectAltNames);
                VLOG(2) << "\t   Subject alternative name count: " << altNameCount;
                for (int32_t i = 0; i < altNameCount; ++i) {
                    GENERAL_NAME* generalName = sk_GENERAL_NAME_value(subjectAltNames, i);
                    if (generalName->type == GEN_URI) {
                        std::string subjectAltName =
                            std::string(reinterpret_cast<const char*>(ASN1_STRING_get0_data(generalName->d.uniformResourceIdentifier)),
                                        static_cast<std::size_t>(ASN1_STRING_length(generalName->d.uniformResourceIdentifier)));
                        VLOG(2) << "\t      SAN (URI): '" + subjectAltName;
                    } else if (generalName->type == GEN_DNS) {
                        std::string subjectAltName =
                            std::string(reinterpret_cast<const char*>(ASN1_STRING_get0_data(generalName->d.dNSName)),
                                        static_cast<std::size_t>(ASN1_STRING_length(generalName->d.dNSName)));
                        VLOG(2) << "\t      SAN (DNS): '" + subjectAltName;
                    } else {
                        VLOG(2) << "\t      SAN (Type): '" + std::to_string(generalName->type);
                    }
                }
                sk_GENERAL_NAME_pop_free(subjectAltNames, GENERAL_NAME_free);

                X509_free(client_cert);
            } else {
                VLOG(2) << "\tClient certificate: no certificate";
                socketConnection->getSocketContext()->close();
            }
        });

        tlsApp.onDisconnect([](tls::WebApp6::SocketConnection* socketConnection) -> void {
            VLOG(0) << "OnDisconnect:";

            VLOG(0) << "\tServer: " + socketConnection->getLocalAddress().toString();
            VLOG(0) << "\tClient: " + socketConnection->getRemoteAddress().toString();
        });

        tlsApp.listen(8088, [](int err) -> void {
            if (err != 0) {
                PLOG(FATAL) << "listen on port 8088";
            } else {
                VLOG(0) << "snode.c listening on port 8088 for SSL/TLS connections";
            }
        });
    }

    return WebApp::start();
}
