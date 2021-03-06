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

#ifndef NET_WRITEEVENTRECEIVER_H
#define NET_WRITEEVENTRECEIVER_H

#include "net/DescriptorEventReceiver.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#define MAX_WRITE_INACTIVITY 60

namespace net {

    class WriteEventReceiver : public DescriptorEventReceiver {
    protected:
        WriteEventReceiver(long timeout = MAX_WRITE_INACTIVITY);

    private:
        virtual void writeEvent() = 0;
        void dispatchEvent() override;
    };

} // namespace net

#endif // NET_WRITEEVENTRECEIVER_H
