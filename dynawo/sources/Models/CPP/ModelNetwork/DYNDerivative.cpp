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
 * @file  DYNDerivative.cpp
 *
 * @brief
 *
 */
#include "DYNMacrosMessage.h"

#include "DYNDerivative.h"

using std::map;

namespace DYN {

Derivatives::Derivatives() :
values_() {
}

Derivatives::~Derivatives() {
}

Derivatives::Derivatives(const Derivatives& src) :
values_(src.values_) {
}

void
Derivatives::reset() {
  values_.clear();
}

void
Derivatives::addValue(const int& numVar, const double& value) {
  values_[numVar] += value;
}

BusDerivatives::BusDerivatives() :
irDerivatives_(),
iiDerivatives_(),
empty_(true) {
}

BusDerivatives::BusDerivatives(const BusDerivatives& src) :
irDerivatives_(src.irDerivatives_),
iiDerivatives_(src.iiDerivatives_) {
}

BusDerivatives::~BusDerivatives() {
}

void
BusDerivatives::reset() {
  irDerivatives_.reset();
  iiDerivatives_.reset();
  empty_ = true;
}

void
BusDerivatives::addDerivative(typeDerivative_t type, const int& numVar, const double& value) {
  if (empty_)
    empty_ = false;

  switch (type) {
    case IR_DERIVATIVE:
      irDerivatives_.addValue(numVar, value);
      break;
    case II_DERIVATIVE:
      iiDerivatives_.addValue(numVar, value);
      break;
    default:
      throw DYNError(Error::MODELER, InvalidDerivativeType, type);
  }
}

std::map<int, double>
BusDerivatives::getValues(typeDerivative_t type) const {
  if (type == IR_DERIVATIVE)
    return irDerivatives_.getValues();
  else if (type == II_DERIVATIVE)
    return iiDerivatives_.getValues();

  return std::map<int, double> ();
}

}  // namespace DYN
