
bin_PROGRAMS = split checkXML xml2pot po2xml
bin_SCRIPTS = xmlizer fixsgml

INCLUDES = -I$(srcdir)/antlr $(all_includes)
KDE_CXXFLAGS = $(USE_EXCEPTIONS) 

SUBDIRS = antlr

split_SOURCES = qxml.cpp split.cpp parser.cpp
split_LDFLAGS = $(all_libraries)
split_LDADD = $(LIB_QT)

checkXML_SOURCES = checkXML.c
checkXML_LDADD = -lxml

xml2pot_SOURCES = qxml.cpp xml2pot.cpp parser.cpp
xml2pot_LDFLAGS = $(all_libraries)
xml2pot_LDADD = $(LIB_QT)

po2xml_SOURCES = GettextLexer.cpp GettextParser.cpp qxml.cpp po2xml.cpp parser.cpp
po2xml_LDFLAGS = $(all_libraries)
po2xml_LDADD = antlr/src/libantlr.la $(LIB_QT)

parser:
	cd $(srcdir) &&	java antlr.Tool gettext.g

$(srcdir)/docbook_kwrite.pot: xml2pot kwrite.xml
	./xml2pot $(srcdir)/kwrite.xml > $(srcdir)/docbook_kwrite.pot

$(srcdir)/docbook_kwrite.po: $(srcdir)/docbook_kwrite.pot
	msgmerge $(srcdir)/docbook_kwrite.po $(srcdir)/docbook_kwrite.pot > d && mv d $(srcdir)/docbook_kwrite.po 
	msgfmt --statistics $(srcdir)/docbook_kwrite.po -o /dev/null

$(srcdir)/kwrite_de.xml: po2xml $(srcdir)/docbook_kwrite.po
	./po2xml $(srcdir)/kwrite.xml $(srcdir)/docbook_kwrite.po > $(srcdir)/kwrite_de.xml

