#include "parser.h"
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <qregexp.h>

using namespace std;

// #undef NDEBUG

static const char *singletags[] = {"imagedata", "colspec", "spanspec",
                                   "anchor", "xref", "area",
                                   "footnoteref", "void", "inlinegraphic",
                                   "glosssee", "graphic", 0};
static const char *cuttingtags[] = {"para", "title", "term", "entry",
                                    "contrib", "keyword", "example",
                                    "note", "footnote", "caution",
                                    "informalexample", "remark", "comment",
                                    "imageobject", "varlistentry", "thead",
                                    "tbody", "row", "screenshot", "screeninfo",
                                    "variablelist", "step", "procedure",
                                    "step", "holder", "listitem", "important",
                                    "author", "itemizedlist", "orderedlist",
                                    "caption", "textobject", "mediaobject",
                                    "tip", "glossdef", "inlinemediaobject",
                                    "simplelist", "member", "glossentry",
				    "areaspec", "corpauthor", "indexterm",
                                    "calloutlist", "callout", "subtitle",
				    "table",
                                    0};
static const char *literaltags[] = {"literallayout", "synopsis", "screen",
				    "programlisting", 0};

static const char *entities[] = {"amp", "quot", "lt", "gt", "QTDOCDIR", 0};

bool StructureParser::fatalError ( const QXmlParseException &e )
{
    cerr << "fatalError " << e.message().latin1() << " " << e.lineNumber() << " "
         << e.columnNumber() << endl;
    return false;
}

bool StructureParser::startDocument()
{
    message = "";
    inside = 0;
    return true;
}

bool StructureParser::isCuttingTag(const QString &qName)
{
    int index = 0;
    while (cuttingtags[index]) {
        if (cuttingtags[index] == qName)
            return true;
        index++;
    }
    return isLiteralTag(qName);
}

bool StructureParser::isSingleTag(const QString &qName)
{
    int index = 0;
    while (singletags[index]) {
        if (singletags[index] == qName)
            return true;
        index++;
    }
    return false;
}

bool StructureParser::isLiteralTag(const QString &qName)
{
    int index = 0;
    while (literaltags[index]) {
        if (literaltags[index] == qName)
            return true;
        index++;
    }
    return false;
}

bool StructureParser::skippedEntity ( const QString & name )
{
    if (inside)
        message += QString("&%1;").arg(name);
    return true;
}

bool StructureParser::startElement( const QString& , const QString& ,
                                    const QString& qName,
                                    const QXmlAttributes & attr )
{
    QString tname = qName.lower();

    bool first = false;

    if (isCuttingTag(tname)) {
        if (!inside) {
            message = QString::null;
            list.pc.increasePara();
            startline = locator->lineNumber();
            startcol = locator->columnNumber();
            first = true;
        }
        inside++;
    }

    if (inside)
    {
        QString tmp = "<" + tname;
        for (int i = 0; i < attr.length(); i++) {
            tmp += QString(" %1=\"%2\"").arg(attr.qName(i)).arg(attr.value(i));
        }
        if (isSingleTag(qName))
            tmp += "/>";
        else
            tmp += ">";
        message += tmp;
        if (first)
            startcol -= message.length();
    }

    if (tname == "anchor" || tname.left(4) == "sect" || tname == "chapter")
        if (!attr.value("id").isEmpty()) list.pc.addAnchor(attr.value("id"));

    return true;
}

bool StructureParser::startCDATA()
{
    if ( inside )
        message += "<![CDATA[";
    return true;
}

bool StructureParser::endCDATA()
{
    if ( inside )
        message += "]]>";
    return true;
}

