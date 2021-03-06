within Dynawo.Electrical.Controls.Machines.VoltageRegulators;

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

model VRProportionalBlock "Simple Proportional Voltage Regulator"

  import Modelica.Blocks;

  import Dynawo.NonElectrical.Blocks.NonLinear.LimiterWithLag;
  import Dynawo.NonElectrical.Logs.Timeline;
  import Dynawo.NonElectrical.Logs.TimelineKeys;

  parameter Real Gain "Control gain";
  parameter Types.AC.VoltageModule EfdMinPu "Minimum allowed EfdPu";
  parameter Types.AC.VoltageModule EfdMaxPu "Maximum allowed EfdPu";
  parameter SIunits.Time LagEfdMin "Time lag before taking action when going below EfdMin";
  parameter SIunits.Time LagEfdMax "Time lag before taking action when going above EfdMax";

  LimiterWithLag limiterWithLag(UMin = EfdMinPu, UMax = EfdMaxPu, LagMin = LagEfdMin, LagMax = LagEfdMax, tUMinReached0 = tEfdMinReached0, tUMaxReached0 = tEfdMaxReached0) "Limiter activated only after a certain period outside the bounds" annotation(
    Placement(visible = true, transformation(origin = {26, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Blocks.Math.Gain gain(k = Gain) annotation(
    Placement(visible = true, transformation(origin = {-24, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Blocks.Math.Feedback feedback annotation(
    Placement(visible = true, transformation(origin = {-60, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Blocks.Interfaces.RealInput UcEfdPu(start = UcEfd0Pu) "General control voltage" annotation(
    Placement(visible = true, transformation(origin = {-108, 0}, extent = {{-20, -20}, {20, 20}}, rotation = 0), iconTransformation(origin = {-108, 0}, extent = {{-20, -20}, {20, 20}}, rotation = 0)));
  Blocks.Interfaces.RealInput UsPu(start = Us0Pu) "Stator voltage" annotation(
    Placement(visible = true, transformation(origin = {-60, -48}, extent = {{-20, -20}, {20, 20}}, rotation = 90), iconTransformation(origin = {-56, -40}, extent = {{-20, -20}, {20, 20}}, rotation = 0)));
  Blocks.Interfaces.RealOutput EfdPu(start = Efd0Pu) "Exciter field voltage" annotation(
    Placement(visible = true, transformation(origin = {70, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0), iconTransformation(origin = {70, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Connectors.ImPin EfdPuPin(value(start = Efd0Pu)) "Exciter field voltage Pin";
  Connectors.BPin  limitationUp (value (start = false)) "Limitation up reached ?";
  Connectors.BPin  limitationDown (value (start = false)) "Limitation down reached ?";

protected

  parameter Types.AC.VoltageModule UcEfd0Pu "Initial control voltage";
  // p.u. = Unom
  parameter Types.AC.VoltageModule Us0Pu "Initial stator voltage";
  // p.u. = Unom
  parameter Types.AC.VoltageModule Efd0Pu "Initial Efd, i.e Efd0PuLF if compliant with saturations";
  parameter SIunits.Time tEfdMaxReached0 "Initial time when Efd went above EfdMax";
  parameter SIunits.Time tEfdMinReached0 "Initial time when Efd went below EfdMin";
  parameter Types.AC.VoltageModule Efd0PuLF "Initial Efd from LoadFlow";

equation

  connect(gain.y, limiterWithLag.u) annotation(
    Line(points = {{-12, 0}, {12, 0}, {12, 0}, {14, 0}}, color = {0, 0, 127}));
  connect(limiterWithLag.y, EfdPu) annotation(
    Line(points = {{37, 0}, {70, 0}}, color = {0, 0, 127}));
  connect(EfdPuPin.value, EfdPu) annotation(
    Line);
  connect(gain.u, feedback.y) annotation(
    Line(points = {{-36, 0}, {-50, 0}}, color = {0, 0, 127}));
  connect(UcEfdPu, feedback.u1) annotation(
    Line(points = {{-108, 0}, {-68, 0}, {-68, 0}, {-68, 0}}, color = {0, 0, 127}));
  connect(UsPu, feedback.u2) annotation(
    Line(points = {{-60, -48}, {-60, -48}, {-60, -8}, {-60, -8}}, color = {0, 0, 127}));

//TimeLine
  when time - limiterWithLag.tUMinReached >= LagEfdMin then
// low limit
    Timeline.logEvent1(TimelineKeys.VRLimitationDownward);
    limitationDown.value = true;
  elsewhen time - limiterWithLag.tUMinReached < LagEfdMin and pre(limitationDown.value) then
    Timeline.logEvent1(TimelineKeys.VRBackToRegulation);
    limitationDown.value = false;
  end when;

  when time - limiterWithLag.tUMaxReached >= LagEfdMax then
// high limit
    Timeline.logEvent1(TimelineKeys.VRLimitationUpward);
    limitationUp.value = true;
  elsewhen time - limiterWithLag.tUMaxReached < LagEfdMax and pre(limitationUp.value) then
    Timeline.logEvent1(TimelineKeys.VRBackToRegulation);
    limitationUp.value = false;
  end when;

end VRProportionalBlock;
