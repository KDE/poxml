#include "parser.h"
#include <iostream.h>
#include <stdlib.h>
#include <assert.h>

static const char *singletags[] = {"imagedata", "colspec", "spanspec", "anchor", "xref",
                                   "area", "glossseealso", "footnoteref", "glosssee", 0};
static const char *cuttingtags[] = {"para", "title", "term", "entry", "contrib", "keyword",
                                    "note", "footnote", 0};

bool StructureParser::fatalError ( const QXmlParseException &e )
{
    cerr << "fatalError" << e.message() << " " << e.lineNumber() << " " << e.columnNumber() << endl;
    return false;
}

bool StructureParser::startDocument()
{
    message = "";
    inside = 0;
    return TRUE;
}

bool StructureParser::isCuttingTag(const QString &qName) const
{
    int index = 0;
    while (cuttingtags[index]) {
        if (cuttingtags[index] == qName)
            return true;
        index++;
    }
    return false;
}

bool StructureParser::isSingleTag(const QString &qName) const
{
    int index = 0;
    while (singletags[index]) {
        if (singletags[index] == qName)
            return true;
        index++;
    }
    return false;
}
bool StructureParser::skippedEntity ( const QString & name ) {
    if (inside)
        message += QString("&%1;").arg(name);
    return TRUE;
}

bool StructureParser::startElement( const QString& , const QString& ,
                                    const QString& qName,
                                    const QXmlAttributes & attr )
{
    QString tname = qName.lower();
    if (!inside) {
        if (isCuttingTag(tname)) {
            message = QString::null;
            list.pc.increasePara();
            startline = locator->lineNumber();
            startcol = locator->columnNumber();
        }
    } else {
        QString tmp = "<" + tname;
        for (int i = 0; i < attr.length(); i++) {
            tmp += QString(" %1=\"%2\"").arg(attr.qName(i)).arg(attr.value(i));
        }
        if (isSingleTag(qName))
            tmp += "/>";
        else
            tmp += ">";
        message += tmp;

    }
    if (isCuttingTag(tname))
        inside++;

    if (tname == "anchor" || tname.left(4) == "sect" || tname == "chapter")
        list.pc.addAnchor(attr.value("id"));

    return TRUE;
}

bool StructureParser::closureTag(const QString& message, const QString &tag)
{
    qDebug("closureTag %s %s", message.latin1(), tag.latin1());
    int inside = 0;
    uint index = 0;
    while (true)
    {
        int nextclose = message.find(QString::fromLatin1("</%1>").arg(tag), index);
        int nextstart = message.find(QRegExp(QString::fromLatin1("<%1[>\\s]").arg(tag)), index);
        qDebug("finding %d %d %d %d", nextstart, nextclose, index, inside);
        if (nextclose == -1) {
            qDebug("ending on no close anymore %d %d %d", inside, index, message.length());
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
            if (!inside)
                return index >= message.length();
        }
    }
}

void StructureParser::descape(QString &message)
{
    message.replace(QRegExp("\\"), "\\\\");
    message.replace(QRegExp("\""), "\\\"");
    message.replace(QRegExp("&amp-internal;"), "&amp;");
    message.replace(QRegExp("&lt-internal;"), "&lt;");
    message.replace(QRegExp("&gt-internal;"), "&gt;");

    message = message.simplifyWhiteSpace();
}

QString StructureParser::formatMessage(QString message, int &offset) const
{
    offset = 0;

    // removing starting single tags
    for (int index = 0; singletags[index]; index++)
    {
        if (message.left(strlen(singletags[index]) + 1) == QString::fromLatin1("<%1").arg(singletags[index])) {
            int strindex = strlen(singletags[index]) + 1;
            while (message.at(strindex) != '>')
                strindex++;
            message = message.mid(strindex + 1);
            offset += strindex + 1;
        }
    }

    for (int index = 0; message.at(index) == ' '; index++, offset++) ;
    message = message.stripWhiteSpace();

    while (true) {
        if (message.at(0) != '<')
            break;
        if (message.at(message.length() - 1) != '>')
            break;
        int strindex = 1;
        while (message.at(strindex) != ' ' && message.at(strindex) != '>') // simplifyWhiteSpace
            strindex++;
        QString starttag = message.mid(1, strindex - 1);
        int endindex = message.length() - 2;
        while (message.at(endindex) != '<' && message.at(endindex + 1) != '/')
            endindex--;
        strindex = endindex;
        while (message.at(strindex) != ' ' && message.at(strindex) != '>') // simplifyWhiteSpace
            strindex++;
        if (message.mid(endindex + 2, strindex - (endindex + 2)) == starttag) {
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
        } else
            break;
    }

    return message;
}

