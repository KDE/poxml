#include "parser.h"
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

#include <fstream>
#include "GettextLexer.hpp"
#include "GettextParser.hpp"
#include "antlr/AST.hpp"
#include "antlr/CommonAST.hpp"

using namespace std;

QString translate(QString xml, QString orig, QString translation)
{
    int index = xml.find(orig);
    if (index == -1) {
        qWarning("can't find\n%s\nin\n%s", orig.latin1(), xml.latin1());
        ASSERT(false);
    }
    if (translation.isEmpty()) {
        // qWarning("no translation for %s found", orig.local8Bit().data());
        xml.replace(index, orig.length(),
                    QString::fromLatin1(""));
    } else
        xml.replace(index, orig.length(), translation);
    return xml;
}

QString escapePO(QString msgid)
{
    int index = 0;
    while (true) {
        index = msgid.find("\\n", index);
        if (index == -1)
            break;
        if (index > 1 && msgid.at(index - 1) == '\\' && msgid.at(index - 2) != '\\') {
            msgid.replace(index - 1, 3, "\\n");
            index += 3;
        } else
            msgid.replace(index, 2, "\n");
    }
    index = 0;
    while (true) {
        index = msgid.find("\\\"", index);
        if (index == -1)
            break;
        if (index > 1 && msgid.at(index - 1) == '\\' && msgid.at(index - 2) != '\\')
            msgid.replace(index - 1, 3, "\\\"");
        else
            msgid.replace(index, 2, "\"");
    }
    index = 0;
    while (true) {
        index = msgid.find("\\t", index);
        if (index == -1)
            break;
        if (msgid.at(index - 1) == '\\')
            msgid.replace(index - 1, 3, "\\t");
        else
            msgid.replace(index, 2, "\t");
    }
    while (true) {
        int index = msgid.find("\\\\");
        if (index == -1)
            break;
        msgid.replace(index, 2, "\\");
    }

    return msgid;
}

