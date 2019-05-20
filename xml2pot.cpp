// #define POXML_DEBUG

#include "parser.h"
#include "gettextpoutils.h"
#include <stdlib.h>
#include <iostream>
#include <qfileinfo.h>
#include <qdatetime.h>

#include <QList>

using namespace std;

int main( int argc, char **argv )
{
    if (argc != 2) {
        qWarning("usage: %s english-XML", argv[0]);
        exit(1);
    }

    MsgList english = parseXML(argv[1]);

    QMap<QString, int> msgids;
    int index = 0;

    for (MsgList::Iterator it = english.begin();
         it != english.end(); )
    {
        if (msgids.contains((*it).msgid)) {
            english[msgids[(*it).msgid]].lines += (*it).lines;
            it = english.erase(it);
        } else {
            msgids.insert((*it).msgid, index);
            index++;
            it++;
        }
    }

    const QDateTime now = QDateTime::currentDateTime().toUTC();
    const QByteArray datestring = now.toString("yyyy-MM-dd hh:mm").toUtf8() + "+0000";

    po_file_t po = NULL;
    po_message_iterator_t out_it = NULL;

    const struct poheader headers[] = {
        { "Project-Id-Version", "PACKAGE VERSION" },
        { "Report-Msgid-Bugs-To", "https://bugs.kde.org" },
        { "POT-Creation-Date", datestring.constData() },
        { "PO-Revision-Date", "YEAR-MO-DA HO:MI+ZONE" },
        { "Last-Translator", "FULL NAME <EMAIL@ADDRESS>" },
        { "Language-Team", "LANGUAGE <kde-i18n-doc@kde.org>" },
        { "MIME-Version", "1.0" },
        { "Content-Type", "text/plain; charset=UTF-8" },
        { "Content-Transfer-Encoding", "8bit" },
        { 0, 0 }
    };
    const char headercomment[] = "SOME DESCRIPTIVE TITLE.\n"
                                  "FIRST AUTHOR <EMAIL@ADDRESS>, YEAR.\n"
                                  "\n";
#ifdef POXML_DEBUG
    qDebug("creating PO header");
#endif
    if (!createPOWithHeader(headers, headercomment, &po, &out_it)) {
        return 1;
    }

    const QByteArray fname = QFileInfo(QFile::decodeName(argv[1])).fileName().toUtf8();

    foreach (const MsgBlock &block, english)
    {
#ifdef POXML_DEBUG
        qDebug("adding message: %s", qPrintable(block.msgid));
#endif
        po_message_t msg = po_message_create();
        if (!msg) {
            po_message_iterator_free(out_it);
            po_file_free(po);
            return 1;
        }

        const QByteArray tagstring = "Tag: " + block.tag.toUtf8();
        po_message_set_extracted_comments(msg, tagstring.constData());
        foreach (const BlockInfo &bi, block.lines) {
            po_message_add_filepos(msg, fname.constData(), bi.start_line);
        }
        po_message_set_format(msg, "c-format", 0);
        po_message_set_msgid(msg, StructureParser::descapeLiterals(block.msgid).toUtf8().constData());
        po_message_set_msgstr(msg, block.msgstr.toUtf8().constData());

        po_message_insert(out_it, msg);
    }

    po_message_iterator_free(out_it);
#ifdef POXML_DEBUG
    qDebug("outputting PO file");
#endif
    po_file_write(po, "/dev/stdout", &po_msg_handler_cerr);
    po_file_free(po);

    return 0;
}
