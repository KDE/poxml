/*
 * Copyright 2014 Pino Toscano <pino@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef GETTEXTPOUTILS_H
#define GETTEXTPOUTILS_H

#include <gettext-po.h>

struct poheader {
    const char *name;
    const char *value;
};

typedef bool (*poiteratefn)(po_message_t msg, void *data);

extern po_xerror_handler po_msg_handler_cerr;
extern po_xerror_handler po_msg_handler_null;

bool createPOWithHeader(const struct poheader *headers, const char *comments,
                        po_file_t *out_po, po_message_iterator_t *out_it);
bool readAndIteratePO(const char *file, const po_xerror_handler_t handler,
                      bool skipheader, poiteratefn fn, void *data);

#endif
