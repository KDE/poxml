#include "parser.h"
#include <iostream.h>
#include <stdlib.h>

static const char *singletags[] = {"imagedata", "colspec", "spanspec", "anchor", "xref", "area", "glossseealso", "footnoteref", "glosssee", 0};

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
    return (qName == "para" || qName == "title" || qName == "term" || qName == "entry" || qName == "contrib" || qName == "keyword");
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
    if (isCuttingTag(tname)) {
        if (!inside) {
            message = QString::null;
            list.pc.increasePara();
            startline = locator->lineNumber();
            startcol = locator->columnNumber();
        }
        inside++;

    } else if (inside) {
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
    if (tname == "anchor" || tname.left(4) == "sect" || tname == "chapter")
        list.pc.addAnchor(attr.value("id"));

    return TRUE;
}

QString StructureParser::formatMessage(QString message) const
{
    message = message.simplifyWhiteSpace();

    while (strchr("\n \t", message.at(message.length() - 1).latin1()))
        message = message.left(message.length() - 1);

    message.replace(QRegExp("\""), "\\\"");

    // removing starting single tags
    for (int index = 0; singletags[index]; index++)
    {
        if (message.left(strlen(singletags[index]) + 1) == QString::fromLatin1("<%1").arg(singletags[index])) {
            int strindex = strlen(singletags[index]) + 1;
            while (message.at(strindex) != '>')
                strindex++;
            message = message.mid(strindex + 1);
        }
    }

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
            // removing start/end tags
            message = message.left(endindex);
            strindex = 0;
            while (message.at(strindex) != '>')
                strindex++;
            message = message.mid(strindex + 1);
            message = message.stripWhiteSpace();
        } else
            break;
    }

    return message;
}

bool StructureParser::endElement( const QString& , const QString&, const QString& qName)
{
    QString tname = qName.lower();

    if (isCuttingTag(tname)) {
        inside--;
        if (!inside) {
            MsgBlock m;
            m.msgid = formatMessage(message);
            BlockInfo bi;
            bi.start_line = startline;
            bi.start_col = startcol;
            bi.end_line = locator->lineNumber();
            bi.end_col = locator->columnNumber();
            m.lines.append(bi);
            list.append(m);
        }
    } else if (inside) {
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
    QXmlInputSource source( xmlFile );
    QXmlSimpleReader reader;
    reader.setFeature( "http://trolltech.com/xml/features/report-start-end-entity", true);
    reader.setContentHandler( &handler );
    reader.parse( source );
    return handler.getList();
}

