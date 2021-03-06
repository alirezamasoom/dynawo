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
 * @file  JOBJobsCollection.h
 *
 * @brief jobs collection description : interface file
 *
 */

#ifndef API_JOB_JOBJOBSCOLLECTION_H_
#define API_JOB_JOBJOBSCOLLECTION_H_

#include <boost/shared_ptr.hpp>

#include "JOBExport.h"

namespace job {
class JobEntry;
class job_const_iterator;
class job_iterator;

/**
 * @class JobsCollection
 * @brief jobs collection interface class
 *
 * Interface class for jobs collection object. This a container for jobs description
 */
class __DYNAWO_JOB_EXPORT JobsCollection {
 public:
  /**
   * @brief Destructor
   */
  virtual ~JobsCollection() { }

  /**
   * @brief add a job to the jobs collection
   *
   * @param jobEntry job to add to the structure
   */
  virtual void addJob(const boost::shared_ptr<JobEntry>& jobEntry) = 0;

  /**
   * @brief Get a const_iterator to the beginning of the jobs' vector
   * @return a const_iterator to the beginning of the jobs' vector
   */
  virtual job_const_iterator cbegin() const = 0;

  /**
   * @brief Get a const_iterator to the end of the jobs' vector
   * @return a const_iterator to the end of the jobs' vector
   */
  virtual job_const_iterator cend() const = 0;

  /**
   * @brief Get an iterator to the beginning of the jobs' vector
   * @return an iterator to the beginning of the jobs' vector
   */
  virtual job_iterator begin() = 0;

  /**
   * @brief Get an iterator to the end of the jobs' vector
   * @return an iterator to the end of the jobs' vector
   */
  virtual job_iterator end() = 0;

  class Impl;  ///< Implementated class
};

}  // namespace job

#endif  // API_JOB_JOBJOBSCOLLECTION_H_