bool StructureParser::closureTag(const QString& message, const QString &tag)
{
    //   qDebug("closureTag %s %s", message.latin1(), tag.latin1());
    int inside = 0;
    uint index = 0;
    while (true)
    {
        int nextclose = message.find(QString::fromLatin1("</%1>").arg(tag), index);
        int nextstart = message.find(QRegExp(QString::fromLatin1("<%1[>\\s]").arg(tag)), index);
        //  qDebug("finding %d %d %d %d", nextstart, nextclose, index, inside);
        if (nextclose == -1) {
            //  qDebug("ending on no close anymore %d %d %d %d", (!inside && index >= message.length()), inside, index, message.length());
            return !inside && index >= message.length();
        }
        if (nextstart == -1)
            nextstart = message.length() + 1;

        if (nextstart < nextclose) {
            inside++;
            index = nextstart + 1;
            while (message.at(index) != '>')
                index++;
            index++;
        } else {
            inside--;
            index = nextclose + 1;
            while (message.at(index) != '>')
                index++;
            index++;
            if (!inside) {
                //  qDebug("ending on exit %d", index >= message.length());
                return index >= message.length();
            }
        }
    }
}

void StructureParser::escapeEntities( QString &contents )
{
    int index = 0;
    while ( entities[index] ) {
        contents.replace(QRegExp(QString( "&%1;" ).arg( entities[index]) ),
                         QString( "!%1-internal!" ).
                         arg( entities[index] ) );
        index++;
    }
}

void StructureParser::descape(QString &message)
{
    uint index = 0;

    while ( entities[index] ) {
        message.replace(QRegExp(QString( "!%1-internal!" ).
                                arg( entities[index] )),
                        QString( "&%1;" ).arg( entities[index]) );
        index++;
    }
    index = 0;

    message = message.stripWhiteSpace();

    int inside = 0;
    bool lastws = false;

    while (index < message.length()) {
        switch (message.at(index).latin1()) {
            case '\n':
            case '\t':
            case '\r':
                if (!inside)
                    message[index] = ' ';
            case ' ':
                if (!inside && lastws)
                    message[index] = '\010';
                lastws = true;
                break;
            case '<': {
                uint endindex = index+1;
                while (endindex < message.length() && !message.at(endindex).isSpace() &&
                       message.at(endindex) != '>')
                    endindex++;
                QString tag = message.mid(index + 1, endindex - index - 1);
                if (tag.at(0) == '/') {
                    if (isLiteralTag(tag.mid(1)))
                        inside--;
                } else
                    if (isLiteralTag(tag))
                        inside++;
                break;
            }
            default:
                lastws = false;
        }

        index++;
    }
    message.replace(QRegExp("\010"), "");
}

bool StructureParser::formatMessage(QString& message, int &offset) const
{
#ifndef NDEBUG
    qDebug("formatMessage %s", message.latin1());
#endif

    offset = 0;
    bool changed = false;
    bool recurse = true;

    if (message.isEmpty())
        return true;

    for (int index = 0; message.at(index) == ' '; index++, offset++) ;
    message = message.stripWhiteSpace();

    // removing starting single tags
    for (int index = 0; singletags[index]; index++)
    {
        int slen = strlen(singletags[index]);

        if (message.left(slen + 1) == QString::fromLatin1("<%1").arg(singletags[index]) &&
            !message.at( slen + 1 ).isLetterOrNumber() )
        {

#ifndef NDEBUG
            qDebug("removing single tag %s", singletags[index]);
#endif
            int strindex = strlen(singletags[index]) + 1;
            while (message.at(strindex) != '>')
                strindex++;
            message = message.mid(strindex + 1);
            changed = true;
            offset += strindex + 1;
            for (int index = 0; message.at(index) == ' '; index++, offset++) ;
            message = message.stripWhiteSpace();
        }
    }

    while (message.right(2) == "/>")
    {
        int strindex = message.length() - 2;
        while (message.at(strindex) != '<')
            strindex--;
        message = message.left(strindex).stripWhiteSpace(); // only removed space at the end
        changed = true;
    }

    for (int index = 0; message.at(index) == ' '; index++, offset++) ;
    message = message.stripWhiteSpace();

    while (true) {
        if (message.at(0) != '<')
            break;
        if (message.at(message.length() - 1) != '>')
            break;
        int strindex = 1;
        while (message.at(strindex) != ' ' && message.at(strindex) != '>')
            strindex++;
        QString starttag = message.mid(1, strindex - 1);
        // qDebug("startTag %s", starttag.latin1());
        int endindex = message.length() - 2;
        while (message.at(endindex) != '<' && message.at(endindex + 1) != '/')
            endindex--;
        strindex = endindex;
        QString orig = message;
        int ooffset = offset;
        // qDebug("endTag %s", message.mid(endindex + 2, message.length() - (endindex + 2) - 1).latin1());
        if (message.mid(endindex + 2, message.length() - (endindex + 2) - 1) == starttag) {
            if (!closureTag(message, starttag))
                break;

            // removing start/end tags
            message = message.left(endindex);
            strindex = 0;
            while (message.at(strindex) != '>')
                strindex++;
            message = message.mid(strindex + 1);
            offset += strindex + 1;
            for (int index = 0; message.at(index) == ' '; index++, offset++) ;
            message = message.stripWhiteSpace();
            if (message.length() < 4) {
                message = orig;
                recurse = false;
                offset = ooffset;
                break;
            } else
                changed = true;
        } else
            break;
    }

    // qDebug("formatMessage result %s %d", message.latin1(), changed && recurse);

    if (changed && recurse) {
        int to;
        formatMessage(message, to);
        offset += to;
    }

    return !recurse; // indicates an abort
}

