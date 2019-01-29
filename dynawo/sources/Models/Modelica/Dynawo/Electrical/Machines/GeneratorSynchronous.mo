within Dynawo.Electrical.Machines;

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

model GeneratorSynchronous "Synchronous machine"

  extends BaseClasses.BaseGeneratorSynchronous;

  /*
   This model represents a sychronous machine.
   The synchronous machine can include a generator transformer.
   The model respects the receptor convention (i > 0 when entering the device)

       Stator side                                                 Terminal side (network side)

                 iStator  rTfo                                i

        uStator ----<------oo------ RTfoPu + jXTfoPu ---------<---- U
        sStator                                                     P
        QStator                                                     Q

  */

  public

    // Voltage, active and reactive power at terminal
    Types.AC.VoltageModule UPu (start = U0Pu) "Voltage amplitude at terminal in p.u (base UNom)";
    Types.AC.ActivePower PGenPu(start = PGen0Pu) "Active power generated by the synchronous machine in p.u (base SnRef)";
    Types.AC.ReactivePower QGenPu(start = QGen0Pu) "Reactive power generated by the synchronous machine in p.u (base SnRef)";
    Types.AC.ActivePower PGen(start = PGen0Pu*SystemBase.SnRef) "Active power generated by the synchronous machine in MW";
    Types.AC.ReactivePower QGen(start = QGen0Pu*SystemBase.SnRef) "Reactive power generated by the synchronous machine in MVar";

    // Output variables for external controllers
    Connectors.ImPin UStatorPu(value(start = UStator0Pu)) "Stator voltage amplitude in p.u (base UNom)";
    Connectors.ImPin IStatorPu(value(start = IStator0Pu)) "Stator current amplitude in p.u (base UNom, SnRef)";
    Connectors.ImPin QStatorPu(value(start = QStator0Pu)) "Stator reactive power generated in p.u (base SnRef)";
    Connectors.ImPin IRotorPu(value(start = IRotor0Pu)) "Rotor current in p.u (base UNom, SNom)";
    Connectors.ImPin thetaInternal(value(start = ThetaInternal0)) "Internal angle in rad";

  protected

    // Start values calculated by the initialization model
    parameter Types.AC.ActivePower PGen0Pu "Start value of active power at terminal in p.u (base SnRef) (generator convention)";
    parameter Types.AC.ReactivePower QGen0Pu "Start value of reactive power at terminal in p.u (base SnRef) (generator convention)";

    parameter Types.AC.ApparentPower sStator0Pu "Start value of complex apparent power at stator side in p.u (base Snref)";
    parameter Types.AC.Voltage uStator0Pu "Start value of complex voltage at stator side in p.u (base UNom)";
    parameter Types.AC.Current iStator0Pu "Start value of complex current at stator side in p.u (base UNom, Snref)";

    parameter SIunits.PerUnit UStator0Pu "Start value of stator voltage amplitude in p.u (base UNom)";
    parameter SIunits.PerUnit IStator0Pu "Start value of stator current amplitude in p.u (base UNom, SnRef)";
    parameter SIunits.PerUnit QStator0Pu "Start value of stator reactive power generated in p.u (base SnRef)";
    parameter SIunits.PerUnit IRotor0Pu "Start value of rotor current in p.u (base SNom)";
    parameter SIunits.Angle ThetaInternal0 "Start value of internal angle in rad";

    // Stator variables
    Types.AC.ApparentPower sStatorPu(re(start = sStator0Pu.re), im(start = sStator0Pu.im)) "Complex apparent power at stator side in p.u (base Snref)";
    Types.AC.Voltage uStatorPu(re(start = uStator0Pu.re), im(start = uStator0Pu.im)) "Complex voltage at stator side in p.u (base UNom)";
    Types.AC.Current iStatorPu(re(start = iStator0Pu.re), im(start = iStator0Pu.im)) "Complex current at stator side in p.u (base UNom, Snref)";

equation

    UPu = ComplexMath.'abs' (terminal.V);

  if running.value then

    // Active and reactive power at terminal
    PGenPu = - ComplexMath.real(terminal.V * ComplexMath.conj(terminal.i));
    QGenPu = - ComplexMath.imag(terminal.V * ComplexMath.conj(terminal.i));
    PGen = PGenPu*SystemBase.SnRef;
    QGen = QGenPu*SystemBase.SnRef;

    // Stator complex variables
    uStatorPu = 1 / rTfoPu * (terminal.V - terminal.i * Complex(RTfoPu, XTfoPu));
    iStatorPu = rTfoPu * terminal.i;
    sStatorPu = uStatorPu * ComplexMath.conj(iStatorPu);

    // Output variables for external controllers
    UStatorPu.value = ComplexMath.'abs' (uStatorPu);
    IStatorPu.value = ComplexMath.'abs' (iStatorPu);
    QStatorPu.value = - ComplexMath.imag(sStatorPu);
    IRotorPu.value = MdPPu / rTfoPu * ifPu;
    thetaInternal.value = ComplexMath.arg(Complex(uqPu, udPu));

  else
    PGenPu = 0;
    QGenPu = 0;
    PGen = 0;
    QGen = 0;
    uStatorPu = Complex(0);
    iStatorPu = Complex(0);
    sStatorPu = Complex(0);
    UStatorPu.value = 0;
    IStatorPu.value = 0;
    QStatorPu.value = 0;
    IRotorPu.value = 0;
    thetaInternal.value = 0;
  end if;

end GeneratorSynchronous;