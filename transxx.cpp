#include <iostream>
using namespace std;
#include "GettextParser.hpp"
#include <fstream>
#include "GettextLexer.hpp"

int main(int argc, char **argv)
{
    if ( argc != 2 ) {
        qWarning( "usage: %s potfile", argv[0] );
        return -1;
    }

    MsgList translated;

    try {
        ifstream s(argv[1]);
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
    cout << "\n";

    for (MsgList::ConstIterator it = translated.begin();
         it != translated.end(); ++it)
    {
        QString msgid = ( *it ).msgid;
        if ( !msgid.isEmpty() ) {
            outputMsg("msgid", escapePO( msgid) );
            QString msgstr = "xx";

            if ( msgid.find( "Definition of PluralForm" ) != -1 ) {
                outputMsg("msgstr", "NoPlural");
                cout << "\n";
                continue;
            }

            if ( is_desktop ) {
                msgstr = msgid.left( msgid.find( '=' ) + 1);
                msgstr += "xx";
                outputMsg( "msgstr", msgstr );
                cout << "\n";
                continue;
            }

            while ( true ) {
                int index = msgid.find( '%' );
                if ( index == -1 )
                    break;
                msgstr += QString( " %%1 xx" ).arg( msgid.at( index + 1 ) );
                msgid.at( index ) = ' ';
                msgid.at( index + 1 ) = ' ';
            }

            if ( msgid.right( 2 ) == "\\n" )
                msgstr += "\n";
            if ( msgid.left( 2 ) == "\\n" )
                msgstr.prepend( "\n" );
            outputMsg("msgstr", msgstr);
            cout << "\n";
        }
    }

}

