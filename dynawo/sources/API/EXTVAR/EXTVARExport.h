//
// Copyright (c) 2015-2019, RTE (http://www.rte-france.com)
// See AUTHORS.txt
// All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, you can obtain one at http://mozilla.org/MPL/2.0/.
// SPDX-License-Identifier: MPL-2.0
//
// This file is part of Dynawo, an hybrid C++/Modelica open source time domain
// simulation tool for power systems.
//

/**
 * @file EXTVARExport.h
 * @brief Export definitions for WIN32 compatibility
 *
 */

#ifndef API_EXTVAR_EXTVAREXPORT_H_
#define API_EXTVAR_EXTVAREXPORT_H_

#if ((defined _WIN32) && (!defined DYNAWO_EXTVAR_STATIC))
# ifdef DYNAWO_EXTVAR_EXPORTS
#  define __DYNAWO_EXTVAR_EXPORT __declspec(dllexport)
# else
#  define __DYNAWO_EXTVAR_EXPORT __declspec(dllimport)
# endif
#else
# define __DYNAWO_EXTVAR_EXPORT
#endif  // WIN32

#endif  // API_EXTVAR_EXTVAREXPORT_H_
