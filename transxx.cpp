#include <iostream>
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
    QCString filename;
    
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

    bool is_desktop = QString( argv[1] ).find( "desktop_") >= 0;

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
	    
            QString msgstr = translation;

            if ( msgid.find( "Definition of PluralForm" ) != -1 ) {
                outputMsg("msgstr", "NoPlural");
                cout << "\n";
                continue;
            }

            if ( is_desktop ) {
                msgstr = msgid.left( msgid.find( '=' ) + 1);
                msgstr += translation;
                outputMsg( "msgstr", msgstr );
                cout << "\n";
                continue;
            }

            while ( true ) {
                int index = msgid.find( '%' );
                if ( index == -1 )
                    break;
                msgstr += QString( " %%1 "+translation ).arg( msgid.at( index + 1 ) );
                msgid.at( index ) = ' ';
                msgid.at( index + 1 ) = ' ';
            }

            if ( msgid.right( 2 ) == "\\n" )
                msgstr += "\n";
            if ( msgid.left( 2 ) == "\\n" )
                msgstr.prepend( "\n" );
		
	    if ( msgid_plural.isEmpty() ) {    
        	outputMsg("msgstr", msgstr);
	    }
	    else
	    {
		outputMsg("msgstr[0]", msgstr);
	    }
            cout << "\n";
        }
    }

}

