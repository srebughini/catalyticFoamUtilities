// OpenSMOKE++ Definitions
#include "OpenSMOKEpp"

// CHEMKIN maps
#include "maps/Maps_CHEMKIN"

// Reactor utilities
#include "reactors/utilities/Utilities"

// ODE solvers
#include "math/multivalue-ode-solvers/MultiValueSolver"
#include "ode/ODE_Parameters.h"

// OpenFOAM
#include "fvCFD.H"
#include "multivariateScheme.H"
#include "pimpleControl.H"

#if OPENFOAM_VERSION == 22
	#include "fvIOoptionList.H"
#elseif OPENFOAM_VERSION == 23
	#include "fvIOoptionList.H"
#endif

// Additional include files
#include "userDefinedFunctions.H"

int main(int argc, char *argv[])
{
	#include "setRootCase.H"
	#include "createTime.H"
	#include "createMesh.H"
	#include "readGravitationalAcceleration.H"

	#include "createBasicFields.H"
	#include "readSolverOptions.H"
	#include "createAdditionalFields.H"

	#if OPENFOAM_VERSION == 22
		#include "createFvOptions.H"
	#elseif OPENFOAM_VERSION == 23
		#include "createFvOptions.H"
	#endif

	#include "finalSetupOperations.H"
	#include "properties.H"
	#include "initContinuityErrs.H"
	#include "readTimeControls.H"
	#include "compressibleCourantNo.H"
	#include "setInitialDeltaT.H"

	pimpleControl pimple(mesh);

	bool cupMixExit = true;
	while (runTime.run())
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

    return 0;
}
