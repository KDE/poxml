
EXTRA_PROGRAMS = checkXML

bin_PROGRAMS = split $(LIBXML_PROGRAMS) xml2pot po2xml
bin_SCRIPTS = xmlizer fixsgml
AM_CPPFLAGS = -DNDEBUG

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
	msgmerge $(srcdir)/docbook_kwrite.pot $(srcdir)/docbook_kwrite.pot > d && mv d $(srcdir)/docbook_kwrite.pot
$(srcdir)/docbook_faq.pot: xml2pot faq.xml
	./xml2pot $(srcdir)/faq.xml > $(srcdir)/docbook_faq.pot
	msgmerge $(srcdir)/docbook_faq.pot $(srcdir)/docbook_faq.pot > d && mv d $(srcdir)/docbook_faq.pot
$(srcdir)/lauri.pot: xml2pot lauri.xml
	./xml2pot $(srcdir)/lauri.xml > $(srcdir)/lauri.pot
	msgmerge $(srcdir)/lauri.pot $(srcdir)/lauri.pot > d && mv d $(srcdir)/lauri.pot

$(srcdir)/docbook_kwrite.po: $(srcdir)/docbook_kwrite.pot
	msgmerge $(srcdir)/docbook_kwrite.po $(srcdir)/docbook_kwrite.pot > d && mv d $(srcdir)/docbook_kwrite.po 
	msgfmt --statistics $(srcdir)/docbook_kwrite.po -o /dev/null
$(srcdir)/docbook_faq.po: $(srcdir)/docbook_faq.pot
	msgmerge $(srcdir)/docbook_faq.po $(srcdir)/docbook_faq.pot > d && mv d $(srcdir)/docbook_faq.po 
	msgfmt --statistics $(srcdir)/docbook_faq.po -o /dev/null
$(srcdir)/lauri.po: $(srcdir)/lauri.pot
	msgmerge $(srcdir)/lauri.po $(srcdir)/lauri.pot > d && mv d $(srcdir)/lauri.po
	msgfmt --statistics $(srcdir)/lauri.po -o /dev/null

$(srcdir)/kwrite_de.xml: po2xml $(srcdir)/docbook_kwrite.po
	./po2xml $(srcdir)/kwrite.xml $(srcdir)/docbook_kwrite.po > $(srcdir)/kwrite_de.xml
$(srcdir)/faq_de.xml: po2xml $(srcdir)/docbook_faq.po
	./po2xml $(srcdir)/faq.xml $(srcdir)/docbook_faq.po > $(srcdir)/faq_de.xml
$(srcdir)/lauri_de.xml: po2xml $(srcdir)/lauri.po
	./po2xml $(srcdir)/lauri.xml $(srcdir)/lauri.po > $(srcdir)/lauri_de.xml

test: $(srcdir)/kwrite_de.xml $(srcdir)/faq_de.xml $(srcdir)/lauri_de.xml checkXML
	./checkXML $(srcdir)/kwrite_de.xml 
	./checkXML $(srcdir)/faq_de.xml
	./checkXML $(srcdir)/lauri_de.xml


