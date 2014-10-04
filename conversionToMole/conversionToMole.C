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

int main(int argc, char **argv)
{
	timeSelector::addOptions();
	#include "setRootCase.H"
	#include "createTime.H"
	#include "createMesh.H"
	#include "readSolverOptions.H"
	#include "readFields.H"
	#include "createFields.H"

	while (runTime.run())
	{
		runTime++;
		#include "properties.H"
		runTime.write();
	}

	Info<< "End\n" << endl;
	return 0; 
}
