#ifndef PARSER_H
#define PARSER_H

#include <qxml.h>

struct BlockInfo {
    int start_line;
    int start_col;
    int end_line;
    int end_col;
};

class MsgBlock {
 public:
    QValueList<BlockInfo> lines;
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
    QStringList splitMessage(const QString &message);

    static bool closureTag(const QString& message, const QString &tag);

private:
    QString formatMessage(QString message) const;

    QXmlLocator *locator;
    QString message;
    int inside, startline, startcol;
    int line;
    QString comment;
    MsgList list;
};

void outputMsg(const char *prefix, const QString &message);
MsgList parseXML(const char *filename);

#endif
