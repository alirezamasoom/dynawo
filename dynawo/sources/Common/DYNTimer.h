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
 * @file  DYNTimer.h
 *
 * @brief Class timer header : timers are used to monitored the code execution
 *
 */
#ifndef COMMON_DYNTIMER_H_
#define COMMON_DYNTIMER_H_

#include <map>
#include <string>
#include <vector>

#include <boost/timer.hpp>

namespace DYN {

/**
 * @class Timers
 * @brief Timers clas description : stored all timer created during execution
 *
 */
class Timers {
 public:
  /**
   * @brief default constructor
   */
  Timers();

  /**
   * @brief destructor
   */
  ~Timers();

  /**
   * @brief add new statistics for a given timer
   *
   * @param name name of timer
   * @param time time elapsed for the timer
   */
  void add(const std::string& name, const double& time);

  static Timers INSTANCE;  ///< unique instance of timers

 private:
  std::map<std::string, double> timers_;  ///< association between timers and time elapsed
  std::map<std::string, int32_t> nbAppels_;  ///< association between timers and number of call
};

/**
 * @class Timer
 * @brief Timer clas description :compute time elapsed between its creation
 * and its destruction
 *
 */
class Timer {
 public:
  /**
   * @brief constructor
   *
   * @param name name of the timer
   */
  explicit Timer(const std::string& name);

  /**
   * @brief destructor
   */
  ~Timer();

  /**
   * @brief start the timer
   */
  void start();

  /**
   * @brief stop the timer
   */
  void stop();

 private:
  std::string name_;  ///< name of timer
  boost::timer timer_;  ///< boost timer to compute time elapsed
  bool isStopped_;  ///< @b true is the timer is stopped
};

}  // namespace DYN

#endif  // COMMON_DYNTIMER_H_
