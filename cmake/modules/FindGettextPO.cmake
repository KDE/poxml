# - Try to find GettextPO
#
# Once done this will define
#
#  GETTEXTPO_FOUND - System has GETTEXTPO
#  GETTEXTPO_INCLUDE_DIR - The GETTEXTPO include directory
#  GETTEXTPO_LIBRARY - The library needed to use GETTEXTPO

# Copyright (c) 2012, Ni Hui <shuizhuyuanluo@126.com>
# Based off FindLibXml2.cmake from CMake 2.6.4 by Alexander Neundorf <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if(GETTEXTPO_INCLUDE_DIR AND GETTEXTPO_LIBRARY)
   # in cache already
   set(GETTEXTPO_FIND_QUIETLY TRUE)
endif(GETTEXTPO_INCLUDE_DIR AND GETTEXTPO_LIBRARY)

find_path(GETTEXTPO_INCLUDE_DIR gettext-po.h)

find_library(GETTEXTPO_LIBRARY NAMES gettextpo)

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set GETTEXTPO_FOUND to TRUE if
# all listed variables are TRUE
find_package_handle_standard_args(GettextPO DEFAULT_MSG GETTEXTPO_LIBRARY GETTEXTPO_INCLUDE_DIR)

mark_as_advanced(GETTEXTPO_INCLUDE_DIR GETTEXTPO_LIBRARY)
