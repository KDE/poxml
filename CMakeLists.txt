project(poxml)

# TODO port away from QT3_SUPPORT
add_definitions(-DQT3_SUPPORT)

add_subdirectory(antlr) 

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/antlr)

########### next target ###############

set(split2po_SRCS split.cpp parser.cpp)


kde4_add_executable(split2po ${split2po_SRCS})

target_link_libraries(split2po ${QT_QTXML_LIBRARY} ${QT_QTCORE_LIBRARY})

install(TARGETS split2po  ${INSTALL_TARGETS_DEFAULT_ARGS})


########### next target ###############

set(xml2pot_SRCS xml2pot.cpp parser.cpp)


kde4_add_executable(xml2pot ${xml2pot_SRCS})

target_link_libraries(xml2pot ${QT_QTXML_LIBRARY} ${QT_QTCORE_LIBRARY})

install(TARGETS xml2pot  ${INSTALL_TARGETS_DEFAULT_ARGS})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${KDE4_ENABLE_EXCEPTIONS}")


########### next target ###############

set(po2xml_SRCS 
   GettextLexer.cpp 
   GettextParser.cpp 
   po2xml.cpp 
   parser.cpp)


kde4_add_executable(po2xml ${po2xml_SRCS})
add_dependencies(po2xml antlr)

target_link_libraries(po2xml antlr ${QT_QTXML_LIBRARY} ${QT_QTCORE_LIBRARY})

install(TARGETS po2xml  ${INSTALL_TARGETS_DEFAULT_ARGS})


########### next target ###############

set(swappo_SRCS 
   GettextLexer.cpp 
   GettextParser.cpp 
   swappo.cpp 
   parser.cpp)


kde4_add_executable(swappo ${swappo_SRCS})
add_dependencies(swappo antlr)

target_link_libraries(swappo  antlr ${QT_QTXML_LIBRARY} ${QT_QTCORE_LIBRARY})

install(TARGETS swappo  ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############





#original (but modified) Makefile.am contents follow:

#
#bin_PROGRAMS = split2po xml2pot po2xml swappo
#
#INCLUDES = -I$(srcdir)/antlr $(all_includes)
#KDE_CXXFLAGS = $(USE_EXCEPTIONS) 
#
#SUBDIRS = antlr
#
#split2po_SOURCES = split.cpp parser.cpp
#split2po_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#split2po_LDADD = $(LIB_QTXML)
#
#xml2pot_SOURCES = xml2pot.cpp parser.cpp
#xml2pot_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#xml2pot_LDADD = $(LIB_QTXML)
#
#po2xml_SOURCES = GettextLexer.cpp GettextParser.cpp po2xml.cpp parser.cpp
#po2xml_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#po2xml_LDADD = antlr/src/libantlr.la $(LIB_QTXML)
#
#swappo_SOURCES = GettextLexer.cpp GettextParser.cpp swappo.cpp parser.cpp
#swappo_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#swappo_LDADD = antlr/src/libantlr.la $(LIB_QTXML)
#
#
#parser:
#	cd $(srcdir) &&	java antlr.Tool gettext.g
#
#SUFFIXES = .pot .po .xml .txml
#
#lauri.pot: xml2pot lauri.xml
#	checkXML $(srcdir)/lauri.xml
#	./xml2pot $(srcdir)/lauri.xml > lauri.pot
#	msgmerge -o lauri.pot lauri.pot lauri.pot 
#
#$(srcdir)/lauri.po: lauri.pot
#	msgmerge -o $(srcdir)/lauri.po $(srcdir)/lauri.po lauri.pot 
#	msgfmt --statistics $(srcdir)/lauri.po -o /dev/null
#
#lauri_de.xml: po2xml $(srcdir)/lauri.po $(srcdir)/lauri.xml
#	./po2xml $(srcdir)/lauri.xml $(srcdir)/lauri.po | \
#	sed -e "s,<!ENTITY % English,<!ENTITY % German," > lauri_de.xml
#
#test: lauri_de.xml
#	checkXML lauri_de.xml
