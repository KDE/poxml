#include "parser.h"
#include <stdlib.h>
#include <iostream>

#include <fstream>
#include "GettextLexer.hpp"
#include "GettextParser.hpp"
#include "antlr/AST.hpp"
#include "antlr/CommonAST.hpp"

using namespace std;

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
        ::exit(1);
    }

    QMap<QString, QString> translations;
    for (MsgList::ConstIterator it = translated.begin();
         it != translated.end(); ++it)
    {
        translations.insert((*it).msgid, (*it).msgstr);
    }

     for (MsgList::ConstIterator it = english.begin();
          it != english.end(); ++it) {
         BlockInfo bi = (*it).lines.first();
         qDebug("replacing %d %d - %d %d %s", bi.start_line, bi.start_col,
                bi.end_line, bi.end_col, translations[(*it).msgid].latin1());
     }

    return 0;
}
