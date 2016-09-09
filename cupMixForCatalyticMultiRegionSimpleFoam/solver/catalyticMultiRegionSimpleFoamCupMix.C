// OpenSMOKE++ Definitions
#include "OpenSMOKEpp"

//C++
#include <functional>
#include <algorithm>

// CHEMKIN maps
#include "maps/Maps_CHEMKIN"

// Reactor utilities
#include "reactors/utilities/Utilities"

// OpenFOAM
#include "fvCFD.H"
#include "multivariateScheme.H"
#include "regionProperties.H"

// Additional include files
#include "userDefinedFunctions.H"
#include "reactions/reactionRates.H"
#include "heterogeneousChemistry.H"
#include "homogeneousChemistry.H"

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"

    regionProperties rp(runTime); 

    #include "createFluidMeshes.H" 
    #include "createSolidMeshes.H" 

    #include "readSolverOptions.H"
    #include "readFluidProperties.H"
    #include "readSolidProperties.H"

    heterogeneousChemistry het(*thermodynamicsMapXML,
                               *kineticsMapXML,
                               *surfaceThermodynamicsMapXML,
                               *surfaceKineticsMapXML);

    homogeneousChemistry   hom(*thermodynamicsMapXML,
                               *kineticsMapXML);

    #include "createFluidFields.H"
    #include "createSolidFields.H"

    #include "propertiesFluid.H"
    #include "propertiesSolid.H"

    #include "createFinalFields.H"

    #include "initContinuityErrs.H"
    
    #include "cupMix.H"

    #include "reactionFluid.H"
    #include "reactionSolid.H"

    bool cupMixExit = true;
    while (runTime.loop())
    {
        if (cupMixExit != true)
        {
            Info << "ERROR: please set writeNow in controlDict options" << endl;
            exit(-1);
        }
        runTime++;
        runTime.write();
        cupMixExit = false;
    }

    Info<< "End\n" << endl;

    return 0;
}
