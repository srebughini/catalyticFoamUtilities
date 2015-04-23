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
#include "fixedGradientFvPatchFields.H" 
#include "regionProperties.H"
#include "compressibleCourantNo.H"
#include "solidRegionDiffNo.H" 

// Additional include files
#include "userDefinedFunctions.H"

double MaxValue(const OpenSMOKE::OpenSMOKEMatrixDouble& M)
{
	double max_value = -1e16;
	for(unsigned int i=1;i<=M.Rows();i++)
		for(unsigned int j=1;j<=M.Columns();j++)
			max_value = (max_value < M[i][j]) ? M[i][j] : max_value;

	return max_value;
}	


int main(int argc, char *argv[])
{
	// Solver setup (folders, mesh, etc.)
	#include "setRootCase.H"
	#include "createTime.H"

	regionProperties rp(runTime); 

	// Solver options   
	#include "readSolverOptions.H"

	// Multiregion
	#include "createFluidMeshes.H" 
	#include "createSolidMeshes.H" 
	#include "createFluidFields.H" 
	#include "createSolidFields.H"
	#include "createCatalyticFields.H"
	#include "memoryAllocation.H"

	#include "propertiesMultiRegion.H"
	#include "finalSetupOperations.H"

	// Solver setup (numerical properties)
	#include "initContinuityErrs.H"
	#include "readTimeControls.H"
	#include "compressibleCourantNo.H"

	// Multiregion
	#include "readSolidTimeControls.H" 
	#include "compressibleMultiRegionCourantNo.H"  

	// Read PIMPLE Control
	#include "readPIMPLEControls.H"


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


