#include <qxml.h>

#include <iostream.h>
#include <qstring.h>
#include <stdlib.h>

class MsgBlock {
 public:
    QString msgid;
    QString msgstr;
};

typedef QValueList<MsgBlock> MsgList;

class StructureParser : public QXmlDefaultHandler
{
public:
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& ,
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );
    bool characters( const QString &ch);
    bool isCuttingTag(const QString &tag) const;
    void setDocumentLocator ( QXmlLocator * l ) { locator = l; }
    bool skippedEntity ( const QString & name );
    bool unparsedEntityDecl ( const QString & name, const QString & publicId,
                              const QString & systemId, const QString & notationName );
    bool fatalError ( const QXmlParseException & );
    bool error(const QXmlParseException &e ) { return fatalError(e); }
    bool warning(const QXmlParseException &e ) { return fatalError(e); }
    MsgList getList() const { return list; }

private:
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
    return (qName == "para" || qName == "title" || qName == "term" || qName == "entry");
}

bool StructureParser::skippedEntity ( const QString & name ) {
    if (inside)
        message += QString("&%1;").arg(name);
    return TRUE;
}

bool StructureParser::unparsedEntityDecl ( const QString & name, const QString & publicId,
                                           const QString & systemId, const QString & notationName ) {
    cout << QString(" &%1;-%2-%3-%4 ").arg(name).arg(publicId).arg(systemId).arg(notationName);
    return TRUE;
}

bool StructureParser::startElement( const QString& uri, const QString& localname,
                                    const QString& qName,
                                    const QXmlAttributes & attr )
{
    QString tname = qName.lower();
    if (isCuttingTag(tname)) {
        if (!inside)
            message = QString::null;
        inside++;

    } else if (inside) {
        QString tmp = "<" + tname;
        for (int i = 0; i < attr.length(); i++) {
            tmp += QString(" %1=%2").arg(attr.qName(i)).arg(attr.value(i));
        }
        tmp += ">";
        message += tmp;
    }

    return TRUE;
}

bool StructureParser::endElement( const QString& uri, const QString&localname, const QString& qName)
{
    QString tname = qName.lower();

    if (isCuttingTag(tname)) {
        inside--;
        if (!inside) {
            while (message.at(message.length() - 1) == '\n')
                message = message.left(message.length() - 1);

            MsgBlock m;
            m.msgid = message;
            list.append(m);
        }
    } else if (inside) {
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
        cout << prefix << " \"" << line << "\"\n";
    } else {

        cout << prefix << " \"\"\n";
        for (QStringList::ConstIterator it = list.begin(); it != list.end(); it++) {
            line = *it;
            line = line.stripWhiteSpace();
            if (!line.isEmpty())
                cout << "      \"" << line;
            if (it == list.fromLast())
                cout << "\"\n";
            else
                cout << "\\n\"\n";
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

    MsgList::ConstIterator tit = translated.begin();
    for (MsgList::Iterator it = english.begin();
         it != english.end() && tit != translated.end();
         ++tit, ++it)
    {
        (*it).msgstr = (*tit).msgid;
    }

    for (MsgList::ConstIterator it = english.begin();
         it != english.end(); ++it)
    {
        outputMsg("msgid", (*it).msgid);
        outputMsg("msgstr", (*it).msgstr);
        cout << "\n";
    }

    return 0;
}