MsgList StructureParser::splitMessage(const MsgBlock &mb)
{

    MsgList result;

    MsgBlock msg1 = mb;
    MsgBlock msg2 = mb;

    QString message = mb.msgid;

#ifndef NDEBUG
    qDebug("splitMessage %s", message.latin1());
#endif

    if (message.at(0) == '<') {
        int endindex = 1;
        while (!message.at(endindex).isSpace() && message.at(endindex) != '>')
            endindex++;
        QString tag = message.mid(1, endindex - 1);

        if (closureTag(message, tag))
            goto error;

        if (isCuttingTag(tag))
        {
            // if the message starts with a cutting tag, this tag has to
            // end in between. We split both messages and format them
            int strindex = endindex;
            strindex++;

            int inside = 1;
            while (true) {
#ifndef NDEBUG
                qDebug("inside %s %d", message.mid(strindex, 15).latin1(), inside);
#endif

                int closing_index = message.find(QString::fromLatin1("</%1>").arg(tag),
                                                 strindex);
                int starting_index = message.find(QRegExp(QString::fromLatin1("<%1[\\s>]").arg(tag)),
                                                  strindex);

#ifndef NDEBUG
                qDebug("index1 %d %d %d", closing_index, starting_index, strindex);
#endif

                // when a new start was found, we set the start_index after the next match
                // (and set strindex to it later - increasing inside)
                if (starting_index != -1) {
                    starting_index += tag.length() + 1;
                    while (message.at(starting_index) != '>')
                        starting_index++;
                    starting_index++;
                }

#ifndef NDEBUG
                qDebug("index %d %d %d", closing_index, starting_index, strindex);
#endif

                assert(closing_index != -1);
                closing_index += 3 + tag.length();

                if (starting_index == -1) {
                    assert(inside == 1);
                    strindex = closing_index;
#ifndef NDEBUG
                    qDebug("set strindex %d", strindex);
#endif
                    inside--;
                    if (!inside)
                        break;
                    continue;
                }
                if (closing_index < starting_index)
                {
                    strindex = closing_index;
                    inside--;
                } else {
                    strindex = starting_index;
                    inside++;
                }

                if (!inside)
                    break;
            }

#ifndef NDEBUG
            qDebug("split into %s -AAAAAANNNNNNDDDDDD- %s", message.left(strindex).latin1(), message.mid(strindex).latin1());
#endif
            int offset;
            msg1.msgid = message.left(strindex);
            bool leave = formatMessage(msg1.msgid, offset);
            msg1.lines.first().offset += offset;

            msg2.msgid = message.mid(strindex);
            leave = leave & formatMessage(msg2.msgid, offset);
            msg2.lines.first().offset += strindex + offset;

            if (leave) {
                result.append(msg1);
                result.append(msg2);
                return result;
            }
            result = splitMessage(msg1);
            result += splitMessage(msg2);
            return result;
        }

    }

    if (message.at(message.length() - 1 ) == '>')
    {
        int endindex = message.length() - 1;
        while (endindex >= 0 && (message.at(endindex) != '<' || message.at(endindex + 1) != '/'))
            endindex--;
        QString tag = message.mid(endindex + 2, message.length() - endindex - 3);

        if (isCuttingTag(tag))
        {
            // if the message ends with a cutting tag, this tag has to
            // start in between. We split both messages and format them
            int strindex = endindex;

            int inside = 1;
            while (true) {
                // qDebug("inside %s %d", message.mid(strindex, 35).latin1(), inside);

                int closing_index = message.findRev(QString::fromLatin1("</%1>").arg(tag),
                                                    strindex - 1);
                int starting_index = message.findRev(QRegExp(QString::fromLatin1("<%1[\\s>]").arg(tag)),
                                                     strindex - 1);

                // qDebug("index1 %d %d %d", closing_index, starting_index, strindex);

                if (starting_index == -1) {
                    assert(inside == 1);
                    break;
                }

                if (closing_index > starting_index)
                {
                    strindex = closing_index;
                    inside++;
                } else {
                    strindex = starting_index;
                    inside--;
                }

                if (!inside)
                    break;
            }


#ifndef NDEBUG
            qDebug("split2 into \"%s\" -AAAAAANNNNNNNNNDDDDDDDDDDD- \"%s\"", message.left(strindex).latin1(), message.mid(strindex).latin1());
#endif

            int offset;
            msg1.msgid = message.left(strindex);
            formatMessage(msg1.msgid, offset);
            msg1.lines.first().offset += offset;

            msg2.msgid = message.mid(strindex);
            formatMessage(msg2.msgid, offset);
            msg2.lines.first().offset += strindex + offset;


            result = splitMessage(msg1);
            result += splitMessage(msg2);

            return result;
        }
    }
error:
    result.append(mb);
    return result;
}

