/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
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

#include "message/MessageBus.h"
#include "util/debug.h"

MessageBus::MessageBus()
{

}

void MessageBus::send(Message message)
{
    if (mMailboxes.has(message.receiver))
        mMailboxes.get(message.receiver)->put(message);
    else
        DEBUG(message << " can't be sent.\n");
}

void MessageBus::addMailbox(Mailbox& mailbox)
{
    Id id = mMailboxes.add(&mailbox);
    mailbox.setId(id);
}

void MessageBus::removeMailbox(Mailbox& mailbox)
{
    mMailboxes.erase(mailbox.getId());
    mailbox.setId(UNDEFINED);
}
