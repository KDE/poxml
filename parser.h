#ifndef PARSER_H
#define PARSER_H

#include <qxml.h>
#include <qmap.h>

struct BlockInfo {
    int start_line;
    int start_col;
    int end_line;
    int end_col;

    // used to detect sub-messages
    int offset;
};

class MsgBlock {
 public:
    QValueList<BlockInfo> lines;
    QString msgid;
    QString msgstr;
    int start, end;
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
    static bool isCuttingTag(const QString &tag);
    static bool isSingleTag(const QString &qName);
    static bool isLiteralTag(const QString &qName);
    void setDocumentLocator ( QXmlLocator * l ) { locator = l; }
    bool skippedEntity ( const QString & name );
    bool fatalError ( const QXmlParseException & );
    bool comment ( const QString & );
    bool error(const QXmlParseException &e ) { return fatalError(e); }
    bool warning(const QXmlParseException &e ) { return fatalError(e); }
    MsgList getList() const { return list; }
    MsgList splitMessage(const MsgBlock &message);

    virtual bool startCDATA();
    virtual bool endCDATA();

    static bool closureTag(const QString& message, const QString &tag);
    static void descape(QString &message);
    static void escapeEntities( QString &contents );
    static void explodeNonSingleTags( QString &contents );

private:
    bool formatMessage(QString& message, int &offset) const;

    QXmlLocator *locator;
    QString message;
    int inside, startline, startcol;
    int line;
    MsgList list;
};

void outputMsg(const char *prefix, const QString &message);
MsgList parseXML(const char *filename);
QString escapePO(QString msgid);

#endif
