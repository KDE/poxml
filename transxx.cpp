#include <iostream>
using namespace std;
#include "GettextParser.hpp"
#include <fstream>
#include "GettextLexer.hpp"

#include <qregexp.h>
#include <qdatetime.h>
#include <qfileinfo.h>

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

    const bool is_desktop = filename.find( "desktop_") >= 0;

    // The header is the last item (due too the sorting)
    MsgList::const_iterator header = --translated.end();
    if ( ( header == translated.end() ) || ( ! ( *header ).msgid.isEmpty() ) )
    {
        cerr << "Cannot find correct header msgid\n";
        cout << "\"Content-Type: text/plain; charset=utf-8\\n\"\n";
        cout << "\"Plural-Forms: nplurals=1; plural=0;\\n\"\n";
    }
    else
    {
        QStringList headerLines = QStringList::split( "\\n", ( *header ).msgstr, false );
        QFileInfo fi( QString::fromLocal8Bit( filename ) );
        QString projectId( "Project-Id-Version: " ); 
        projectId += fi.baseName( false );
        headerLines.gres( QRegExp( "^Project-Id-Version:.*" ), projectId );
        headerLines.gres( QRegExp( "^Last-Translator:.*" ), "Last-Translator: transxx program <null@kde.org>" );
        headerLines.gres( QRegExp( "^Language-Team:.*" ), "Language-Team: Test Language <kde-i18n-doc@kde.org>" );
        QString revisionDate ( "PO-Revision-Date: " );
        const QDateTime dt = QDateTime::currentDateTime( Qt::UTC );
        revisionDate += dt.toString( "yyyy-MM-dd hh:mm+0000" );
        headerLines.gres( QRegExp( "^PO-Revision-Date:.*" ), revisionDate );
        headerLines << "Plural-Forms: nplurals=1; plural=0;";
        outputMsg ( "msgid", "" );
        outputMsg ( "msgstr", escapePO( headerLines.join("\\n") + "\\n" ) );
    }
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

            // Note: msgid has been modified, so we need to go back to the original version by the help of the iterator
            // (Gettext is not aware of the KDE-specific handling, so it really wants a \n at start and at end in the msgstr if they were in the msgid )
            if ( ( *it ).msgid.endsWith( "\\n" ) && ! ( *it ).msgid.endsWith( "\\\\n" ))
                msgstr += "\n";
            if ( ( *it ).msgid.startsWith( "\\n" ) )
                msgstr.prepend( "\n" );

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

