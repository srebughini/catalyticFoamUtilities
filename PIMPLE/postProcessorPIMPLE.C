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
#include "fvIOoptionList.H"
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
	#include "createCatalyticFields.H"
	#include "createFvOptions.H"
	#include "memoryAllocation.H"
	#include "properties.H"
	#include "finalSetupOperations.H"
	#include "fluxes.H"

	#include "evaluatingArea.H"
	#include "evaluatingConversion.H"
	#include "evaluatingAtomBalance.H"
	#include "evaluatingPressureDrops.H"
	#include "evaluatingTransportProperties.H"
	#include "write.H"

	return 0;
}
