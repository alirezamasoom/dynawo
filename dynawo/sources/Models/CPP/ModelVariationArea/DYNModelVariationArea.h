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
 * @file  DYNModelVariationArea.h
 *
 * @brief
 *
 */
#ifndef MODELS_CPP_MODELVARIATIONAREA_DYNMODELVARIATIONAREA_H_
#define MODELS_CPP_MODELVARIATIONAREA_DYNMODELVARIATIONAREA_H_

#include "DYNModelCPPImpl.h"
#include "DYNSubModelFactory.h"

namespace DYN {

/**
 * @brief ModelVariationAreaFactory Model factory
 *
 * Implementation of @p SubModelFactory template for ModelVariationAreaFactory Model
 */
class ModelVariationAreaFactory : public SubModelFactory {
 public:
  /**
   * @brief default constructor
   *
   */
  ModelVariationAreaFactory() { }
  /**
   * @brief default destructor
   *
   */
  ~ModelVariationAreaFactory() { }

  /**
   * @brief Model VariationArea getter
   *
   * @return A pointer to a new instance of Model VariationArea
   */
  SubModel * create() const;
};

/**
 * class ModelVariationArea
 */
class ModelVariationArea : public ModelCPP::Impl {
 public:
  /**
   * @brief define type of calculated variables
   *
   */
  typedef enum {
    nbCalculatedVars_ = 0
  } CalculatedVars_t;

  /**
   * @brief Default constructor
   *
   * Creates a new ModelVariationArea instance.
   */
  ModelVariationArea();
  /**
   * @brief Default destructor
   *
   * Creates a new ModelVariationArea instance.
   */
  ~ModelVariationArea() { }

  // instantiate virtual methods of the Model class

  /**
   * @brief  VariationArea model initialization routine
   * @param t0 : initial time of the simulation
   */
  void init(const double& t0);

  /**
   * @brief  VariationArea model's sizes getter
   *
   * Get the sizes of the vectors and matrixs used by the solver to simulate
   * Model VariationArea instance. Used by @p ModelMulti to generate right size matrixs
   * and vector for the solver.
   */
  void getSize();

  // methodes avec std::vector
  /**
   * @brief  VariationArea F(t,y,y') function evaluation
   *
   * Get the residues' values at a certain instant time with given state variables,
   * state variables derivatives
   * @param[in] t Simulation instant
   */
  void evalF(const double & t);
  /**
   * @brief  VariationArea G(t,y,y') function evaluation
   *
   * Get the roots' value
   * @param[in] t Simulation instant
   */
  void evalG(const double & t);
  /**
   * @brief  VariationArea discrete variables evaluation
   *
   * Get the discrete variables' value depending on current simulation instant and
   * current state variables values.
   * @param[in] t Simulation instant
   * @throws Error::MODELER typed @p Error. Shouldn't, but if it happens
   * it shows that there is a bug in the selection of activated shunt.
   */
  void evalZ(const double & t);
  /**
   * @brief  VariationArea modes' evaluation
   *
   * Set the modes' value depending on current simulation instant and
   * current state variables values. Modes are considered to be member
   * variables.
   * @param[in] t Simulation instant
   */
  void evalMode(const double & t);
  /**
   * @brief calculate calculated variables
   */
  void evalCalculatedVars();

  /**
   * @brief  VariationArea transposed jacobian evaluation
   *
   * Get the sparse transposed jacobian
   * @param[in] t Simulation instant
   * @param[in] cj Jacobian prime coefficient
   * @param jt jacobian matrix to fullfill
   * @param rowOffset offset to use to identify the row where data should be added
   */
  void evalJt(const double & t, const double & cj, SparseMatrix& jt, const int& rowOffset);
  /**
   * @brief calculate jacobien prime matrix
   *
   * @param[in] t Simulation instant
   * @param[in] cj Jacobian prime coefficient
   * @param jt jacobian matrix to fullfill
   * @param rowOffset offset to use to identify the row where data should be added
   */
  void evalJtPrim(const double & t, const double & cj, SparseMatrix& jt, const int& rowOffset);

  /**
   * @copydoc ModelCPP::evalFType()
   */
  void evalFType();

  /**
   * @copydoc ModelCPP::getY0()
   */
  void getY0();

  /**
   * @copydoc ModelCPP::evalYType()
   */
  void evalYType();

  /**
   * @brief get the index of variables used to define the jacobian associated to a calculated variable
   *
   * @param iCalculatedVar index of the calculated variable
   *
   * @return index of variables used to define the jacobian
   */
  std::vector<int> getDefJCalculatedVarI(int iCalculatedVar);
  /**
   * @brief evaluate the jacobian associated to a calculated variable
   *
   * @param iCalculatedVar index of the calculated variable
   * @param y value of the variable used to calculate the jacobian
   * @param yp value of the derivatives of variable used to calculate the jacobian
   * @param res values of the jacobian
   */
  void evalJCalculatedVarI(int iCalculatedVar, double* y, double* yp, std::vector<double>& res);
  /**
   * @brief evaluate the value of a calculated variable
   *
   * @param iCalculatedVar index of the calculated variable
   * @param y values of the variables used to calculate the variable
   * @param yp values of the derivatives used to calculate the variable
   *
   * @return value of the calculated variable
   */
  double evalCalculatedVarI(int iCalculatedVar, double* y, double* yp);

  /**
   * @brief  VariationArea parameters setter
   */
  void setSubModelParameters();

  /**
   * @brief  VariationArea elements initializer
   *
   * Define  VariationArea elements (connection variables for output and other models).
   * @param[out] elements Reference to elements' vector
   * @param[out] mapElement Map associating each element index in the elements vector to its name
   */
  //---------------------------------------------------------------------
  void defineElements(std::vector<Element> &elements, std::map<std::string, int>& mapElement);

  /**
   * @copydoc ModelCPP::defineVariables(std::vector<boost::shared_ptr<Variable> >& variables)
   */
  void defineVariables(std::vector<boost::shared_ptr<Variable> >& variables);

  /**
   * @copydoc ModelCPP::defineParameters(std::vector<ParameterModeler>& parameters)
   */
  void defineParameters(std::vector<ParameterModeler>& parameters);

  /**
   * @brief get check sum number
   * @return checksum string
   */
  std::string getCheckSum() const;

  /**
   * @copydoc ModelCPP::initializeStaticData()
   */
  void initializeStaticData();

  /**
   * @copydoc ModelCPP::initializeFromData(const boost::shared_ptr<DataInterface> &data)
   */
  void initializeFromData(const boost::shared_ptr<DataInterface>& data);

  /**
   * @copydoc ModelCPP::setFequations()
   */
  void setFequations();

  /**
   * @copydoc ModelCPP::setGequations()
   */
  void setGequations();

  /**
   * @copydoc ModelCPP::initParams()
   */
  void initParams() { /* not needed */ }

 private:
  // parameters
  double deltaP_;  ///< variation of active power
  double deltaQ_;  ///< variation of reactive power
  double startTime_;  ///< start time
  double stopTime_;  ///< stop time
  int nbLoads_;  ///< number of loads

  // variables
  double stateVariationArea_;  ///< equal to 1 if activated, 2 if ended, 0 either
};

}  // namespace DYN

#endif  // MODELS_CPP_MODELVARIATIONAREA_DYNMODELVARIATIONAREA_H_

