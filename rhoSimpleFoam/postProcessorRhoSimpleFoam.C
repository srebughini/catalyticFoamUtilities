#include "fvCFD.H"
#include "psiThermo.H"
#include "RASModel.H"
#include "simpleControl.H"
#include "fvIOoptionList.H"
#include "userDefinedFunctions.H"

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    simpleControl simple(mesh);

    #include "createFields.H"
    #include "extraFields.H"
    #include "createFvOptions.H"
    #include "initContinuityErrs.H"
    #include "evaluatingValues.H"
    #include "write.H"

    return 0;
}
