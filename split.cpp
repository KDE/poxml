#include "parser.h"
#include "gettextpoutils.h"
#include <stdlib.h>
#include <iostream>

#include <QList>
#include <QSet>

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

// this merges the msgid of the messages in list2 (removing them) to the
// index1-1-ith msgid of list1, so list1[index1] and list2[index2] have the
// same tag
static void mergeTranslationsUntilSameTag(MsgList &list1, int index1, MsgList &list2, int index2)
{
    const int list2Length = list2.length();

    if (index1 == list1.length() || index2 == list2Length) {
        return;
    }

    int j;
    for (j = index2; j < list2Length && list1[index1].tag != list2[j].tag; ++j) { }
    if (j == list2Length) {
        return;
    }

    // build a fake message with the data we can gather that could represent
    // the XML
    // FIXME: it seems the XML locator does not report correctly line and
    // column when there are whitespaces within the tags, otherwise we could
    // just pick the content from the end of the previous block to the start
    // of the following ones
    for (int index = index2; index < j; ++index) {
        if (!list1[index1 - 1].msgstr.isEmpty()) {
            list1[index1 - 1].msgstr += " ";
        }
        list1[index1 - 1].msgstr += QString::fromLatin1("<%1>%2</%3>").arg(list2[index].tag, list2[index].msgid, list2[index].tag);
    }
    for (int count = j - index2; count > 0; --count) {
        list2.removeAt(index2 - 1);
    }
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

    int indexTranslated = 0;
    for (int i = 0; i < english.length() && indexTranslated < translated.length(); )
    {
        english[i].msgstr = translated[indexTranslated].msgid;
        ++i;
        ++indexTranslated;

        if (english[i - 1].msgid == QLatin1String("ROLES_OF_TRANSLATORS") ||
            english[i - 1].msgid == QLatin1String("CREDIT_FOR_TRANSLATORS")) {
            mergeTranslationsUntilSameTag(english, i, translated, indexTranslated);
        }
    }

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

    bool have_roles_of_translators = false;
    bool have_credit_for_translators = false;

    QMap<QString, int> msgids;
    QSet<QString> fuzzies;
    int index = 0;

    for (MsgList::Iterator it = english.begin();
         it != english.end(); )
    {
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
            // since we may have forged them by joining multiple tags,
            // mark them as fuzzy to indicate they will need manual
            // intervention
            if ((*it).msgid == QLatin1String("ROLES_OF_TRANSLATORS")) {
                have_roles_of_translators = true;
                fuzzies << (*it).msgid;
            } else if ((*it).msgid == QLatin1String("CREDIT_FOR_TRANSLATORS")) {
                have_credit_for_translators = true;
                fuzzies << (*it).msgid;
            }

            msgids.insert((*it).msgid, index);
            index++;
            it++;
        }
    }

    int counter = 1;

    while (counter + indexTranslated < translated.length())
    {
        MsgBlock mb;
        mb.msgid = QString::fromLatin1("appended paragraph %1").arg(counter++);
        mb.msgstr = translated[counter + indexTranslated].msgid;
        mb.lines += translated[counter + indexTranslated].lines;
        english.append(mb);
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

    foreach (const MsgBlock &block, english)
    {
        po_message_t msg = po_message_create();
        if (!msg) {
            po_message_iterator_free(out_it);
            po_file_free(po);
            return 1;
        }

        foreach (const BlockInfo &bi, block.lines) {
            po_message_add_filepos(msg, "index.docbook", bi.start_line);
        }
        po_message_set_msgid(msg, StructureParser::descapeLiterals(block.msgid).toUtf8().constData());
        po_message_set_msgstr(msg, StructureParser::descapeLiterals(block.msgstr).toUtf8().constData());

        // if necessary, mark the message as dubious
        if (fuzzies.contains(block.msgid)) {
            po_message_set_fuzzy(msg, 1);
        }

        po_message_insert(out_it, msg);
    }

    if ( !getenv( "NO_CREDITS" ) ) {

        if ( !have_roles_of_translators ) {
            if (!insertSimpleMessage(out_it, "ROLES_OF_TRANSLATORS", "")) {
                po_message_iterator_free(out_it);
                po_file_free(po);
                return 1;
            }
        }

	if ( !have_credit_for_translators) {
            if (!insertSimpleMessage(out_it, "CREDIT_FOR_TRANSLATORS", "")) {
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