int main( int argc, char **argv )
{
    if (argc != 3) {
        qWarning("usage: %s english-XML translated-PO", argv[0]);
        ::exit(1);
    }

    MsgList english = parseXML(argv[1]);
    MsgList translated;

    try {
        ifstream s(argv[2]);
        GettextLexer lexer(s);
        GettextParser parser(lexer);
        translated = parser.file();

    } catch(exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    QMap<QString, QString> translations;
    for (MsgList::ConstIterator it = translated.begin();
         it != translated.end(); ++it)
    {
        QString msgid = escapePO((*it).msgid);
        QString msgstr = escapePO((*it).msgstr);
        translations.insert(msgid, msgstr);
    }

    QFile xml(argv[1]);
    xml.open(IO_ReadOnly);
    QTextStream ds(&xml);
    ds.setEncoding(QTextStream::UnicodeUTF8);
    QString xml_text = ds.read();
    xml.close();
    QString output;
    QTextStream ts(&output, IO_WriteOnly);
    if (xml_text.left(5) != "<?xml") {
        FILE *p = popen(QString::fromLatin1("xmlizer %1").arg(argv[1]).latin1(), "r");
        xml.open(IO_ReadOnly, p);
        char buffer[5001];
        xml_text.truncate(0);
        int len;
        while ((len = xml.readBlock(buffer, 5000)) != 0) {
            buffer[len] = 0;
            xml_text += QString::fromUtf8(buffer);
        }
        xml.close();
        pclose(p);

    }
    xml_text.replace(QRegExp("&amp;"), "|amp-internal|");
    xml_text.replace(QRegExp("&lt;"), "|lt-internal|");
    xml_text.replace(QRegExp("&gt;"), "|gt-internal|");
    xml_text.replace(QRegExp("&quot;"), "|quot-internal|");

    QValueList<int> line_offsets;
    line_offsets.append(0);
    int index = 0;
    while (true) {
        index = xml_text.find('\n', index) + 1;
        if (index <= 0)
            break;
        line_offsets.append(index);
    }

    int old_start_line = -1, old_start_col = -1;
    QString old_text;
    MsgList::Iterator old_it = english.end();

    for (MsgList::Iterator it = english.begin();
         it != english.end(); ++it)
    {
        BlockInfo bi = (*it).lines.first();
        int start_pos = line_offsets[bi.start_line - 1] + bi.start_col;
        int end_pos = line_offsets[bi.end_line - 1] + bi.end_col - 1;

        (*it).start = start_pos;
        if (old_start_line == bi.start_line &&
            old_start_col == bi.start_col)
        {
            (*old_it).end = bi.offset;
            (*it).end = end_pos;
        } else {
            (*it).lines.first().offset = 0;
            (*it).end = 0;
        }

        old_start_line = bi.start_line;
        old_start_col = bi.start_col;
        old_it = it;
    }

    int old_pos = 0;

    for (MsgList::Iterator it = english.begin();
         it != english.end(); ++it)
    {
        BlockInfo bi = (*it).lines.first();
        int start_pos = line_offsets[bi.start_line - 1] + bi.start_col;
        int end_pos = line_offsets[bi.end_line - 1] + bi.end_col - 1;

        QString xml = xml_text.mid(start_pos, end_pos - start_pos);
        StructureParser::descape(xml);
        xml.replace(QRegExp("\\s*<!--.*-->\\s*"), " ");

#ifndef NDEBUG
        qDebug("english \"%s\" \"%s\" %d(%d-%d) %d(%d-%d) %d %d \"%s\"", xml.latin1(), (*it).msgid.latin1(),
               start_pos, bi.start_line, bi.start_col,
               end_pos, bi.end_line, bi.end_col,
               (*it).lines.first().offset, (*it).end, xml.mid((*it).lines.first().offset, 15).latin1());
#endif
        if ((*it).end) {
            if (!(*it).lines.first().offset && end_pos != old_pos) {
                assert(start_pos >= old_pos);
                ts << xml_text.mid(old_pos, start_pos - old_pos);
            }
            assert((*it).end >= bi.offset);
            ts << translate(xml.mid(bi.offset, (*it).end - bi.offset),
                                    (*it).msgid, translations[(*it).msgid]);
            old_pos = end_pos;
        } else {
            if (start_pos != old_pos) {
                assert(start_pos >= old_pos);
                ts << xml_text.mid(old_pos, start_pos - old_pos);
            }
            old_pos = end_pos;
            ts << translate(xml,
                            (*it).msgid, translations[(*it).msgid]);
        }

    }

    ts << xml_text.mid(old_pos);

    index = 0;

    if (!translations["ROLES_OF_TRANSLATORS"].isEmpty()) {
        if (output.find("<!-- TRANS:ROLES_OF_TRANSLATORS") == -1)
            qWarning("%s: missing ROLES_OF_TRANSLATORS, but having translation", argv[1]);
    }

    if (!translations["CREDIT_FOR_TRANSLATORS"].isEmpty()) {
        if (output.find("<!-- TRANS:CREDIT_FOR_TRANSLATORS") == -1)
            qWarning("%s: missing CREDIT_FOR_TRANSLATORS, but having translation", argv[1]);
    }

    while (true) {
        index = output.find("<!-- TRANS:", index);
        if (index == -1)
            break;
        int endindex = index + 11;
        while (output.at(endindex) != '>')
            endindex++;
        assert(output.at(endindex-1) == '-');
        assert(output.at(endindex-2) == '-');
        QString totrans = output.mid(index + 11,
                                     (endindex - 2) - (index + 11)).
                          stripWhiteSpace();
        QString trans = translations[totrans];
        if (trans.isEmpty()) {
            qWarning("no translation for %s found",
                     totrans.local8Bit().data());
        } else
            output.replace(index, endindex - index + 1, trans);
        index = endindex;
    }

    cout << output.utf8().data();
    return 0;
}
