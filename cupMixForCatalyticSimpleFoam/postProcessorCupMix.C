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
#include "lumped.H"
#include "userDefinedFunctions.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "readGravitationalAcceleration.H"
    #include "readSolverOptions.H"

    simpleControl simple(mesh);

    #include "createBasicFields.H"
    #include "createAdditionalFields.H"
    #include "createFvOptions.H"
    #include "properties.H"
    #include "finalSetupOperations.H"
    
    #include "initContinuityErrs.H"

    dimensionedScalar initialMass = fvc::domainIntegrate(rho);

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;


	bool cupMixExit = true;  
    while (simple.loop())
    {
		if (cupMixExit != true)
		{
			Info << "ERROR: please set writeNow in controlDict options" << endl;
			exit(-1);
		}
		runTime.write();
		cupMixExit = false;
    }
    
    Info<< "End\n" << endl;
    
    return 0;
}

// ************************************************************************* //
