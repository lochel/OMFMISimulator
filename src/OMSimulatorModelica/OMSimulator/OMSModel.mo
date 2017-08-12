within OMSimulator;
class OMSModel
extends ExternalObject;
  encapsulated function constructor
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input String filename = "" "Full path to the composite model xml representation or the empty string for creating a new empty composite model";
    output OMSModel omsmodel;
    external "C" omsmodel =  oms_newOrLoadModel(filename)
    annotation (Include=
          "
#include \"OMSimulator.h\"
void* oms_newOrLoadModel(const char* filename) {
  if (filename[0] == '\\0') {
    return oms_newModel();
  } else {
    return oms_loadModel(filename);
  }
}
", Library={"OMSimulatorLib"});
  end constructor;

  encapsulated function destructor
    import Modelica;
    extends Modelica.Icons.Function;
    import OMSimulator.OMSModel;
    input OMSModel omsmodel;
    external "C" oms_unload(omsmodel)
    annotation (
     Include = "#include \"OMSimulator.h\"",
     Library = {"OMSimulatorLib"});
  end destructor;
end OMSModel;
