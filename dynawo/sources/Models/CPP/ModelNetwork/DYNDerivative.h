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
 * @file  DYNDerivative.h
 *
 * @brief
 *
 */
#ifndef MODELS_CPP_MODELNETWORK_DYNDERIVATIVE_H_
#define MODELS_CPP_MODELNETWORK_DYNDERIVATIVE_H_

#include <map>

namespace DYN {
// Structure dedicated to the network Jacobian filling
// -------------------------------------------------------

/**
 * define derivative Ir, Ii type
 */
typedef enum {
  IR_DERIVATIVE = 0,
  II_DERIVATIVE = 1
} typeDerivative_t;

/**
 * class Derivatives
 */
class Derivatives {
 public:
  /**
   * @brief default constructor
   */
  Derivatives();
  /**
   * @brief destructor
   */
  ~Derivatives();
  /**
   * @brief constructor
   * @param src source
   */
  Derivatives(const Derivatives& src);
  /**
   * @brief reset
   */
  void reset();
  /**
   * @brief add value
   * @param numVar : number of variable
   * @param value: value
   */
  void addValue(const int& numVar, const double& value);

  /**
   * @brief get values
   * @return map of varibles' values
   */
  inline std::map<int, double> getValues() const {
    return values_;
  }

 private:
  // values_->first : num of the variable
  // values_->second: value of the derivative
  // @I/@Y[values_->first]= values_->second
  std::map<int, double> values_;  ///< associated num var with value of the derivative
};

/**
 * class Bus Derivatives
 */
class BusDerivatives {
 public:
  /**
   * @brief default constructor
   */
  BusDerivatives();
  /**
   * @brief constructor
   * @param src
   */
  BusDerivatives(const BusDerivatives& src);
  /**
   * @brief destructor
   */
  ~BusDerivatives();
  /**
   * @brief reset
   */
  void reset();
  /**
   * @brief add derivative
   *
   * @param type: derivative type
   * @param numVar: number of variable
   * @param value: number of value
   */
  void addDerivative(typeDerivative_t type, const int& numVar, const double& value);
  /**
   * @brief get values
   * @param type type of derivatives
   * @return map of varibles' values
   */
  std::map<int, double> getValues(typeDerivative_t type) const;

  /**
   * @brief state whether empty
   * @return @b empty_
   */
  inline bool empty() const {
    return empty_;
  }

 private:
  Derivatives irDerivatives_;  ///< ir derivative
  Derivatives iiDerivatives_;  ///< ii derivative
  bool empty_;  ///< empty state
};  ///< Class for Derivative elements of Bus Model

}  // namespace DYN

#endif  // MODELS_CPP_MODELNETWORK_DYNDERIVATIVE_H_
