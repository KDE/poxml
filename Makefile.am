
bin_PROGRAMS = split2po xml2pot po2xml swappo transxx

INCLUDES = -I$(srcdir)/antlr $(all_includes)
KDE_CXXFLAGS = $(USE_EXCEPTIONS) 

SUBDIRS = antlr

split2po_SOURCES = split.cpp parser.cpp
split2po_LDFLAGS = $(all_libraries) $(KDE_RPATH)
split2po_LDADD = $(LIB_QT)

xml2pot_SOURCES = xml2pot.cpp parser.cpp
xml2pot_LDFLAGS = $(all_libraries) $(KDE_RPATH)
xml2pot_LDADD = $(LIB_QT)

po2xml_SOURCES = GettextLexer.cpp GettextParser.cpp po2xml.cpp parser.cpp
po2xml_LDFLAGS = $(all_libraries) $(KDE_RPATH)
po2xml_LDADD = antlr/src/libantlr.la $(LIB_QT)

swappo_SOURCES = GettextLexer.cpp GettextParser.cpp swappo.cpp parser.cpp
swappo_LDFLAGS = $(all_libraries) $(KDE_RPATH)
swappo_LDADD = antlr/src/libantlr.la $(LIB_QT)

transxx_SOURCES = GettextLexer.cpp GettextParser.cpp transxx.cpp parser.cpp
transxx_LDFLAGS = $(all_libraries) $(KDE_RPATH)
transxx_LDADD = antlr/src/libantlr.la $(LIB_QT)

parser:
	cd $(srcdir) &&	java antlr.Tool gettext.g

SUFFIXES = .pot .po .xml .txml

lauri.pot: xml2pot lauri.xml
	checkXML $(srcdir)/lauri.xml
	./xml2pot $(srcdir)/lauri.xml > lauri.pot
	msgmerge -o lauri.pot lauri.pot lauri.pot 

$(srcdir)/lauri.po: lauri.pot
	msgmerge -o $(srcdir)/lauri.po $(srcdir)/lauri.po lauri.pot 
	msgfmt --statistics $(srcdir)/lauri.po -o /dev/null

lauri_de.xml: po2xml $(srcdir)/lauri.po $(srcdir)/lauri.xml
	./po2xml $(srcdir)/lauri.xml $(srcdir)/lauri.po | \
	sed -e "s,<!ENTITY % English,<!ENTITY % German," > lauri_de.xml

test: lauri_de.xml
	checkXML lauri_de.xml
