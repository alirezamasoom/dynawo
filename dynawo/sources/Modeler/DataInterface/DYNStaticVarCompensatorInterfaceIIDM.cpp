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

//======================================================================
/**
 * @file  DYNStaticVarCompensatorInterfaceIIDM.cpp
 *
 * @brief Static Var Compensator data interface : implementation file for IIDM interface
 *
 */
//======================================================================

#include <IIDM/components/StaticVarCompensator.h>

#include <IIDM/extensions/StandbyAutomaton.h>

#include "DYNStaticVarCompensatorInterfaceIIDM.h"


using IIDM::extensions::standbyautomaton::StandbyAutomaton;
using std::string;
using boost::shared_ptr;

namespace DYN {

StaticVarCompensatorInterfaceIIDM::~StaticVarCompensatorInterfaceIIDM() {
}

StaticVarCompensatorInterfaceIIDM::StaticVarCompensatorInterfaceIIDM(IIDM::StaticVarCompensator& svc) :
InjectorInterfaceIIDM<IIDM::StaticVarCompensator>(svc, svc.id()),
staticVarCompensatorIIDM_(svc) {
  setType(ComponentInterface::SVC);
  sa_ = staticVarCompensatorIIDM_.findExtension<StandbyAutomaton>();
  stateVariables_.resize(4);
  stateVariables_[VAR_STATE] = StateVariable("state", StateVariable::INT);  // connectionState
  stateVariables_[VAR_Q] = StateVariable("q", StateVariable::DOUBLE);  // Q
  stateVariables_[VAR_REGULATINGMODE] = StateVariable("regulatingMode", StateVariable::INT);  // regulatingMode
  stateVariables_[VAR_USETPOINT] = StateVariable("uSetPoint", StateVariable::DOUBLE);  // uSetPoint
}

int
StaticVarCompensatorInterfaceIIDM::getComponentVarIndex(const std::string& varName) const {
  int index = -1;
  if ( varName == "regulatingMode" )
    index = VAR_REGULATINGMODE;
  else if ( varName == "q" )
    index = VAR_Q;
  else if ( varName == "state" )
    index = VAR_STATE;
  else if ( varName == "uSetPoint" )
    index = VAR_USETPOINT;
  return index;
}

void
StaticVarCompensatorInterfaceIIDM::exportStateVariablesUnitComponent() {
  bool connected = (getValue<int>(VAR_STATE) == CLOSED);
  staticVarCompensatorIIDM_.q(-1 * getValue<double>(VAR_Q));
  staticVarCompensatorIIDM_.voltageSetPoint(getValue<double>(VAR_USETPOINT));
  int regulatingMode = getValue<int>(VAR_REGULATINGMODE);
  bool standbyMode(false);
  switch (regulatingMode) {
    case StaticVarCompensatorInterface::OFF:
      staticVarCompensatorIIDM_.regulationMode(IIDM::StaticVarCompensator::regulation_off);
      break;
    case StaticVarCompensatorInterface::STANDBY:
      standbyMode = true;
      break;
    case StaticVarCompensatorInterface::RUNNING_Q:
      staticVarCompensatorIIDM_.regulationMode(IIDM::StaticVarCompensator::regulation_reactive_power);
      break;
    case StaticVarCompensatorInterface::RUNNING_V:
      staticVarCompensatorIIDM_.regulationMode(IIDM::StaticVarCompensator::regulation_voltage);
      break;
    default:
      throw DYNError(Error::STATIC_DATA, RegulationModeNotInIIDM, regulatingMode, staticVarCompensatorIIDM_.id());
  }
  if (sa_) {
    sa_->standBy(standbyMode);
  } else {
    if (standbyMode)
      throw DYNError(Error::STATIC_DATA, NoExtension, "standbyMode", "StandbyAutomaton");
  }
  if (staticVarCompensatorIIDM_.has_connection()) {
    if (staticVarCompensatorIIDM_.connectionPoint()->is_bus()) {
      if (connected)
        staticVarCompensatorIIDM_.connect();
      else
        staticVarCompensatorIIDM_.disconnect();
    } else {  // is node()
      // should be removed once a solution has been found to propagate switches (de)connection
      // following component (de)connection (only Modelica models)
      if (connected && !getInitialConnected())
        getVoltageLevelInterface()->connectNode(staticVarCompensatorIIDM_.node());
      else if (!connected && getInitialConnected())
        getVoltageLevelInterface()->disconnectNode(staticVarCompensatorIIDM_.node());
    }
  }
}

bool
StaticVarCompensatorInterfaceIIDM::checkCriteria(bool /*checkEachIter*/) {
  return true;
}

void
StaticVarCompensatorInterfaceIIDM::importStaticParameters() {
  staticParameters_.clear();
  staticParameters_["p"] = StaticParameter("p", StaticParameter::DOUBLE).setValue(getP());
  staticParameters_["q"] = StaticParameter("q", StaticParameter::DOUBLE).setValue(getQ());
  int regulatingMode = getRegulationMode();
  staticParameters_["regulatingMode"] = StaticParameter("regulatingMode", StaticParameter::INT).setValue(regulatingMode);
  if (busInterface_) {
    double U0 = busInterface_->getV0();
    double teta = busInterface_->getAngle0();
    staticParameters_["v"] = StaticParameter("v", StaticParameter::DOUBLE).setValue(U0);
    staticParameters_["angle"] = StaticParameter("angle", StaticParameter::DOUBLE).setValue(teta);
  } else {
    staticParameters_["v"] = StaticParameter("v", StaticParameter::DOUBLE).setValue(0.);
    staticParameters_["angle"] = StaticParameter("angle", StaticParameter::DOUBLE).setValue(0.);
  }
}

void
StaticVarCompensatorInterfaceIIDM::setBusInterface(const shared_ptr<BusInterface>& busInterface) {
  InjectorInterfaceIIDM<IIDM::StaticVarCompensator>::setBusInterface(busInterface);
}

void
StaticVarCompensatorInterfaceIIDM::setVoltageLevelInterface(const shared_ptr<VoltageLevelInterface>& voltageLevelInterface) {
  InjectorInterfaceIIDM<IIDM::StaticVarCompensator>::setVoltageLevelInterface(voltageLevelInterface);
}

shared_ptr<BusInterface>
StaticVarCompensatorInterfaceIIDM::getBusInterface() const {
  return InjectorInterfaceIIDM<IIDM::StaticVarCompensator>::getBusInterface();
}

bool
StaticVarCompensatorInterfaceIIDM::getInitialConnected() {
  return InjectorInterfaceIIDM<IIDM::StaticVarCompensator>::getInitialConnected();
}

double
StaticVarCompensatorInterfaceIIDM::getVNom() const {
  return InjectorInterfaceIIDM<IIDM::StaticVarCompensator>::getVNom();
}

string
StaticVarCompensatorInterfaceIIDM::getID() const {
  return staticVarCompensatorIIDM_.id();
}

double
StaticVarCompensatorInterfaceIIDM::getBMin() const {
  return staticVarCompensatorIIDM_.bmin();
}

double
StaticVarCompensatorInterfaceIIDM::getBMax() const {
  return staticVarCompensatorIIDM_.bmax();
}

double
StaticVarCompensatorInterfaceIIDM::getQ() {
  return InjectorInterfaceIIDM<IIDM::StaticVarCompensator>::getQ();
}

double
StaticVarCompensatorInterfaceIIDM::getVSetPoint() const {
  if (!staticVarCompensatorIIDM_.has_voltageSetPoint()) {
    Trace::warn("DATAINTERFACE") << DYNLog(VariableNotSet, "staticVarCompensator", staticVarCompensatorIIDM_.id(), "VSetPoint") << Trace::endline;
    return 0;
  }
  return staticVarCompensatorIIDM_.voltageSetPoint();
}

double
StaticVarCompensatorInterfaceIIDM::getReactivePowerSetPoint() const {
  if (!staticVarCompensatorIIDM_.has_reactivePowerSetPoint()) {
    Trace::warn("DATAINTERFACE") << DYNLog(VariableNotSet, "staticVarCompensator", staticVarCompensatorIIDM_.id(), "ReactivePowerSetPoint") << Trace::endline;
    return 0;
  }
  return staticVarCompensatorIIDM_.reactivePowerSetPoint();
}

double
StaticVarCompensatorInterfaceIIDM::getUMinActivation() const {
  if (!sa_)
    throw DYNError(Error::STATIC_DATA, NoExtension, "lowVoltageThreshold", "StandbyAutomaton");
  return sa_->lowVoltageThreshold();
}

double
StaticVarCompensatorInterfaceIIDM::getUMaxActivation() const {
  if (!sa_)
    throw DYNError(Error::STATIC_DATA, NoExtension, "highVoltageThreshold", "StandbyAutomaton");
  return sa_->highVoltageThreshold();
}

double
StaticVarCompensatorInterfaceIIDM::getUSetPointMin() const {
  if (!sa_)
    throw DYNError(Error::STATIC_DATA, NoExtension, "lowVoltageSetPoint", "StandbyAutomaton");
  return sa_->lowVoltageSetPoint();
}

double
StaticVarCompensatorInterfaceIIDM::getUSetPointMax() const {
  if (!sa_)
    throw DYNError(Error::STATIC_DATA, NoExtension, "highVoltageSetPoint", "StandbyAutomaton");
  return sa_->highVoltageSetPoint();
}

bool
StaticVarCompensatorInterfaceIIDM::hasStandbyAutomaton() const {
  if (!sa_)
    return false;
  return true;
}

bool
StaticVarCompensatorInterfaceIIDM::isStandBy() const {
  if (!sa_)
    throw DYNError(Error::STATIC_DATA, NoExtension, "standBy", "StandbyAutomaton");
  return sa_->standBy();
}

double
StaticVarCompensatorInterfaceIIDM::getB0() const {
  if (!sa_)
    throw DYNError(Error::STATIC_DATA, NoExtension, "b0", "StandbyAutomaton");
  return sa_->b0();
}

StaticVarCompensatorInterface::RegulationMode_t StaticVarCompensatorInterfaceIIDM::getRegulationMode() const {
  if (sa_) {
    if (sa_->standBy())
      return StaticVarCompensatorInterface::STANDBY;
  }

  IIDM::StaticVarCompensator::e_regulation_mode regMode = staticVarCompensatorIIDM_.regulationMode();
  switch (regMode) {
    case IIDM::StaticVarCompensator::regulation_voltage:
      return StaticVarCompensatorInterface::RUNNING_V;
      break;
    case IIDM::StaticVarCompensator::regulation_reactive_power:
      return StaticVarCompensatorInterface::RUNNING_Q;
      break;
    case IIDM::StaticVarCompensator::regulation_off:
      return StaticVarCompensatorInterface::OFF;
      break;
    default:
      return StaticVarCompensatorInterface::OFF;
      break;
  }

  return StaticVarCompensatorInterface::OFF;  // avoid compiler warning, should not happen
}

}  // namespace DYN
