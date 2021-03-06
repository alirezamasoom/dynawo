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
 * @file  DYNModelRatioTapChanger.h
 *
 * @brief Model of ratio tap changer : header file
 *
 */
#ifndef MODELS_CPP_MODELNETWORK_DYNMODELRATIOTAPCHANGER_H_
#define MODELS_CPP_MODELNETWORK_DYNMODELRATIOTAPCHANGER_H_

#include "DYNModelTapChanger.h"

namespace DYN {

/**
 * @brief ModelRatioTapChanger class
 */
class ModelRatioTapChanger : public ModelTapChanger {
 public:
  /**
   * @brief default constructor
   *
   * @param id : name of the tap changer
   */
  explicit ModelRatioTapChanger(const std::string& id);

  /**
   * @brief destructor
   */
  ~ModelRatioTapChanger();

  /**
   * @brief  evaluate the zero crossing functions
   *
   * @param t : time to use during the evaluation
   * @param uValue: voltage monitored by the tap changer
   * @param nodeOff: unused
   * @param g : value of the zero crossing function
   * @param disable : is the tap changer disabled ?
   * @param locked : is the tap changer locked ?
   * @param tfoClosed : is the transformer connected ?
   */
  void evalG(const double& t, const double& uValue, bool nodeOff, state_g* g, const double& disable, const double& locked, bool tfoClosed);

  /**
   * @copydoc ModelTapChanger::evalZ(const double& t, state_g* rootFound, ModelNetwork* network, const double& disable, bool nodeOff, const double& locked, bool tfoClosed)
   */
  void evalZ(const double& t, state_g* g, ModelNetwork* network, const double& disable, bool nodeOff, const double& locked, bool tfoClosed);

  /**
   * @brief  get the size of the local G function
   * @return size of G function
   */
  int sizeG() const;

  /**
   * @brief  get size of discretes variables
   * @return number of discretes variables
   */
  int sizeZ() const;

  /**
   * @brief Set if the nominal voltage of side 1 is superior to the nominal voltage of side 2
   *
   * @param v1SupV2 @b true if V1nom > V2Nom
   */
  void setV1SupV2(bool v1SupV2);

  /**
   * @brief set the dead band around the target of the tap changer
   *
   * @param tolerance dead band to use
   */
  void setTolV(const double& tolerance);

  /**
   * @brief set the target of the tap changer
   *
   * @param target target to use
   */
  void setTargetV(const double& target);

  /**
   * @brief get the current dead band of the tap changer
   * @return value of the current dead band
   */
  double getTolV() const;

 private:
  /**
   * @brief decide whether if the tap changer should increase/decrease tap to increase the target U
   * @return @b true if one tap up increase the voltage
   */
  bool getUpIncreaseTargetU();

 private:
  bool V1SupV2_;  ///< @b true if the side one of the tap changer has a nominal voltage > side 2
  double tolV_;  ///< dead band around targetV
  double targetV_;  ///< target voltage
  double whenUp_;  ///< when the voltage reached a value over the target+deadBand
  double whenDown_;  ///< when the voltage reached a value under the target-deadBand
  double whenLastTap_;  ///< last time when a tap changer
  bool moveUp_;  ///< @b true if tap should be increased
  bool moveDown_;  ///< @b false if tap should be decreased
  int tapRefDown_;  ///< initial tap when trying to decrease tap
  int tapRefUp_;  ///<  initial tap when trying to increase tap
  bool uMaxState_;  ///< @b true if U > uTarget + uDeadBand
  bool uMinState_;  ///< @b true if U < uTarget - uDeadBand
  bool uTargetState_;  ///< @b true if uTarget - uDeadBand < U < uTarget + uDeadBand
};
}  // namespace DYN

#endif  // MODELS_CPP_MODELNETWORK_DYNMODELRATIOTAPCHANGER_H_
