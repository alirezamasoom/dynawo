within Dynawo.Electrical.Controls.Transformers;

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

model PhaseShifterI "Phase-shifter monitoring the current so that it remains under iMax"
  import Dynawo.Electrical.Controls.Basics.SwitchOff;
  
  extends BaseClasses.BaseTapChangerPhaseShifter_MAX (valueMax = iMax, valueStop = iStop, valueToMonitor0 = I0, tapChangerType = tapChangerType0);
  extends SwitchOff.SwitchOffPhaseShifter;
  
  protected
    parameter TapChangerType tapChangerType0 = TapChangerType.phaseShifter;

  public
    parameter Types.AC.CurrentModule iMax "Maximum allowed current";
    parameter Types.AC.CurrentModule iStop  "Current below which the phase-shifter will stop action";
    parameter Types.AC.CurrentModule I0 "Initial current module";

    Connectors.ImPin iMonitored (value (start = I0)) "Monitored current";

equation
  connect (iMonitored.value, valueToMonitor.value);

end PhaseShifterI;

