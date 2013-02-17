project(poxml)

# search packages used by KDE
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${KDEPIMLIBS_INCLUDE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )

set(HAVE_ANTLR FALSE)

macro_optional_find_package(Antlr2)
if (ANTLR2_FOUND AND ANTLR2_EXECUTABLE)
  set(HAVE_ANTLR TRUE)
endif (ANTLR2_FOUND AND ANTLR2_EXECUTABLE)
macro_log_feature(HAVE_ANTLR "Antlr" "Language tool for constructing recognizers, compilers, etc" "http://www.antlr2.org/" FALSE "2.x" "Used for the po2xml and swappo tools.")

macro_optional_find_package(Gettext)

add_subdirectory( doc )
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

if (HAVE_ANTLR)

add_custom_command(
   OUTPUT GettextLexer.cpp GettextLexer.hpp
          GettextParser.cpp GettextParser.hpp
          GettextParserTokenTypes.hpp GettextParserTokenTypes.txt
   COMMAND ${ANTLR2_EXECUTABLE}
   ARGS -o ${CMAKE_CURRENT_BINARY_DIR} gettext.g
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/gettext.g
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


set(po2xml_SRCS
   ${CMAKE_CURRENT_BINARY_DIR}/GettextLexer.cpp
   ${CMAKE_CURRENT_BINARY_DIR}/GettextParser.cpp
   po2xml.cpp
   parser.cpp)


kde4_add_executable(po2xml ${po2xml_SRCS})

target_link_libraries(po2xml ${ANTLR2_LIBRARIES} ${QT_QTXML_LIBRARY} ${QT_QTCORE_LIBRARY})

install(TARGETS po2xml  ${INSTALL_TARGETS_DEFAULT_ARGS})


########### next target ###############

set(swappo_SRCS
   ${CMAKE_CURRENT_BINARY_DIR}/GettextLexer.cpp
   ${CMAKE_CURRENT_BINARY_DIR}/GettextParser.cpp
   swappo.cpp
   parser.cpp)


kde4_add_executable(swappo ${swappo_SRCS})

target_link_libraries(swappo ${ANTLR2_LIBRARIES} ${QT_QTXML_LIBRARY} ${QT_QTCORE_LIBRARY})

install(TARGETS swappo  ${INSTALL_TARGETS_DEFAULT_ARGS})


add_custom_target(parser
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/GettextParser.cpp
)

endif (HAVE_ANTLR)


add_subdirectory(tests)

