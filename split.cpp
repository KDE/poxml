#include "parser.h"
#include "gettextpoutils.h"
#include <stdlib.h>
#include <iostream>

#include <QList>

using namespace std;

static bool insertSimpleMessage(po_message_iterator_t it, const char *msgid,
                                const char *msgstr)
{
    po_message_t msg = po_message_create();
    if (!msg) {
        return false;
    }
    po_message_set_msgid(msg, msgid);
    po_message_set_msgstr(msg, msgstr);
    po_message_insert(it, msg);
    return true;
}

int main( int argc, char **argv )
{
    bool report_mismatches = qstrcmp(getenv("REPORT_MISMATCHES"), "no");

    if (argc != 3) {
        qWarning("usage: %s english-XML translated-XML", argv[0]);
        exit(1);
    }

    MsgList english = parseXML(argv[1]);
    MsgList translated = parseXML(argv[2]);

    QMap<QString, int>::ConstIterator eit2 = english.pc.anchors.constBegin();

    QMap<int, QString> errors;

    while (eit2 != english.pc.anchors.constEnd())
    {
        if (eit2.value() == translated.pc.anchors[eit2.key()]) {
            QString key = eit2.key();
            eit2++;
            translated.pc.anchors.remove(key);
            english.pc.anchors.remove(key);
        } else {
            errors[eit2.value()] = eit2.key();
            eit2++;
        }
    }

    if (report_mismatches && errors.count()) {
        for (QMap<int, QString>::ConstIterator it = errors.constBegin(); it != errors.constEnd(); ++it)
        {
            if (translated.pc.anchors.contains(it.value()))
                fprintf(stderr, "id=\"%s\" not in the same paragraphs (%d vs %d)\n", qPrintable(it.value()),
                        english.pc.anchors[it.value()], translated.pc.anchors[it.value()]);
            else {
                fprintf(stderr, "id=\"%s\" not in the translated paragraphs (it is in paragraph %d in english)\n",
                        qPrintable(it.value()), english.pc.anchors[it.value()]);
            }
        }
        ::exit(1);
    }

    MsgList::ConstIterator tit = translated.constBegin();
    for (MsgList::Iterator it = english.begin();
         it != english.end() && tit != translated.constEnd();
         ++tit, ++it)
    {
        (*it).msgstr = (*tit).msgid;
    }

    bool have_roles_of_translators = false;
    bool have_credit_for_translators = false;

    QMap<QString, int> msgids;
    int index = 0;

    for (MsgList::Iterator it = english.begin();
         it != english.end(); )
    {
	if ((*it).msgid == "ROLES_OF_TRANSLATORS") {
            if ((*it).msgstr.length() && !(*it).msgstr.contains("ROLES_OF_TRANSLATORS")) {
	        have_roles_of_translators = true;
            }
            else {
	        it = english.erase(it);
            }
            continue;
	}

        if ((*it).msgid == "CREDIT_FOR_TRANSLATORS") {
            if ((*it).msgstr.length() && !(*it).msgstr.contains("CREDIT_FOR_TRANSLATORS")) {
	        have_credit_for_translators = true;
            }
            else {
	        it = english.erase(it);
            }
            continue;
	}

        if (msgids.contains((*it).msgid)) {
            english[msgids[(*it).msgid]].lines += (*it).lines;
            if (english[msgids[(*it).msgid]].msgstr != (*it).msgstr) {
                fprintf(stderr, "two different translations for \"%s\" (\"%s\" and \"%s\") - choosing first one\n",
                        qPrintable((*it).msgid),
                        english[msgids[(*it).msgid]].msgstr.toLocal8Bit().data(),
                        (*it).msgstr.toLocal8Bit().data());

            }
            it = english.erase(it);
        } else {
            msgids.insert((*it).msgid, index);
            index++;
            it++;
        }
    }

    int counter = 1;

    while (tit != translated.constEnd())
    {
        MsgBlock mb;
        mb.msgid = QString::fromLatin1("appended paragraph %1").arg(counter++);
        mb.msgstr = (*tit).msgid;
        mb.lines += (*tit).lines;
        english.append(mb);
        tit++;
    }

    po_file_t po = NULL;
    po_message_iterator_t out_it = NULL;

    const struct poheader headers[] = {
        { "PO-Revision-Date", "YEAR-MO-DA HO:MI+ZONE" },
        { "Last-Translator", "FULL NAME <EMAIL@ADDRESS>" },
        { "Content-Type", "text/plain; charset=utf-8" },
        { 0, 0 }
    };
    if (!createPOWithHeader(headers, NULL, &po, &out_it)) {
        return 1;
    }

    for (MsgList::ConstIterator it = english.constBegin();
         it != english.constEnd(); ++it)
    {
        po_message_t msg = po_message_create();
        if (!msg) {
            po_message_iterator_free(out_it);
            po_file_free(po);
            return 1;
        }

        for (QList<BlockInfo>::ConstIterator it2 =
                 (*it).lines.begin(); it2 != (*it).lines.end(); it2++) {
            po_message_add_filepos(msg, "index.docbook", (*it2).start_line);
        }
        po_message_set_msgid(msg, StructureParser::descapeLiterals((*it).msgid).toUtf8().constData());
        po_message_set_msgstr(msg, StructureParser::descapeLiterals((*it).msgstr).toUtf8().constData());

        po_message_insert(out_it, msg);
    }

    if ( !getenv( "NO_CREDITS" ) ) {

        if ( !have_roles_of_translators ) {
            if (!insertSimpleMessage(out_it, "ROLES_OF_TRANSLATORS",
                                     "<!--TRANS:ROLES_OF_TRANSLATORS-->")) {
                po_message_iterator_free(out_it);
                po_file_free(po);
                return 1;
            }
        }

	if ( !have_credit_for_translators) {
            if (!insertSimpleMessage(out_it, "CREDIT_FOR_TRANSLATORS",
                                     "<!--TRANS:CREDIT_FOR_TRANSLATORS-->")) {
                po_message_iterator_free(out_it);
                po_file_free(po);
                return 1;
            }
        }
    }

    po_message_iterator_free(out_it);
    po_file_write(po, "/dev/stdout", &po_msg_handler_cerr);
    po_file_free(po);

    return 0;
}
