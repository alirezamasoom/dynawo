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
 * @file  FSImporter.h
 *
 * @brief Final State collection importer : interface file
 *
 */

#ifndef API_FS_FSIMPORTER_H_
#define API_FS_FSIMPORTER_H_

#include <boost/shared_ptr.hpp>

#include "FSExport.h"

namespace finalState {
class FinalStateCollection;

/**
 * @class Importer
 * @brief Importer interface class
 *
 * Import class for final state collection.
 */
class __DYNAWO_FS_EXPORT Importer {
 public:
  /**
   * @brief Import final state's collection from file
   *
   * @param fileName file name
   *
   * @return final state request imported
   */
  virtual boost::shared_ptr<FinalStateCollection> importFromFile(const std::string& fileName) const = 0;

  /**
   * @brief Import final state's collection from stream
   *
   * @param stream stream to import
   *
   * @return final state request imported
   */
  virtual boost::shared_ptr<FinalStateCollection> importFromStream(std::istream& stream) const = 0;
};

}  // namespace finalState

#endif  // API_FS_FSIMPORTER_H_


