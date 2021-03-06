//
// Copyright (c) 2016-2019, RTE (http://www.rte-france.com)
// All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// SPDX-License-Identifier: MPL-2.0
//
// This file is part of Libiidm, a library to model IIDM networks and allows
// importing and exporting them to files.
//

/**
 * @file ConnectablePositionBuilder.cpp
 * @brief Provides ConnectablePositionBuilder
 */

#include <IIDM/extensions/connectablePosition/ConnectablePositionBuilder.h>

namespace IIDM {
namespace extensions {
namespace connectable_position {

ConnectablePosition ConnectablePositionBuilder::build() const {
  ConnectablePosition builded;
  builded.m_feeder = m_feeder;
  builded.m_feeder1 = m_feeder1;
  builded.m_feeder2 = m_feeder2;
  builded.m_feeder3 = m_feeder3;
  return builded;
}

} // end of namespace IIDM::extensions::connectable_position::
} // end of namespace IIDM::extensions::
} // end of namespace IIDM::
