# Copyright (c) 2015-2019, RTE (http://www.rte-france.com)
# See AUTHORS.txt
# All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, you can obtain one at http://mozilla.org/MPL/2.0/.
# SPDX-License-Identifier: MPL-2.0
# 
# This file is part of Dynawo, an hybrid C++/Modelica open source time domain simulation tool for power systems.

SET(COMMON_VERSION_STRING ${DYNAWO_VERSION_STRING})
SET(COMMON_VERSION_MAJOR ${DYNAWO_VERSION_MAJOR})


SET(DYNAWO_DICTIONARIES
  Dictionaries/DYNError_en_GB.dic
  Dictionaries/DYNTimeline_en_GB.dic
  Dictionaries/DYNConstraint_en_GB.dic
  Dictionaries/DYNLog_en_GB.dic
  )


# custom target to generate files associated to dictionnaries
ADD_CUSTOM_TARGET( create_keys_files ALL
                  DEPENDS ${DYNAWO_DICTIONARIES})

ADD_CUSTOM_COMMAND(TARGET create_keys_files
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/Dictionaries/validateDictionaries.py --inputDir=${CMAKE_CURRENT_SOURCE_DIR}/Dictionaries --outputDir=${CMAKE_CURRENT_SOURCE_DIR} --modelicaDir=${CMAKE_CURRENT_SOURCE_DIR}/../Models/Modelica/Dynawo/NonElectrical/Logs/
  COMMENT "Generate key files for dictionnaries"
  )

# Find keys.cpp files (generated files)
SET(CPP_KEYS
  DYNTimeline_keys.cpp
  DYNConstraint_keys.cpp
  DYNError_keys.cpp
  DYNLog_keys.cpp
  )
# Find keys.h files  (generated files)
SET(INCLUDE_KEYS
  DYNTimeline_keys.h
  DYNConstraint_keys.h
  DYNError_keys.h
  DYNLog_keys.h
  )

SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/DYNTimeline_keys.cpp PROPERTIES GENERATED 1)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/DYNConstraint_keys.cpp PROPERTIES GENERATED 1)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/DYNError_keys.cpp PROPERTIES GENERATED 1)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/DYNLog_keys.cpp PROPERTIES GENERATED 1)

SET(COMMON_SOURCES
    DYNCommun.cpp
    DYNError.cpp
    DYNTerminate.cpp
    DYNExecUtils.cpp
    DYNEnumUtils.cpp
    DYNFileSystemUtils.cpp
    DYNTimer.cpp
    DYNTrace.cpp
    DYNTraceStream.cpp
    DYNMessage.cpp
    DYNMessageTimeline.cpp
    DYNIoDico.cpp
    DYNGraph.cpp
    DYNParameter.cpp
    DYNSparseMatrix.cpp
    ${CPP_KEYS}
    )


SET(COMMON_INCLUDE_HEADERS
    DYNCommun.h
    DYNError.h
    DYNTerminate.h
    DYNExecUtils.h
    DYNEnumUtils.h
    DYNFileSystemUtils.h
    DYNTimer.h
    DYNTrace.h
    DYNTraceStream.h
    DYNGraph.h
    DYNMessage.h
    DYNMessage.hpp
    DYNMessageTimeline.h
    DYNMessageTimeline.hpp
    DYNMacrosMessage.h
    DYNIoDico.h
    DYNParameter.h
    DYNSparseMatrix.h
    DYNParameter.hpp
    ${INCLUDE_KEYS}
    )


INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR}
                    )

ADD_LIBRARY(dynawo_Common SHARED ${COMMON_SOURCES})

ADD_DEPENDENCIES(dynawo_Common create_keys_files)

TARGET_LINK_LIBRARIES(dynawo_Common
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_LOG_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    )
SET_TARGET_PROPERTIES(dynawo_Common PROPERTIES VERSION ${COMMON_VERSION_STRING}
                                        SOVERSION ${COMMON_VERSION_MAJOR})

INSTALL(TARGETS dynawo_Common DESTINATION ${libdir})
INSTALL(FILES ${COMMON_INCLUDE_HEADERS} DESTINATION ${includedir})
INSTALL(FILES ${DYNAWO_DICTIONARIES} DESTINATION ${sharedir})