bool StructureParser::endElement( const QString& , const QString&, const QString& qName)
{
    QString tname = qName.lower();

    if (inside) {
        if (!isSingleTag(qName)) {
            message += QString("</%1>").arg(tname);
        }
    }

    if (isCuttingTag(tname)) {
        inside--;
        if (!inside) {
            MsgBlock m;
            int offset = 0;

            descape(message);

            m.msgid = message;

            formatMessage(m.msgid, offset);

            BlockInfo bi;
            bi.start_line = startline;
            bi.start_col = startcol;
            bi.end_line = locator->lineNumber();
            bi.end_col = locator->columnNumber() + 1;
            bi.offset = offset;
            m.lines.append(bi);

            MsgList messages = splitMessage(m);
            for (MsgList::ConstIterator it = messages.begin();
                 it != messages.end(); it++)
            {
#ifndef NDEBUG
                qDebug("parser %s %d %s", (*it).msgid.latin1(), (*it).lines.first().offset, message.mid((*it).lines.first().offset, 15).latin1());
#endif
                if (!(*it).msgid.isEmpty())
                    list.append(*it);
            }
        }
    }

    return true;
}

bool StructureParser::comment ( const QString &c )
{
    if (c.left(7) != " TRANS:")
        return true;
    if (inside) {
        qWarning("ERROR: to translated string in nested block. Ignoring %s!", c.stripWhiteSpace().utf8().data());
        return true;
    }
    QString string = c.mid(7).stripWhiteSpace();
    MsgBlock m;
    m.msgid = c.mid(7).stripWhiteSpace();
    BlockInfo bi;
    bi.start_line = locator->lineNumber();
    bi.end_line = 0;
    bi.start_col = bi.end_col = 0;
    m.lines.append(bi);
    list.append(m);
    return true;
}

