
add_subdirectory( antlr ) 

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/antlr )


########### next target ###############

set(split2po_SRCS split.cpp parser.cpp )

kde4_automoc(${split2po_SRCS})

kde4_add_executable(split2po ${split2po_SRCS})

target_link_libraries(split2po  ${KDE4_KDECORE_LIBS} )

install(TARGETS split2po  DESTINATION ${BIN_INSTALL_DIR} )


########### next target ###############

set(xml2pot_SRCS xml2pot.cpp parser.cpp )

kde4_automoc(${xml2pot_SRCS})

kde4_add_executable(xml2pot ${xml2pot_SRCS})

target_link_libraries(xml2pot  ${KDE4_KDECORE_LIBS} )

install(TARGETS xml2pot  DESTINATION ${BIN_INSTALL_DIR} )

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${KDE4_ENABLE_EXCEPTIONS}")


########### next target ###############

set(po2xml_SRCS 
   GettextLexer.cpp 
   GettextParser.cpp 
   po2xml.cpp 
   parser.cpp )

kde4_automoc(${po2xml_SRCS})

kde4_add_executable(po2xml ${po2xml_SRCS})

target_link_libraries(po2xml ${KDE4_KDECORE_LIBS} ${CMAKE_CURRENT_BINARY_DIR}/antlr/src/libantlr.la )

install(TARGETS po2xml  DESTINATION ${BIN_INSTALL_DIR} )


########### next target ###############

set(swappo_SRCS 
   GettextLexer.cpp 
   GettextParser.cpp 
   swappo.cpp 
   parser.cpp )

kde4_automoc(${swappo_SRCS})

kde4_add_executable(swappo ${swappo_SRCS})

target_link_libraries(swappo  ${KDE4_KDECORE_LIBS} ${CMAKE_CURRENT_BINARY_DIR}/antlr/src/libantlr.a )

install(TARGETS swappo  DESTINATION ${BIN_INSTALL_DIR} )


########### next target ###############

set(transxx_SRCS 
   GettextLexer.cpp 
   GettextParser.cpp 
   transxx.cpp 
   parser.cpp )

kde4_automoc(${transxx_SRCS})

kde4_add_executable(transxx ${transxx_SRCS})

target_link_libraries(transxx  ${KDE4_KDECORE_LIBS} ${CMAKE_CURRENT_BINARY_DIR}/antlr/src/libantlr.a )

install(TARGETS transxx  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############





#original Makefile.am contents follow:

#
#bin_PROGRAMS = split2po xml2pot po2xml swappo transxx
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
#transxx_SOURCES = GettextLexer.cpp GettextParser.cpp transxx.cpp parser.cpp
#transxx_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#transxx_LDADD = antlr/src/libantlr.la $(LIB_QTXML)
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
