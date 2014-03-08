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

// #define POXML_DEBUG

#include "gettextpoutils.h"

#include <iostream>

static void gettext_xerror(int severity,
                           po_message_t /*message*/,
                           const char *filename, size_t /*lineno*/, size_t /*column*/,
                           int /*multiline_p*/, const char *message_text)
{
    std::cerr << severity << " " << filename << " " << message_text << std::endl;
}

static void gettext_xerror2(int severity,
                            po_message_t /*message1*/,
                            const char *filename1, size_t /*lineno1*/, size_t /*column1*/,
                            int /*multiline_p1*/, const char *message_text1,
                            po_message_t /*message2*/,
                            const char *filename2, size_t /*lineno2*/, size_t /*column2*/,
                            int /*multiline_p2*/, const char *message_text2)
{
    std::cerr << severity << " " << filename1 << " " << message_text1 << ", "
              << filename2 << " " << message_text2 << std::endl;
}

static void gettext_xerror_null(int,
                                po_message_t, const char *, size_t, size_t,
                                int, const char *)
{
}

static void gettext_xerror2_null(int,
                                 po_message_t, const char *, size_t, size_t,
                                 int, const char *,
                                 po_message_t, const char *, size_t, size_t,
                                 int, const char *)
{
}

po_xerror_handler po_msg_handler_cerr = { gettext_xerror, gettext_xerror2 };
po_xerror_handler po_msg_handler_null = { gettext_xerror_null, gettext_xerror2_null };

bool createPOWithHeader(const struct poheader *headers, const char *comments,
                        po_file_t *out_po, po_message_iterator_t *out_it)
{
#ifdef POXML_DEBUG
    std::cerr << "createPOWithHeader " << headers << ", " << comments << std::endl;
#endif
    po_file_t po = po_file_create();
    if (!po) {
        return false;
    }
    po_message_iterator_t it = po_message_iterator(po, NULL);
    if (!it) {
        po_file_free(po);
        return false;
    }

    po_message_t msg = po_message_create();
    if (!msg) {
        po_message_iterator_free(it);
        po_file_free(po);
        return false;
    }
    if (comments) {
        po_message_set_comments(msg, comments);
    }
    po_message_set_fuzzy(msg, 1);
    po_message_set_msgid(msg, "");
    po_message_set_msgstr(msg, "");
    po_message_insert(it, msg);

#ifdef POXML_DEBUG
    std::cerr << "start filling the header" << std::endl;
#endif
    const char *header = po_file_domain_header(po, NULL);
    char *newheader = (char *)header;
    for (const struct poheader *h = headers; h->name; ++h) {
#ifdef POXML_DEBUG
    std::cerr << "\theader, " << (void*)newheader << ", " << newheader << ", " << h->name << std::endl;
#endif
        char *oldheader = newheader;
        newheader = po_header_set_field(newheader, h->name, h->value);
        if (oldheader != header) {
            free(oldheader);
        }
        if (!newheader) {
            po_message_iterator_free(it);
            po_file_free(po);
            return false;
        }
    }
#ifdef POXML_DEBUG
    std::cerr << "done with the new header, setting it" << std::endl;
#endif
    po_message_set_msgstr(msg, newheader);
    free(newheader);

    *out_po = po;
    *out_it = it;

    return true;
}

bool readAndIteratePO(const char *file, const po_xerror_handler_t handler,
                      bool skipheader, poiteratefn fn, void *data)
{
    po_file_t po = po_file_read(file, handler);
    if (!po) {
        return false;
    }

    po_message_iterator_t it = po_message_iterator(po, NULL);
    po_message_t msg;
    if (skipheader) {
        msg = po_next_message(it);
        if (!msg || po_message_msgid(msg)[0] != '\0') {
            po_message_iterator_free(it);
            po_file_free(po);
            return false;
        }
    }

    while ((msg = po_next_message(it))) {
        if (!fn(msg, data)) {
            po_message_iterator_free(it);
            po_file_free(po);
            return false;
        }
    }

    po_message_iterator_free(it);
    po_file_free(po);

    return true;
}
