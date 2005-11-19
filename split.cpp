#include "parser.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

int main( int argc, char **argv )
{
    bool report_mismatches = qstrcmp(getenv("REPORT_MISMATCHES"), "no");

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

    if (report_mismatches && errors.count()) {
        for (QMap<int, QString>::ConstIterator it = errors.begin(); it != errors.end(); ++it)
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

    bool have_roles_of_translators = false;
    bool have_credit_for_translators = false;

    QMap<QString, int> msgids;
    int index = 0;

    for (MsgList::Iterator it = english.begin();
         it != english.end(); )
    {
	if ((*it).msgid == "ROLES_OF_TRANSLATORS") {
            if ((*it).msgstr.length() && !(*it).msgstr.contains("ROLES_OF_TRANSLATORS")) {
	        have_roles_of_translators = true; 
            }
            else {
                MsgList::Iterator tmp = it;
	        ++it;
	        english.remove(tmp);
            }
            continue;
	}

        if ((*it).msgid == "CREDIT_FOR_TRANSLATORS") {
            if ((*it).msgstr.length() && !(*it).msgstr.contains("CREDIT_FOR_TRANSLATORS")) {
	        have_credit_for_translators = true; 
            }
            else {
                MsgList::Iterator tmp = it;
	        ++it;
	        english.remove(tmp);
            }
            continue;
	}

        if (msgids.contains((*it).msgid)) {
            english[msgids[(*it).msgid]].lines += (*it).lines;
            if (english[msgids[(*it).msgid]].msgstr != (*it).msgstr) {
                fprintf(stderr, "two different translations for \"%s\" (\"%s\" and \"%s\") - choosing first one\n",
                        (*it).msgid.latin1(),
                        english[msgids[(*it).msgid]].msgstr.local8Bit().data(),
                        (*it).msgstr.local8Bit().data());

            }
            MsgList::Iterator tmp = it;
            it++;
            english.remove(tmp);
        } else {
            msgids.insert((*it).msgid, index);
            index++;
            it++;
        }
    }

    int counter = 1;

    while (tit != translated.end())
    {
        MsgBlock mb;
        mb.msgid = QString::fromLatin1("appended paragraph %1").arg(counter++);
        mb.msgstr = (*tit).msgid;
        mb.lines += (*tit).lines;
        english.append(mb);
        tit++;
    }

    cout << "#, fuzzy\n";
    cout << "msgid \"\"\n";
    cout << "msgstr \"\"\n";
    cout << "\"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\\n\"\n";
    cout << "\"Last-Translator: FULL NAME <EMAIL@ADDRESS>\\n\"\n";
    cout << "\"Content-Type: text/plain; charset=utf-8\\n\"\n";

    for (MsgList::ConstIterator it = english.begin();
         it != english.end(); ++it)
    {
        cout << "#: ";
        for (QValueList<BlockInfo>::ConstIterator it2 =
                 (*it).lines.begin(); it2 != (*it).lines.end(); it2++) {
            if (it2 != (*it).lines.begin())
                cout << ", ";
            cout << "index.docbook:" << (*it2).start_line;

        }
        cout << "\n";
        outputMsg("msgid", StructureParser::descapeLiterals( (*it).msgid ));
        outputMsg("msgstr", StructureParser::descapeLiterals( (*it).msgstr ));
        cout << "\n";
    }

    if ( !getenv( "NO_CREDITS" ) ) {

        if ( !have_roles_of_translators ) {
            outputMsg("msgid", "ROLES_OF_TRANSLATORS");
            outputMsg("msgstr", "<!--TRANS:ROLES_OF_TRANSLATORS-->");
            cout << "\n";
        }

	if ( !have_credit_for_translators) {
           outputMsg("msgid", "CREDIT_FOR_TRANSLATORS");
           outputMsg("msgstr", "<!--TRANS:CREDIT_FOR_TRANSLATORS-->");
           cout << "\n";
        }
    }

    return 0;
}
