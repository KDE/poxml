#include <qxml.h>

#include <iostream.h>
#include <qstring.h>
#include <stdlib.h>

const char *singletags[] = {"imagedata", "colspec", "spanspec", "anchor", "xref", "area", "glossseealso", "footnoteref", "glosssee", 0};

class MsgBlock {
 public:
    QString msgid;
    QString msgstr;
};

class ParaCounter
{
public:
    ParaCounter() { current = 0; }
    void addAnchor(QString anchor) { anchors.insert(anchor, current); }
    void increasePara() { current++; }

    QMap<QString, int> anchors;
    int current;
};

class MsgList : public QValueList<MsgBlock>
{
public:
    MsgList() {}
    ParaCounter pc;
};

class StructureParser : public QXmlDefaultHandler
{
public:
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& ,
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );
    bool characters( const QString &ch);
    bool isCuttingTag(const QString &tag) const;
    bool isSingleTag(const QString &qName) const;
    void setDocumentLocator ( QXmlLocator * l ) { locator = l; }
    bool skippedEntity ( const QString & name );
    bool fatalError ( const QXmlParseException & );
    bool error(const QXmlParseException &e ) { return fatalError(e); }
    bool warning(const QXmlParseException &e ) { return fatalError(e); }
    MsgList getList() const { return list; }

private:
    QString formatMessage(QString message) const;

    QXmlLocator *locator;
    QString message;
    int inside;
    int line;
    QString comment;
    MsgList list;
};

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
    return (qName == "para" || qName == "title" || qName == "term" || qName == "entry" || qName == "contrib");
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
            message.stripWhiteSpace();
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

void outputMsg(const char *prefix, const QString &message)
{
    QStringList list = QStringList::split('\n', message, true);
    QString line;

    if (list.count() == 1) {
        line = list.first().stripWhiteSpace();
        if (line.isEmpty())
            cout << prefix << " \"\"\n";
        else
            cout << prefix << " \"" << line << "\"\n";
    } else {
        cout << prefix << " \"\"\n";
        for (QStringList::ConstIterator it = list.begin(); it != list.end(); it++) {
            line = *it;
            line = line.stripWhiteSpace();
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

int main( int argc, char **argv )
{
    if (argc != 3) {
        qWarning("usage: %s english-XML translated-XML", argv[0]);
        exit(1);
    }

    MsgList english = parseXML(argv[1]);
    MsgList translated = parseXML(argv[2]);

    QMap<QString, int>::ConstIterator eit2 = english.pc.anchors.begin();

    QMap<int, QString> errors;

    while (eit2 != english.pc.anchors.end())
    {
        if (eit2.data() == translated.pc.anchors[eit2.key()]) {
            QString key = eit2.key();
            eit2++;
            translated.pc.anchors.remove(key);
            english.pc.anchors.remove(key);
        } else {
            errors[eit2.data()] = eit2.key();
            eit2++;
        }
    }

    if (errors.count()) {
        for (QMap<int, QString>::ConstIterator it = errors.begin(); it != errors.end(); it++)
        {
            if (translated.pc.anchors.contains(it.data()))
                fprintf(stderr, "id=\"%s\" not in the same paragraphs (%d vs %d)\n", it.data().latin1(),
                        english.pc.anchors[it.data()], translated.pc.anchors[it.data()]);
            else {
                fprintf(stderr, "id=\"%s\" not in the translated paragraphs (it's in paragraph %d in english)\n",
                        it.data().latin1(), english.pc.anchors[it.data()]);
            }
        }
        ::exit(1);
    }

    MsgList::ConstIterator tit = translated.begin();
    for (MsgList::Iterator it = english.begin();
         it != english.end() && tit != translated.end();
         ++tit, ++it)
    {
        (*it).msgstr = (*tit).msgid;
    }
    int counter = 1;

    while (tit != translated.end())
    {
        MsgBlock mb;
        mb.msgid = QString::fromLatin1("appended paragraph %1").arg(counter++);
        mb.msgstr = (*tit).msgid;
        english.append(mb);
        tit++;
    }

    for (MsgList::ConstIterator it = english.begin();
         it != english.end(); ++it)
    {
        outputMsg("msgid", (*it).msgid);
        outputMsg("msgstr", (*it).msgstr);
        cout << "\n";
    }

    outputMsg("msgid", "ROLES_OF_TRANSLATORS");
    outputMsg("msgstr", "");
    cout << "\n";

    outputMsg("msgid", "CREDIT_FOR_TRANSLATORS");
    outputMsg("msgstr", "");
    cout << "\n";

    return 0;
}
