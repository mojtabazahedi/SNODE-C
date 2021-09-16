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

#include "net/Descriptor.h"

#include "net/system/unistd.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net {

    Descriptor::Descriptor(int fd, enum Descriptor::FLAGS flags)
        : flags(flags)
        , fd(fd) {
    }

    Descriptor::~Descriptor() {
        if (!dontClose()) {
            net::system::close(fd);
        }
    }

    void Descriptor::attach(int fd, enum Descriptor::FLAGS flags) {
        this->fd = fd;
        this->flags = flags;
    }

    int Descriptor::getFd() const {
        return fd;
    }

    void Descriptor::dontClose(bool dontClose) {
        this->flags = dontClose ? FLAGS::dontClose : FLAGS::none;
    }

    bool Descriptor::dontClose() const {
        return (flags & FLAGS::dontClose) == FLAGS::dontClose;
    }

    enum Descriptor::FLAGS operator|(const enum Descriptor::FLAGS& f1, const enum Descriptor::FLAGS& f2) {
        return static_cast<enum Descriptor::FLAGS>(static_cast<unsigned short>(f1) | static_cast<unsigned short>(f2));
    }

    enum Descriptor::FLAGS operator&(const enum Descriptor::FLAGS& f1, const enum Descriptor::FLAGS& f2) {
        return static_cast<enum Descriptor::FLAGS>(static_cast<unsigned short>(f1) & static_cast<unsigned short>(f2));
    }

} // namespace net
