// OpenSMOKE++ Definitions
#include "OpenSMOKEpp"

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
	#include "setRootCase.H"
	#include "createTime.H"

	regionProperties rp(runTime); 

    #include "createFluidMeshes.H" 
    #include "createSolidMeshes.H" 

    #include "readSolverOptions.H"
    #include "readFluidProperties.H"
    #include "readSolidProperties.H"

    #include "createFluidFields.H"
    #include "createSolidFields.H"

    #include "propertiesFluid.H"
    #include "propertiesSolid.H"

    #include "createFinalFields.H"

    #include "initContinuityErrs.H"


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


	return 0;
}


