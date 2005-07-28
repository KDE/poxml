#include <iostream>
//Added by qt3to4:
#include <Q3CString>
using namespace std;
#include "GettextParser.hpp"
#include <fstream>
#include "GettextLexer.hpp"

int main(int argc, char **argv)
{
    if ( argc != 2 && argc != 4 ) {
        qWarning( "usage: %s [--text translation] potfile", argv[0] );
        return -1;
    }

    QString translation = "xx";
    Q3CString filename;

    if( argc == 4 ) {
	if( argv[1]!=QString("--text") ) {
    	    qWarning( "usage: %s [--text translation] potfile", argv[0] );
    	    return -1;
	}
	translation = QString::fromLocal8Bit(argv[2]);
	filename = argv[3];
    } else {
	filename = argv[1];
    }

    MsgList translated;

    try {
        ifstream s(filename);
        GettextLexer lexer(s);
        GettextParser parser(lexer);
        translated = parser.file();

    } catch(exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    bool is_desktop = filename.find( "desktop_") >= 0;

    cout << "msgid \"\"\n";
    cout << "msgstr \"\"\n";
    cout << "\"Content-Type: text/plain; charset=utf-8\\n\"\n";
    cout << "\"Plural-Forms: nplurals=1; plural=0;\\n\"\n";
    cout << "\n";

    for (MsgList::ConstIterator it = translated.begin();
         it != translated.end(); ++it)
    {
        QString msgid = ( *it ).msgid;
	QString msgid_plural = ( *it ).msgid_plural;
        if ( !msgid.isEmpty() ) {
            outputMsg("msgid", escapePO( msgid) );

	    if ( ! msgid_plural.isEmpty() ) {
        	outputMsg("msgid_plural", escapePO( msgid_plural ) );
	    }

            QString msgstr;

            if ( msgid.find( "Definition of PluralForm" ) != -1 ) {
                outputMsg("msgstr", "NoPlural");
                cout << "\n";
                continue;
            }

            if ( is_desktop ) {
                msgstr = msgid.left( msgid.find( '=' ) + 1);
                msgstr += translation + msgid.mid( msgid.find( '=' ) + 1) + translation;
                outputMsg( "msgstr", escapePO(msgstr) );
                cout << "\n";
                continue;
            }

            if (msgid.startsWith("_n: ") || msgid.startsWith("_: ") ) { // KDE extentions
                msgid = msgid.mid(msgid.find("\\n") + 2, msgid.length());
            }

            if (msgid.endsWith("%"))
                msgstr = translation + msgid + " " + translation;
            else
                msgstr = translation + msgid + translation;

            // We need to keep a \n (real Line Feed)
            //  at start or end or msgfmt will complain
            if ( msgid.endsWith( "\n" ) ) // do not escape \n
                msgstr += "\n"; // do not escape \n
            if ( msgid.startsWith( "\n" ) ) // do not escape \n
                msgstr.prepend( "\n" ); // do not escape \n

	    if ( msgid_plural.isEmpty() ) {
        	outputMsg("msgstr", escapePO( msgstr) );
	    }
	    else
	    {
		outputMsg("msgstr[0]", escapePO( msgstr) );
	    }
            cout << "\n";
        }
    }

}

