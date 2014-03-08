
set(lauri_xml "${CMAKE_CURRENT_SOURCE_DIR}/lauri.xml")

if (GETTEXTPO_FOUND AND GETTEXT_FOUND)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lauri.pot
  COMMAND checkXML ${lauri_xml}
  COMMAND xml2pot ${lauri_xml} > lauri.pot
  COMMAND msgmerge -o lauri.pot lauri.pot lauri.pot
  DEPENDS ${lauri_xml}
          xml2pot
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lauri.po
  COMMAND msgmerge -o lauri.po ${CMAKE_CURRENT_SOURCE_DIR}/lauri.po lauri.pot
  COMMAND msgfmt --statistics lauri.po -o /dev/null
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lauri.po
          ${CMAKE_CURRENT_BINARY_DIR}/lauri.pot
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lauri_de.xml
  COMMAND po2xml ${lauri_xml} lauri.po | sed -e "s,<!ENTITY % English,<!ENTITY % German," > lauri_de.xml
  DEPENDS ${lauri_xml}
          ${CMAKE_CURRENT_BINARY_DIR}/lauri.po
          po2xml
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
)

add_custom_target(tests-data ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lauri_de.xml
)

add_test(
  NAME lauri_de
  COMMAND checkXML lauri_de.xml
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

endif ()
