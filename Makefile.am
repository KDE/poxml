
bin_PROGRAMS = split checkXML xml2pot
bin_SCRIPTS = xmlizer fixsgml

INCLUDES = $(all_includes)

split_SOURCES = qxml.cpp split.cpp parser.cpp
split_LDFLAGS = $(all_libraries)
split_LDADD = $(LIB_QT)

checkXML_SOURCES = checkXML.c
checkXML_LDADD = -lxml

xml2pot_SOURCES = qxml.cpp xml2pot.cpp parser.cpp
xml2pot_LDFLAGS = $(all_libraries)
xml2pot_LDADD = $(LIB_QT)