MsgList StructureParser::splitMessage(const MsgBlock &mb)
{
    // qDebug("splitMessage %s", message.latin1());

    MsgList result;
    MsgBlock msg1 = mb;
    MsgBlock msg2 = mb;

    int index = 0;
    while (cuttingtags[index]) {
        QString message = mb.msgid;
        if (message.left( 1 + strlen(cuttingtags[index])) ==
            QString::fromLatin1("<%1").arg(cuttingtags[index]))
        {
            // if the message starts with a cutting tag, this tag has to
            // end in between. We split both messages and format them
            int strindex = 1 + strlen(cuttingtags[index]);
            while (message.at(strindex) != '>')
                strindex++;
            strindex = message.find(QString::fromLatin1("</%1>").arg(cuttingtags[index]),
                                    strindex + 1);
            while (message.at(strindex) != '>')
                strindex++;

            assert(strindex != -1);
            int endtag =  message.find(QRegExp(QString::fromLatin1("<%1[\\s>]").
                                               arg(cuttingtags[index])), 2);
            if (endtag != -1 && strindex > endtag)
            {
                // qDebug("found another open tag before closing tag in %s", message.utf8().data());
                goto error;
            }

            int offset;
            msg1.msgid = formatMessage(message.left(strindex + 1), offset);
            msg1.lines.first().offset += offset;

            msg2.msgid = formatMessage(message.mid(strindex + 1), offset);
            msg2.lines.first().offset += strindex + 1 + offset;

            // qDebug("split into %s - %s", msg1.latin1(), msg2.latin1());
            result = splitMessage(msg1);
            result += splitMessage(msg2);
            return result;
        }

        if (message.right( 3 + strlen(cuttingtags[index])) ==
            QString::fromLatin1("</%1>").arg(cuttingtags[index]))
        {
            // if the message ends with a cutting tag, this tag has to
            // end in between. We split both messages and format them
            int strindex = message.length() - (3 + strlen(cuttingtags[index]));
            strindex = message.findRev(QRegExp(QString::fromLatin1("<%1[\\s>]").
                                               arg(cuttingtags[index])),
                                       strindex) - 1;

            assert(strindex != -1);
            int endtag =  message.find(QString::fromLatin1("</%1>").
                                       arg(cuttingtags[index]), strindex);
            if (endtag != -1 && strindex > endtag)
            {
                // qDebug("found another closing tag before opening tag in %s", message.utf8().data());
                goto error;
            }
            int offset;
            msg1.msgid = formatMessage(message.left(strindex + 1), offset);
            msg1.lines.first().offset += offset;

            msg2.msgid = formatMessage(message.mid(strindex + 1), offset);
            msg2.lines.first().offset += strindex + 1 + offset;

            // qDebug("split into %s - %s", msg1.latin1(), msg2.latin1());
            result = splitMessage(msg1);
            result += splitMessage(msg2);

            return result;
        }
        index++;
    }
error:
    result.append(mb);
    return result;
}

bool StructureParser::endElement( const QString& , const QString&, const QString& qName)
{
    QString tname = qName.lower();

    if (isCuttingTag(tname)) {
        inside--;
        if (!inside) {
            MsgBlock m;
            descape(message);
            int offset;
            m.msgid = formatMessage(message, offset);
            BlockInfo bi;
            bi.start_line = startline;
            bi.start_col = startcol;
            bi.end_line = locator->lineNumber();
            bi.end_col = locator->columnNumber();
            bi.offset = offset;
            m.lines.append(bi);

            MsgList messages = splitMessage(m);
            for (MsgList::ConstIterator it = messages.begin();
                 it != messages.end(); it++)
            {
                list.append(*it);
            }
        }
    }

    if (inside) {
        if (!isSingleTag(qName))
            message += QString("</%1>").arg(tname);
    }
    return TRUE;
}

bool StructureParser::characters(const QString &ch)
{
    if (inside && !ch.isEmpty())
        message += ch;
    return TRUE;
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
            cout << prefix << " \"" << line << "\"\n";
    } else {
        cout << prefix << " \"\"\n";
        for (QStringList::ConstIterator it = list.begin(); it != list.end(); it++) {
            line = *it;
            if (!line.isEmpty()) {
                cout << "      \"" << line;
                if (it == list.fromLast())
                    cout << "\"\n";
                else
                    cout << "\\n\"\n";
            }
        }
    }
}

MsgList parseXML(const char *filename)
{
    StructureParser handler;
    QFile xmlFile( filename );
    xmlFile.open(IO_ReadOnly);
    QCString contents;
    contents.assign(xmlFile.readAll());
    xmlFile.close();
    contents.replace(QRegExp("&amp;"), "&amp-internal;");
    contents.replace(QRegExp("&lt;"), "&lt-internal;");
    contents.replace(QRegExp("&gt;"), "&gt-internal;");
    QTextStream ts(contents, IO_ReadOnly);
    QXmlInputSource source( ts );
    QXmlSimpleReader reader;
    reader.setFeature( "http://trolltech.com/xml/features/report-start-end-entity", true);
    reader.setContentHandler( &handler );
    reader.parse( source );
    return handler.getList();
}
