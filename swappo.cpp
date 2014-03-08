#include <iostream>
#include "gettextpoutils.h"
using namespace std;

static bool swapAndInsert(po_message_t msg, void *data)
{
    if (po_message_is_obsolete(msg)) {
        return true;
    }

    po_message_t new_msg = po_message_create();
    if (!new_msg) {
        return false;
    }

    po_message_iterator_t it = (po_message_iterator_t)data;

    po_message_set_msgid(new_msg, po_message_msgstr(msg));
    po_message_set_msgstr(new_msg, po_message_msgid(msg));
    po_message_insert(it, new_msg);

    return true;
}

int main(int argc, char **argv)
{
    if ( argc != 2 ) {
        cerr << "usage: " << argv[0] << " pofile" << endl;
        return -1;
    }

    po_file_t out = po_file_create();
    if (!out) {
        return -1;
    }
    po_message_iterator_t it = po_message_iterator(out, NULL);
    if (!it) {
        po_file_free(out);
        return -1;
    }

    if (!readAndIteratePO(argv[1], &po_msg_handler_cerr,
                          false, swapAndInsert, it)) {
        po_message_iterator_free(it);
        po_file_free(out);
        return -1;
    }

    po_message_iterator_free(it);
    po_file_write(out, "/dev/stdout", &po_msg_handler_null);
    po_file_free(out);

    return 0;
}

