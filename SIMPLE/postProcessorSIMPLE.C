// OpenSMOKE++ Definitions
#include "OpenSMOKEpp"

// CHEMKIN maps
#include "maps/Maps_CHEMKIN"

// Reactor utilities
#include "reactors/utilities/Utilities"

// ODE solvers
#include "math/multivalue-ode-solvers/MultiValueSolver"
#include "ode/ODE_Parameters.h"

#include "fvCFD.H"
#include "psiCombustionModel.H"
#include "multivariateScheme.H"
#include "RASModel.H"
#include "simpleControl.H"
#include "radiationModel.H"
#include "fvIOoptionList.H"
#include "userDefinedFunctions.H"
int main(int argc, char *argv[])
{
	#include "setRootCase.H"
	#include "createTime.H"
	#include "createMesh.H"
	#include "readGravitationalAcceleration.H"
	#include "createFields.H"
	#include "createOpenSMOKEFields.H"
	#include "createFvOptions.H"
	#include "createRadiationModel.H"
	#include "initContinuityErrs.H"
	#include "properties.H"
	#include "readSolverOptions.H"

	#include "evaluatingArea.H"
	#include "evaluatingConversion.H"
	#include "evaluatingAtomBalance.H"
	#include "evaluatingPressureDrops.H"
	#include "evaluatingTransportProperties.H"
	#include "write.H"
    return 0;
}


// ************************************************************************* //
