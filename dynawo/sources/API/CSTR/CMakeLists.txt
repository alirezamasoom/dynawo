# Copyright (c) 2015-2019, RTE (http://www.rte-france.com)
# See AUTHORS.txt
# All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, you can obtain one at http://mozilla.org/MPL/2.0/.
# SPDX-License-Identifier: MPL-2.0
# 
# This file is part of Dynawo, an hybrid C++/Modelica open source time domain simulation tool for power systems.

# API Version
SET(API_CSTR_VERSION_STRING ${DYNAWO_VERSION_STRING})
SET(API_CSTR_VERSION_MAJOR ${DYNAWO_VERSION_MAJOR})

SET(API_CSTR_SHARED
    xsd/constraints.xsd
   )

INSTALL(FILES ${API_CSTR_SHARED} DESTINATION ${sharedir}/xsd/)

SET(API_CSTR_SOURCES
  CSTRConstraintImpl.cpp
  CSTRConstraintFactory.cpp
  CSTRConstraintsCollection.cpp
  CSTRConstraintsCollectionImpl.cpp
  CSTRConstraintsCollectionFactory.cpp
  CSTRXmlExporter.cpp
  CSTRTxtExporter.cpp
  )

SET(API_CSTR_INCLUDE_HEADERS
  CSTRConstraint.h
  CSTRConstraintFactory.h
  CSTRConstraintCommon.h
  CSTRConstraintsCollection.h
  CSTRConstraintsCollectionFactory.h
  CSTRExport.h
  CSTRExporter.h
  CSTRXmlExporter.h
  CSTRTxtExporter.h
)

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR}
                    ${LIBXML_INCLUDE_DIR}
                   )

INCLUDE_DIRECTORIES(../../Common)

ADD_LIBRARY(dynawo_API_CSTR SHARED ${API_CSTR_SOURCES})

TARGET_LINK_LIBRARIES(dynawo_API_CSTR
                      ${Boost_SYSTEM_LIBRARY}
                      XMLSAXFormatter
                      dynawo_Common
                      )

SET_TARGET_PROPERTIES(dynawo_API_CSTR PROPERTIES VERSION ${API_CSTR_VERSION_STRING}
                                                 SOVERSION ${API_CSTR_VERSION_MAJOR})

INSTALL(TARGETS dynawo_API_CSTR DESTINATION ${libdir})
INSTALL(FILES ${API_CSTR_INCLUDE_HEADERS} DESTINATION ${includedir})
