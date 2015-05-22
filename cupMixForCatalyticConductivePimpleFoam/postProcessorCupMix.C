// OpenSMOKE++ Definitions
#include "OpenSMOKEpp"

// CHEMKIN maps
#include "maps/Maps_CHEMKIN"

// Reactor utilities
#include "reactors/utilities/Utilities"

// OpenFOAM
#include "fvCFD.H"
#include "multivariateScheme.H"
#include "pimpleControl.H"
#include "fvIOoptionList.H"
#include "regionProperties.H"
#include "compressibleCourantNo.H"
#include "solidRegionDiffNo.H"

// Additional include files
#include "userDefinedFunctions.H"

#include "reactions/reactionRates.H"

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"

    regionProperties rp(runTime); 

    #include "createFluidMeshes.H" 
    #include "createSolidMeshes.H" 

    #include "readSolverOptions.H"
    
    #include "createFluidFields.H"
    #include "createCatalyticFields.H"
    #include "createSolidFields.H"
    #include "properties.H"
    #include "createFinalFields.H"

    #include "initContinuityErrs.H"
    #include "readTimeControls.H"
    #include "readSolidTimeControls.H"

    #include "compressibleMultiRegionCourantNo.H"
    #include "solidRegionDiffusionNo.H"
    #include "setInitialMultiRegionDeltaT.H"

	bool cupMixExit = true;
    while (runTime.run())
    {
/*
        #include "readTimeControls.H"
        #include "readSolidTimeControls.H"
        #include "readPIMPLEControls.H"

        #include "compressibleMultiRegionCourantNo.H"
        #include "solidRegionDiffusionNo.H"
        #include "setMultiRegionDeltaT.H"
*/
		if (cupMixExit != true)
		{
			Info << "ERROR: please set writeNow in controlDict options" << endl;
			exit(-1);
		}
		runTime++;
		cupMixExit = false;
        runTime.write();
    }

    Info<< "End\n" << endl;

    return 0;
}


