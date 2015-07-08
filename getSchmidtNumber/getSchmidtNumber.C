// OpenSMOKE++ Definitions
#include "OpenSMOKEpp"

// CHEMKIN maps
#include "maps/Maps_CHEMKIN"

// Reactor utilities
#include "reactors/utilities/Utilities"

// OpenFOAM
#include "fvCFD.H"
#include "multivariateScheme.H"
#include "simpleControl.H"
#include "fvIOoptionList.H"

// Additional classes
#include "userDefinedFunctions.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "readGravitationalAcceleration.H"
    #include "readSolverOptions.H"

    #include "createBasicFields.H"
    #include "createAdditionalFields.H"
    #include "properties.H"
    #include "finalSetupOperations.H"
    #include "SchmidtEvaluation.H"

    return 0;
}

// ************************************************************************* //
