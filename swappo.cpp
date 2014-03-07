#include <iostream>
#include <gettext-po.h>
#include <QDebug>
using namespace std;

static void gettext_xerror(int severity,
                           po_message_t /*message*/,
                           const char *filename, size_t /*lineno*/, size_t /*column*/,
                           int /*multiline_p*/, const char *message_text)
{
    cerr << severity << " " << filename << " " << message_text;
}

static void gettext_xerror2(int severity,
                            po_message_t /*message1*/,
                            const char *filename1, size_t /*lineno1*/, size_t /*column1*/,
                            int /*multiline_p1*/, const char *message_text1,
                            po_message_t /*message2*/,
                            const char *filename2, size_t /*lineno2*/, size_t /*column2*/,
                            int /*multiline_p2*/, const char *message_text2)
{
    cerr << severity << " " << filename1 << " " << message_text1 << " " << filename2 << " " << message_text2;
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

int main(int argc, char **argv)
{
    if ( argc != 2 ) {
        qWarning( "usage: %s pofile", argv[0] );
        return -1;
    }

    const po_xerror_handler handler = { gettext_xerror, gettext_xerror2 };
    const po_xerror_handler handler_null = { gettext_xerror_null, gettext_xerror2_null };

    po_file_t po = po_file_read(argv[1], &handler);
    if (!po) {
        return -1;
    }
    po_message_iterator_t it = po_message_iterator(po, NULL);
    if (!it) {
        po_file_free(po);
        return -1;
    }
    po_message_t msg = po_next_message(it);
    if (!msg) {
        po_message_iterator_free(it);
        po_file_free(po);
        return -1;
    }

    po_file_t out = po_file_create();
    if (!out) {
        po_message_iterator_free(it);
        po_file_free(po);
        return -1;
    }
    po_message_iterator_t out_it = po_message_iterator(out, NULL);
    if (!out_it) {
        po_message_iterator_free(it);
        po_file_free(po);
        po_file_free(out);
        return -1;
    }

    do {
        if (po_message_is_obsolete(msg)) {
            continue;
        }

        po_message_t new_msg = po_message_create();
        if (!new_msg) {
            po_message_iterator_free(it);
            po_file_free(po);
            po_message_iterator_free(out_it);
            po_file_free(out);
            return -1;
        }

        po_message_set_msgid(new_msg, po_message_msgstr(msg));
        po_message_set_msgstr(new_msg, po_message_msgid(msg));

        po_message_insert(out_it, new_msg);
    } while ((msg = po_next_message(it)));

    po_message_iterator_free(it);
    po_file_free(po);

    po_message_iterator_free(out_it);
    po_file_write(out, "/dev/stdout", &handler_null);
    po_file_free(out);

    return 0;
}

