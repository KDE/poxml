
bin_PROGRAMS = split2po xml2pot po2xml swappo
bin_SCRIPTS = fixsgml
AM_CPPFLAGS = -DNDEBUG

INCLUDES = -I$(srcdir)/antlr  -I/usr/local/include $(all_includes)
KDE_CXXFLAGS = $(USE_EXCEPTIONS) 

SUBDIRS = antlr

split2po_SOURCES = qxml.cpp split.cpp parser.cpp
split2po_LDFLAGS = $(all_libraries)
split2po_LDADD = $(LIB_QT)

xml2pot_SOURCES = qxml.cpp xml2pot.cpp parser.cpp
xml2pot_LDFLAGS = $(all_libraries)
xml2pot_LDADD = $(LIB_QT)

po2xml_SOURCES = GettextLexer.cpp GettextParser.cpp qxml.cpp po2xml.cpp parser.cpp
po2xml_LDFLAGS = $(all_libraries)
po2xml_LDADD = antlr/src/libantlr.la $(LIB_QT)

swappo_SOURCES = GettextLexer.cpp GettextParser.cpp swappo.cpp parser.cpp qxml.cpp
swappo_LDFLAGS = $(all_libraries)
swappo_LDADD = antlr/src/libantlr.la $(LIB_QT)

parser:
	cd $(srcdir) &&	java antlr.Tool gettext.g

SUFFIXES = .pot .po .xml .txml

$(srcdir)/lauri.pot: xml2pot lauri.xml
	./xml2pot $(srcdir)/lauri.xml > $(srcdir)/lauri.pot
	msgmerge -o $(srcdir)/lauri.pot $(srcdir)/lauri.pot $(srcdir)/lauri.pot 

$(srcdir)/lauri.po: $(srcdir)/lauri.pot
	msgmerge -o $(srcdir)/lauri.po $(srcdir)/lauri.po $(srcdir)/lauri.pot 
	msgfmt --statistics $(srcdir)/lauri.po -o /dev/null

$(srcdir)/lauri_de.xml: po2xml $(srcdir)/lauri.po
	./po2xml $(srcdir)/lauri.xml $(srcdir)/lauri.po | \
	sed -e "s,<!ENTITY % English,<!ENTITY % German," > $(srcdir)/lauri_de.xml

test: $(srcdir)/lauri_de.xml
	checkXML $(srcdir)/lauri_de.xml
