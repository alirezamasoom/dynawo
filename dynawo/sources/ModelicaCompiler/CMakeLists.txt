# Copyright (c) 2015-2019, RTE (http://www.rte-france.com)
# See AUTHORS.txt
# All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, you can obtain one at http://mozilla.org/MPL/2.0/.
# SPDX-License-Identifier: MPL-2.0
# 
# This file is part of Dynawo, an hybrid C++/Modelica open source time domain simulation tool for power systems.

#CompilerModelicaOMC
SET(COMPILER_MODELICA_OMC_SOURCES
    DYNCompilerModelicaOMC.cpp
    )

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR}
                   )

INCLUDE_DIRECTORIES(../Common
                   )

ADD_EXECUTABLE(compilerModelicaOMC ${COMPILER_MODELICA_OMC_SOURCES})

TARGET_LINK_LIBRARIES(compilerModelicaOMC
                      dynawo_Common
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      dl
                      )

INSTALL(TARGETS compilerModelicaOMC DESTINATION ${sbindir})


#compileLibModelicaOMC
CONFIGURE_FILE(compileLibModelicaOMC.in ${CMAKE_CURRENT_BINARY_DIR}/compileLibModelicaOMC @ONLY)

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/compileLibModelicaOMC DESTINATION ${sbindir})

#cleanCompilerModelicaOMC
INSTALL(PROGRAMS cleanCompilerModelicaOMC DESTINATION ${sbindir})


#Add post processing scripts
ADD_SUBDIRECTORY(Scripts_OMC_${OPENMODELICA_VERSION})
