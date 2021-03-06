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
 * @file DYNCompiler.h
 * @brief Dynawo models compile header file
 */
#ifndef MODELER_COMMON_DYNCOMPILER_H_
#define MODELER_COMMON_DYNCOMPILER_H_

#include <map>
#include <vector>
#include <string>
#include <sstream>

#include <boost/shared_ptr.hpp>


#include "DYNFileSystemUtils.h"

namespace dynamicdata {
class BlackBoxModel;
class ModelicaModel;
class UnitDynamicModel;
class ModelTemplate;
class ModelTemplateExpansion;
class Connector;
}  // namespace dynamicdata

namespace DYN {

class ModelDescription;
class DynamicData;
class DydAnalyser;

/**
 * class Compiler
 */
class Compiler {
  /**
   * variable type
   */
  typedef enum {
    CONTINUOUS_VAR = 0,
    DISCRETE_VAR = 1
  } typeVar_t;

 public:
  /**
   * @brief Compiler constructor.
   * @param dyd : dynamic data instance  where data of models to compile are stored
   * @param useStandardPrecompiledModels: whether to use standard precompiled models
   * @param precompiledModelsDirs: precompiled models directory
   * @param precompiledModelsExtension: precompiled modesls extension
   * @param useStandardModelicaModels: whether to use standard modelica models
   * @param modelicaModelsDirs modelica models directories
   * @param modelicaModelsExtension: modelica models extension
   * @param outputDir: output directory
   */
  Compiler(const boost::shared_ptr<DynamicData>& dyd,
          const bool useStandardPrecompiledModels, const std::vector <UserDefinedDirectory> & precompiledModelsDirs,
          const std::string & precompiledModelsExtension,
          const bool useStandardModelicaModels, const std::vector <UserDefinedDirectory> & modelicaModelsDirs, const std::string & modelicaModelsExtension,
          std::string outputDir) :
  dyd_(dyd),
  unitDynamicModelsMap_(),
  compiledModelDescriptions_(),
  useStandardPrecompiledModels_(useStandardPrecompiledModels),
  precompiledModelsDirsPaths_(precompiledModelsDirs),
  precompiledModelsExtension_(precompiledModelsExtension),
  useStandardModelicaModels_(useStandardModelicaModels),
  modelicaModelsDirsPaths_(modelicaModelsDirs),
  modelicaModelsExtension_(modelicaModelsExtension),
  compileDirPath_(outputDir) { }

  /**
   * @brief default destructor.
   */
  ~Compiler() { }

  /**
   * @brief Compile models in a dyd files.
   */
  void compile();

  /**
   * @brief get compiled model lib.
   * @returns list of compiled models lib file .so
   */
  std::vector<std::string>& getCompiledLib() {
    return compiledLib_;
  }

  /**
   * @brief concatenation of connections
   */
  void concatConnects();

  /**
   * @brief concatenation references
   * translate unitDynamic Model name to new unitDynamic Model name
   * reset old static ref and add new static ref
   */
  void concatRefs();

 private:
  /**
   * @brief compile a black box model 's model description.
   * @param blackBoxModelDescription black box Model Description to compile
   */
  void compileBlackBoxModelDescription(const boost::shared_ptr<ModelDescription>& blackBoxModelDescription);

  /**
   * @brief compile a modelica Model 's model description.
   * @param modelicaModelDescription modelica Model Description to compile
   */
  void compileModelicaModelDescription(const boost::shared_ptr<ModelDescription>& modelicaModelDescription);

  /**
   * @brief compile a model template's model description.
   * @param modelTemplateExpansionDescription modelica Model Description to compile
   */
  void compileModelTemplateExpansionDescription(const boost::shared_ptr<ModelDescription>& modelTemplateExpansionDescription);

  /**
   * @brief concatenation a modelica model description
   * @param modelicaModelDescription modelica Model Description to concatenate
   */
  void concatModel(const boost::shared_ptr<ModelDescription>& modelicaModelDescription);

  /**
   * @brief get dynamic data base and external variables
   */
  void getDDB();

  /**
   * @brief generate the full connect variable name (including alias variable name when relevant)
   * @param model the model to connect
   * @param rawVariableName the variable name as written in the .dyd file
   * @returns the full internal variable name to use for the connect
   */
  std::string connectVariableName(const boost::shared_ptr<ModelDescription>& model, const std::string& rawVariableName);

  /**
   * @brief compute a Modelica model variable name (based on possible model aliasing)
   * @param rawVariableName the variable name as written in the .dyd file
   * @param modelId the Modelica model id
   * @param unitDynamicModels a map of unit dynamic models withint the model
   * @returns the full internal variable name to use for the connect
   */
  std::string modelicaModelVariableName(const std::string& rawVariableName, const std::string& modelId,
                                        const std::map<std::string, boost::shared_ptr<dynamicdata::UnitDynamicModel> > & unitDynamicModels);

 private:
  boost::shared_ptr<DynamicData> dyd_;  ///< dynamic data instance where data of models are stored
  std::map< std::string, boost::shared_ptr<ModelDescription> > modelTemplateDescriptions_;  ///< modelTemplateDescriptions by ID, for internal use
  std::map< boost::shared_ptr<dynamicdata::UnitDynamicModel>,
            boost::shared_ptr<dynamicdata::UnitDynamicModel> > unitDynamicModelsMap_;  ///< map of unit dynamic model between two composed modelica models

  std::map<std::string, boost::shared_ptr<ModelDescription> > compiledModelDescriptions_;  ///< model description already compiled

  bool useStandardPrecompiledModels_;  ///< whether to rely on DDB precompiled models
  std::vector <UserDefinedDirectory> precompiledModelsDirsPaths_;  ///< absolute paths to precompiled models directories
  std::string precompiledModelsExtension_;  ///< file extension to discriminate precompiled models
  bool useStandardModelicaModels_;  ///< whether to rely on DDB Modelica models
  std::vector <UserDefinedDirectory> modelicaModelsDirsPaths_;  ///< absolute paths to Modelica models directories
  std::string modelicaModelsExtension_;  ///< file extension to discriminate Modelica models

  std::string compileDirPath_;  ///< compiled files' directory

  // Available models listing
  std::map<std::string, std::string> extVarFiles_;  ///< files gathering "external" variables (connected to C++ models)
  std::map<std::string, std::string> libFiles_;  ///< precompiled models

  // Only modelica files are allowed to have duplicate names
  std::map<std::string, std::string> moFilesAll_;  ///< All Modelica models located within models directories
  std::map<std::string, std::string> moFilesCompilation_;  ///< Modelica models which are needed for models compilation

  // Compiled lib (for DydLibGenerator)
  std::vector<std::string> compiledLib_;  ///< Compiled lib
};

}  // namespace DYN

#endif  // MODELER_COMMON_DYNCOMPILER_H_
