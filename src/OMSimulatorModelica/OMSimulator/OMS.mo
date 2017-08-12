within OMSimulator;
package OMS
  extends Modelica.Icons.Package;

  encapsulated function instantiateFMU
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input String filename;
    input String instanceName;
    // Seems that (at least for Linux) all dependencies are covered just by adding OMSimulatorLib
    // {"OMSimulatorLib", "fmilib_shared", "sundials_cvode", "sundials_nvecserial"}
    external "C" oms_instantiateFMU(omsmodel, filename, instanceName)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end instantiateFMU;

  encapsulated function setReal
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input String var;
    input Real value;
    external "C" oms_setReal(omsmodel, var, value)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end setReal;

  encapsulated function getReal
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input String var;
    output Real value;
    external "C" value = oms_getReal(omsmodel, var)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end getReal;

  encapsulated function addConnection
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input String from;
    input String to;
    external "C" oms_addConnection(omsmodel, from, to)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end addConnection;

  encapsulated function simulate
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    output Integer status;
    external "C" status = oms_simulate(omsmodel)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end simulate;

  encapsulated function doSteps
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input Integer numberOfSteps;
    output Integer status;
    external "C" status = oms_doSteps(omsmodel, numberOfSteps)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end doSteps;

  encapsulated function stepUntil
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input Real timeValue;
    output Integer status;
    external "C" status = oms_stepUntil(omsmodel, timeValue)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end stepUntil;

  encapsulated function describe "PLEASE NOTE: This external function writes to stdout and this output might not be visible in a Modelica simulation environment!"
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    external "C" oms_describe(omsmodel)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end describe;

  encapsulated function exportXML
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input String filename;
    external "C" oms_exportXML(omsmodel, filename)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end exportXML;

  encapsulated function importXML
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input String filename;
    external "C" oms_importXML(omsmodel, filename)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end importXML;

  encapsulated function exportDependencyGraph
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input String filename;
    external "C" oms_exportDependencyGraph(omsmodel, filename)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end exportDependencyGraph;

  encapsulated function initialize
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    output Integer status;
    external "C" status = oms_initialize(omsmodel)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end initialize;

  encapsulated function terminateOMS "Appending 'OMS' for avoiding ambiguity with Modelica operator terminate()"
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    output Integer status;
    external "C" status = oms_terminate(omsmodel)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end terminateOMS;

  encapsulated function reset
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    output Integer status;
    external "C" status = oms_reset(omsmodel)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end reset;

  encapsulated function getCurrentTime
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    output Real simTime;
    output Integer status;
    external "C" status = oms_reset(omsmodel, simTime)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end getCurrentTime;

  encapsulated function setTempDirectory
    import Modelica;
    extends Modelica.Icons.Function;
    input String filename;
    external "C" oms_setTempDirectory(filename)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end setTempDirectory;

  encapsulated function setStartTime
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input Real startTime;
    external "C" oms_setStartTime(omsmodel, startTime)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end setStartTime;

  encapsulated function setStopTime
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input Real stopTime;
    external "C" oms_setStopTime(omsmodel, stopTime)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end setStopTime;

  encapsulated function setTolerance
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input Real tolerance;
    external "C" oms_setTolerance(omsmodel, tolerance)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end setTolerance;

  encapsulated function setCommunicationInterval
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input Real communicationInterval;
    external "C" oms_setCommunicationInterval(omsmodel, communicationInterval)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end setCommunicationInterval;

  encapsulated function setResultFile
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input String filename;
    external "C" oms_setResultFile(omsmodel, filename)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end setResultFile;

  encapsulated function setSolverMethod
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    input String instanceName;
    input String method;
    external "C" oms_setSolverMethod(omsmodel, instanceName, method)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end setSolverMethod;

  encapsulated function logToStdStream
    import Modelica;
    extends Modelica.Icons.Function;
    input Boolean useStdStream;
    external "C" oms_logToStdStream(useStdStream)
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end logToStdStream;

  encapsulated function getVersion "WARNING: FIXME return Strings need be handled by ModelicaAllocateString() to be compliant"
    import Modelica;
    extends Modelica.Icons.Function;
    output String version;
    external "C" version = oms_getVersion()
    annotation (
         Include = "#include \"OMSimulator.h\"",
         Library = {"OMSimulatorLib"});

  end getVersion;
end OMS;
