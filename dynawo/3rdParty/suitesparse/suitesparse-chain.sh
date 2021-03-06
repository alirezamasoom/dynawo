#!/bin/bash
#
# Copyright (c) 2015-2019, RTE (http://www.rte-france.com)
# See AUTHORS.txt
# All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, you can obtain one at http://mozilla.org/MPL/2.0/.
# SPDX-License-Identifier: MPL-2.0
#
# This file is part of Dynawo, an hybrid C++/Modelica open source time domain
# simulation tool for power systems.
#

error_exit()
{
	echo "${1:-"Unknown Error"}" 1>&2
	exit -1
}

export_var_env() {
  var=$@
  name=${var%=*}
  value=${var##*=}

  if eval "[ \$$name ]"; then
    eval "value=\${$name}"
    ##echo "Environment variable for $name already set : $value"
    return
  fi

  if [  "$value" = UNDEFINED ]; then
    error_exit "You must define the value of $name"
  fi
  export $name="$value"
}

SUITE_SPARSE_VERSION=4.5.4
SUITE_SPARSE_ARCHIVE=SuiteSparse-${SUITE_SPARSE_VERSION}.tar.gz
SUITE_SPARSE_DIRECTORY=SuiteSparse
export_var_env SUITE_SPARSE_DOWNLOAD_URL=http://faculty.cse.tamu.edu/davis/SuiteSparse

download_suitesparse() {
  if [ ! -f "${SUITE_SPARSE_ARCHIVE}" ]; then
    if [ -x "$(command -v wget)" ]; then
      wget --timeout 10 --tries 3 ${SUITE_SPARSE_DOWNLOAD_URL}/${SUITE_SPARSE_ARCHIVE} || error_exit "Error while downloading SuiteSparse."
    elif [ -x "$(command -v curl)" ]; then
      curl --connect-timeout 10 --retry 2 ${SUITE_SPARSE_DOWNLOAD_URL}/${ADEPT_ARCHIVE} --output ${SUITE_SPARSE_ARCHIVE} || error_exit "Error while downloading SuiteSparse."
    else
      error_exit "You need to install either wget or curl."
    fi
  fi
}

HERE=$PWD

BUILD_DIR=$HERE
export INSTALL_LIB=$HERE/lib
export INSTALL_INCLUDE=$HERE/include

patch_suitesparse() {
  if [ ! -d "$BUILD_DIR/$SUITE_SPARSE_DIRECTORY" ]; then
    tar -xzf $SUITE_SPARSE_ARCHIVE -C $BUILD_DIR
  fi
}

install_suitesparse() {
  patch_suitesparse
  if [ ! -d "$INSTALL_LIB" ]; then
    mkdir -p $INSTALL_LIB
  fi
  if [ ! -d "$INSTALL_INCLUDE" ]; then
    mkdir -p $INSTALL_INCLUDE
  fi
  if [ "${BUILD_TYPE}" = "Debug" ]; then
    export CC_FLAG="-g"
    export OPTIMIZATION=""
  else
    export CC_FLAG=""
  fi
  cd $BUILD_DIR/$SUITE_SPARSE_DIRECTORY/SuiteSparse_config
  (make CC="$C_COMPILER $CC_FLAG" CXX="$CXX_COMPILER $CC_FLAG $CXX_STDFLAG" library && make CC="$C_COMPILER $CC_FLAG" CXX="$CXX_COMPILER $CC_FLAG $CXX_STDFLAG" install) || error_exit "Error while building SuiteSparse"
  cd $BUILD_DIR/$SUITE_SPARSE_DIRECTORY/AMD
  (make CC="$C_COMPILER $CC_FLAG" CXX="$CXX_COMPILER $CC_FLAG $CXX_STDFLAG" library && make CC="$C_COMPILER $CC_FLAG" CXX="$CXX_COMPILER $CC_FLAG $CXX_STDFLAG" install) || error_exit "Error while building AMD"
  cd $BUILD_DIR/$SUITE_SPARSE_DIRECTORY/COLAMD
  (make CC="$C_COMPILER $CC_FLAG" CXX="$CXX_COMPILER $CC_FLAG $CXX_STDFLAG" library && make CC="$C_COMPILER $CC_FLAG" CXX="$CXX_COMPILER $CC_FLAG $CXX_STDFLAG" install) || error_exit "Error while building COLAMD"
  cd $BUILD_DIR/$SUITE_SPARSE_DIRECTORY/BTF
  (make CC="$C_COMPILER $CC_FLAG" CXX="$CXX_COMPILER $CC_FLAG $CXX_STDFLAG" library && make CC="$C_COMPILER $CC_FLAG" CXX="$CXX_COMPILER $CC_FLAG $CXX_STDFLAG" install) || error_exit "Error while building BTF"
  cd $BUILD_DIR/$SUITE_SPARSE_DIRECTORY/KLU
  (make CC="$C_COMPILER $CC_FLAG" CXX="$CXX_COMPILER $CC_FLAG $CXX_STDFLAG" library && make CC="$C_COMPILER $CC_FLAG" CXX="$CXX_COMPILER $CC_FLAG $CXX_STDFLAG" install) || error_exit "Error while building KLU"
}

while (($#)); do
  case $1 in
    --install-dir=*)
	    INSTALL_DIR=`echo $1 | sed -e 's/--install-dir=//g'`
      if [ ! -d "$INSTALL_DIR" ]; then
        mkdir -p $INSTALL_DIR
      fi
      export INSTALL_LIB=$INSTALL_DIR/lib
      export INSTALL_INCLUDE=$INSTALL_DIR/include
      break
      ;;
    --build-dir=*)
	    BUILD_DIR=`echo $1 | sed -e 's/--build-dir=//g'`
      if [ ! -d "$BUILD_DIR" ]; then
        mkdir -p $BUILD_DIR
      fi
      ;;
    *)
      break
      ;;
  esac
  shift
done

download_suitesparse || error_exit "Error while downloading Adept"
install_suitesparse || error_exit "Error while building SuiteSparse suite"
