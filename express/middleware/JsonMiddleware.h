/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021  Volker Christian <me@vchrist.at>
 * Json Middleware 2020, 2021 Marlene Mayr, Anna Moser, Matteo Prock, Eric Thalhammer
 * Github <MarleneMayr><moseranna><MatteoMatteoMatteo><peregrin-tuk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXPRESS_MIDDLEWARE_JSONMIDDLEWARE_H
#define EXPRESS_MIDDLEWARE_JSONMIDDLEWARE_H

#include "express/Router.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <nlohmann/json.hpp>     // IWYU pragma: export
#include <nlohmann/json_fwd.hpp> // IWYU pragma: export

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace express::middleware {

    class JsonMiddleware : public Router {
        JsonMiddleware(const JsonMiddleware&) = delete;
        JsonMiddleware& operator=(const JsonMiddleware&) = delete;

    protected:
        JsonMiddleware();

        static const class JsonMiddleware& instance();
        friend const class JsonMiddleware& JsonMiddleware();
    };

    const class JsonMiddleware& JsonMiddleware();

} // namespace express::middleware

#endif // EXPRESS_MIDDLEWARE_JSONMIDDLEWARE_H
