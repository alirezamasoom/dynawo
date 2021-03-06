within Dynawo.Electrical;

/*
* Copyright (c) 2015-2019, RTE (http://www.rte-france.com)
* See AUTHORS.txt
* All rights reserved.
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, you can obtain one at http://mozilla.org/MPL/2.0/.
* SPDX-License-Identifier: MPL-2.0
*
* This file is part of Dynawo, an hybrid C++/Modelica open source time domain simulation tool for power systems.
*/

package SystemBase "Dynawo system base definition (for per-unit purposes)"
  import Modelica.Constants;

  public
    final constant Types.AC.ApparentPowerModule SnRef = 100 "System base";
    final constant SIunits.Frequency fNom = 50 "AC system frequency";
    final constant SIunits.AngularVelocity omegaNom = 2 * Constants.pi * fNom "System angular frequency";
    final constant SIunits.AngularVelocity omegaRef0Pu = 1 "Reference for system angular frequency (p.u. base omegaNom)";
    final constant SIunits.AngularVelocity omega0Pu = 1 "System angular frequency (p.u. base omegaNom)";
end SystemBase;
