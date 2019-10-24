project(poxml)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)

find_package(ECM 1.7.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

find_package(GettextPO REQUIRED)
set_package_properties("gettext-po" PROPERTIES
        DESCRIPTION "A library for processing PO files"
        URL "http://www.gnu.org/software/gettext"
        TYPE REQUIRED
        PURPOSE "Required to build the poxml tools.")

find_package(Gettext)
find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Core Xml)
find_package(KF5 REQUIRED COMPONENTS DocTools)


add_subdirectory( doc )

include_directories(${GETTEXTPO_INCLUDE_DIR})

########### next target ###############

set(split2po_SRCS split.cpp parser.cpp gettextpoutils.cpp)


add_executable(split2po ${split2po_SRCS})

target_link_libraries(split2po ${GETTEXTPO_LIBRARY} Qt5::Xml Qt5::Core)

install(TARGETS split2po  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


########### next target ###############

set(xml2pot_SRCS xml2pot.cpp parser.cpp gettextpoutils.cpp)


add_executable(xml2pot ${xml2pot_SRCS})

target_link_libraries(xml2pot ${GETTEXTPO_LIBRARY} Qt5::Xml Qt5::Core)

install(TARGETS xml2pot  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


########### next target ###############

set(po2xml_SRCS
   po2xml.cpp
   gettextpoutils.cpp
   parser.cpp)


add_executable(po2xml ${po2xml_SRCS})

target_link_libraries(po2xml ${GETTEXTPO_LIBRARY} Qt5::Xml Qt5::Core)

install(TARGETS po2xml  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


########### next target ###############

set(swappo_SRCS
   swappo.cpp
   gettextpoutils.cpp)


add_executable(swappo ${swappo_SRCS})

target_link_libraries(swappo ${GETTEXTPO_LIBRARY})

install(TARGETS swappo  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})



if (BUILD_TESTING)
    add_subdirectory(tests)
endif()


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
