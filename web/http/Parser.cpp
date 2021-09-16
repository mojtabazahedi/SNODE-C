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

#include "Parser.h"

#include "net/socket/stream/SocketContext.h"
#include "web/http/http_utils.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cctype> // for isblank
#include <cstring>
#include <tuple> // for tie, tuple

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::http {

    // HTTP/x.x
    std::regex Parser::httpVersionRegex("^HTTP/([[:digit:]])\\.([[:digit:]])$");

    Parser::Parser(net::socket::stream::SocketContext* socketContext, const enum Parser::HTTPCompliance& compliance)
        : hTTPCompliance(compliance)
        , socketContext(socketContext) {
    }

    void Parser::reset() {
        parserState = ParserState::BEGIN;
        headers.clear();
        contentLength = 0;
        if (content != nullptr) {
            delete[] content;
            content = nullptr;
        }
    }

    void Parser::parse() {
        std::size_t consumed = 0;
        bool parsingError = false;

        do {
            switch (parserState) {
                case ParserState::BEGIN:
                    parserState = ParserState::FIRSTLINE;
                    begin();
                    [[fallthrough]];
                case ParserState::FIRSTLINE:
                    consumed = readStartLine();
                    break;
                case ParserState::HEADER:
                    consumed = readHeaderLine();
                    break;
                case ParserState::BODY:
                    consumed = readContent();
                    break;
                case ParserState::ERROR:
                    parsingError = true;
                    reset();
                    break;
            };
        } while (consumed > 0 && !parsingError && parserState != ParserState::BEGIN);
    }

    std::size_t Parser::readStartLine() {
        std::size_t consumed = 0;
        std::size_t ret = 0;

        do {
            char ch = 0;
            ret = socketContext->readFromPeer(&ch, 1);

            if (ret > 0) {
                consumed += ret;
                if (ch == '\r' || ch == '\n') {
                    if (ch == '\n') {
                        parserState = parseStartLine(line);
                        line.clear();
                    }
                } else {
                    line += ch;
                }
            }
        } while (ret > 0 && parserState == ParserState::FIRSTLINE);

        return consumed;
    }

    std::size_t Parser::readHeaderLine() {
        std::size_t consumed = 0;
        std::size_t ret = 0;

        do {
            char ch = 0;
            ret = socketContext->readFromPeer(&ch, 1);

            if (ret > 0) {
                if (ch == '\r' || ch == '\n') {
                    consumed++;
                    if (ch == '\n') {
                        if (EOL) {
                            splitHeaderLine(line);
                            line.clear();
                            if (parserState != ParserState::ERROR) {
                                parserState = parseHeader();
                            }
                            EOL = false;
                        } else {
                            if (line.empty()) {
                                if (parserState != ParserState::ERROR) {
                                    parserState = parseHeader();
                                }
                            } else {
                                EOL = true;
                            }
                        }
                    }
                } else if (EOL) {
                    if (std::isblank(ch)) {
                        if ((hTTPCompliance & HTTPCompliance::RFC7230) == HTTPCompliance::RFC7230) {
                            parserState = parsingError(400, "Header Folding");
                        } else {
                            line += ch;
                            consumed++;
                        }
                    } else {
                        splitHeaderLine(line);
                        line.clear();
                        line += ch;
                        consumed++;
                    }
                    EOL = false;
                } else {
                    line += ch;
                    consumed++;
                }
            }
        } while (ret > 0 && parserState == ParserState::HEADER);

        return consumed;
    }

    void Parser::splitHeaderLine(const std::string& line) {
        if (!line.empty()) {
            std::string field;
            std::string value;
            std::tie(field, value) = httputils::str_split(line, ':');

            if (field.empty()) {
                parserState = parsingError(400, "Header-field empty");
            } else if (std::isblank(field.back()) || std::isblank(field.front())) {
                parserState = parsingError(400, "White space before or after header-field");
            } else if (value.empty()) {
                parserState = parsingError(400, "Header-value of field \"" + field + "\" empty");
            } else {
                httputils::to_lower(field);
                httputils::str_trimm(value);

                if (headers.find(field) == headers.end()) {
                    headers.insert({field, value});
                } else {
                    headers[field] += "," + value;
                }
            }
        } else {
            parserState = parsingError(400, "Header-line empty");
        }
    }

    std::size_t Parser::readContent() {
        if (contentRead == 0) {
            content = new char[contentLength];
        }

        std::size_t contentJunkLen =
            (contentLength - contentRead < MAX_CONTENT_JUNK_LEN) ? contentLength - contentRead : MAX_CONTENT_JUNK_LEN;

        contentJunkLen = socketContext->readFromPeer(contentJunk, contentJunkLen);

        if (contentJunkLen > 0) {
            if (contentRead + contentJunkLen <= contentLength) {
                memcpy(content + contentRead, contentJunk, contentJunkLen); // NOLINT(clang-analyzer-core.NonNullParamChecker)

                contentRead += contentJunkLen;
                if (contentRead == contentLength) {
                    parserState = parseContent(content, contentLength);

                    delete[] content;
                    content = nullptr;
                    contentRead = 0;
                }
            } else {
                parserState = parsingError(400, "Content to long");

                if (content != nullptr) {
                    delete[] content;
                    content = nullptr;
                }
            }
        }

        return contentJunkLen;
    }

    enum Parser::HTTPCompliance operator|(const enum Parser::HTTPCompliance& c1, const enum Parser::HTTPCompliance& c2) {
        return static_cast<enum Parser::HTTPCompliance>(static_cast<unsigned short>(c1) | static_cast<unsigned short>(c2));
    }

    enum Parser::HTTPCompliance operator&(const enum Parser::HTTPCompliance& c1, const enum Parser::HTTPCompliance& c2) {
        return static_cast<enum Parser::HTTPCompliance>(static_cast<unsigned short>(c1) & static_cast<unsigned short>(c2));
    }

} // namespace web::http
