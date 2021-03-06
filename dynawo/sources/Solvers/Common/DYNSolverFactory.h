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
 * @file DYNSolverFactory.h
 * @brief Solver factory header
 *
 */

#ifndef SOLVERS_COMMON_DYNSOLVERFACTORY_H_
#define SOLVERS_COMMON_DYNSOLVERFACTORY_H_

#include <map>

namespace DYN {
class Solver;
class SolverFactories;

/**
 * @brief SolverFactory class
 *
 * Utility interface used to define a building class for solvers.
 */
class SolverFactory {
 public:
  /**
   * @brief Constructor
   */
  SolverFactory();

  /**
   * @brief Destructor
   */
  virtual ~SolverFactory();

  /**
   * @brief create a new instance of a solver
   *
   * @return the new instance of solver
   */
  virtual Solver* create() const = 0;

  /**
   * @brief Create a solver loading given lib
   *
   * @param lib : Name of the solver library to load
   * @return Pointer to the created solver
   */
  static Solver* createSolverFromLib(const std::string & lib);

  /**
   * @brief reset all factories of solver
   */
  static void resetFactories();

 public:
  void* handle_;  ///< handle return by dlopen when the library is loaded
  static SolverFactories factories_;  ///< Factories already available
};

/**
 * @brief SolverFactories class
 *
 * Manage solver factories to avoid loading the same library multiple
 * time.
 */
class SolverFactories {
 public:
  /**
   * @brief Constructor
   */
  SolverFactories();

  /**
   * @brief destructor
   */
  ~SolverFactories();

  /**
   * @brief Get available factory for a given library name
   *
   * @param lib : Name of the solver library to load
   * @return A map iterator whose key is the library name and whose value
   * is a pointer to the SolverFactory.
   */
  std::map<std::string, SolverFactory*>::iterator find(const std::string & lib);

  /**
   * @brief Test if a map iterator is the end operator of factories map
   *
   * @param iter : Iterator to test, typically obtained via
   * @p SolverFactories::find() function
   * @return @b true if the iterator is the end operator of the factories,
   * @b false otherwise.
   */
  bool end(std::map<std::string, SolverFactory*>::iterator & iter);

  /**
   * @brief Add a factory associated to a given given library name
   *
   * @param lib : Name of the solver library to add - key in the map
   * @param factory : Pointer to the SolverFactory.to add - value in the
   * map
   */
  void add(const std::string & lib, SolverFactory * factory);

  /**
   * @brief reset all factory : delete all factories
   *
   */
  void reset();

 private:
  std::map<std::string, SolverFactory* > factoryMap_;  ///< associate a library factory with the name of the library
};

}  // end of namespace DYN
#endif  // SOLVERS_COMMON_DYNSOLVERFACTORY_H_
