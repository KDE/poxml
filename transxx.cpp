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

    for (MsgList::ConstIterator it = translated.begin();
         it != translated.end(); ++it)
    {
        if ( !( *it ).msgstr.isEmpty() ) {
            outputMsg("msgid", (*it).msgit);
            outputMsg("msgstr", "xx");
            cout << "\n";
        }
    }

}

