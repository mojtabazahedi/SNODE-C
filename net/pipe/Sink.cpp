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

#include "net/pipe/Sink.h"

#include "net/pipe/Source.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::pipe {

    Sink::Sink()
        : source(nullptr) {
    }

    Sink::~Sink() {
        if (source != nullptr) {
            source->disconnect(*this);
        }
    }

    void Sink::connect(Source& source) {
        this->source = &source;
    }

    void Sink::disconnect(Source& source) {
        if (&source == this->source) {
            this->source = nullptr;
        }
    }

} // namespace net::pipe