void StructureParser::cleanupTags( QString &contents )
{
    // qDebug("xml %s", contents.latin1());

    QRegExp unclosed("</(\\w*)\\s\\s*>");
    int index = -1;
    while (true) {
        index = unclosed.search(contents, index + 1);
        if (index < 0)
            break;
        QString tag = unclosed.cap(1);
        contents.replace(index, unclosed.matchedLength(), QString("</%1>").arg(tag));
    }

    QRegExp start("<((\\s*[^<>\\s])*)\\s\\s*(/?)>");
    start.setMinimal(true);

    index = -1;
    while (true) {
        index = start.search(contents, index + 1);
        if (index < 0)
            break;
        QString tag = start.cap(1);
        // qDebug("UNCLO %s %d -%s-", start.cap(0).latin1(), index, tag.latin1());
        contents.replace(index, start.matchedLength(), QString("<%1%2>").arg(tag).arg(start.cap(2)));
    }

    QRegExp singletag("<(\\w*)\\s([^><]*)/>");

    index = -1;
    while (true) {
        index = singletag.search(contents, index + 1);
        if (index < 0)
            break;
        QString tag = singletag.cap(1);
        if (!StructureParser::isSingleTag(tag)) {
            contents.replace(index, singletag.matchedLength(), QString("<%1 %2></%3>").arg(tag).arg(singletag.cap(2)).arg(tag));
        }
    }
}

bool StructureParser::characters(const QString &ch)
{
    if (inside && !ch.isEmpty())
        message += ch;
    return true;
}

QString escape(QString message)
{
    message.replace(QRegExp("\\\\"), "\\\\");
    message.replace(QRegExp("\""), "\\\"");
    return message;
}

void outputMsg(const char *prefix, const QString &message)
{
    QStringList list = QStringList::split('\n', message, true);
    QString line;

    if (list.count() == 1) {
        line = list.first();
        if (line.isEmpty())
            cout << prefix << " \"\"\n";
        else
            cout << prefix << " \"" << escape(line).utf8().data() << "\"\n";
    } else {
        cout << prefix << " \"\"\n";
        for (QStringList::ConstIterator it = list.begin(); it != list.end(); it++) {
            line = *it;
            if (!line.isEmpty()) {
                cout << "      \"" << escape(line).utf8().data();
                if (it == list.fromLast())
                    cout << "\"\n";
                else
                    cout << "\\n\"\n";
            } else
                cout << "      \"\\n\"\n";
        }
    }
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


MsgList parseXML(const char *filename)
{
    StructureParser handler;
    QFile xmlFile( filename );
    xmlFile.open(IO_ReadOnly);

    QCString ccontents;
    ccontents.fill(0, xmlFile.size() + 1);
    memcpy(ccontents.data(), xmlFile.readAll().data(), xmlFile.size());
    xmlFile.close();

    QString contents = QString::fromUtf8( ccontents );
    StructureParser::escapeEntities( contents );
    StructureParser::cleanupTags(contents);

    while (true) {
        int index = contents.find("<!ENTITY");
        if (index < 0)
            break;
        int inside = 0;
        int endindex = index + 1;
        QString replacement = "";
        while (contents.at(endindex) != '>' || inside)
        {
            switch (contents.at(endindex).latin1()) {
                case '<':
                    inside++; break;
                case '>':
                    inside--; break;
                case '\n':
                    replacement += '\n';
                    break;
                default:
                    break;
            }
            endindex++;
        }
        endindex++;
        contents.replace(index, endindex - index, replacement);
    }

    QTextStream ts(contents.utf8(), IO_ReadOnly);
    QXmlInputSource source( ts );
    QXmlSimpleReader reader;
    reader.setFeature( "http://trolltech.com/xml/features/report-start-end-entity", true);
    reader.setContentHandler( &handler );
    reader.setLexicalHandler( &handler );
    reader.setDTDHandler( &handler );
    // reader.setErrorHandler( &handler );
    reader.parse( source );
    return handler.getList();
}

