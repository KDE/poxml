#include "parser.h"
#include <stdlib.h>
#include <iostream>
#include <qfileinfo.h>

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
            MsgList::Iterator tmp = it;
            it++;
            english.remove(tmp);
        } else {
            msgids.insert((*it).msgid, index);
            index++;
            it++;
        }
    }

    cout << "# SOME DESCRIPTIVE TITLE.\n";
    cout << "# Copyright (C) YEAR Free Software Foundation, Inc.\n";
    cout << "# FIRST AUTHOR <EMAIL@ADDRESS>, YEAR.\n";
    cout << "#\n";
    cout << "#, fuzzy\n";
    cout << "msgid \"\"\n";
    cout << "msgstr \"\"\n";
    cout <<"\"Project-Id-Version: PACKAGE VERSION\\n\"\n";
    cout << "\"POT-Creation-Date: 2001-02-09 01:25+0100\\n\"\n";
    cout << "\"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\\n\"\n";
    cout << "\"Last-Translator: FULL NAME <EMAIL@ADDRESS>\\n\"\n";
    cout << "\"Language-Team: LANGUAGE <LL@li.org>\\n\"\n";
    cout << "\"MIME-Version: 1.0\\n\"\n";
    cout << "\"Content-Type: application/x-xml2pot; charset=utf-8\\n\"\n";
    cout << "\"Content-Transfer-Encoding: ENCODING\\n\"\n\n";

    QString fname = QFileInfo(argv[1]).fileName();

    for (MsgList::ConstIterator it = english.begin();
         it != english.end(); ++it)
    {
        cout << "#: ";
        for (QValueList<BlockInfo>::ConstIterator it2 =
                 (*it).lines.begin(); it2 != (*it).lines.end(); it2++) {
            if (it2 != (*it).lines.begin())
                cout << " ";
            cout << fname.utf8().data() << ":" << (*it2).start_line;

        }
        cout << "\n";
	cout << "#, no-c-format\n";
        outputMsg("msgid", StructureParser::descapeLiterals( (*it).msgid ));
        outputMsg("msgstr", (*it).msgstr);
        cout << "\n";
    }

    return 0;
}
