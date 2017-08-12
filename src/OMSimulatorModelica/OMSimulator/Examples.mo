within OMSimulator;
package Examples
  constant String pathToFMUs = "/home/bernhard/data/workspace/OMFMISimulator/testsuite/FMUs/" "Path to the directory with the FMUs used in the examples";
  extends Modelica.Icons.ExamplesPackage;
  model CS_BouncingBall
   extends Modelica.Icons.Example;
   import OMSimulator.OMS.*;
   parameter String filename = OMSimulator.Examples.pathToFMUs + "cs_BouncingBall.fmu";
   OMSimulator.OMSModel omsmodel = OMSimulator.OMSModel();
  // String version= getVersion();
  Real h(start=0, fixed=true);
  Real v(start=0, fixed=true);
  algorithm
    when time > 0.1 then
      // Modelica.Utilities.Streams.print("Version: " + version);
      setTempDirectory(".");

      //describe(omsmodel);

      instantiateFMU(omsmodel, filename, "BouncingBall");

      setStopTime(omsmodel, 5.0);
      setTolerance(omsmodel, 1e-5);

      initialize(omsmodel);
      simulate(omsmodel);

      h := getReal(omsmodel, "BouncingBall.h");
      v :=getReal(omsmodel, "BouncingBall.v");

      terminateOMS(omsmodel);
    end when;
    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end CS_BouncingBall;





  model Describe
   extends Modelica.Icons.Example;
   OMSimulator.OMSModel omsmodel = OMSimulator.OMSModel();
  equation
    when time > 0.1 then
      Modelica.Utilities.Streams.print("Hello Where");
      OMSimulator.OMS.describe(omsmodel);
    end when;
    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Describe;

end Examples;
