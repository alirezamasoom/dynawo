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

SUNDIALS_VERSION=2.7.0
SUNDIALS_ARCHIVE=sundials-${SUNDIALS_VERSION}.tar.gz
SUNDIALS_DIRECTORY=sundials-${SUNDIALS_VERSION}
export_var_env SUNDIALS_DOWNLOAD_URL=https://computation.llnl.gov/projects/sundials/download
BUILD_DIR=./
BUILD_TYPE=Debug
HERE=$PWD

patch_sundials() {

  if [ ! -d "$BUILD_DIR/$SUNDIALS_DIRECTORY" ]; then
    tar -xzf $SUNDIALS_ARCHIVE -C $BUILD_DIR
    patch -d $BUILD_DIR -p0 < sundials-${SUNDIALS_VERSION}.patch
    if [ -f "$BUILD_DIR/../nicslu/_nicslu/lib/libnicslu.so" ]; then
     patch -d $BUILD_DIR -p1 < nicslu_sundials-${SUNDIALS_VERSION}.patch
    fi
  fi
}

download_sundials() {
  if [ ! -f "${SUNDIALS_ARCHIVE}" ]; then
    if [ -x "$(command -v wget)" ]; then
      wget --timeout 10 --tries 3 ${SUNDIALS_DOWNLOAD_URL}/${SUNDIALS_ARCHIVE} || error_exit "Error while downloading Sundials"
    elif [ -x "$(command -v curl)" ]; then
      curl --connect-timeout 10 --retry 2 ${SUNDIALS_DOWNLOAD_URL}/${SUNDIALS_ARCHIVE} --output ${SUNDIALS_ARCHIVE} || error_exit "Error while downloading Sundials"
    else
      error_exit "You need to install either wget or curl."
    fi
  fi
}

install_sundials() {
  patch_sundials

  CMAKE_OPTIONS="-D CMAKE_INSTALL_PREFIX:STRING=$INSTALL_DIR"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D BUILD_ARKODE:BOOL=OFF"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D BUILD_SHARED_LIBS:BOOL=ON"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D BUILD_STATIC_LIBS:BOOL=OFF"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D BUILD_CVODE:BOOL=OFF"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D BUILD_CVODES:BOOL=OFF"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D BUILD_IDAS:BOOL=OFF"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D KLU_ENABLE:BOOL=ON"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D EXAMPLES_ENABLE:BOOL=OFF"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D EXAMPLES_INSTALL:BOOL=OFF"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D KLU_INCLUDE_DIR:STRING=$SUITESPARSE_INSTALL_DIR/include"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D KLU_LIBRARY_DIR:STRING=$SUITESPARSE_INSTALL_DIR/lib"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D CMAKE_BUILD_TYPE:STRING=$BUILD_TYPE"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D CMAKE_C_COMPILER:STRING=$C_COMPILER"
  CMAKE_OPTIONS="$CMAKE_OPTIONS -D CMAKE_CXX_COMPILER:STRING=$CXX_COMPILER"

  if [ -f "$BUILD_DIR/../nicslu/_nicslu/lib/libnicslu.so" ]; then
    CMAKE_OPTIONS="$CMAKE_OPTIONS -D NICSLU_ENABLE:BOOL=ON"
    CMAKE_OPTIONS="$CMAKE_OPTIONS -D NICSLU_INCLUDE_DIR:STRING=$NICSLU_INSTALL_DIR/include"
    CMAKE_OPTIONS="$CMAKE_OPTIONS -D NICSLU_LIBRARY_DIR:STRING=$NICSLU_INSTALL_DIR/lib"
  fi

  cd $BUILD_DIR
  cmake $CMAKE_OPTIONS $SUNDIALS_DIRECTORY
  make && make install
  RETURN_CODE=$?
  if [ ${RETURN_CODE} -ne 0 ]; then
    exit ${RETURN_CODE}
  fi
}

while (($#)); do
  case $1 in
    --install-dir=*)
	    export INSTALL_DIR=`echo $1 | sed -e 's/--install-dir=//g'`
      if [ ! -d "$INSTALL_DIR" ]; then
        mkdir -p $INSTALL_DIR
      fi
      ;;
    --suitesparse-install-dir=*)
	    export SUITESPARSE_INSTALL_DIR=`echo $1 | sed -e 's/--suitesparse-install-dir=//g'`
      ;;
    --nicslu-install-dir=*)
	    export NICSLU_INSTALL_DIR=`echo $1 | sed -e 's/--nicslu-install-dir=//g'`
      ;;
    --build-type=*)
          BUILD_TYPE=`echo $1 | sed -e 's/--build-type=//g'`
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

download_sundials || error_exit "Error while downloading Sundials"
install_sundials || error_exit "Error while building Sundials"
